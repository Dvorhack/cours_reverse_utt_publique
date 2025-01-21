import pure_salsa20
from hashlib import md5

with open('tcp_content.raw','rb') as f:
    nonce = f.read(24)
    key = f.read(32)
    ciphertext = f.read()

plaintext = pure_salsa20.xsalsa20_xor(key, nonce, ciphertext)

with open("out.jpeg", 'wb') as f:
    f.write(plaintext)