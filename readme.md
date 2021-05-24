# LAOLOCM

## Requirements

- Python3
- Cython
- JRE
- Nim
- Rust
	- nightly

### Cython

```sh
pip3 install Cython
```

### JRE

For LOCM referee.

```sh
sudo apt install default-jre
```

### Nim

```sh
wget https://nim-lang.org/download/nim-1.4.2-linux_x64.tar.xz
tar Jxvf nim-1.4.2-linux_x64.tar.xz
cd nim-1.4.2
sudo sh install.sh /usr/bin
```

### Rust

(For current baseline agent: Chad)

```sh
curl https://sh.rustup.rs -sSf | sh
source $HOME/.cargo/env
```

#### install nightly

```sh
rustup install nightly
```

## Compilation

### Server

```sh
bash mpi-server/build.sh
```

### Client

```sh
bash mpi-client/build.sh
```

#### clean

```sh
make clean
```

<!-- ### Chad

```sh
make -C Agents/Chad/agent
```

#### clean

```sh
make -C Agents/Chad/agent clean
``` -->

## Run

### Server

```sh
bash mpi-server/start.sh <number_of_islands> <number_of_migrations>
```

For the first time, first run the following commands:

#### Docker Swarm

##### Init

```sh
docker swarm init
docker network create -d overlay --attachable mpi-overlay
```

##### Get join-token

```sh
docker swarm join-token worker
```

### Client

Run the Server first, then the Client.  
You will get `<mpi-uri>` from the information when the Server starts.

```sh
bash mpi-server/start.sh <mpi-uri>
```

For the first time, first run the following command:

#### Docker Swarm

##### Join Swarm

```sh
docker swarm join --token <join-token>
```
