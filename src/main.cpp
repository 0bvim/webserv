#include <cstring>
#include <cstdlib>
#include <errno.h>
#include <fcntl.h>
#include <iostream>
#include <poll.h> // to use poll functions and macros
#include <unistd.h>

int main() {
  // Open files using open function
  int fd1 = open("file1.txt", O_RDONLY | O_NONBLOCK | O_CREAT, 0666);
  int fd2 = open("file2.txt", O_RDONLY | O_NONBLOCK | O_CREAT, 0666);

  if (fd1 == -1 || fd2 == -1) {
    std::cerr << "Error opening file(s): " << strerror(errno) << std::endl;
    return 1;
  }

  // Initialize pollfd structures
  struct pollfd fds[2];
  fds[0].fd = fd1;
  fds[0].events = POLLIN; // Monitor fd1 for input
  fds[1].fd = fd2;
  fds[1].events = POLLIN; // Monitor fd2 for input

  int timeout = 5000; // Timeout in milliseconds (5 seconds)

  // Call poll
  int ret = poll(fds, 2, timeout);

  if (ret == -1) {
    std::cerr << "poll error: " << strerror(errno) << std::endl;
    return 1;
  } else if (ret == 0) {
    std::cout << "Timeout occurred! No data available." << std::endl;
  } else {
    for (int i = 0; i < 2; ++i) {
      if (fds[i].revents & POLLIN) {
        std::cout << "File descriptor " << fds[i].fd << " is ready to read."
                  << std::endl;
        // Read data or perform necessary operations
      }
    }
  }

  // Close file descriptors
  close(fd1);
  close(fd2);

	system("rm file1.txt");
	system("rm file2.txt");

  return 0;
}
