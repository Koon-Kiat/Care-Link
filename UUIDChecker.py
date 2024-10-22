import asyncio
import os
from bleak import BleakClient
from dotenv import load_dotenv

# Load environment variables from .env file
load_dotenv()

# Get the MAC address from the environment variable
BLE_DEVICE_MAC_ADDRESS = os.getenv("BLE_DEVICE_MAC_ADDRESS")

async def print_services_and_characteristics():
    max_retries = 3
    for attempt in range(max_retries):
        try:
            print(f"Attempt {attempt + 1}: Connecting to {BLE_DEVICE_MAC_ADDRESS}")
            async with BleakClient(BLE_DEVICE_MAC_ADDRESS, timeout=30.0) as client:
                print("Connected to device, fetching services...")
                services = await client.get_services()
                print("Services fetched successfully.")
                for service in services:
                    print(f"Service: {service.uuid}")
                    for characteristic in service.characteristics:
                        print(f"  Characteristic: {characteristic.uuid}")
                return  # Exit the function if successful
        except asyncio.TimeoutError:
            print("Connection to the Bluetooth device timed out. Please ensure the device is on and in range.")
        except Exception as e:
            print(f"An error occurred: {e}")
        print("Retrying...\n")
    print("Failed to connect after multiple attempts.")

asyncio.run(print_services_and_characteristics())