
#include <security/pam_appl.h>
#include <security/pam_misc.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>


static 
int
tty_conv(
	int num_msg,
	const struct pam_message **msg,
	struct pam_response **out_resp,
	void *appdata_ptr)
{
	struct pam_response *resp;
	struct termios ti, tibak;
	char input[PAM_MAX_MSG_SIZE];
	int len;
	int i;

	if(!isatty(STDIN_FILENO)) {
		fprintf(stderr, "conv error: not in a tty\n");
		return PAM_CONV_ERR;
	}

	if((resp = calloc(num_msg, sizeof(struct pam_response))) == NULL)
		return PAM_CONV_ERR;

	for(i = 0; i < num_msg; i ++) {
		/* unused field, set to zero */
		resp[i].resp_retcode = PAM_SUCCESS;

		switch(msg[i]->msg_style) {
		case PAM_PROMPT_ECHO_OFF:
			/* disable echo */
			if(tcgetattr(STDIN_FILENO, &ti) == -1)
				goto fail;
			tibak = ti;
			ti.c_lflag &= ~ECHO;
			if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &ti) == -1)
				goto fail;
			/* fall through */
		case PAM_PROMPT_ECHO_ON:
			fprintf(stdout, "%s", msg[i]->msg);
			fflush(stdout);

			if(fgets(input, sizeof(input), stdin) == NULL)
				input[0] = '\0';

			len = strlen(input);
			if(input[len-1] == '\n')
				input[len-1] = '\0';

			resp[i].resp = strdup(input);

			if(msg[i]->msg_style == PAM_PROMPT_ECHO_OFF)
				tcsetattr(STDIN_FILENO, TCSAFLUSH, &tibak); /* ignore error */

			fprintf(stdout, "\n");
			break;
		case PAM_ERROR_MSG:
			fprintf(stderr, "%s\n", msg[i]->msg);
			break;
		case PAM_TEXT_INFO:
			fprintf(stdout, "%s\n", msg[i]->msg);
			break;
		default:
			goto fail;
		}
	}

	*out_resp = resp;
	return PAM_SUCCESS;

fail:
	while(--i > 0)
		free(resp[i].resp);
	free(resp);
	return PAM_CONV_ERR;
}

int
main(int argc, char *argv[])
{
	struct pam_conv conv = {
		tty_conv,
		NULL
	};

    const char *user;
	const char *service;
    pam_handle_t *pamh = NULL;
	int ret;

    if(argc != 3) {
        fprintf(stderr, "Usage: check_user USERNAME SERVICE\n");
        exit(1);
    }

	user = argv[1];
	service = argv[2];

	printf("user   : %s\n", user);
	printf("service: %s\n", service);

	if((ret = pam_start(service, user, &conv, &pamh)) != PAM_SUCCESS)
		goto fail_end;

	if((ret = pam_authenticate(pamh, 0)) != PAM_SUCCESS)
		goto fail_end;

	fprintf(stderr, "Authenticated!\n");

	pam_end(pamh, ret);

	return 0;

fail_end:
	fprintf(stderr, "%s\n", pam_strerror(pamh, ret));
	pam_end(pamh, ret);
	return -1;
}

