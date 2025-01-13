#!/usr/bin/env python3
import os
os.environ['PWNLIB_NOTERM'] = 'True'

from pwnlib.tubes.process import process


def gen_license(username):
    return sum(x ^ 3 for x in username)

def check_license(username: str, license: str):
    io = process(['./chall', username, license], level='error')
    res = io.recvall()

    if res == b'Your license seems legit\n':
        return True
    else:
        return False

def test():
    username = b'admin'
    license = gen_license(username)
    print(hex(license))

    check_license(username.decode(), license)

def main():
    username = input('Username: ').strip()
    license = input('License: ').strip()

    if check_license(username, license):
        print(f"Bienvenue {username} :)")
        if username == 'admin':
            print(open('FLAG','r').read())
    else:
        print("Wrong !")
        print("You haven't reversed enough")

if __name__ == "__main__":
    main()