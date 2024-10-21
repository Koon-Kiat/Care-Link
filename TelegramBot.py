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
YOUR_CHAT_ID = int(os.getenv("TELEGRAM_CHAT_ID"))
BLE_DEVICE_MAC_ADDRESS = os.getenv("BLE_DEVICE_MAC_ADDRESS")
ALERT_SERVICE_UUID = os.getenv("ALERT_SERVICE_UUID")
ALERT_CHARACTERISTIC_UUID = os.getenv("ALERT_CHARACTERISTIC_UUID")

# Store user's location
user_location = None
location_shared = False
last_temperature_alert_time = 0  # Variable to store last alert time
temperature_alert_cooldown = 30 * 60  # Cooldown period in seconds
last_alert_type = None  # To keep track of the last alert type
last_temperature_status = None  
startup = True

# Function to handle notifications from BLE characteristic
async def notification_handler(sender, data):
    global last_alert_type, startup  # Declare it as global to use it in this function

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

        # Handle the shortened fall status messages
        if fall_status == "S":
            logger.info("User is safe.")
        elif fall_status == "SV":
            await send_alert_message("SEVERE FALL DETECTED")
        elif fall_status == "M":
            await send_alert_message("MODERATE FALL DETECTED")
        elif fall_status == "MI":
            await send_alert_message("MINOR FALL DETECTED")
        else:
            logger.warning(f"Received unrecognized fall status: {fall_status}")

    elif message.startswith("TEMP:"):
        logger.info("Temperature alert received.")  # Log receipt of temperature alert
        
        # Avoid sending alerts immediately after startup
        if startup:
            logger.info("Startup in progress, skipping immediate temperature alert.")
            return

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

    logger.info(f"Detected fall severity: {fall_type}")  # Log the fall severity

    async with Application.builder().token(TELEGRAM_BOT_API_KEY).build() as app:
        await app.bot.send_message(chat_id=YOUR_CHAT_ID, text=fall_type)

        # Send current user's location if available
        if user_location:
            latitude, longitude = user_location
            logger.info(
                f"Sending location: Latitude: {latitude}, Longitude: {longitude}"
            )
            await app.bot.send_location(
                chat_id=YOUR_CHAT_ID, latitude=latitude, longitude=longitude
            )
        else:
            logger.warning("User location not available!")

    logger.info("Alert message and location sent.")




async def send_temperature_alert(message):
    global last_temperature_alert_time, location_shared, last_alert_type, last_temperature_status, startup  # Include startup

    # Check if location has been shared
    if not location_shared:
        logger.info("Location not shared yet. Skipping temperature alert.")
        return

    logger.info("Processing temperature alert...")

    # Extract temperature value from the message
    try:
        if "TEMP:" in message:
            temp_start = message.index("TEMP:") + len("TEMP: ")
            temperature_status = message[temp_start:].strip()
        else:
            logger.warning("TEMP_STATUS not found in message.")
            return
    except Exception as e:
        logger.warning(f"Could not parse temperature from message: {e}")
        return

    # Determine the temperature message based on the shortened status
    if temperature_status == "C":
        temp_message = "Temperature is COOL. No action needed."
        logger.info(f"Detected temperature category: COOL")
    elif temperature_status == "N":
        temp_message = "Temperature is NORMAL. Keep hydrated!"
        logger.info(f"Detected temperature category: NORMAL")
    elif temperature_status == "H":
        temp_message = "Temperature is HIGH. Seek shelter; there is a possible risk of heat injury!"
        logger.info(f"Detected temperature category: HIGH")
    else:
        logger.warning(f"Received unrecognized temperature status: {temperature_status}")
        return

    # Check if the cooldown period has elapsed or if the temperature status has changed
    current_time = time.time()
    if (current_time - last_temperature_alert_time >= temperature_alert_cooldown) or (last_alert_type != "TEMP") or (last_temperature_status != temperature_status):
        async with Application.builder().token(TELEGRAM_BOT_API_KEY).build() as app:
            await app.bot.send_message(chat_id=YOUR_CHAT_ID, text=temp_message)

        # Update the last alert time and alert type
        last_temperature_alert_time = current_time
        last_alert_type = "TEMP"  # Update last_alert_type after sending alert
        last_temperature_status = temperature_status  # Update last_temperature_status
        logger.info("Temperature alert sent.")
    else:
        logger.info("Cooldown period has not elapsed or status has not changed. Temperature alert not sent.")




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
    global user_location, location_shared  # Make sure to access the global variables

    if update.message and update.message.location:
        user_location = (
            update.message.location.latitude,
            update.message.location.longitude,
        )
        location_shared = True  # Mark location as shared

        logger.info(
            f"User location updated: Latitude: {user_location[0]}, Longitude: {user_location[1]}"
        )
        await update.message.reply_text("Thank you! Your live location has been saved.")
        await update.message.reply_text("Your location will be updated continuously.")
    else:
        logger.warning("No location found in the update message.")



async def connect_ble_device():
    try:
        async with BleakClient(BLE_DEVICE_MAC_ADDRESS) as client:
            logger.info(f"Connected to BLE device: {BLE_DEVICE_MAC_ADDRESS}")
            await client.start_notify(ALERT_CHARACTERISTIC_UUID, notification_handler)

            # Keep the connection open
            while True:
                await asyncio.sleep(1)
    except BleakError as e:
        logger.error(f"BLE error: {str(e)}")
    except Exception as e:
        logger.error(f"General error connecting to BLE device: {str(e)}")

async def main_startup():
    global startup
    startup = True
    # Add a delay here if necessary
    await asyncio.sleep(1)  # Adjust the sleep time as needed
    startup = False


def main() -> None:
    app = Application.builder().token(TELEGRAM_BOT_API_KEY).build()

    app.add_handler(CommandHandler("start", start))
    app.add_handler(MessageHandler(filters.LOCATION, location_handler))

    # Start the BLE connection in a separate task
    loop = asyncio.get_event_loop()
    loop.create_task(connect_ble_device())
    loop.create_task(main_startup())
    app.run_polling(allowed_updates=Update.ALL_TYPES)


if __name__ == "__main__":
    main()
