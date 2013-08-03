/*-
 * Copyright (c) 1992, 1993
 *      The Regents of the University of California.  All rights reserved.
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
 *
 * $FreeBSD: src/include/fnmatch.h,v 1.10 2002/03/23 17:24:53 imp Exp $
 *      @(#)fnmatch.h   8.1 (Berkeley) 6/2/93
 *
 * Modified December 2006 for Open Watcom use.
 *
 */

/*
 * fnmatch(filename, pattern, flags)
 *
 * Test whether the filename string matches the pattern string, returning true
 * or false. If the operating system is case-insensitive, then both parameters
 * will be normalized to all lower- or upper-case before the comparison is
 * performed. If you require a case-sensitive comparison regardless of whether
 * that's standard for your operating system, use fnmatchcase() instead.
 *
 *   Pattern Meaning
 *     *        matches everything
 *     ?        matches any single character
 *     [seq] 	matches any character in seq
 *     [!seq] 	matches any character not in seq
 *
 * FNM_FILE_NAME
 *  Treat the `/' character specially, for matching file names. If this flag
 *  is set, wildcard constructs in pattern cannot match `/' in string. Thus,
 *  the only way to match `/' is with an explicit `/' in pattern.
 * FNM_PATHNAME
 *  This is an alias for FNM_FILE_NAME; it comes from POSIX.2. We don't recommend
 *  this name because we don't use the term "pathname" for file names.
 * FNM_PERIOD
 *  Treat the `.' character specially if it appears at the beginning of string. If
 *  this flag is set, wildcard constructs in pattern cannot match `.' as the first
 *  character of string. If you set both FNM_PERIOD and FNM_FILE_NAME, then the
 *  special treatment applies to `.' following `/' as well as to `.' at the beginning
 *  of string. (The shell uses the FNM_PERIOD and FNM_FILE_NAME falgs together for
 *  matching file names.)
 * FNM_NOESCAPE
 *  Don't treat the `\' character specially in patterns. Normally, `\' quotes the
 * following character, turning off its special meaning (if any) so that it matches
 * only itself. When quoting is enabled, the pattern `\?' matches only the string `?',
 * because the question mark in the pattern acts like an ordinary character. If you
 * use FNM_NOESCAPE, then `\' is an ordinary character.
 * FNM_LEADING_DIR
 *  Ignore a trailing sequence of characters starting with a `/' in string; that is
 *  to say, test whether string starts with a directory name that pattern matches.
 *  If this flag is set, either `foo*' or `foobar' as a pattern would match the
 *  string `foobar/frobozz'.
 * FNM_CASEFOLD
 *  Ignore case in comparing string to pattern.
*/

#ifndef _FNMATCH_H_
#define _FNMATCH_H_

#define FNM_NOMATCH     1       /* The string does not match the specified pattern. */

#define FNM_NOSYS      (-1)

#define FNM_NOESCAPE    0x01    /* Disable backslash escaping. */
#define FNM_PATHNAME    0x02    /* Slash in string only matches slash in pattern. */
#define FNM_PERIOD      0x04    /* Leading period in string must be exactly matched by period in pattern. */

#if !defined(_ANSI_SOURCE) && !defined(_POSIX_SOURCE)
#define FNM_LEADING_DIR 0x08    /* Ignore /<tail> after Imatch. */
#define FNM_CASEFOLD    0x10    /* Case insensitive search. */
#define FNM_IGNORECASE  FNM_CASEFOLD
#define FNM_FILE_NAME   FNM_PATHNAME
#endif


#ifdef __cplusplus
    extern "C" {
#endif

int  fnmatch(const char *, const char *, int);

#ifdef __cplusplus
}
#endif

#endif /* !_FNMATCH_H_ */

