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
}

#Keep track of pressed keye to avoid flooding commands
pressed_keys = set()

print("Connecting to Arduino...")


try:
    s = socket.socket()
    s.settimeout(5)
    s.connect((ARUINO_IP, ARUINO_PORT))
    print("Connected!")

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
