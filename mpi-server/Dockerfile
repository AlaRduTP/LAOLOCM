FROM laolocm/mpi

RUN mkdir ~/server

WORKDIR /home/mpi/server

COPY ./src .

RUN mpicc -std=c99 -O2 server.c -o server
