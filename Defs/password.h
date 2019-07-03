/*
**  Password.h
**
**  Definitions for accessing the OS9 password file.
**  Two different delimiters are accepted, ',' and ':'.
**  In the second (Unix like) case, an extra field is
**  defined for comments (ugcos).
*/

#ifdef TEST
#define PASSWORD "/dd/sys/massword"
#else
#define PASSWORD "/dd/sys/password"
#endif

#define  PWEMAX   64       /* mamimum lines in password file */
#define  PWSIZ    132  /* maximum size of password file line */
#define  PWNSIZ   32          /* maximum size of user's name */
#define  PWPSIZ   32      /* maximum size of user's password */
#define  UNXDLM   ':'      /* Unix style password file delim */
#define  OS9DLM   ','       /* OS9 style password file delim */

typedef struct {
      char  *unam,
            *upw,
            *uid,
            *upri,
            *ugcos,
            *ucmd,
            *udat,
            *ujob;                      /* field pointers */
} PWENT;

/* returns a pointer to broken down password entry */
PWENT    *getpwent();

/* same, but for the given int uid */
PWENT    *getpwuid();

/* same, but for the given char *name */
PWENT    *getpwnam();

/* rewinds the password file for another scan */
/*void*/ setpwent();

/* terminates password file access (closes) file */
/*void*/ endpwent();

/* returns the current password entry delimiter */
/*void*/ getpwdlm();
