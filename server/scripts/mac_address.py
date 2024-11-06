import asyncio
from bleak import BleakScanner


async def run():
    devices = await BleakScanner.discover()
    for d in devices:
        print(f"Device: {d.name}, Address: {d.address}")


asyncio.run(run())
