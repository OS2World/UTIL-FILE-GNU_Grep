/*
 *  fnmatch.h
 *
 *                    MiscLib Open Watcom Project
 *
 *             Modified December 2006 for Open Watcom use.
 *
 *                  Michael Greene <os2@mgreene.org>
 *
 * =========================================================================
 */

/*
 * Copyright (c) 1989, 1993, 1994
 *      The Regents of the University of California.  All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Guido van Rossum.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *      This product includes software developed by the University of
 *      California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

// #if defined(LIBC_SCCS) && !defined(lint)
// static char sccsid[] = "@(#)fnmatch.c   8.2 (Berkeley) 4/16/94";
// #endif /* LIBC_SCCS and not lint */

/*
 * Function fnmatch() as specified in POSIX 1003.2-1992, section B.6.
 * Compares a filename or pathname to a pattern.
 */

#include <ctype.h>
#include <fnmatch.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>

#define EOS     '\0'

static const char *rangematch(const char *pattern, char test, int flags);
static int  __collate_range_cmp(int, int);  // MKG: add collate.h and collcmp functions

int  fnmatch(const char *pattern, const char *string, int flags)
{
    const char *stringstart;
    char c;
    char test;

    for (stringstart = string;;)

        switch (c = *pattern++) {
        case EOS:
            if ((flags & FNM_LEADING_DIR) && *string == '/') return (0);
            return (*string == EOS ? 0 : FNM_NOMATCH);

        case '?':
            if (*string == EOS) return (FNM_NOMATCH);
            if (*string == '/' && (flags & FNM_PATHNAME)) return (FNM_NOMATCH);
            if (*string == '.' && (flags & FNM_PERIOD) &&
                (string == stringstart ||
                ((flags & FNM_PATHNAME) && *(string - 1) == '/'))) return (FNM_NOMATCH);
            ++string;
            break;

        case '*':
            c = *pattern;

            /* Collapse multiple stars. */
            while (c == '*') c = *++pattern;

            if (*string == '.' && (flags & FNM_PERIOD) &&
                (string == stringstart ||
                ((flags & FNM_PATHNAME) && *(string - 1) == '/'))) return (FNM_NOMATCH);

            /* Optimize for pattern with * at end or before /. */
            if (c == EOS) if (flags & FNM_PATHNAME)
                              return ((flags & FNM_LEADING_DIR) ||
                                      strchr(string, '/') == NULL ? 0 : FNM_NOMATCH);
                    else return (0);

            else if (c == '/' && flags & FNM_PATHNAME) {
                 if ((string = strchr(string, '/')) == NULL) return (FNM_NOMATCH);
                 break;
            }

            /* General case, use recursion. */
            while ((test = *string) != EOS) {
                if (!fnmatch(pattern, string, flags & ~FNM_PERIOD)) return (0);
                if (test == '/' && flags & FNM_PATHNAME) break;
                ++string;
            }
            return (FNM_NOMATCH);

        case '[':
            if (*string == EOS) return (FNM_NOMATCH);
            if (*string == '/' && flags & FNM_PATHNAME) return (FNM_NOMATCH);
            if ((pattern = rangematch(pattern, *string, flags)) == NULL) return (FNM_NOMATCH);
            ++string;
            break;

        case '\\':
            if (!(flags & FNM_NOESCAPE)) {
                if ((c = *pattern++) == EOS) {
                    c = '\\';
                    --pattern;
                }
            }
            /* FALLTHROUGH */

        default:
            if (c == *string);
            else if ((flags & FNM_CASEFOLD) &&
                    (tolower((unsigned char)c) == tolower((unsigned char)*string)));
                 else return (FNM_NOMATCH);
            string++;
            break;
        }
        /* NOTREACHED */
}


static const char *rangematch(const char *pattern, char test, int flags)
{
    int  negate;
    int  ok;
    char c;
    char c2;

    /*
     * A bracket expression starting with an unquoted circumflex
     * character produces unspecified results (IEEE 1003.2-1992,
     * 3.13.2).  This implementation treats it like '!', for
     * consistency with the regular expression syntax.
     * J.T. Conklin (conklin@ngai.kaleida.com)
     */
    if ( (negate = (*pattern == '!' || *pattern == '^'))) ++pattern;

    if (flags & FNM_CASEFOLD) test = tolower((unsigned char)test);

    for (ok = 0; (c = *pattern++) != ']';) {
        if (c == '\\' && !(flags & FNM_NOESCAPE)) c = *pattern++;
        if (c == EOS) return (NULL);

        if (flags & FNM_CASEFOLD) c = tolower((unsigned char)c);

        if (*pattern == '-' && (c2 = *(pattern+1)) != EOS && c2 != ']') {
            pattern += 2;
            if (c2 == '\\' && !(flags & FNM_NOESCAPE)) c2 = *pattern++;
            if (c2 == EOS) return (NULL);

            if (flags & FNM_CASEFOLD) c2 = tolower((unsigned char)c2);

            if ( __collate_range_cmp(c, test) <= 0
                && __collate_range_cmp(test, c2) <= 0 ) ok = 1;
        } else if (c == test) ok = 1;
    }

    return (ok == negate ? NULL : pattern);
}


/*
 * Copyright (C) 1996 by Andrey A. Chernov, Moscow, Russia.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#define ASCII_COMPATIBLE_COLLATE        /* see share/colldef */

/*
 * Compare two characters converting collate information
 * into ASCII-compatible range, it allows to handle
 * "[a-z]"-type ranges with national characters.
 */

int __collate_range_cmp (int c1, int c2)
{
    int ret;

#ifndef ASCII_COMPATIBLE_COLLATE
    int  as1;
    int  as2;
    int  al1;
    int  al2;
#endif

    static char s1[2];
    static char s2[2];

    c1 &= UCHAR_MAX;
    c2 &= UCHAR_MAX;

    if (c1 == c2) return (0);

#ifndef ASCII_COMPATIBLE_COLLATE
   as1 = isascii(c1);
   as2 = isascii(c2);
   al1 = isalpha(c1);
   al2 = isalpha(c2);

   if (as1 || as2 || al1 || al2) {
       if ((as1 && as2) || (!al1 && !al2)) return (c1 - c2);

       if (al1 && !al2) {
           if (isupper(c1)) return ('A' - c2);
           else return ('a' - c2);
       } else if (al2 && !al1) {
           if (isupper(c2)) return (c1 - 'A');
           else return (c1 - 'a');
       }
   }
#endif

    s1[0] = c1;
    s2[0] = c2;

    if ((ret = strcoll(s1, s2)) != 0) return (ret);

    return (c1 - c2);
}


#ifdef TEST

#include <stdlib.h>
#include <unistd.h>

void usage( void)
{
    printf("fnmatch test program.\n");
    printf("Usage:\n");
    printf("fnmatch <pattern> <string> [-flags -flag ...]\n");
    printf("  flags -\n");
    printf("  POSIX Flags:  NOESCAPE PATHNAME PERIOD\n");
    printf("  Non-POSIX:    LEADING_DIR CASEFOLD IGNORECASE FILE_NAME\n\n");
}



int main(int argc,char *argv[])
{
    int  ch;
    int  flags  = 0;
    int  ret    = 0;
    int  output = 1; // output printf by default

    char *pattern = NULL;
    char *strname = NULL;

#ifdef NOUTPUT
    output = 0;
#endif

    if(argc == 1) {
        if (output) usage( );
        exit(0);
    }

    while ((ch = getopt(argc, argv, "n:N:p:P:l:L:c:C:i:I:f:F:")) != -1)
    {
        switch (ch)
        {
            case 'n':
            case 'N':
                if(stricmp(optarg, "OESCAPE") == 0) flags |= FNM_NOESCAPE;
                break;

            case 'f':
            case 'F':
            case 'p':
            case 'P':
                if((stricmp(optarg, "ATHNAME") == 0) ||
                   (stricmp(optarg, "ILE_NAME") == 0)) flags |= FNM_PATHNAME;
                if(stricmp(optarg, "ERIOD") == 0) flags |= FNM_PERIOD;
                break;

            case 'l':
            case 'L':
                if(stricmp(optarg, "EADING_DIR") == 0) flags |= FNM_LEADING_DIR;
                break;

            case 'i':
            case 'I':
            case 'c':
            case 'C':
                if((stricmp(optarg, "GNORECASE") == 0) ||
                   (stricmp(optarg, "ASEFOLD") == 0)) flags |= FNM_CASEFOLD;
                break;

            default:
                usage( );
                exit(0);
        }
    }

    if (argc != (optind + 2)) {
        if (output) usage( );
        exit(0);
    }

    pattern = argv[optind++];
    strname = argv[optind];

    if (output) printf("Pattern %s  Strname %s  Flags %d * ", pattern, strname, flags);

    ret = fnmatch(pattern, strname, flags);

    if (output) if(ret) printf("No match!\n");
                else printf("Match!\n");

    return ret;
}
#endif

