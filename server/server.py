from flask import Flask, request, jsonify, render_template
from datetime import datetime
import requests
import os
import time

# Set timezone to Singapore Time (SGT)
os.environ['TZ'] = 'Asia/Singapore'
time.tzset()

app = Flask(__name__,
            template_folder='templates',
            static_folder='static')

# In-memory medication schedule
medication_schedule = [
    {"name": "Aspirin", "time": "08:00"},
    {"name": "Vitamin D", "time": "12:00"}
]

# In-memory storage for the latest data
latest_sensor_data = {
    "fall_status": "SAFE",
    "activity_status": "UNKNOWN",
    "temperature": 0.0,
    "timestamp": None,
    "med_status": "MED_CONFIRM"
}

@app.route('/api/sensordata', methods=['POST'])
def receive_sensor_data():
    """
    Receive and update sensor data.

    Parses the incoming JSON payload and updates the latest_sensor_data with it.
    Adds a timestamp to the sensor data.

    Returns:
        tuple: JSON response with status and HTTP status code.
    """
    global latest_sensor_data
    try:
        print(f"Raw request data: {request.data}")  # Log raw request data

        # Parse JSON payload
        data = request.get_json()
        if not data:
            print("Error: No JSON data received.")
            return jsonify({"status": "error", "message": "No JSON data received"}), 400

        # Log parsed data
        print(f"Received JSON: {data}")

        # Update the latest sensor data
        latest_sensor_data.update(data)
        latest_sensor_data["timestamp"] = datetime.now().isoformat()  # Add timestamp
        print(f"Updated latest_sensor_data: {latest_sensor_data}")

        return jsonify({"status": "success"}), 200
    except Exception as e:
        print(f"Error processing request: {e}")
        return jsonify({"status": "error", "message": str(e)}), 500


@app.route('/api/sensordata', methods=['GET'])
def get_sensor_data():
    """
    Retrieve the latest sensor data.

    Returns:
        JSON: The latest sensor data.
    """
    return jsonify(latest_sensor_data)

@app.route('/')
def home():
    """
    Serve the homepage template.

    Returns:
        HTML: Rendered HTML template with sensor data.
    """
    return render_template('index.html', sensor_data=latest_sensor_data)

@app.route('/api/medication', methods=['GET'])
def get_medication_schedule():
    """
    Get the medication schedule.

    Returns:
        JSON: Medication schedule.
    """
    return jsonify(medication_schedule)


@app.route('/api/medication', methods=['POST'])
def update_medication_schedule():
    """
    Update the medication schedule.

    Parses the incoming JSON payload and updates the medication_schedule list.
    Each medication entry must contain 'name' and 'time'.

    Returns:
        tuple: JSON response with status, message, and HTTP status code.
    """
    global medication_schedule

    try:
        # Parse JSON payload
        data = request.get_json()
        if not isinstance(data, list):
            return jsonify({"status": "error", "message": "Invalid data format. Expected a list."}), 400

        # Update the medication schedule
        medication_schedule = [{"name": med["name"], "time": med["time"]} for med in data if "name" in med and "time" in med]
        print(f"Updated Medication Schedule: {medication_schedule}")

        return jsonify({"status": "success", "message": "Medication schedule updated successfully."}), 200
    except Exception as e:
        print(f"Error updating medication schedule: {e}")
        return jsonify({"status": "error", "message": str(e)}), 500

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)