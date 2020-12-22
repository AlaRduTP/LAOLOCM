target = main
cybridge = caller

CC = g++
CFLAGS = -std=c++17 `python3-config --cflags`
LDFLAGS = `python3-config --ldflags`

all:
	cython $(cybridge).pyx -o $(cybridge).cpp
	$(CC) $(CFLAGS) -c *.cpp
	$(CC) $(LDFLAGS) *.o -o $(target)

clean:
	rm -f $(cybridge).{cpp,h} *.o $(target)
	rm -rf __pycache__