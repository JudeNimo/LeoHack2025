import socket
from path_finder import sequence

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
    'o': 'o', #servo open
    'p': 'p', #servo close
    'm': 'm', #toggle autonomous mode
}

# Toggle commands that should only send once (not continuously)
toggle_commands = {'o', 'p', 'm'}

#Keep track of pressed keye to avoid flooding commands
pressed_keys = set()

print("Connecting to Arduino...")


try:
    s = socket.socket()
    s.settimeout(5)
    s.connect((ARUINO_IP, ARUINO_PORT))
    print("Connected!")
<<<<<<< HEAD

    power = 7
    duration = 1

    name_to_command = {
        'forward': f'f {power} {duration}', #forward
        'backward': f'b {power} {duration}', #backward
        'left': f'a {power} {duration}', #left
        'right': f'd {power} {duration}', #right
        'left_turn': f'l {power} {duration}', #left turn
        'right_turn': f'r {power} {duration}', #right turn
    }
=======
    print("\n=== MANUAL CONTROL MODE ===")
    print("Movement Controls:")
    print("  W - Forward")
    print("  S - Backward")
    print("  A - Translate Left")
    print("  D - Translate Right")
    print("  Q - Rotate Left")
    print("  E - Rotate Right")
    print("  O - Servo Open")
    print("  P - Servo Close")
    print("  M - Toggle Autonomous Mode")
    print("  S (when held) - Stop")
    print("  ESC - Exit program")
    print("\nHold keys to move. Release to stop.")
    print("Press 'm' to toggle autonomous docking mode.\n")

    while True:
        # Check for pressed keys
        for key in key_to_command.keys():
            if keyboard.is_pressed(key) and key not in pressed_keys:
                pressed_keys.add(key)
                cmd = key_to_command[key]
                print(f"Sending command: {cmd}")
                s.send(cmd.encode())
                # For toggle commands, remove immediately so they can be pressed again
                if key in toggle_commands:
                    # Small delay to ensure command is sent
                    time.sleep(0.1)
                    pressed_keys.remove(key)

        # Check for released keys
        keys_to_remove = []
        for key in list(pressed_keys):
            if not keyboard.is_pressed(key):
                print(f"Key released: {key}")
                # Only send stop command for movement keys, not toggle commands
                if key not in toggle_commands:
                    s.send(stop_command.encode())
                keys_to_remove.append(key)
                
        #remove released keys from the set
        for key in keys_to_remove:
            pressed_keys.remove(key)
>>>>>>> 32899b6b285c960574f86afbc8414517807bd53b

    def forward():
        cmd = name_to_command['forward']
        print(f"Sending command: {cmd}") 
        s.send(cmd.encode())

    def left():
        cmd = name_to_command['left']
        print(f"Sending command: {cmd}") 
        s.send(cmd.encode())

    def right():
        cmd = name_to_command['right']
        print(f"Sending command: {cmd}") 
        s.send(cmd.encode())

    def left_turn():
        cmd = name_to_command['left_turn']
        print(f"Sending command: {cmd}") 
        s.send(cmd.encode())
        
    def right_turn():
        cmd = name_to_command['right_turn']
        print(f"Sending command: {cmd}") 
        s.send(cmd.encode())

    sequence(forward, left, right, left_turn, right_turn)

    s.close()

except Exception as e:
    print("Connection failed:", e)
