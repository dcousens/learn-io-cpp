#include <array>
#include <cstdio>
#include <cstring>

#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>

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

	// accept() loop
	while (1) {
		fprintf(stderr, "loop\n");
		auto const cfd = accept(lfd, nullptr, nullptr);
		if (cfd == -1) continue;

		fprintf(stderr, "accept %i\n", cfd);

		// echo loop
		while (1) {
			auto buffer = std::array<char, 65556>{};
			auto const count = read(cfd, buffer.begin(), buffer.size());
			if (count < 0) continue; // error
			if (count == 0) break; // closed
			if (static_cast<size_t>(count) >= buffer.size()) return 1; // uh oh
			fprintf(stderr, "  recv %i\n", cfd);

			if (write(cfd, buffer.data(), static_cast<size_t>(count)) == -1) continue;
			fprintf(stderr, "  send %i\n", cfd);
		}

		if (close(cfd) != 0) return 1;
		fprintf(stderr, "close %i\n", cfd);
	}

	return 0;
}
