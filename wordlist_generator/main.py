from tqdm import tqdm
import random
import sys
import string
import os
import argparse

GROUPE = set()

parser = argparse.ArgumentParser(prog='wlg',epilog='made by red_el',description='Word list Generater.',add_help=True)
parser.add_argument('-l','--lenght',required=False,type=int,action='store',help='lenght of the word.',default=8)
parser.add_argument('-n','--number',required=False,type=int,action='store',help='The number of lines.',default=None)
parser.add_argument('-f','--file',required=False,default="word_list.txt",action='store',help='The file name.')
parser.add_argument('-p','--pattern',required=False,default=None,action='store',help='Complete the word with Pattern ***.')
parser.add_argument('-c','--characters',required=False,default=string.printable,action='store',help='Use specific characters.')

var = parser.parse_args()

lenght = var.lenght
number = var.number
file = var.file
pattern = var.pattern
characters = var.characters

class Word_list:
    def __init__(self,lenght,file,pattern,characters):
        self.lenght = lenght
        self.file = file
        self.pattern = pattern 
        self.characters = characters
    def proba(self):
        return len(self.characters)**self.lenght
    def create(self):
        if os.path.isfile(self.file):
            res = input(f"\nNote!! \n[{self.file}] already exists.\nDo you want to continue [Y/N]: ").upper()
            if res == 'Y':
                with open(self.file,'a') as f:
                    for word in GROUPE:
                        f.write(f'{word}\n')
                print(f"Check {self.file}")
            else:
                sys.exit(0)
        else:
            with open(self.file,'a') as f:
                for word in GROUPE:
                    f.write(f"{word}\n")
            print(f"Check {self.file}")
            
    def patt(self):
        word = ''
        for char in self.pattern:
            if char != '*':
                word += char
            else:
                word += random.choice(self.characters)
        if len(word) == len(self.pattern):
            GROUPE.add(word)
    def generate(self):
        word = ''
        for _ in range(len(self.characters)):
            word += random.choice(self.characters)
            if len(word) == self.lenght:
                GROUPE.add(word)
                word = ''

attribute = Word_list(lenght,file,pattern,characters)

def loop():
    total = number if number is not None else (
        len(characters)**pattern.count('*') if pattern else attribute.proba()
    )

    with tqdm(total=total, desc="جاري التوليد", unit="كلمة") as bar:
        current_len = 0
        if pattern:
            if number is not None and attribute.proba() >= number:
                while len(GROUPE) < number:
                    attribute.patt()
                    new_len = len(GROUPE)
                    bar.update(new_len - current_len)
                    current_len = new_len
            elif number is not None and attribute.proba() <= number:
                sys.exit("\nImpossible to generate that much of words.\nDon't use [-n] Option.")
            else:
                while len(GROUPE) < total:
                    attribute.patt()
                    new_len = len(GROUPE)
                    bar.update(new_len - current_len)
                    current_len = new_len
        else:
            if number is not None and number <= attribute.proba():
                while len(GROUPE) < number:
                    attribute.generate()
                    new_len = len(GROUPE)
                    bar.update(new_len - current_len)
                    current_len = new_len
            elif number is not None and number >= attribute.proba():
                sys.exit(f"Impossible to generate that much of words {number} > {attribute.proba()}.\nDon't use [-n] Option")
            else:
                while len(GROUPE) < attribute.proba():
                    attribute.generate()
                    new_len = len(GROUPE)
                    bar.update(new_len - current_len)
                    current_len = new_len

if __name__ == '__main__':
    loop()
    attribute.create()

