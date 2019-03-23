FROM i386/ubuntu:trusty
LABEL maintainer="DavidLee <dwlee@pku.edu.cn>" \
      date="2019-03-07" 
# ============ Start Installation  ============
RUN apt update && apt install -y \
    ed \
    csh \
    build-essential \
    gdb \
    && rm -rf /var/lib/apt/lists/*
# Copy local files into image
COPY nachos /nachos
# Initial compile
RUN cd /nachos/nachos-3.4/code && make
# ============ End Installation  ============