from pwn import *


io = process('dist/reverse_2')


flag = b'ptrace_lov'
# flag = b'wrong_flag'

passwd = flag

io.sendlineafter(b'a password: ', passwd)
print(io.recvall())