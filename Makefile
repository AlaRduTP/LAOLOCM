target = main
cybridge = caller

CC = g++
CCFLAGS = -std=c++17 -fPIC -O3

PYVERSION := $(shell python3 -c "import sys; print(sys.version_info.minor)")

CFLAGS = `python3-config --cflags`

ifeq ($(shell test $(PYVERSION) -ge 8; echo $$?), 0)
	LDFLAGS = `python3-config --ldflags --embed`
else
	LDFLAGS = `python3-config --ldflags`
endif

MAKE = make

all:
	cython $(cybridge).pyx -o $(cybridge).cpp
	$(CC) $(CCFLAGS) -c *.cpp $(CFLAGS)
	$(CC) *.o $(LDFLAGS) -o $(target)
	@$(MAKE) -C referee

clean:
	rm -f $(cybridge).cpp $(cybridge).h *.o $(target)
	rm -rf __pycache__
	@$(MAKE) -C referee clean