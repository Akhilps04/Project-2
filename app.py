from flask import Flask, send_file, request, render_template, json, jsonify
import pandas as pd
import serial
from threading import Thread
from datetime import datetime
import atexit
import os

app = Flask(__name__)

# Path to the data files
log_file_path = 'buslog22.xlsx'
rfid_file_path = 'rfid_tags.json'

# Data storage
bus_events = []
# Dictionary to track the last status of each bus number
last_status = {}

def read_from_arduino():
    try:
        ser = serial.Serial('COM3', 9600, timeout=1)
        while ser.is_open:
            line = ser.readline().decode('utf-8').strip()
            if line.startswith("Access granted for:"):
                bus_name = line.split(':')[1].strip()
                timestamp = datetime.now()
                process_data(bus_name, timestamp)
    except serial.SerialException as e:
        print("Error reading from serial port:", e)
    finally:
        if ser.is_open:
            ser.close()

def process_data(bus_name, timestamp): 
    global bus_events, last_status
    # If the bus is not in the last_status dictionary, initialize it with 'Entered'
    # so the first event recorded will be 'Exited'
    if bus_name not in last_status:
        last_status[bus_name] = 'Entered'

    # If the last status was 'Entered', change to 'Exited'
    if last_status[bus_name] == 'Entered':
        current_status = 'Exited'
    else:
        current_status = 'Entered'

    # Update the last status for the bus
    last_status[bus_name] = current_status

    # Append the event to bus_events
    bus_events.append({
        'Date': timestamp.strftime('%Y-%m-%d'),
        'Day': timestamp.strftime('%A'),
        'Time': timestamp.strftime('%H:%M:%S'),
        'Busnumber': bus_name,
        'Status': current_status
    })

    # Save the event immediately
    save_to_excel()


def save_to_excel():
    # Convert the bus_events list to a DataFrame and sort by Date and Time
    df = pd.DataFrame(bus_events)
    df.sort_values(by=['Date', 'Time'], inplace=True)
    df.to_excel(log_file_path, index=False)


# Start the serial reading in a separate thread
thread = Thread(target=read_from_arduino)
thread.start()

@app.route('/')
def index():
    try:
        with open(rfid_file_path) as file:
            rfid_tags = json.load(file)
        return render_template('admin.html', rfid_tags=rfid_tags)
    except FileNotFoundError:
        return "The file rfid_tags.json was not found in the directory.", 404

@app.route('/download')
def download_file():
    if os.path.exists(log_file_path):
        return send_file(log_file_path, as_attachment=True)
    else:
        return "The log file does not exist.", 404

@app.route('/add-uid', methods=['POST'])
def add_uid():
    new_uid = request.form['uid']
    new_name = request.form['name']
    # Add new UID to the system
    with open(rfid_file_path, 'r+') as file:
        rfid_tags = json.load(file)
        rfid_tags[new_uid] = new_name
        file.seek(0)
        json.dump(rfid_tags, file, indent=4)
    return "UID added"

@app.route('/get-data')
def get_data():
    # Endpoint to get the latest RFID data
    return jsonify(bus_events)

@atexit.register
def cleanup():
    # This will be executed when the app closes
    pass

if __name__ == "__main__":
    app.run(debug=False, host='0.0.0.0')  # Accessible from other devices in the local network