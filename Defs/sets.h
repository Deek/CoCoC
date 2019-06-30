#define SETMAX   255    /* ie 0..255 */
char  *allocset();
char  *addc2set(s, c);
char  *adds2set(s, p);
char  *rmfmset(s, c);
int   smember(s, c);
char  *sunion(s1, s2);
char  *sintersect(s1, s2);
char  *sdifference(s1, s2);
char  *copyset(s1, s2);
char  *dupset(s);
