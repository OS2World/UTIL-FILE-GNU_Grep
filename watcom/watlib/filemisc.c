/*
 *
 *                    MiscLib Open Watcom Project
 *
 * This file pulled from netlabs svn - klibc by Knut St. Osmundsen
 *
 * Modified December 2006 for Open Watcom use by
 * Michael Greene <os2@mgreene.org>
 *
 * Original:
 * (emx+gcc) -- Copyright (c) 1992-1996 by Eberhard Mattes
 *
 * Function included:
 * _fnlwr2, _fnlwr, _rfnlwr, _sfnlwr, _fngetdrive, _filesys
 *
 * =========================================================================
 */

// The following defines are for the only API calls included -
// see _filesys( )
#ifdef __OS2__
#define INCL_DOSFILEMGR
#include <os2.h>
#else
#include <windows.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <direct.h>
#include "misclib.h"

static char cache_curdrive    = 0;
static char cache_filesys[26] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,
                                  0,0,0,0,0,0,0,0,0,0,0,0,0 };

#define DIRSEP_P(c) ((c) == '\\' || (c) == '/')

void _fnlwr2 (char *name, const char *base)
{
    int  i;
    char c;
    char dr[3];
    char fs[16];

    /* Do not translate UNC paths to lower case. */
    // if not normal drive return
    if (DIRSEP_P (base[0]) && DIRSEP_P (base[1]) && !DIRSEP_P (base[2])) return;

    // upper case drive letter
    c = _fngetdrive(base);

    // if drive & colon not included in base get current drive (_getdrive() )
    if (c == 0) {
        if (cache_curdrive == 0) cache_curdrive = _getdrive ();
        c = cache_curdrive;
    }
    else if (name == base) name[0] = (char)(c - 'A' + 'a');

    i = c - 'A';

    if (cache_filesys[i] == 0) {
        dr[0] = c;
        dr[1] = ':';
        dr[2] = 0;

        if (_filesys(dr, fs, sizeof (fs)) != 0) return;

        if (strcmp (fs, "FAT") == 0 || strcmp (fs, "CDFS") == 0)
            cache_filesys[i] = 'u';     /* upper-casing */
        else
            cache_filesys[i] = 'p';     /* case-preserving, perhaps */
    }

    if (cache_filesys[i] == 'u') strlwr(name);
}


void _fnlwr (char *name)
{
    _fnlwr2 (name, name);
}


void _rfnlwr (void)
{
    int i;

    for (i = 0; i < 26; ++i) cache_filesys[i] = 0;
    cache_curdrive = 0;
}


void _sfnlwr (const char *name)
{
    cache_curdrive = _fngetdrive (name);
    if (cache_curdrive == 0) cache_curdrive = _getdrive ();
}


char _fngetdrive (const char *src)
{
    /* As we check the first character we can't hit by accident a DBCS
       character having ':' as second byte. */
    if (isalpha(src[0]) && src[1] == ':') return toupper(src[0]);
    else return 0;
}


int _filesys (const char *drive, char *name, size_t size)
{
#ifdef __OS2__
    ULONG rc;
    ULONG len;
    FSQBUFFER2 *buf;

    len = (sizeof(FSQBUFFER2) + _MAX_PATH);
    buf = malloc(len);

    rc = DosQueryFSAttach (drive, 1, FSAIL_QUERYNAME, buf, &len);

    if ((rc != 0) ||
        (buf->iType != FSAT_LOCALDRV && buf->iType != FSAT_REMOTEDRV) ||
        (buf->cbFSDName >= size)) {
        return -1;
    }

    strcpy (name, buf->szFSDName + buf->cbName);

    free(buf);
#else  // NT
    char windrv[4];

    strcpy(windrv, drive);

    windrv[2] = '\\';
    windrv[3] = 0;

    switch(GetDriveType(windrv)) {

    case DRIVE_UNKNOWN:
    case DRIVE_NO_ROOT_DIR:
        return -1;
        break;

    case DRIVE_CDROM:
        strcpy(name, "CDFS");
        return 0;
        break;

    default:
        GetVolumeInformation(windrv, NULL, 0, NULL, NULL, NULL, name, size);
    }
#endif

    return 0;
}

