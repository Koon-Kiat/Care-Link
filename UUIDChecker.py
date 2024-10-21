import asyncio
import os
from bleak import BleakClient
from dotenv import load_dotenv

# Load environment variables from .env file
load_dotenv()

# Get the MAC address from the environment variable
BLE_DEVICE_MAC_ADDRESS = os.getenv("BLE_DEVICE_MAC_ADDRESS")  # Make sure this variable is set

async def print_services_and_characteristics():
    async with BleakClient(BLE_DEVICE_MAC_ADDRESS) as client:
        services = await client.get_services()
        for service in services:
            print(f"Service: {service.uuid}")
            for characteristic in service.characteristics:
                print(f"  Characteristic: {characteristic.uuid}")

asyncio.run(print_services_and_characteristics())
