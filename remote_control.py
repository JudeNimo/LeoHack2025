import socket
import time
import keyboard  # pip install keyboard
from datetime import datetime, timedelta

ARUINO_IP = "192.168.4.1"
ARUINO_PORT = 8080
# Default values setup
power = 5
duration = 1
stop_command = 's'

#Mapping the movements of keys to Arduino command
key_to_command = {
    'w': f'f {power} {duration}', #forward
    's': f'b {power} {duration}', #backward
    'a': f'a {power} {duration}', #left
    'd': f'd {power} {duration}', #right
    'q': f'l {power} {duration}', #left turn
    'e': f'r {power} {duration}', #right turn
}

#Keep track of pressed keye to avoid flooding commands
pressed_keys = set()

print("Connecting to Arduino...")


try:
    s = socket.socket()
    s.settimeout(5)
    s.connect((ARUINO_IP, ARUINO_PORT))
    print("Connected!")
    print("Hold WASD/QE to move. Release to stop. Press ESC to exit.")

    while True:
        # Check for pressed keys
        for key in key_to_command.keys():
            if keyboard.is_pressed(key) and key not in pressed_keys:
                pressed_keys.add(key)
                cmd = key_to_command[key]
                print(f"Sending command: {cmd}")
                s.send(cmd.encode())

        # Check for released keyswsqeswswswqeqqeeqqwwsewww
        keys_to_remove = []
        for key in list(pressed_keys):
            if not keyboard.is_pressed(key):
                print(f"Key released: {key}")
                s.send(stop_command.encode())
                keys_to_remove.append(key)
                
        #remove released keys from the set
        for key in keys_to_remove:
            pressed_keys.remove(key)

        # Check for exit key
        if keyboard.is_pressed('esc'):
            print("Exiting...")
            break

        time.sleep(0.05) # Small delay to avoid high CPU usage

    s.close()

    # s.send(b" ====== \n")
    # data = s.recv(1024)
    # print("Received:", data)

    # while True:
    #     msg = input("Send to Arduino: ")
    #     s.send(msg.encode())
    #     # delay = timedelta(seconds=0.5)
    #     # print("Reply:", s.recv(1024))


except Exception as e:
    print("Connection failed:", e)
