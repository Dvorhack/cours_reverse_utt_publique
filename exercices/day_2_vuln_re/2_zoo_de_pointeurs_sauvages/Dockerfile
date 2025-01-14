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
COPY src/flag4.txt ./
COPY dist/2_chal ./
RUN chmod +x 2_chal

EXPOSE 4000
USER ctf

CMD "socat" "tcp-listen:4000,reuseaddr,fork" "exec:'/app/2_chal'"