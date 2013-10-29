/* sockun.c - UNIX domain socket example
 *
 * compile with
     ${CC} -DSERVER -o server sockun.c
     ${CC} -DCLIENT -o client sockun.c
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>

#define TRY(expr) \
	if((expr) < 0) do { \
		perror(#expr); \
		exit(EXIT_FAILURE); \
	} while(0)
#define toaddrptr(addr) ((struct sockaddr *)(addr))

#define SOCKNAME "/tmp/alive.sock"

#if defined SERVER
int
main()
{
	int lsock;
	int sock;
	struct sockaddr_un addr;
	socklen_t addrlen;
       
	memset(&addr, 0, sizeof(addr));
	addr.sun_family = AF_UNIX;
	strncpy(addr.sun_path, SOCKNAME, 108);

	(unlink(SOCKNAME));
	TRY(lsock = socket(AF_UNIX, SOCK_STREAM, 0));
	TRY(bind(lsock, toaddrptr(&addr), sizeof(addr)));
	TRY(listen(lsock, 8)); 

	TRY(sock = accept(lsock, toaddrptr(&addr), &addrlen));

	write(sock, "hello world", 11);

	close(sock);
	close(lsock);

	return 0;
}

#elif defined CLIENT
int
main()
{
	int sock;
	int ret;
	int buf[128];
	struct sockaddr_un addr;
	socklen_t addrlen;
       
	memset(&addr, 0, sizeof(addr));
	addr.sun_family = AF_UNIX;
	strncpy(addr.sun_path, SOCKNAME, 108);

	TRY(sock = socket(AF_UNIX, SOCK_STREAM, 0));
	TRY(connect(sock, toaddrptr(&addr), sizeof(addr)));

	for(;;) {
		ret = read(sock, buf, 128);
		if (ret == 0) break;

		write(STDOUT_FILENO, buf, ret);
	}

	close(sock);
}

#else
  #error Please choose which program to compile (SERVER/CLIENT)
#endif
