from flask import Flask, request, jsonify, render_template
from datetime import datetime

app = Flask(__name__)

# In-memory storage for the latest data
latest_sensor_data = {
    "fall_status": "SAFE",
    "activity_status": "UNKNOWN",
    "temperature": 0.0,
    "timestamp": None
}

@app.route('/api/sensordata', methods=['POST'])
def receive_sensor_data():
    global latest_sensor_data
    try:
        # Parse incoming JSON data
        data = request.json
        data["timestamp"] = datetime.now().isoformat()  # Add a timestamp
        latest_sensor_data = data
        print(f"Received data: {data}")
        return jsonify({"status": "success"}), 200
    except Exception as e:
        print(f"Error: {e}")
        return jsonify({"status": "error", "message": str(e)}), 400

@app.route('/api/sensordata', methods=['GET'])
def get_sensor_data():
    return jsonify(latest_sensor_data)

@app.route('/')
def home():
    # Render a simple HTML page showing the latest data
    return render_template('index.html', sensor_data=latest_sensor_data)

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)
