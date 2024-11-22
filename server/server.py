from flask import Flask, request, jsonify, render_template
from datetime import datetime
import requests

app = Flask(__name__)

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
    return jsonify(latest_sensor_data)

@app.route('/')
def home():
    # Render the HTML page showing the latest sensor data
    return render_template('index.html', sensor_data=latest_sensor_data)

@app.route('/api/medication', methods=['GET'])
def get_medication_schedule():
    """Serve the current medication schedule to the frontend."""
    return jsonify(medication_schedule)


@app.route('/api/medication', methods=['POST'])
def update_medication_schedule():
    global medication_schedule  # Reference the global variable

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


@app.route('/api/send_medication', methods=['POST'])
def send_medication_to_arduino():
    global medication_schedule  # Reference the global variable

    # Arduino details
    arduino_ip = "6.10.20.172"  # Replace with your Arduino's actual IP
    arduino_port = 80  # Replace with your Arduino's port
    arduino_endpoint = f"http://{arduino_ip}:{arduino_port}/update_medication"

    try:
        # Create JSON payload with the updated medication schedule
        payload = {"schedule": medication_schedule}

        # Send the schedule to the Arduino via HTTP
        response = requests.post(arduino_endpoint, json=payload)

        # Check Arduino response
        if response.status_code == 200:
            return jsonify({"status": "success", "message": "Medication schedule sent to Arduino."}), 200
        else:
            return jsonify({"status": "error", "message": f"Failed to send schedule. Arduino responded with status {response.status_code}"}), 500
    except Exception as e:
        return jsonify({"status": "error", "message": str(e)}), 500


@app.route('/update_medication', methods=['POST'])
def update_medication():
    data = request.get_json()  # Parse the incoming JSON payload
    if not data:
        return jsonify({"status": "error", "message": "No data received"}), 400

    # Process the data (e.g., log it, send it to Arduino, etc.)
    print("Received data:", data)

    # Respond with a success message
    return jsonify({"status": "success", "message": "Medication schedule updated successfully."}), 200

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)
