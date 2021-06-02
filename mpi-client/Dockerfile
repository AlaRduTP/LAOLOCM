FROM laolocm/mpi

USER root

WORKDIR /build

RUN apt-get update

RUN apt-get install -y python3.8 python3-pip default-jre time && \
	pip3 install Cython jinja2 numpy scipy sortedcontainers && \
	wget https://nim-lang.org/download/nim-1.4.2-linux_x64.tar.xz && \
	tar Jxvf nim-1.4.2-linux_x64.tar.xz && \
	cd nim-1.4.2 && \
	sh install.sh /usr/bin

RUN rm -rf /var/lib/apt/lists/*

USER mpi

WORKDIR /home/mpi
