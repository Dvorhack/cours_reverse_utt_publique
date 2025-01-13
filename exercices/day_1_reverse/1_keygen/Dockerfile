FROM pwntools/pwntools:stable@sha256:5464f92b5deb32872282d21788fb9d5dc4cde3966f95c32ae0b6a012bd09ecb1

USER root

WORKDIR /home/pwntools/
COPY src/FLAG FLAG
COPY dist/1_chall chall
COPY src/checker.py checker.py
COPY src/ynetd /bin
RUN chmod +x /bin/ynetd checker.py chall

CMD ["/bin/ynetd", "-p", "1337", "-u", "pwntools", "-d", "/home/pwntools/", "-lt", "20", "./checker.py"]
EXPOSE 1337