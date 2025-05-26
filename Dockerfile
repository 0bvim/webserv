FROM alpine:3.19

# Install build tools, CMake, Ninja, and inotify-tools for file watching 
RUN apk add --no-cache \
    build-base \
    cmake \
    ninja \
    inotify-tools \
    bash \
	g++ \
	make \
	php \
	php-cgi \
    valgrind \
	python3

WORKDIR /app

COPY . .

RUN chmod +x /app/scripts/entrypoint.sh

RUN make re

ENTRYPOINT ["/app/scripts/entrypoint.sh"]
