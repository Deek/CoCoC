#include <stdio.h>
#include <fcntl.h>

#include <time.h>
#include <sgstat.h>
#include <signal.h>
#include <password.h>

direct char	namebuf[PWNSIZ];
direct char	passbuf[PWPSIZ];

#define LOGIN_RETRIES 3
#define FAIL_DELAY 2

#define prompt(x) \
	writeln (STDOUT_FILENO, x, sizeof (x) - 1)

/*
	These arrays cause the program to use slightly more memory than using
	pointers to constant strings, but they allow us to use 'sizeof' to get
	their lengths instead of 'strlen', which means no math being done at
	run-time.
*/
char LogPrompt[] = "Login: ";
char PassPrompt[] = "Password: ";

char NoPass[] = "Login incorrect\n";
char Bye[] = "Goodbye.\n";

char NewLine[] = "\n";

char Sorry[] = "can't open password file.\n";
char Dir[] = "can't find home directory; will use current.\n";
char XDir[] = "can't find exec directory; will use current.\n";

direct struct sgbuf savedSet, newSet;
static direct saved = 0;

direct char *username = NULL;

direct char motdbuf[120];	/* Buffer for message of the day */

trylogin();
chaincmd();

trap(sig)
{
	restTerm();
	exit(sig);
}

setTerm()
{
	if (!saved && -1 != _gs_opt (STDIN_FILENO, &savedSet)) {
		memcpy (&newSet, &savedSet, sizeof (struct sgbuf));
		saved = 1;
	}

	newSet.sg_echo = 0;

	_ss_opt (STDIN_FILENO, &newSet);
}

restTerm()
{
	if (saved) _ss_opt (STDIN_FILENO, &savedSet);
}

getstr(buf, siz)
char	buf[];
int		siz;
{
	register char *i = buf;

	readln (STDIN_FILENO, buf, siz);

	while (*i && *i != '\n')
		i++;
	*i = '\0';
}

getuser()
{
	prompt (LogPrompt);
	getstr (namebuf, sizeof (namebuf));
	if (*namebuf)
		return namebuf;

	return NULL;
}

getpass()
{
	setTerm();
	prompt (PassPrompt);
	getstr (passbuf, sizeof (passbuf));
	prompt (NewLine);
	restTerm();

	return passbuf;
}

main (argc, argv)
int		argc;
char	**argv;
{
	register int	i;

	intercept (trap);
	asetuid (0);		// We're gonna need to be root a lot

	if (argc > 1) {		/* called with username, prefill it once */
		username = argv[1];
	} else {			/* No username, print banner */
		int	fd;

		if (-1 != (fd = open ("/DD/SYS/banner", O_RDONLY))) {
			while (readln (fd, motdbuf, sizeof (motdbuf)))
				writeln (STDOUT_FILENO, motdbuf, sizeof (motdbuf));

			close(fd);
		}
	}

	for (i = 0; i < LOGIN_RETRIES; i++) {
		trylogin (username);
		// if we even get here, it's a failure
		sleep (FAIL_DELAY);
		prompt (NoPass);
		username = NULL;
	}
	prompt (Bye);
	exit (1);
}

trylogin(name)
char	*name;
{
	char	*pass;
	PWENT	*entry;
	int		fd;

	if ((!name || !*name) && !(name = getuser()))
		return;

	if (!(pass = getpass()))
		return;

	if (entry = getpwnam (name)) {
		if (*(entry->upw) == '*' || strucmp (pass, entry->upw))
			goto done;

		/* matched */
		setpr (getpid (), atoi (entry->upri));	// set priority, user inherits

		setuid (atoi (entry->uid));	// set new uid

		if (-1 == chdir (entry->udat))
			prompt (Dir);

		if (-1 == chxdir(entry->ucmd))
			prompt (XDir);

		/* Print MOTD */
		if (-1 != (fd = open ("/DD/SYS/motd", O_RDONLY))) {
			while (readln (fd, motdbuf, sizeof (motdbuf)))
				writeln (STDOUT_FILENO, motdbuf, sizeof (motdbuf));
			close (fd);
		}

		chaincmd (entry->ujob);
		exit (1);
	}
done:;
	endpwent();
}


chaincmd (cmd)
char *cmd;
{
	char			buf[80];
	char			*args;
	register char	*tmp = buf;

	/* First, fill buf from cmd */
	while (*cmd)
		*tmp++ = *cmd++;	// poor man's strcpy
	*tmp = '\0';

	/* Now, terminate the "program' at the first space */
	tmp = buf;
	while (*++tmp != ' ') ;	// find the first space
	*tmp++ = '\0';			// turn it into a nul
	args = tmp;

	/*
	 * now args points at the start of the arguments, but it needs to end
	 * with a newline, not a nul...
	 */
	while (*++tmp) ;
	*tmp++ = '\n';
	*tmp = '\0';
	/*
	 * By happy accident, the difference between tmp and args should now be
	 * exactly the length of the parameters, so _again_ we don't need strlen.
	 */
	chain (buf, tmp - args, args, 1, 1, 0);
}
