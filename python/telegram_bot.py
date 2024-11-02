import logging
import os
import time
import asyncio
from bleak import BleakClient
from bleak import BleakError
from telegram import Update, KeyboardButton, ReplyKeyboardMarkup
from telegram.ext import (
    Application,
    CommandHandler,
    MessageHandler,
    filters,
    CallbackContext,
)
from dotenv import load_dotenv

# Load environment variables from .env file
load_dotenv()

# Enable logging
logging.basicConfig(
    format="%(asctime)s - %(name)s - %(levelname)s - %(message)s", level=logging.INFO
)
logger = logging.getLogger(__name__)

# Get the API key, chat ID, and BLE configuration from environment variables
TELEGRAM_BOT_API_KEY = os.getenv("TELEGRAM_BOT_API_KEY")
TELEGRAM_CHAT_ID = os.getenv("TELEGRAM_CHAT_ID")
BLE_DEVICE_MAC_ADDRESS = os.getenv("BLE_DEVICE_MAC_ADDRESS")
ALERT_SERVICE_UUID = os.getenv("ALERT_SERVICE_UUID")
ALERT_CHARACTERISTIC_UUID = os.getenv("ALERT_CHARACTERISTIC_UUID")

if TELEGRAM_CHAT_ID is None or not TELEGRAM_CHAT_ID.isdigit():
    logger.error(
        "Invalid TELEGRAM_CHAT_ID. Please check your environment variables.")
    exit(1)

YOUR_CHAT_ID = int(TELEGRAM_CHAT_ID)

user_location = None
location_shared = False
last_temperature_alert_time = 0  # Variable to store last alert time
temperature_alert_cooldown = 1800  # Cooldown period in seconds
last_alert_type = None  # To keep track of the last alert type
last_temperature_status = None
previous_fall_status = None
previous_temperature_status = None
startup = True

ble_connected_event = asyncio.Event()


async def notification_handler(sender, data):
    # Declare all necessary variables as global
    global last_alert_type, startup, previous_fall_status, previous_temperature_status

    try:
        message = data.decode("utf-8").strip()  # Decode the message
    except UnicodeDecodeError:
        logger.error("Received data could not be decoded as UTF-8.")
        return

    logger.info(f"Decoded message: {message}")

    # Check the type of message and log accordingly
    if message.startswith("FALL:"):
        fall_status = message[len("FALL:"):].strip()
        logger.info(f"Fall status: {fall_status}")

        if fall_status != previous_fall_status:  # Check if the fall status has changed
            previous_fall_status = fall_status  # Update the previous status

            if fall_status == "S":
                logger.info("User is safe.")
            elif fall_status == "SV":
                await send_alert_message("SEVERE FALL DETECTED")
            elif fall_status == "M":
                await send_alert_message("MODERATE FALL DETECTED")
            elif fall_status == "MI":
                await send_alert_message("MINOR FALL DETECTED")
            else:
                logger.warning(
                    f"Received unrecognized fall status: {fall_status}")

    elif message.startswith("TEMP:"):
        # Log receipt of temperature alert
        logger.info("Temperature alert received.")

        # Process the temperature alert
        await send_temperature_alert(message)

    else:
        logger.warning(f"Received unrecognized message: {message}")


async def send_alert_message(message):
    logger.info("Sending alert message to Telegram...")

    # Determine the type of fall from the message
    if "SEVERE FALL DETECTED" in message:
        fall_type = "SEVERE FALL DETECTED"
    elif "MODERATE FALL DETECTED" in message:
        fall_type = "MODERATE FALL DETECTED"
    elif "MINOR FALL DETECTED" in message:
        fall_type = "MINOR FALL DETECTED"
    else:
        logger.warning("Received unrecognized fall message.")
        return

    # Log the fall severity
    logger.info(f"Detected fall severity: {fall_type}")

    async with Application.builder().token(TELEGRAM_BOT_API_KEY).build() as app:
        await app.bot.send_message(chat_id=YOUR_CHAT_ID, text=fall_type)

        # Send current user's location if available
        if user_location:
            latitude, longitude = user_location
            logger.info(f"Sending location: Latitude: {
                        latitude}, Longitude: {longitude}")
            await app.bot.send_location(chat_id=YOUR_CHAT_ID, latitude=latitude, longitude=longitude)
        else:
            logger.warning("User location not available!")

    logger.info("Alert message and location sent.")


async def send_temperature_alert(message):
    global last_temperature_alert_time, location_shared, last_alert_type, previous_temperature_status

    if not location_shared:
        logger.info("Location not shared yet. Skipping temperature alert.")
        return

    logger.info("Processing temperature alert...")

    # Extract temperature value from the message
    try:
        temp_start = message.index("TEMP:") + len("TEMP: ")
        temperature_status = message[temp_start:].strip()
    except Exception as e:
        logger.warning(f"Could not parse temperature from message: {e}")
        return

    # Determine the temperature message based on the shortened status
    temp_message = ""
    if temperature_status == "C":
        temp_message = "Temperature is COOL. No action needed."
    elif temperature_status == "N":
        temp_message = "Temperature is NORMAL. Keep hydrated!"
    elif temperature_status == "H":
        temp_message = "Temperature is HIGH. Seek shelter; there is a possible risk of heat injury!"
    else:
        logger.warning(f"Received unrecognized temperature status: {
                       temperature_status}")
        return

    current_time = time.time()

    # Check if the cooldown period has elapsed or if the temperature status has changed
    if (current_time - last_temperature_alert_time >= temperature_alert_cooldown) or (previous_temperature_status != temperature_status):
        async with Application.builder().token(TELEGRAM_BOT_API_KEY).build() as app:
            await app.bot.send_message(chat_id=YOUR_CHAT_ID, text=temp_message)

        # Update the last alert time and alert type
        last_temperature_alert_time = current_time
        last_alert_type = "TEMP"
        # Update the previous temperature status
        previous_temperature_status = temperature_status

        logger.info("Temperature alert sent.")
    else:
        logger.info(
            "Cooldown period has not elapsed or status has not changed. Temperature alert not sent.")


async def connect_ble_device():
    try:
        client = BleakClient(BLE_DEVICE_MAC_ADDRESS)
        # Call connect_ble_with_retries directly
        await connect_ble_with_retries(client)

        # Keep the connection open once connected
        while True:
            await asyncio.sleep(10)
    except BleakError as e:
        logger.error(f"BLE error: {str(e)}")
    except Exception as e:
        logger.error(f"General error connecting to BLE device: {str(e)}")


async def connect_ble_with_retries(client, retries=3, delay=5):
    attempt = 0
    while attempt < retries:
        try:
            logger.info(f"Attempt {attempt + 1} to connect to BLE device...")
            await client.connect()
            await client.start_notify(ALERT_CHARACTERISTIC_UUID, notification_handler)
            ble_connected_event.set()  # Signal BLE connection is established
            logger.info("BLE connection established.")

            # Keep the connection open once successful
            while True:
                await asyncio.sleep(1)
            break
        except BleakError as e:
            logger.error(f"BLE error on attempt {attempt + 1}: {str(e)}")
            attempt += 1
        except Exception as e:
            logger.error(f"General error on attempt {attempt + 1}: {str(e)}")
            attempt += 1

        if attempt < retries:
            logger.info(f"Retrying in {delay} seconds...")
            await asyncio.sleep(delay)

    if attempt == retries:
        logger.error(f"Failed to connect to BLE device after {
                     retries} attempts.")


async def start(update: Update, context: CallbackContext):
    logger.info("Start command received.")
    await update.message.reply_text(
        "Welcome! Please share your live location with me so I can notify your caretaker in case of an emergency."
    )

    # Request live location
    logger.info("Requesting live location...")
    await update.message.reply_text(
        "Please share your live location:",
        reply_markup=ReplyKeyboardMarkup(
            [[KeyboardButton("Share Live Location", request_location=True)]],
            one_time_keyboard=True,
            resize_keyboard=True,
        ),
    )


async def location_handler(update: Update, context: CallbackContext):
    # Make sure to access the global variables
    global user_location, location_shared

    if update.message and update.message.location:
        user_location = (
            update.message.location.latitude,
            update.message.location.longitude,
        )
        location_shared = True  # Mark location as shared

        logger.info(
            f"User location updated: Latitude: {
                user_location[0]}, Longitude: {user_location[1]}"
        )
        await update.message.reply_text("Thank you! Your live location has been saved.")
        await update.message.reply_text("Your location will be updated continuously.")
    else:
        logger.warning("No location found in the update message.")


async def send_initial_message(app: Application):
    await ble_connected_event.wait()  # Wait for BLE connection to be established
    await app.bot.send_message(chat_id=TELEGRAM_CHAT_ID, text="Welcome! Please share your live location with me so I can notify your caretaker in case of an emergency.")
    await app.bot.send_message(chat_id=TELEGRAM_CHAT_ID, text="Please share your live location:", reply_markup=ReplyKeyboardMarkup(
        [[KeyboardButton("Share Live Location", request_location=True)]],
        one_time_keyboard=True,
        resize_keyboard=True,
    ))


async def main_startup():
    global startup
    startup = True
    # Add a delay here if necessary
    await asyncio.sleep(1)  # Adjust the sleep time as needed
    startup = False


def main() -> None:
    app = Application.builder().token(TELEGRAM_BOT_API_KEY).build()
    app.add_handler(MessageHandler(filters.LOCATION, location_handler))
    loop = asyncio.get_event_loop()
    loop.create_task(connect_ble_device())
    loop.create_task(main_startup())
    loop.run_until_complete(send_initial_message(app))

    app.run_polling(allowed_updates=Update.ALL_TYPES)


if __name__ == "__main__":
    main()