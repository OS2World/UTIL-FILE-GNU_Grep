/*
 *  wildcard.c
 *
 *                    MiscLib Open Watcom Project
 *
 * This file pulled from netlabs svn - klibc by Knut St. Osmundsen
 *
 * Modified December 2006 for Open Watcom use by
 * Michael Greene <os2@mgreene.org>
 *
 * Original:
 * wildcard.c (emx+gcc) -- Copyright (c) 1990-1996 by Eberhard Mattes
 * =========================================================================
 */

//#include "libc-alias.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <emx/startup.h>
//#include <emx/syscalls.h>
#include <io.h>
#include "misclib.h"

#define WPUT(x) do { \
    if (new_argc >= new_alloc) \
      { \
        new_alloc += 20; \
        new_argv = (char **)realloc (new_argv, new_alloc * sizeof (char *)); \
        if (new_argv == NULL) \
            goto out_of_memory; \
      } \
    new_argv[new_argc++] = x; \
  } while (0)

char *_getname (const char *path);

void _wildcard (int *argcp, char ***argvp)
{
    int handle;

    int  i;
    int  old_argc;
    int  new_argc;
    int  new_alloc;

    char **old_argv;
    char **new_argv;
    char line[256];
    char *p;
    char *q;
    struct _finddata_t find;

    old_argc = *argcp;
    old_argv = *argvp;

    _rfnlwr ();

    // walkthrough the args - check for un-quoted wildcards
    for (i = 1; i < old_argc; ++i) if (old_argv[i] != NULL &&
                                   !(old_argv[i][-1] & (_ARG_DQUOTE|_ARG_RESPONSE)) &&
                                   strpbrk (old_argv[i], "?*") != NULL)
                                   break;

    // no wildcards to expand so return
    if (i >= old_argc) return;                 /* do nothing */

    new_argv = NULL;
    new_alloc = 0;
    new_argc = 0;

    for (i = 0; i < old_argc; ++i) {
        if (i == 0 ||
            old_argv[i] == NULL ||
            (old_argv[i][-1] & (_ARG_DQUOTE|_ARG_RESPONSE)) ||
            strpbrk (old_argv[i], "?*") == NULL             ||
            (handle =_findfirst (old_argv[i], &find)) == -1) WPUT (old_argv[i]);
        else {
            line[0] = _ARG_NONZERO|_ARG_WILDCARD;
            strcpy (line+1, old_argv[i]);
            p = _getname (line + 1);
            do {
                if (   find.name[0] != '.'
                    || (   find.name[1] != '\0'
                      && (find.name[1] != '.' || find.name[2] != '\0')))
                {
                    strcpy (p, find.name);
                    _fnlwr2 (p, line+1);
                    q = strdup (line);
                    if (q == NULL) goto out_of_memory;
                    WPUT (q+1);
                }
            } while (_findnext (handle, &find) == 0);
        }
    }

    WPUT (NULL);

    --new_argc;

    *argcp = new_argc;
    *argvp = new_argv;

    _rfnlwr ();

    _findclose( handle );

    return;

out_of_memory:
    fputs ("Out of memory while expanding wildcards\n", stderr);
    exit (255);
}



char *_getname (const char *path)
{
  const char *p;

  p = path;
  while (*path != 0)
      switch (*path++)
        {
        case ':':
        case '/':
        case '\\':
          p = path;             /* Note that PATH has been incremented */
          break;
        }
  return (char *)p;
}


#ifdef TESTWC

void main(int argc,char *argv[])
{
    int  i;

    _wildcard(&argc, &argv);
    for(i=0; i<argc; i++) printf("O: %s\n", argv[i]);
}

#endif
