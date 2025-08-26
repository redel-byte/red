import socket
import sys


ip_target = str(input())
port_target = int(input())

client = socket.socket(socket.AF_INET,socket.SOCK_STREAM)

client.connect((ip_target,port_target))

def handel_messages():
    while True:
        sys.stdout.write("$ ")
        message = str(input())
        client.send(message.encode())
        output = client.recv(1024)
        print(output.decode())
