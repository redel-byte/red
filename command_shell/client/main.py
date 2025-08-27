import socket
import argparse
import sys


parser = argparse.ArgumentParser(prog="CSTC", usage="Remote Commands Shell", description="Remote Commands Shell",
epilog="Made by redel-byte", formatter_class=argparse.HelpFormatter,conflict_handler='error',add_help=True, allow_abbrev=True, exit_on_error=True)

parser.add_argument("-t","--target",action="store",type=str,required=True,help="IP of the Target Server.")
parser.add_argument("-p","--port",action="store",type=int,required=True,help="Port of Server.")

parametre = parser.parse_args()

client = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
client.connect((parametre.target,parametre.port))

def main():
    while True:
        sys.stdout.write("$ ")
        command = str(input())
        client.send(command.encode())
        output = client.recv(1024)
        print(output.decode())

if __name__ == "__main__":
    main()
