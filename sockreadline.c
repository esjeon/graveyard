/* sockreadline.c - reading lines from socket
 *
 */
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct reader {
	int sock;
	char *buf;
	size_t bufsize;
	size_t buflen;
};

struct reader*
reader_init(int sock, size_t bufsize)
{
	struct reader *reader;

	reader = (struct reader*)malloc(sizeof(struct reader));
	reader->sock = sock;
	reader->buf = (char *)malloc(bufsize);
	reader->bufsize = bufsize;
	reader->buflen = 0;

	return reader;
}

/* TODO: better error code? */
int
reader_readline(struct reader *reader, char *buf, size_t bufsize)
{
	int cnt;
	int i;

	for(;;) {
		for (i = 0; i < reader->buflen && reader->buf[i] != '\n'; i++);
		if (reader->buf[i] == '\n')
			break;

		cnt = recv(reader->sock, reader->buf + reader->buflen,
			reader->bufsize - reader->buflen, MSG_DONTWAIT);
		if (cnt == 0) {
			return 0; /* EOF */
		} else if (cnt == -1) {
			if (errno == EAGAIN || errno == EWOULDBLOCK)
				return -2; /* no data available yet */
			return -1;
		}

		reader->buflen += cnt;
	}

	if (++i >= bufsize)
		return -3; /* buffer too small */
	strncpy(buf, reader->buf, i);
	buf[i] = '\0';

	reader->buflen -= i;
	memmove(reader->buf, &reader->buf[i], reader->buflen);
	return i;
}

int
main()
{
	int lsock;
	struct sockaddr_un addr;
	int conn;

	lsock = socket(AF_UNIX, SOCK_STREAM, 0);

	addr.sun_family = AF_UNIX;
	strncpy(addr.sun_path, "test.sock", sizeof(addr.sun_path) - 1);
	if (bind(lsock, (struct sockaddr *)&addr, sizeof(struct sockaddr_un)) == -1) {
		perror("bind");
		return 1;
	}

	if (listen(lsock, 0) == -1) {
		perror("listen");
		return 1;
	}

	conn = accept(lsock, NULL, NULL);

	struct reader *reader = reader_init(conn, 1024);
	char buf[8];
	int rc;
	int lineno = 0;
	fd_set rfds;

	for(;;) {
		FD_ZERO(&rfds);
		FD_SET(conn, &rfds);

		rc = select(conn + 1, &rfds, NULL, NULL, NULL);
		if (rc == -1) {
			perror("select");
			break;
		}

		for(;;) {
			rc = reader_readline(reader, buf, sizeof(buf));
			if (rc == -1 || rc == -3) {
				fprintf(stderr, "readline status=%d\n", rc);
				goto stop;
			} else if (rc == 0) {
				fprintf(stderr, "client disconnected\n");
				goto stop;
			} else if (rc == -2) {
				break;
			}

			fprintf(stderr, "%04d (rc=%2d): %s", ++lineno, rc, buf);
		}
	}

stop:
	close(conn);
	close(lsock);
	unlink("test.sock");
}

