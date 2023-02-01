#include <cstdio>
#include <cstring>

#include <netdb.h>
#include <sys/socket.h>

int main () {
	auto hints = addrinfo{};
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	auto address = static_cast<addrinfo*>(nullptr);
	if (getaddrinfo(nullptr, "5000", &hints, &address)) return 1;
	if (address == nullptr) return 1;

	// listen()
	auto const lfd = socket(address->ai_family, address->ai_socktype, address->ai_protocol);
	if (lfd == -1) return 1;
	if (bind(lfd, address->ai_addr, address->ai_addrlen) == -1) return 1;
	freeaddrinfo(address);
	if (listen(lfd, SOMAXCONN) == -1) return 1;

	return 0;
}
