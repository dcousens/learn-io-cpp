#include <array>
#include <cstdio>
#include <cstring>

#include <netdb.h>
#include <sys/epoll.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <unistd.h>

auto epctl (int const ep, int const fd, int const operation) {
	auto epev = epoll_event{};
	epev.data.fd = fd;
	epev.events = POLLIN;
	return epoll_ctl(ep, operation, fd, &epev);
}

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

	// epoll init
	auto ep = epoll_create1(0);
	if (epctl(ep, lfd, EPOLL_CTL_ADD)) return 1;

	// epoll loop
	while (1) {
		fprintf(stderr, "loop\n");
		auto events = std::array<epoll_event, 224>{};
		auto const eventsCount = epoll_wait(ep, events.data(), events.size(), -1);
		if (eventsCount == -1) return 1;

		for (auto i = 0; i < eventsCount; ++i) {
			auto const& event = events.at(static_cast<size_t>(i));
			auto const efd = event.data.fd;

			// accept() ?
			if (efd == lfd) {
				auto const cfd = accept(efd, nullptr, nullptr);
				if (cfd == -1) continue;
				if (epctl(ep, cfd, EPOLL_CTL_ADD)) return 1;

				fprintf(stderr, "  accept fd:%i\n", cfd);
				continue;
			}
		}
	}

	return 0;
}
