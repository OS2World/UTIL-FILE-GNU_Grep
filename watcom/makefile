#----------------------------------------------------------------------------
#
# Makefile for GNU 2.5.1a using OpenWatcom 1.6 and above
#
#   Michael Greene <os2@mgreene.org>   22 December 2006
#
#----------------------------------------------------------------------------

# uncomment for debug version
#DEBUG = 1

# defaults
COMSYSTEM = os2
LNKSYSTEM = os2v2

!ifdef %WATCTAGS
COMSYSTEM = $(%WATCTAGS)
!endif

!ifeq COMSYSTEM OS2
LNKSYSTEM = os2v2
!endif

!ifeq COMSYSTEM NT
LNKSYSTEM = nt
!endif

!ifdef %WATPCRE
!ifeq COMSYSTEM OS2
LIBS = LIB $(WATLIB)\libpcre.lib
!endif
!ifeq COMSYSTEM NT
LIBS = LIB $(WATLIB)\libpcre_nt.lib
NTSTATIC = -DPCRE_STATIC
!endif
PCREDEF = -DWITH_PCRE
!endif

!ifdef %WATCHARSUP
WCDEF = -DWIDECHAR_SUPPORT
!endif

!ifdef %WATDEBUG
DEBUG = 1
DBDEF = -DDEBUG
!endif

# Machine type -5r Pent -6r Pent Pro
MACHINE= -6r -fp6

#Optimization None -od  - Fastest possible -otexan
OPT= -otexan

CC = wcc386
LD = wlink
LB = wlib

SRCDIR = ..\src
LIBDIR = ..\lib
WATLIB = .\watlib

INCLUDE  = .\;$(SRCDIR);$(WATLIB);$(%watcom)\h;$(%watcom)\h\$(COMSYSTEM);$(LIBDIR)

DEFS = $(DBDEF) $(WCDEF) $(PCREDEF) $(NTSTATIC) -DHAVE_CONFIG_H

!ifdef DEBUG
CFLAGS   = -i=$(INCLUDE) -za99 -wx -od -d3  $(DEFS) $(MACHINE) -bt=$(COMSYSTEM) -mf
LDFLAGS  =  SYS $(LNKSYSTEM) d all op map,symf
!else
CFLAGS   = -i=$(INCLUDE) -za99 -wcd302 -wcd201 -d0 -wx -zq $(OPT) $(DEFS) &
           $(MACHINE) -bt=$(COMSYSTEM) -mf
LDFLAGS  = SYS $(LNKSYSTEM) op el
!endif

LIBFLAGS = -b -c -n -p=512

GREPOBJS = grep.obj search.obj kwset.obj dfa.obj grepmat.obj

LIBOBJS  = closeout.obj error.obj exclude.obj hard-locale.obj isdir.obj quotearg.obj regex.obj &
           savedir.obj xmalloc.obj xstrtol.obj xstrtoumax.obj obstack.obj getopt.obj getopt1.obj &
           stpcpy.obj fnmatch.obj getpagesize.obj langinfo.obj filemisc.obj response.obj wildcard.obj

.c:$(SRCDIR);$(WATLIB);$(LIBDIR)

.c.obj:
  $(CC) $(CFLAGS) $<

default: .SYMBOLIC
  @echo Targets:
  @echo    os2       build os2 release version
  @echo    os2debug  build os2 debug version
  @echo    os2pcre   build os2 pcre version
  @echo    nt        build nt release version
  @echo    ntdebug   build nt debug version
  @echo    ntpcre    build nt pcre version
  @echo    clean     clean up the build
  @echo    ctagsarc  zip archive
  @echo    .
  @echo    os2w      build os2 release version
  @echo              with wide character support
  @echo    os2pcrew  build os2 pcre version
  @echo              with wide character support
#****************** TARGETS **********************

# os/2 without pcre
os2: .SYMBOLIC
  set WATCTAGS=OS2
  wmake -f makefile all

# os/2 without pcre and debug
os2debug: .SYMBOLIC
  set WATCTAGS=OS2
  set WATDEBUG=1
  wmake -f makefile all

# os/2 with pcre
os2pcre: .SYMBOLIC
  set WATCTAGS=OS2
  set WATPCRE=1
  wmake -f makefile all

# os/2 without pcre and with wide char support
os2w: .SYMBOLIC
  set WATCTAGS=OS2
  set WATCHARSUP=1
  wmake -f makefile all

# os/2 with pcre and with wide char support
os2pcrew: .SYMBOLIC
  set WATCTAGS=OS2
  set WATCHARSUP=1
  set WATPCRE=1
  wmake -f makefile all

nt: .SYMBOLIC
  set WATCTAGS=NT
  wmake -f makefile all

ntdebug: .SYMBOLIC
  set WATCTAGS=NT
  set WATDEBUG=1
  wmake -f makefile all

ntpcre: .SYMBOLIC
  set WATCTAGS=NT
  set WATPCRE=1
  wmake -f makefile all

# end of targets

all: grep.exe

grep.exe: $(GREPOBJS) $(LIBOBJS)
  $(LD) NAME grep $(LDFLAGS) $(LIBS) FILE {$(GREPOBJS) $(LIBOBJS)}

greparc: grep.exe .SYMBOLIC
  @%make cleanrel
  copy ..\NEWS .
  copy ..\COPYING .
#  lxlite grep.exe
  zip -9 grep-2_5_1a_ow.zip grep.exe &
                            NEWS &
                            COPYING &
                            watcom.readme

clean : .SYMBOLIC
  -@rm *.exe
  -@rm *.lib
  -@rm NEWS
  -@rm COPYING
  -@rm *.zip
  @%make cleanrel

cleanrel : .PROCEDURE
  -@rm *.obj
  -@rm *.sym
  -@rm *.err
  -@rm *.lst
  -@rm *.map
  -@rm *.err




