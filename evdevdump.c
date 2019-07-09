/* evdevdump.c - uinput event dumper based on libevdev 
 *
 * Compile with:
 * 	gcc $(pkg-config --cflags --libs libevdev) evdevdump.c
 */
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <libevdev/libevdev.h>

int fd;
struct libevdev *dev;
struct input_event ev;
int qcnt;

int
main(int argc, char *argv[])
{
	int rc;

	if ((fd = open(argv[1], O_RDONLY)) < 0) {
		perror("open");
		return 1;
	}

	if ((rc = libevdev_new_from_fd(fd, &dev)) != 0) {
		fprintf(stderr, "libevdev_new_from_fd: rc=%d\n", rc);
		return 1;
	}

	while(1) {
		rc = libevdev_next_event(dev, LIBEVDEV_READ_FLAG_NORMAL, &ev);
		if (rc == LIBEVDEV_READ_STATUS_SYNC)
			printf("ignoring sync...");
		else if (rc == -EAGAIN)
			break;
		else
			printf("type=0x%x\tcode=0x%x\tvalue=0x%x\n", ev.type, ev.code, ev.value);

		if (ev.type == EV_KEY) {
			if (ev.code == KEY_Q) {
				if (ev.value == 1) {
					if (++qcnt >= 2)
						break;
				}
			} else
				qcnt = 0;
		}
	}

	return 0;
}
