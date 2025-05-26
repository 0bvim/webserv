# Webserv - HTTP Server in C++98

[![License](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)

A lightweight, non-blocking HTTP server written in C++98, compliant with HTTP 1.1 standards. Designed to handle static content, file uploads, CGI scripts, and more. Includes bonus features like session management and multiple CGI support.

---

## Table of Contents
- [Features](#features)
- [Installation](#installation)
- [Usage](#usage)
- [Configuration](#configuration)
- [Testing](#testing)
- [Bonus Features](#bonus-features)
- [Limitations](#limitations)
- [Contributing](#contributing)
- [License](#license)
- [Acknowledgments](#acknowledgments)
- [Resources](#resources)

---

## Features
- **HTTP/1.1 Compliance**: Supports GET, POST, DELETE methods, chunked transfers, and error handling.
- **Non-Blocking I/O Multiplexing**: Uses `epoll()` for efficient client-server communication.
- **Static Website Hosting**: Serve static files (HTML, CSS, JS, images) with directory listing options.
- **File Uploads**: Accept client file uploads via POST requests.
- **CGI Support**: Execute scripts (e.g., PHP, Python) with proper environment setup.
- **Configurable**: Inspired by NGINX syntax for defining ports, routes, error pages, and client limits.
- **Resilient**: No crashes allowed, even under memory constraints.
- **Bonus**: Session management, cookies, and multiple CGI handlers.

---

## Installation

### Dependencies
- C++98-compatible compiler (e.g., `g++, clang`)
- `make`
- Testing tools: `curl`, web browser, siege, or custom scripts (Python/Go).

### Build Instructions
1. Clone the repository:
   ```bash
   git clone https://github.com/yourusername/webserv.git
   cd webserv
   ```
2. Compile using the provided Makefile:
   ```bash
   make
   ```
   Output: `./webserv`

---

## Usage
Run the server with an optional configuration file:
```bash
./webserv [config_file.conf]
```
If no file is provided, the server uses a default configuration (see [`configurations/default.conf`](configurations/default.conf)).

---

## Configuration
Configure ports, routes, error pages, and more using an NGINX-style file. Example:
```nginx
http{
	client_max_body_size 10M;
	autoindex on;
	root ./var/www/html;
	server{
		listen 8090;
		server_name phpserver;
		location /
    	{
		    web_dav on;
		    limit_except GET POST {
			    deny all;
			}
			error_page 404 /404.html;
			fastcgi_pass /usr/bin/php-cgi;
			fastcgi_extension php;
		}

		location /deletable {
			web_dav on;
			limit_except POST DELETE {
		        deny all;
			}
		}

		location /coffee {
			return 418;
		}
	}
	server{
		listen 8092;
		server_name meusite;
		location / {
			root var/www/html;
		}
	}
```

Key options:
- `listen`: Port and host (e.g., `127.0.0.1:8080`).
- `client_max_body_size`: Max upload size.
- `limit_except`: Allowed HTTP methods per route.
- `fastcgi_pass`: Define cgi path.
- `fastcgi_extension`: Map file extensions to executables.

---

## Testing
1. **Basic Checks**:
   ```bash
   curl http://localhost:8080
   ```
2. **Stress Testing**:
   Use tools like Siege Bench:
   ```bash
   siege -b -t 60s http://localhost:8090/
   ```
3. **Browser Testing**: Open `http://localhost:8090` in a browser.

4. **Google Test Lib**:
   ```
   make test
   ```

---

## Bonus Features
- **Cookies & Sessions**: Track user sessions using cookies. Example config:
- **Multiple CGI Handlers**: Run Python, PHP, and other scripts simultaneously.

---

## Limitations
- No regex support in route paths.
- Limited to HTTP/1.1 features outlined in the RFC (no WebSocket or HTTP/2).

---

## License
Distributed under the MIT License. See [`LICENSE`](LICENSE) for details.

---

## Acknowledgments
- Inspired by NGINX configuration syntax.
- RFC 7230/7231 for HTTP standards.
- 42 School for the project guidelines.

---

## Resources
- [RFC STD](https://www.rfc-editor.org/standards)
- [MDN](https://developer.mozilla.org/en-US/docs/Web/HTTP)
- [Beej’s Guide to Network Programming](https://amzn.to/4mw0Vqv)
- [Computer Networks Tanembaum](https://amzn.to/3ZoGh1J)
- [TCP vs UDP](https://www.avast.com/c-tcp-vs-udp-difference)
- [Simple Http Server](https://github.com/shuveb/zerohttpd)
- [Jacob Sorber Video](https://www.youtube.com/watch?v=esXw4bdaZkc)
- [How the web works: HTTP and CGI explained](https://www.garshol.priv.no/download/text/http-tut.html)
- [MIME](https://developer.mozilla.org/en-US/docs/Web/HTTP/Basics_of_HTTP/MIME_types)
- [HTTP Status Codes](https://umbraco.com/knowledge-base/http-status-codes/)
- [How to Read an RFC](https://www.tutorialspoint.com/cplusplus/cpp_web_programming.htm)
- [RFC 9110 - HTTP Semantics ](https://www.rfc-editor.org/info/rfc9110)
- [RFC 9112 - HTTP/1.1 ](https://www.rfc-editor.org/info/rfc9112) 
- [RFC 2068 - ABNF](https://www.cs.columbia.edu/sip/syntax/rfc2068.html) 
- [RFC 3986 -  (URI) Generic Syntax](https://www.ietf.org/rfc/rfc3986)
- [RFC 6265 - HTTP State Management Mechanism (Cookies)](https://www.rfc-editor.org/rfc/rfc6265)
- [RFC 3875 - CGI](https://datatracker.ietf.org/doc/html/rfc3875)
- [Python web Programming](https://www.tutorialspoint.com/python/python_cgi_programming.htm)
- [CPP web Programming](https://www.tutorialspoint.com/cplusplus/cpp_web_programming.htm)
- [(Video) Creating a file upload page](https://www.youtube.com/watch?v=_j5spdsJdV8&t=562s)
- [URL Encoding](https://www.urlencoder.io/learn/#:~:text=A%20URL%20is%20composed%20from,%22%20%2C%20%22~%22%20)
- [Nginx](https://nginx.org/en/)
- [(Video) IO Multiplexing](https://www.youtube.com/watch?v=dEHZb9JsmOU)
- [IO Multiplex](https://www.softprayog.in/programming/io-multiplexing-select-poll-epoll-in-linux)
- [Tests using CMake](https://www.youtube.com/watch?v=pxJoVRfpRPE)
- [(Video) Async vs Multithreading](https://www.youtube.com/watch?v=IGv5WYYmyfo)
- [Nginx Source Code](https://github.com/nginx/nginx)

[Back to Top](#webserv---http-server-in-c98)
