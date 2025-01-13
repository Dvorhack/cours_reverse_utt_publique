def gen_license(username):
    return sum(x ^ 3 for x in username)


username = b'admin'
license = gen_license(username)
print(hex(license))