import re
import shlex
import subprocess
from termcolor import colored

args = shlex.split("nmcli d wifi l")


def output():
    result  = subprocess.run(args,capture_output=True,text=True).stdout
    lines = re.findall(r"(\w{2}:\w{2}:\w{2}:\w{2}:\w{2}:\w{2})",result)
    print(lines)

def the():
    result = subprocess.run(args,capture_output=True,text=True).stdout
    elements = result.splitlines()
    print(" "*4 + elements[0])
    for element in range(1,len(elements) ): 
        print(f"[{colored(element,"red")}] {elements[element]}\n") 
the()
