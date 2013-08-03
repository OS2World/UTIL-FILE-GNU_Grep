

#ifndef _MISCLIB_H_
#define _MISCLIB_H_

/* argv[i][-1] contains some flag bits: */

#define _ARG_DQUOTE   0x01          /* Argument quoted (")                  */
#define _ARG_RESPONSE 0x02          /* Argument read from response file     */
#define _ARG_WILDCARD 0x04          /* Argument expanded from wildcard      */
#define _ARG_ENV      0x08          /* Argument from environment            */
#define _ARG_NONZERO  0x80          /* Always set, to avoid end of string   */


int  _filesys (const char *, char *, size_t);
char _fngetdrive (const char *);
void _fnlwr (char *);
void _fnlwr2 (char *, const char *);
int  getpagesize(void);
void _rfnlwr (void);
void _sfnlwr (const char *);
void _wildcard (int *, char ***);
void _response (int *, char ***);

#endif
