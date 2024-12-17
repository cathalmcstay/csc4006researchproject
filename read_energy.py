import serial
import time
import socket
from threading import Thread

# Arduino and Server Config
arduino_port = '/dev/ttyACM0'
baud_rate = 115200
server_host = "0.0.0.0"
server_port = 5000

# Constants
VOLTAGE = 230.0  # UK Mains Voltage
time_interval = 1  # Interval in seconds for energy calculations

# Variables
current_energy = 0.0
power = 0.0

# Connect to Arduino
ser = serial.Serial(arduino_port, baud_rate, timeout=1)

def read_energy():
    global current_energy, power
    print("Starting energy monitoring...")

    while True:
        if ser.in_waiting > 0:
            data = ser.readline().decode('utf-8', errors='ignore').strip()
            if "A" in data:
                try:
                    current = float(data.split(" ")[-2])  # Extract current (e.g., 0.21 A)
                    power = VOLTAGE * current  # Power in Watts
                    current_energy += (power * time_interval / 3600) / 1000  # Increment energy in kWh
                    print(f"Current: {current:.2f} A | Power: {power:.2f} W | Energy: {current_energy:.6f} kWh")
                except ValueError:
                    print("Error parsing current data")
        time.sleep(time_interval)

# Start Energy Monitoring Thread
monitor_thread = Thread(target=read_energy, daemon=True)
monitor_thread.start()

# Create a Socket Server
server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server.bind((server_host, server_port))
server.listen(1)
print(f"Energy Server Running on {server_host}:{server_port}...")

# Handle Requests
while True:
    client, address = server.accept()
    print(f"Connection from {address}")
    client.send(f"{current_energy:.6f}".encode())  # Send current energy in kWh
    client.close()