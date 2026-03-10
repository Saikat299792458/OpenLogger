import socket
import json
import argparse
import subprocess
from datetime import datetime
import time

def start_hotspot(ssid, password):
    print(f"Attempting to start Hotspot: {ssid}...")
    try:
        # 1. Configure the hotspot credentials
        subprocess.run(f'netsh wlan set hostednetwork mode=allow ssid={ssid} key={password}', shell=True)
        # 2. Start the network
        subprocess.run('netsh wlan start hostednetwork', shell=True)
        print("Hotspot is active.")
    except Exception as e:
        print(f"Failed to start hotspot via netsh: {e}")
        print("Please ensure Mobile Hotspot is enabled in Windows Settings.")

def stop_hotspot():
    subprocess.run('netsh wlan stop hostednetwork', shell=True)
    print("Hotspot stopped.")

def run_server(session_name, interval):
    # Check if admin
    try:
        subprocess.check_output('net session', shell=True, stderr=subprocess.STDOUT)
    except subprocess.CalledProcessError:
        print("This program must be run as an administrator to manage the hotspot.")
        return
    start_hotspot("WemosLogger", "Pass1234")

    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.bind(('0.0.0.0', 8080))
    server.listen(1)
    
    print(f"--- SERVER ACTIVE ---")
    print(f"Logging to: session_{session_name}.csv")

    try:
        while True:
            conn, addr = server.accept()
            data = conn.recv(4096).decode('utf-8')
            if not data: continue

            # Parse and Save
            incoming = json.loads(data)
            filename = f"session_{session_name}.csv"
            
            with open(filename, 'a') as f:
                for entry in incoming['logs']:
                    # Format: Time, Temp, Battery
                    f.write(f"{datetime.now()},{entry['v']},{incoming['battery']}\n")

            # Send Sync Info back to Wemos
            response = {"status": "OK", "interval": interval, "epoch": int(datetime.now().timestamp())}
            conn.send(json.dumps(response).encode('utf-8'))
            conn.close()
            print(f"Data received from {addr[0]} and saved.")

    except KeyboardInterrupt:
        print("\nShutting down...")
        # stop_hotspot()

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--session", default="Lab_Test")
    parser.add_argument("--interval", type=int, default=1)
    args = parser.parse_args()
    
    run_server(args.session, args.interval)