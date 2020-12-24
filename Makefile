target = main
cybridge = caller

CC = g++
CCFLAGS = -std=c++17 -fPIC

CFLAGS = `python3-config --cflags`
LDFLAGS = `python3-config --ldflags`

MAKE = make

all:
	cython $(cybridge).pyx -o $(cybridge).cpp
	$(CC) $(CCFLAGS) -c *.cpp $(CFLAGS)
	$(CC) *.o $(LDFLAGS) -o $(target)
	$(MAKE) -C referee

clean:
	rm -f $(cybridge).cpp $(cybridge).h *.o $(target)
	rm -rf __pycache__
	$(MAKE) -C referee clean