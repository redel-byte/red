import socket

target_ip = str(input())
target_port = int(input())

client = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
client.sendto(b"Hello Server!",(target_ip,target_port))


data,addr = client.recvfrom(1024)


print(data)

    
