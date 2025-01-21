from pwn import *
from random import randint
from Crypto.Cipher import AES
from Crypto.Util.Padding import unpad, pad

io = remote('ssp.serviel.fr', 1234, ssl=True)

cipher = None

def init_connexion():
    global cipher
    io.recvuntil(b'P = ')
    P = int(io.recvline())

    io.recvuntil(b'G = ')
    G = int(io.recvline())

    io.recvuntil(b'pubKey = ')
    A = int(io.recvline(), 16)

    # Create the private key
    b = randint(1, P-1)

    # Compute the public key
    B = pow(G, b, P)

    # Compute the shared key
    kb = pow(A, b, P)

    io.sendlineafter(b'publique ? ', str(B).encode())

    log.info(f"{P = } {G = } {b = } {B = } {kb = }")

    io.recvuntil(b'maintenant\n')
    key = f"{kb:x}".encode().ljust(16, b'\0')
    cipher = AES.new(key, AES.MODE_ECB)

def recv_and_decrypt():
    ciphered = io.recv()
    print(unpad(cipher.decrypt(ciphered), 16))

def encrypt_and_send(m):
    # m += b'\0' * (16-(len(m)%16))
    io.send(cipher.encrypt(pad(m, 16)))


init_connexion()
recv_and_decrypt()
encrypt_and_send(b'Get FLAG\0')


ciphered = io.recv()
print(unpad(cipher.decrypt(ciphered), 16))



io.interactive()