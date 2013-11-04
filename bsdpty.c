/* bsdpty.c - BSD tty utility function example
 * Actually, this only uses forkpty, but it calls both openpty and
 * login_tty internally.
 *
 */

#include <pty.h>
#include <stdio.h>
#include <sys/select.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>

struct termios cfgbak;

void
term_setraw() {
	struct termios cfg;

	if(tcgetattr(STDIN_FILENO, &cfg) == -1) {
		perror("tcgetattr");
		exit(EXIT_FAILURE);
	}

	cfgbak = cfg;
	cfmakeraw(&cfg);
	cfg.c_lflag &= ~ECHO;

	if(tcsetattr(STDIN_FILENO, TCSANOW, &cfg)) {
		perror("tcsetattr");
		exit(EXIT_FAILURE);
	}
}

int
main()
{
	struct winsize wsz;
	pid_t pid;
	int fd;
	int ret;
	char buf[128];
	fd_set fds;

	ioctl(STDIN_FILENO, TIOCGWINSZ, &wsz);

	pid = forkpty(&fd, NULL, NULL, &wsz);
	switch(pid) {
	case -1:
		perror("forkpty");
		return EXIT_FAILURE;
	case 0:
		execl("/bin/sh", "-i", NULL);
		perror("execl");
		return EXIT_FAILURE;
	default:
		break;
	}

	term_setraw();

	while(1) {
		FD_ZERO(&fds);
		FD_SET(STDIN_FILENO, &fds);
		FD_SET(fd, &fds);

		ret = select(fd+1, &fds, NULL, NULL, NULL);

		if(FD_ISSET(fd, &fds)) {
			ret = read(fd, buf, 128);
			if(ret <= 0)
				break;
			write(STDOUT_FILENO, buf, ret);
		}

		if(FD_ISSET(STDIN_FILENO, &fds)) {
			ret = read(STDIN_FILENO, buf, 128);
			if(ret <= 0)
				break;
			write(fd, buf, ret);
		}
	}

	tcsetattr(STDIN_FILENO, TCSANOW, &cfgbak);

	return EXIT_SUCCESS;
}

