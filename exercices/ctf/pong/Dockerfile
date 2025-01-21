FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update --fix-missing                 
RUN apt-get install -qy --no-install-recommends  socat python3 libssl3 gcc-multilib
RUN apt-get clean  

RUN useradd -d /home/ctf -m ctf -s /bin/bash   
RUN chown ctf:ctf /usr/bin/socat   
RUN chmod 500 /usr/bin/socat

WORKDIR /home/ctf
COPY dist/pong.org pong
COPY src/flag.txt flag.txt
RUN chown root:root pong && chmod u+s pong
RUN chown root:root flag.txt && chmod 400 flag.txt

EXPOSE 4000
USER ctf

CMD "socat" "tcp-listen:4000,reuseaddr,fork" "exec:'/bin/bash',pty,stderr,setsid,sigint,sane"
