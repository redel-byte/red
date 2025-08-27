from subprocess import run
import threading
import socket 
import shlex
import argparse

parser = argparse.ArgumentParser(prog="CSTS", usage="Echo Command Server Via TCP.", description="Command Shell Server.", epilog="Made by redel-byte", formatter_class=argparse.HelpFormatter, conflict_handler='error', add_help=True, allow_abbrev=True, exit_on_error=True)
parser.add_argument("-t","--target",required=True,action="store",type=str,help="Target Band IP.",)
parser.add_argument("-p","--port",required=True,action="store",type=int,help="Target band port.")
device = parser.parse_args()

server = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
server.bind((device.target,device.port))
server.listen(5)
print(f"listening on {device.target}:{device.port}")

def handle_client_and_commands(client_socket):
    while True:
        request = client_socket.recv(1024)

        if not request:
            print("[!] Client disconnected.")
            client_socket.close()
            break  

        try:
            args = shlex.split(request.decode())
            result = run(
                args,
                capture_output=True,
                text=True
            )

            output = result.stdout + result.stderr
            if not output:
                output = "[+] Command executed but no output.\n"

        except Exception as e:
            output = f"[!] Error: {e}\n"

        client_socket.send(output.encode())

if __name__ == "__main__":
    while True:
        client,addr = server.accept()
        print(f"[+] Accepting connection from {addr[0]}:{addr[1]}")
        client_handle = threading.Thread(target=handle_client_and_commands,args=(client,))
        client_handle.start()
