import socket
import sys
import threading
import argparse
import time

parser = argparse.ArgumentParser(prog="TCP_Proxy",
usage="main.py -t 127.0.0.1 -p 1234 -r google.com -a 80 -w file.txt -b ",
description="Advance proxy tool.",
epilog="Made by redel-byte.",
formatter_class=argparse.HelpFormatter,
conflict_handler='error',
add_help=True,
allow_abbrev=True, 
exit_on_error=True)

parser.add_argument("-l","--local_addr",action="store",required=False,default="0.0.0.0",type=str,help="Adress IP that will be bind by the Proxy_Server.")
parser.add_argument("-p","--local_port",action="store",required=False,type=int,help="Server listening Port.")
parser.add_argument("-r","--remote_addr",action="store",required=True,default="",type=str,help="Adress IP of The remote host")
parser.add_argument("-e","--remote_port",action="store",required=True,default=0,type=int,help="Remote connection Port")
parser.add_argument("-f","--file",action="store",required=False,default="irzm_kra.log",type=str,help="Save default to [file.log]")
parser.add_argument("-u","--bloked_URLs.",action="store",required=False,default="",type=str,help="File that containing the bloked URLs")

args = parser.parse_args()

local_addr = args.local_addr
local_port = args.local_port
remote_addr = args.remote_addr
remote_port = args.remote_port
file = args.file
URL = args.bloked_URLs


class Tcpproxy:
    def __init__(self,local_addr,local_port,remote_addr,remote_port,file,URL):
        self.local_addr = local_addr
        self.local_port = local_port
        self.remote_addr = remote_addr
        self.remote_port = remote_port
        self.file = file
        self.URL = URL
        self.time_out = 5
    def local_server(self):
        server = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
        server.bind((self.local_addr,self.local_port))
        server.listen(5)
        while True:
            client_socket,addr=server.accept() 
            threading.Thread(target=remote_server_handeler,args=(client_socket,))
        return client_socket
    def remote_client(self):
        client = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
        try:
            client.connect((self.remote_addr,self.remote_port))
            client.send(remote_server_handeler(self).encode())

        except Exception as e:
            print(f"[!] Error : {e}.")
    def remote_server_handeler(self):
            while True:
                requiset = local_server(self).recv(4096)
                if not requiset:
                    client_socket.close()
                    break
            return requiset
        w
def main():
    pass


if __name__ == "__main__":
    pass

