CFLAGS=$(shell cat compile_flags.txt)
IFLAGS=
LFLAGS=

all: epoll posix

epoll: epoll.cpp
	clang++ $(CFLAGS) $(IFLAGS) epoll.cpp -o epoll

posix: posix.cpp
	clang++ $(CFLAGS) $(IFLAGS) posix.cpp -o posix
