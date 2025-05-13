This project is a Flask-based web application that logs the entry and exit times of buses using RFID tags and stores the data in an Excel file (buslog22.xlsx). The system reads data from a serial-connected RFID reader (e.g., Arduino), automatically toggles the bus status (Entered/Exited), and provides a user interface for administration.

🔧 Features
📟 Real-time data logging via serial port (COM3).

🕒 Timestamped log entries of bus movement (entry/exit).

🧾 Excel file generation for all activity (buslog22.xlsx).

🌐 Flask-based UI to view and manage RFID tags.

📥 Downloadable logs through /download.

➕ Add new RFID UIDs to the system.

🔁 Status toggling logic based on last recorded action.

📁 Project Structure
pgsql
Copy
Edit
├── app.py                 # Main Flask application
├── buslog22.xlsx          # Excel log file (generated)
├── rfid_tags.json         # Stores UID to bus number mapping
├── templates/
│   └── admin.html         # Admin interface
🚀 Getting Started
Prerequisites
Python 3.x

Flask

Pandas

pyserial

Install Dependencies
bash
Copy
Edit
pip install flask pandas pyserial
Connect RFID Reader
Ensure your RFID reader (via Arduino or similar) is connected to COM3. Update COM3 in the script if needed.

Run the App
bash
Copy
Edit
python app.py
Visit http://localhost:5000 in your browser.

🔌 Endpoints
Endpoint	Description
/	Admin panel showing UID-bus mappings
/download	Download the Excel log file
/add-uid	POST route to add a new UID + name
/get-data	Returns JSON of current bus events

🧠 Logic Overview
Serial Reader Thread: Reads RFID tag from Arduino via COM3.

Toggle System: First scan = Entered, next = Exited (and so on).

Log Storage: Every event is saved with timestamp and status.

Web Interface: Admin can add UID-bus mappings and download logs.

📌 Note
Ensure your Arduino is programmed to send messages like:

yaml
Copy
Edit
Access granted for: Bus42
The status alternates based on the last recorded action for a bus.

🛠 Future Improvements
Authentication for the admin panel

Graphical log analytics

Remote sync to cloud storage (Google Sheets, Firebase, etc.)

Enhanced UI with filters/search

👨‍💻 Author
Crafted with care for smarter campus transport systems 🚍
