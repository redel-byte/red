from subprocess import run
import shlex


def monitormod(iface):
    arg = shlex.split(f"sudo airmon-ng start {iface}")
    result = run(arg ,text=True,capture_output=True)
    if result.returncode:
        return "[!]Monitor mode dosn't startn\nRun 'main.py --available_interfaces'  to see available interfaces"
    else:
        return "[+]Monitor mode is start.\n"

if __name__ == "__main__":
    iface = str(input("interface="))
    monitormod(iface)
