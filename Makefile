CFLAGS=$(shell cat compile_flags.txt)
IFLAGS=
LFLAGS=

all: epoll posix

epoll: epoll.cpp
	g++ $(CFLAGS) $(IFLAGS) epoll.cpp -o epoll

posix: posix.cpp
	g++ $(CFLAGS) $(IFLAGS) posix.cpp -o posix
