FROM ubuntu:22.04

WORKDIR /app/

ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update --fix-missing                 
RUN apt-get install -qy --no-install-recommends  socat python3 libssl3
RUN apt-get clean  
RUN rm -rf /var/lib/apt/lists/  
RUN useradd -d /home/ctf -m ctf -s /bin/bash   
RUN chown ctf:ctf /usr/bin/socat   
RUN chmod 500 /usr/bin/socat

COPY src/flag1.txt ./
COPY src/flag2.txt ./
COPY dist/server ./

EXPOSE 1337
USER ctf

CMD "/app/server"