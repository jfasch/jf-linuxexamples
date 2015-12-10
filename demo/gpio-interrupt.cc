#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/select.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include <iostream>

using std::cout;
using std::cerr;
using std::endl;

int main()
{
    int gpio_fd1 = open("/sys/class/gpio/gpio23/value", O_RDONLY);
    if (gpio_fd1 == -1) {
        perror("open 1");
        exit(1);
    }
    int gpio_fd2 = open("/sys/class/gpio/gpio24/value", O_RDONLY);
    if (gpio_fd2 == -1) {
        perror("open 2");
        exit(1);
    }

    fd_set fds;

    int max = (gpio_fd1 < gpio_fd2)? gpio_fd2: gpio_fd1;

    while (true) {
      FD_ZERO(&fds);
      FD_SET(gpio_fd1, &fds);
      FD_SET(gpio_fd2, &fds);

      int rv = select(max+1, NULL, NULL, &fds, NULL);

      if (rv == 0) {
	cerr << "select returned 0 though no timeout specified?" << endl;
	exit(1);
      }
      if (rv == -1) {
	perror("select");
	exit(1);
      }

      bool active1 = FD_ISSET(gpio_fd1, &fds);
      bool active2 = FD_ISSET(gpio_fd2, &fds);
	
      if (! (active1 || active2)) {
	cerr << "select returned though nothing happened?" << endl;
	continue;
      }

      if (active1)
	cout << "READY: 1" << endl;
      if (active2)
	cout << "READY: 2" << endl;

      off_t seek_rv = lseek(gpio_fd1, 0, SEEK_SET);
      if (seek_rv == -1) {
	perror("lseek 1");
	exit(1);
      }
      seek_rv = lseek(gpio_fd2, 0, SEEK_SET);
      if (seek_rv == -1) {
	perror("lseek 2");
	exit(1);
      }

      {
	char buf[10];
	memset(buf, 0, sizeof(buf));
	ssize_t nread = read(gpio_fd1, buf, sizeof(buf));

	if (nread == -1) {
	  perror("read 1");
	  exit(1);
	}
	if (nread == 0) {
	  cerr << "read returns EOF?" << endl;
	  exit(1);
	}
      }
      {
	char buf[10];
	memset(buf, 0, sizeof(buf));
	ssize_t nread = read(gpio_fd2, buf, sizeof(buf));

	if (nread == -1) {
	  perror("read 2");
	  exit(1);
	}
	if (nread == 0) {
	  cerr << "read returns EOF?" << endl;
	  exit(1);
	}
      }
    }    
}
