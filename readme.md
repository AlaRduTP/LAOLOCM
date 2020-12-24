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

```sh
make
```

#### clean

```sh
make clean
```

### Chad

```sh
make -C Agents/Chad/agent
```

#### clean

```sh
make -C Agents/Chad/agent clean
```
