from flask import Flask, render_template, request
import serial.tools.list_ports
import time
import atexit

# Create Flask app
app = Flask(__name__)

# Global variable for Arduino connection
arduino = None
# Find and connect to Arduino
def connect_arduino():
    global arduino

    # Close any existing connection to avoid permission errors
    if arduino is not None:
        try:
            arduino.close()
            print("Closed existing connection")
        except:
            pass

    # List all available ports
    print("Looking for Arduino...")
    ports = list(serial.tools.list_ports.comports())
    for p in ports:
        print(f"Found port: {p.device}")

    # Try to find the Arduino
    arduino_port = None
    for p in ports:
        if "Arduino" in p.description or "CH340" in p.description:
            arduino_port = p.device
            print(f"Arduino found at: {arduino_port}")
            break

    # If not found, try COM3 or COM4 (common Arduino ports)
    if not arduino_port:
        if any(p.device == "COM4" for p in ports):
            arduino_port = "COM4"
            print("Using COM4 for Arduino")
        elif any(p.device == "COM3" for p in ports):
            arduino_port = "COM3"
            print("Using COM3 for Arduino")
        else:
            print("Arduino not found.")
            return False

    # Connect to the port
    try:
        arduino = serial.Serial(arduino_port, 9600, timeout=1)
        time.sleep(2)  # Wait for Arduino to reset
        print("Connected to Arduino")
        return True
    except Exception as e:
        print(f"Error connecting to Arduino: {e}")
        return False

# Close Arduino connection when the app exits
def close_arduino():
    global arduino
    if arduino:
        try:
            arduino.close()
            print("Arduino connection closed")
        except:
            pass

# Register the function to be called on exit
atexit.register(close_arduino)

# Send command to Arduino using a context manager to avoid port conflicts
def send_to_arduino(command):
    try:
        # If we don't have a connection or it's closed, try to reconnect
        if not arduino or not arduino.is_open:
            if not connect_arduino():
                return "Not connected to Arduino"

        # Send the command
        arduino.write(f"{command}\n".encode())
        print(f"Sent: {command}")

        # Wait for response
        time.sleep(0.1)
        response = ""
        while arduino.in_waiting:
            response += arduino.readline().decode().strip()

        if response:
            print(f"Arduino says: {response}")
            return f"Command {command} sent"

        return f"Command {command} sent"

    except Exception as e:
        print(f"Error sending command: {e}")
        # Try to close and reset connection for next attempt
        try:
            if arduino:
                arduino.close()
        except:
            pass
        arduino = None
        return f"Error sending command: {e}"
        # Store the state of each LED
led_states = {
    "1": False,
    "2": False,
    "3": False
}

# Web page
@app.route("/", methods=["GET", "POST"])
def home():
    global led_states
    message = ""

    # Handle button clicks
    if request.method == "POST":
        # LED 1
        if "led1_on" in request.form:
            message = send_to_arduino("on1")
            if "Error" not in message and "Not connected" not in message:
                led_states["1"] = True
            elif "led1_off" in request.form:
                message = send_to_arduino("off1")
                if "Error" not in message and "Not connected" not in message:
                    led_states["1"] = False

        # LED 2
        elif "led2_on" in request.form:
            message = send_to_arduino("on2")
            if "Error" not in message and "Not connected" not in message:
                led_states["2"] = True
            elif "led2_off" in request.form:
                message = send_to_arduino("off2")
                if "Error" not in message and "Not connected" not in message:
                    led_states["2"] = False

        # LED 3
        elif "led3_on" in request.form:
            message = send_to_arduino("on3")
            if "Error" not in message and "Not connected" not in message:
                led_states["3"] = True
            elif "led3_off" in request.form:
                message = send_to_arduino("off3")
                if "Error" not in message and "Not connected" not in message:
                    led_states["3"] = False

    # Show the web page with current LED states
    return render_template("index.html", message=message, led_states=led_states)

# New route to get current LED states (for AJAX updates)
@app.route("/status", methods=["GET"])
def status():
    return {
        "led_states": led_states,
        "connected": arduino is not None and arduino.is_open
    }

# Start the web server
if __name__ == "__main__":
    # Connect to Arduino when starting
    connect_arduino()

    # Start the web server
    print("Starting web server...")
    print("Open your browser and go to: http://127.0.0.1:5000")
    
    # Run with debug=False to avoid port conflicts
    app.run(debug=False, host="127.0.0.1", port=5000)