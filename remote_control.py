import socket
from datetime import datetime, timedelta

try:
    s = socket.socket()
    s.settimeout(5)
    s.connect(("192.168.4.1", 8080))
    print("Connected!")

    s.send(b"PC Connected\n")
    data = s.recv(1024)
    print("Received:", data)

    while True:
        msg = input("Send to Arduino: ")
        s.send(msg.encode())
        # delay = timedelta(seconds=0.5)
        # print("Reply:", s.recv(1024))


except Exception as e:
    print("Connection failed:", e)
