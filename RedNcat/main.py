import sys
import subprocess
import shlex
import argparse
import socket
import threading

# Create parser
parser = argparse.ArgumentParser(
    prog="netcat",
    usage="Servers injection",
    description="Exploiting TCP protocol to inject Servers.",
    epilog="Made by redel-byte.",
    formatter_class=argparse.HelpFormatter,
    conflict_handler='error',
    add_help=True,
    allow_abbrev=True,
    exit_on_error=True
)

# Adding args
parser.add_argument("-l", "--listen", action="store_true", help="Listen mode, for inbound connects")
parser.add_argument("-c", "--command", action="store_true", help="Command shell mode")
parser.add_argument("-u", "--upload", action="store_true", help="Upload mode")
parser.add_argument("-e", "--execute", default="", help="Program to execute after connect")
parser.add_argument("-t", "--target", default="", help="Target IP address")
parser.add_argument("-d", "--upload_destination", default="./", help="Upload destination path")
parser.add_argument("-p", "--port", default=0, type=int, help="Port number")

setting = parser.parse_args()


def main():
    if not setting.listen and (len(setting.target) and setting.port) > 0:
        print("Ctrl+D to send it...\n")
        buffer = sys.stdin.read()
        client_sender(buffer)

    if setting.listen:
        server_loop()


def client_sender(buffer):
    client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    try:
        client.connect((setting.target, setting.port))

        if len(buffer):
            client.send(buffer.encode())

        while True:
            response = b""
            while True:
                data = client.recv(4096)
                if not data:
                    break
                response += data
                if len(data) <= 4096:
                    break
            print(response.decode(errors="ignore"))

            buffer = input("")
            buffer += "\n"
            client.send(buffer.encode())
    except Exception as e:
        print(f"[*] Exception: {e}, Exiting")
        client.close()


def server_loop():
    if not len(setting.target):
        setting.target = "0.0.0.0"
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.bind((setting.target, setting.port))
    server.listen(5)

    while True:
        client_socket, addr = server.accept()
        client_thread = threading.Thread(target=client_handler, args=(client_socket,))
        client_thread.start()


def run_command(command):
    try:
        output = subprocess.check_output(shlex.split(command), stderr=subprocess.STDOUT)
    except Exception:
        output = b"[!] Failed to execute command.\r\n"
    return output


def client_handler(client_socket):
    if setting.upload:
        file_buffer = b""
        while True:
            data = client_socket.recv(1024)
            if not data:
                break
            file_buffer += data
        try:
            with open(setting.upload_destination, "wb") as f:
                f.write(file_buffer)
            client_socket.send(f"[+] Successfully saved file to {setting.upload_destination}\r\n".encode())
        except:
            client_socket.send(f"[!] Failed to save file to {setting.upload_destination}\r\n".encode())

    if len(setting.execute):
        output = run_command(setting.execute)
        client_socket.send(output)

    if setting.command:
        while True:
            client_socket.send(b"$ ")
            cmd_buffer = b""
            while b"\n" not in cmd_buffer:
                cmd_buffer += client_socket.recv(1024)
            response = run_command(cmd_buffer.decode())
            client_socket.send(response)


if __name__ == "__main__":
    main()

