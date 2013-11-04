/* pselect.c - pselect example
 *
 * 
 *
 */

#include <sys/select.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

#define LEN(arr) (sizeof(arr) / sizeof(arr[0]))

struct sigslot
{
	char *name;
	bool terminate;
};

bool sigflags[128];


struct sigslot sigslot[] = {
	{ "SIGHUP"  , false },
	{ "SIGINT"  , false },
	{ "SIGQUIT" , false },
	{ "SIGILL"  , false },
	{ "SIGTRAP" , false },
	{ "SIGABRT" , false }, // "SIGIOT"
	{ "SIGBUS"  , false },
	{ "SIGFPE"  , false },
	{ "SIGKILL" , true  }, // cannot catch this
	{ "SIGUSR1" , false },
	{ "SIGSEGV" , false },
	{ "SIGUSR2" , false },
	{ "SIGPIPE" , false },
	{ "SIGALRM" , false },
	{ "SIGTERM" , false },
	{ "SIGSTKFL", false }, 
	{ "SIGCHLD" , false },
	{ "SIGCONT" , false },
	{ "SIGSTOP" , false }, // cannot catch this
	{ "SIGTSTP" , false },
	{ "SIGTTIN" , false },
	{ "SIGTTOU" , false },
	{ "SIGURG"  , false },
	{ "SIGXCPU" , false },
	{ "SIGXFSZ" , false },
	{ "SIGVTALR", false },
	{ "SIGPROF" , false },
	{ "SIGWINCH", false },
	{ "SIGIO"   , false }, // "SIGPOLL", "SIGLOST"
	{ "SIGPWR"  , false },
	{ "SIGSYS"  , false }, // "SIGUNUSED"
	{ "SIGRTMIN", false }
};


void
handle_signal(int sig)
{
	/* signal number starts from 1 */
	sigflags[sig-1] = true;
}

int
main()
{
	struct sigaction sa;
	sigset_t allsigs;
	sigset_t emptysigs;
	fd_set fds;
	char buf[64];
	int ret;
	int i;

	sigfillset(&allsigs);
	sigemptyset(&emptysigs);

	/* Block all signals, so that only pselect gets signals */
	sigprocmask(SIG_BLOCK, &allsigs, NULL);

	/* Set signal handlers. Here, our handler is a flag setter */
	sa.sa_handler = handle_signal;
	sa.sa_mask = allsigs;
	for(i = 1; i <= LEN(sigslot); i++)
		sigaction(i, &sa, NULL);

	while(1)
	{
		fprintf(stderr, "=== LOOP ===\n");

		FD_ZERO(&fds);
		FD_SET(0, &fds);

		/* What happens here:
		 * "pselect" sets the set of blocked signals to the
		 * given signal set.
		 * (In our case, "pselect" unblocks all signals.)
		 *
		 * After unblocking signals, pselect calls "select".
		 * "select" returns when FDs become available or
		 * signal is received.
		 *
		 * Before "pselect" returns, signal masks are restored
		 * to the old value.
		 * (Here, "pselect" re-blocks all signals)
		 *
		 */
		ret = pselect(1, &fds, NULL, NULL, NULL, &emptysigs);

		if(ret == -1 && errno == EINTR)
		{
			for(i = 0; i < LEN(sigslot); i ++)
			{
				if(!sigflags[i])
					continue;

				fprintf(stderr, "got %s\n",
						sigslot[i].name);
				sigflags[i] = false;
				if(sigslot[i].terminate)
					goto END;
			}
			continue;
		}
		else if (ret == -1)
		{
			goto FAILED;
		}
		else
		{
			ret = read(0, buf, 64);
			if(ret == 0)
			{
				fprintf(stderr, "got EOF, exiting...\n");
				goto END;
			}

			write(1, buf, ret); 
		}
	}

END:
	return EXIT_SUCCESS;

FAILED:
	perror("pselect");
	return EXIT_FAILURE;
}
