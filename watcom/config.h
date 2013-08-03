/**********************************************************
 *
 * config.h
 *    Built from grep config.hin for compiling grep with
 *    Open Watcom 1.6 for OS/2 and NT targets
 *
 *    Really generic so I can use with several projects
 *
 *    December 2006
 *
 */

#define GREP                             1
#define HAVE_ALLOCA                      1
#define HAVE_ALLOCA_H                    1
#define HAVE_ATEXIT                      1
#define HAVE_DECL_STRTOUL                1
#define HAVE_DECL_STRTOULL               1
#define HAVE_DONE_WORKING_MALLOC_CHECK   1
#define HAVE_DONE_WORKING_REALLOC_CHECK  1
#define HAVE_FNMATCH                     1
#define HAVE_GETCWD                      1
#define HAVE_GETPAGESIZE                 1
#define HAVE_INTTYPES_H                  1
#define HAVE_ISASCII                     1
#define HAVE_LANGINFO_CODESET            1
#define HAVE_LIMITS_H                    1
#define HAVE_LOCALE_H                    1
#define HAVE_MALLOC_H                    1
#define HAVE_MEMCHR                      1
#define HAVE_MEMMOVE                     1
#define HAVE_MEMORY_H                    1
#define HAVE_NL_TYPES_H                  1
#define HAVE_PUTENV                      1
#define HAVE_SETENV                      1
#define HAVE_SETLOCALE                   1
#define HAVE_SETMODE                     1
#define HAVE_STDDEF_H                    1
#define HAVE_STDINT_H                    1
#define HAVE_STDLIB_H                    1
#define HAVE_STPCPY                      1
#define HAVE_STRCASECMP                  1
#define HAVE_STRCHR                      1
#define HAVE_STRDUP                      1
#define HAVE_STRERROR                    1
#define HAVE_STRINGS_H                   1
#define HAVE_STRING_H                    1
#define HAVE_STRTOUL                     1
#define HAVE_STRTOULL                    1
#define HAVE_STRTOUMAX                   1
#define HAVE_SYS_PARAM_H                 1
#define HAVE_SYS_STAT_H                  1
#define HAVE_SYS_TYPES_H                 1
#define HAVE_UNISTD_H                    1
#define HAVE_UNSIGNED_LONG_LONG          1
#define HAVE_VPRINTF                     1
#define HAVE_WCHAR_H                     1
#define PROTOTYPES                       1

/*
   This is left in as a reminder to not
   use!!!
*/
//#define HAVE_DOS_FILE_CONTENTS           1
//#define HAVE_DOS_FILE_NAMES              1

/*
   Have to have PCRE lib to link with...
 */
#ifdef WITH_PCRE
#define HAVE_LIBPCRE                     1
#endif

/* Don't know if wide char stuff works and have no way
   to test it */
#ifdef WIDECHAR_SUPPORT
#define HAVE_ISWCTYPE                    1
#define HAVE_MBRLEN                      1
#define HAVE_WCRTOMB                     1
#define HAVE_WCSCOLL                     1
#define HAVE_WCTYPE                      1
#define HAVE_WCTYPE_H                    1
#define HAVE_MBRTOWC                     1
#define HAVE_BTOWC                       1
#define MB_CUR_MAX                       1
#endif


/*
   I don't see this as a real bug but OW returns
   EACCES in the grepfile( ) when open( ) directory.
   So, this define returns the correct result from
   is_EISDIR( )
*/
#define HAVE_DIR_EACCES_BUG              1

/* My define for using direct.h instead of dirent.h */
#define HAVE_DIRECT_H                    1

#define VERSION                      "2.5.1a"

#define STDC_HEADERS                     1

/*
   The following were ripped from the klibc for gcc
   and were original from emx source. Handles wildcard
   expansion and response file for non unix shells.
 */
void _wildcard (int *, char ***);
void _response (int *, char ***);

#define initialize_main(argcp, argvp) \
               { _response(argcp, argvp); _wildcard(argcp, argvp); }

/* Add these to quiet compiler warnings */
extern int isdir (const char *path);
extern char *stpcpy (char *dest, const char *src);

/* should be in unistd.h in a BSD system?*/
int getpagesize(void);

