/* keydump.c - read stdin in raw mode and dump the input
 *             press q twice in a row to quit
 */

#include <termios.h>
#include <unistd.h>
#include <stdio.h>

int
main()
{
	struct termios bak;
	struct termios cfg;
	int ret;
	int q;
	int i;
	char buf[128];

	tcgetattr(STDIN_FILENO, &cfg);
	bak = cfg;
	cfmakeraw(&cfg);
	cfg.c_lflag &= ~ECHO;
	tcsetattr(STDIN_FILENO, TCSADRAIN, &cfg);

	q = 0;
	while((ret = read(STDIN_FILENO, buf, 128)) > 0) {
		if(ret == 1 && buf[0] == 'q') {
			if(++q == 2)
				break;
		} else {
			q = 0;
		}

		for(i = 0; i < ret; i ++) {
			printf("%X ", buf[i]);
		}
		printf("\n\r");
	}

	tcsetattr(STDIN_FILENO, TCSADRAIN, &bak);
	return 0;
}
