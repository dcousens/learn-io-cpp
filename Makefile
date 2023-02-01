CFLAGS=$(shell cat compile_flags.txt)
IFLAGS=
LFLAGS=

epoll: epoll.cpp
	g++ $(CFLAGS) $(IFLAGS) epoll.cpp -o epoll
