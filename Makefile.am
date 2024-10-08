# Copyright (C) 1989-2024 Free Software Foundation, Inc.
# Original Makefile.in written by James Clark (jjc@jclark.com)
# Migrated to Automake by Bertrand Garrigues
#
# This file is part of groff.
#
# groff is free software; you can redistribute it and/or modify it under
# the terms of the GNU General Public License as published by the Free
# Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# groff is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
# for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
# Makefile.am
#

# Variables listed in the following comments, but not set, will be
# automatically set by Automake in the generated Makefile.in file.

# SHELL
# PACKAGE_TARNAME
# srcdir
# top_srcdir
# VPATH
# top_builddir

# 'RT_SEP' is the target system's native path element separator, used
# in runtime PATHs compiled into groff executables.
RT_SEP=@GROFF_PATH_SEPARATOR@

# 'SH_SEP' is a alternative path separator character used in shell
# scripts and Makefile rules; it may be the same as 'RT_SEP', but,
# particularly in some Microsoft environments, it may differ.
SH_SEP=@PATH_SEPARATOR@

# 'GLIBC21' is "yes" if the host operating system uses GNU libc 2.1 or
# newer, otherwise "no".

# 'ALT_GHOSTSCRIPT_PROGS' specifies a list of alternative names to be
# tried if 'GHOSTSCRIPT' cannot be found at run time.
# XXX: only pdfmark.am uses this

# 'ALT_AWK_PROGS' similarly specifies a list of alternative names,
# which can be tried at run time, to identify the awk program.
# XXX: only pdfmark.am uses this

# BROKEN_SPOOLER_FLAGS
#
# Normally the Postscript driver, grops, produces output that conforms
# to version 3.0 of the Adobe Document Structuring Conventions.
# Unfortunately some spoolers and previewers can't handle such output.
# The BROKEN_SPOOLER_FLAGS variable tells grops what it should do to
# make its output acceptable to such programs.  This variable controls
# only the default behaviour of grops; the behaviour can be changed at
# runtime by the grops '-b' option (and so by 'groff -P-b').
#
# Use a value of 0 if your spoolers and previewers are able to handle
# conforming PostScript correctly.
#
# Add 1 if no %%{Begin,End}DocumentSetup comments should be generated;
# this is needed for early versions of TranScript that get confused by
# anything between the %%EndProlog line and the first %%Page: comment.
#
# Add 2 if lines in included files beginning with %! should be stripped
# out; this is needed for the OpenWindows 2.0 pageview previewer.
#
# Add 4 if %%Page, %%Trailer and %%EndProlog comments should be stripped
# out of included files; this is needed for spoolers that don't
# understand the %%{Begin,End}Document comments.  I suspect this
# includes early versions of TranScript.
#
# Add 8 if the first line of the PostScript output should be
# %!PS-Adobe-2.0 rather than %!PS-Adobe-3.0; this is needed when using
# Sun's Newsprint with a printer that requires page reversal.

# 'DEVICE' is the default output device.
DEVICE=ps

# 'XPROGDIRS' is either "src/devices/xditview src/utils/xtotroff" or
# empty.

# 'XLIBDIRS' is either `src/libs/libxutil' or empty.

# 'PSPRINT' is the command to use for printing a PostScript file,
# for example 'lpr'.

# 'DVIPRINT' is the command to use for printing a TeX dvi file,
# for example `lpr -d'.

# 'g' is the prefix for names of programs that have Unix counterparts.
# For example, if 'g' is "g" then troff will be installed as gtroff.
# This doesn't affect programs like grops or groff that have no Unix
# counterparts.  The groff versions of eqn and tbl do not work with Unix
# troff.

# 'prefix' is a common prefix for installation directories.  It is used
# in definitions of 'exec_prefix', 'datasubdir', 'fontpath', 'manroot'.

# 'bindir' says where to install executables.

# 'libdir' says where to install platform-dependent data.

# 'libprogramdir' is "$(libdir)/groff".

# datadir
# datarootdir
dataprogramdir=$(datadir)/groff

# 'datasubdir' says where to install platform-independent data files.
datasubdir=$(dataprogramdir)/$(SHORT_VERSION)

# 'infodir' says where to install info files.

# 'docdir' says where to install documentation files.  The default
# location is ${datarootdir}/doc/${PACKAGE}, but we add the version
docdir=$(datarootdir)/doc/${PACKAGE}-$(SHORT_VERSION)

# 'exampledir' says where to install example files.
exampledir=$(docdir)/examples

# 'htmldocdir' says where to install documentation in HTML format.
htmldocdir=$(docdir)/html

# 'pdfdocdir' says where to install documentation in PDF format.
pdfdocdir=$(docdir)/pdf

# 'fontdir' says where to install dev*/*.
fontdir=$(datasubdir)/font

# 'oldfontdir' says where to install old font sets (as dev*/*).
oldfontdir=$(datasubdir)/oldfont

# 'localfontdir' says where local fonts will be installed (as dev*/*).
localfontdir=$(dataprogramdir)/site-font

# 'legacyfontdir' is for compatibility with AT&T/DWB troff.
legacyfontdir=/usr/lib/font

# 'fontpath' says where to look for dev*/*.
fontpath=$(localfontdir)$(RT_SEP)$(fontdir)$(RT_SEP)$(legacyfontdir)

# 'tmacdir' says where to install macros.
tmacdir=$(datasubdir)/tmac

# 'systemtmacdir' says where to install platform-dependent macros.
systemtmacdir=$(libprogramdir)/site-tmac

# 'localtmacdir' says where local files will be installed.
localtmacdir=$(dataprogramdir)/site-tmac

# 'appdefdir' says where to install the application defaults file for
# gxditview.

# glilypond_dir
# gpinyin_dir

# 'tmacpath' says where to look for macro files.  The current directory
# is prepended at run time in unsafe mode only; the user's home
# directory is always added.  "troffrc" and "troffrc-end" (and "eqnrc")
# are sought neither in the current nor in the home directory.
tmacpath=$(systemtmacdir)$(RT_SEP)$(localtmacdir)$(RT_SEP)$(tmacdir)

# 'sys_tmac_prefix' is prefix (if any) for system macro packages.

# 'pnmtops_nosetpage' is the command to be run to generate an eps
# file.  Some versions of pnmtops provide the -nosetpage option.
# We detect this and use it if present.

# 'tmac_wrap' is list of system macro packages that should be made
# available to groff by creating a corresponding macro package in the
# groff macro directory that references the system macro package.

# 'compatibility_wrappers' controls the creation and installation of
# compatibility wrappers for vendor-provided, non-GNU macro packages
# named in 'tmac_wrap'.
#
# Its value must be one of:
#   "yes"    install compatibility wrappers as the original macro
#            package name (that is, the default implementation of that
#            macro package), and install the groff implementation with a
#            prefix (usually "g").
#   "no"     do not install compatibility wrappers, but only the groff
#            implementation of each macro package.
#   "manual" install the compatibility wrappers as "<package>-os" and
#            the groff implementation of each package as the default
#            implementation of that package.

# Don't make this empty.
wrapper_suffix_default="-os"

wrapper_suffix=\
  `if test $(compatibility_wrappers) = manual; then \
     echo $(wrapper_suffix_default); \
   fi`

# When 'compatibility_wrappers' is "yes", if there is a groff
# implementation of a macro package listed in $(tmac_wrap), then the
# groff implementation will be installed with a prefix of $(tmac_prefix)
# via the collision detection variables $(tmac_m_prefix),
# $(tmac_s_prefix), and $(tmac_an_prefix).

# Don't make this empty.
tmac_prefix=g

# The groff mm macros will be available as -m$(tmac_m_prefix)m.
tmac_m_prefix=\
 `if test $(compatibility_wrappers) = yes; then \
    for i in $(tmac_wrap) ""; do \
      case "$$i" in m) echo $(tmac_prefix);; esac; \
    done; \
  fi`

# The groff ms macros will be available as -m$(tmac_s_prefix)s.
tmac_s_prefix=\
 `if test $(compatibility_wrappers) = yes; then \
    for i in $(tmac_wrap) ""; do \
      case "$$i" in s) echo $(tmac_prefix);; esac; \
    done; \
  fi`

# The groff man macros will be available as -m$(tmac_an_prefix)an.
tmac_an_prefix=\
 `if test $(compatibility_wrappers) = yes; then \
    for i in $(tmac_wrap) ""; do \
      case "$$i" in an) echo $(tmac_prefix);; esac; \
    done; \
  fi`

# Extension to be used for refer index files.  Index files are not
# shareable between different architectures, so you might want to use
# different suffixes for different architectures.  Choose an extension
# that doesn't conflict with refer or any other indexing program.
indexext=.i

# Directory containing the default index for refer.
indexdir=/usr/dict/papers

# The filename (without suffix) of the default index for refer.
indexname=Ind

# list of common words not suitable for bibliographic keyword searches.
# If your system provides "/usr/lib/eign", it is copied to this
# location; otherwise groff's eign file is used.
common_words_file=$(datasubdir)/eign

# mandir

# 'manroot' is the root of the man page directory tree.
manroot=$(mandir)

# 'man1ext' is the man section for user commands.
man1ext=1
man1dir=$(manroot)/man$(man1ext)

# 'man5ext' is the man section for file formats.
man5ext=5
man5dir=$(manroot)/man$(man5ext)

# 'man7ext' is the man section for macro packages and general topics.
man7ext=7
man7dir=$(manroot)/man$(man7ext)

# Windows '.cmd' files
# make_winscripts
# make_install_winscripts
# make_uninstall_winscripts

# All the previous installation directories, when used, are prefixed
# with $(DESTDIR) during install and uninstall to support staged
# installations.

# config.h might set the following C/C++ #defines:
#
# WORDS_BIGENDIAN		if your target platform is big-endian
#
# HAVE_DIRECT_H		if you have <direct.h>
# HAVE_DIRENT_H		if you have <dirent.h>
# HAVE_CC_INTTYPES_H		if you have a C++ <inttypes.h>
# HAVE_PROCESS_H		if you have <process.h>
# HAVE_LIMITS_H		if you have <limits.h>
# HAVE_CC_LIMITS_H		if you have a C++ <limits.h>
# HAVE_MATH_H			if you have <math.h>
# HAVE_CC_OSFCN_H		if you have a C++ <osfcn.h>
# HAVE_STDDEF_H		if you have <stddef.h>
# HAVE_STDLIB_H		if you have <stdlib.h>
# HAVE_STRING_H		if you have <string.h>
# HAVE_STRINGS_H		if you have <strings.h>
# HAVE_SYS_DIR_H		if you have <sys/dir.h>
# HAVE_SYS_PARAM_H		if you have <sys/param.h>
# HAVE_SYS_STAT_H		if you have <sys/stat.h>
# HAVE_SYS_TIME_H		if you have <sys/time.h>
# HAVE_SYS_TYPES_H		if you have <sys/types.h>
# HAVE_UNISTD_H		if you have <unistd.h>
#
# HAVE_FMOD			if you have fmod()
# HAVE_GETCWD			if you have getcwd()
# HAVE_GETTIMEOFDAY		if you have gettimeofday()
# HAVE_ICONV			if you have iconv()
# HAVE_ISATTY			if you have isatty()
# HAVE_KILL			if you have kill()
# HAVE_LANGINFO_CODESET	if you have nl_langinfo()
# HAVE_MKSTEMP		if you have mkstemp()
# HAVE_MMAP			if you have mmap()
# HAVE_PUTENV			if you have putenv()
# HAVE_RENAME			if you have rename()
# HAVE_SETLOCALE		if you have setlocale()
# HAVE_STRCASECMP		if you have strcasecmp()
# HAVE_STRNCASECMP		if you have strncasecmp()
# HAVE_STRERROR		if you have strerror()
# HAVE_STRSEP			if you have strsep()
# HAVE_STRTOL			if you have strtol()
# HAVE_SYMLINK		if you have symlink()
#
# NEED_DECLARATION_GETTIMEOFTODAY
#				if your C++ <sys/time.h> doesn't declare
#				gettimeofday()
# NEED_DECLARATION_HYPOT	if your C++ <math.h> doesn't declare hypot()
# NEED_DECLARATION_PCLOSE	if your C++ <stdio.h> doesn't declare pclose()
# NEED_DECLARATION_POPEN	if your C++ <stdio.h> doesn't declare popen()
# NEED_DECLARATION_PUTENV	if your C++ <stdlib.h> doesn't declare
#				putenv()
# NEED_DECLARATION_RAND	if your C++ <stdlib.h> doesn't declare rand()
# NEED_DECLARATION_SRAND	if your C++ <stdlib.h> doesn't declare srand()
# NEED_DECLARATION_STRCASECMP	if your C++ <string.h> doesn't declare
#				strcasecmp()
# NEED_DECLARATION_STRNCASECMP
#				if your C++ <string.h> doesn't declare
#				strncasecmp()
# NEED_DECLARATION_VFPRINTF	if your C++ <stdio.h> doesn't declare
#				vfprintf()
#
# HAVE_DECL_GETC_UNLOCKED	if you have getc_unlocked()
# HAVE_DECL_SYS_SIGLIST		if you have sys_siglist[]
#
# HAVE_STRUCT_EXCEPTION		if <math.h> defines struct exception
# HAVE_SYS_NERR			if you have sysnerr in <errno.h> or <stdio.h>
# HAVE_SYS_ERRLIST		if you have sys_errlist in <errno.h> or
#				<stdio.h>
# ICONV_CONST=const		if declaration of iconv() needs const
# LONG_FOR_TIME_T		if localtime() takes a long * not a time_t *
# RETSIGTYPE=int		if signal handlers return int not void
# RET_TYPE_SRAND_IS_VOID	if srand() returns void not int
#
# WCOREFLAG=0200		if the 0200 bit of the status returned by
#				wait() indicates whether a core image was
#				produced for a process that was terminated
#				by a signal
#
# HAVE_WORKING_O_NOATIME	define if <fcntl.h>'s O_NOATIME flag works
# HAVE_WORKING_O_NOFOLLOW	define if <fcntl.h>'s O_NOFOLLOW flag works
#
# uintmax_t=<value>		define to `unsigned long' or `unsigned long
#				long' if <inttypes.h> does not exist
#
# PAGE=A4			if the printer's page size is A4
# GHOSTSCRIPT=gs		the name (and directory if required) of the
#				ghostscript program

# Include
#
#   {fmod,getcwd,mkstemp,putenv,strcasecmp,
#    strerror,strncasecmp,strtol}.$(OBJEXT)
#
# in 'LIBOBJS' if your C library is missing the corresponding function.

# 'CCC' is the compiler for C++ (.cpp) files.
CCC=@CXX@
# CC
# CFLAGS
# CPPFLAGS
# LDFLAGS

# X_CFLAGS
# X_LIBS
# X_EXTRA_LIBS
# X_PRE_LIBS

# YACC

# GREP
# EGREP

# MAKEINFO

# EXEEXT
# OBJEXT
# LIBEXT
# LIBS
# LIBM
# LIBICONV
# AR
# RANLIB
# INSTALL
# INSTALL_PROGRAM
# INSTALL_SCRIPT
# INSTALL_DATA
# INSTALL_INFO
# LN_S

ETAGS=etags
ETAGSFLAGS=
# Flag that tells etags to assume C++.
ETAGSCCFLAG=-C
# Full path to perl.
# PERL
# PERLVERSION
# Sed command with which to edit sh scripts.
# SH_SCRIPT_SED_CMD
# Sed script to deal with OS dependencies in sh scripts.
SH_DEPS_SED_SCRIPT=$(top_builddir)/shdeps.sed

# On some platforms we need to set LANG and LC_ALL to C for sed
SED=\
  LANG=C \
  LC_ALL=C \
  sed

# The program to create directory hierarchies.
# mkinstalldirs is now in gnulib/build-aux

PURIFY=purify
PURIFYCCFLAGS=
#PURIFYCCFLAGS=-g++=yes \
#  -collector=`dirname \`$(CCC) -print-libgcc-file-name\``/ld

# Add groff and gnulib m4 macros
ACLOCAL_AMFLAGS = -I m4 -I gnulib_m4

# Common preprocessor flags: gnulib includes, common includes in
# src/include, and config.h that is generated in the build tree
AM_CPPFLAGS = \
  -I$(top_srcdir)/src/include \
  -I$(top_srcdir)/lib \
  -I$(top_builddir)/src/include \
  -I$(top_builddir)/lib

# Define a string for rules that call groff in make's silent mode.
GROFF_V = $(GROFF_V_@AM_V@)
GROFF_V_ = $(GROFF_V_@AM_DEFAULT_V@)
GROFF_V_0 = @echo "  GROFF   " $@;

# The following Automake variables will be supplemented by the various
# .am files.
bin_PROGRAMS =
nobase_bin_PROGRAMS =
bin_SCRIPTS =
dist_bin_SCRIPTS =
# stuff that should be in distribution archives but not in source repo
MAINTAINERCLEANFILES =
MOSTLYCLEANFILES =
noinst_LIBRARIES =
BUILT_SOURCES =
CLEANFILES =
# gnulib asks to include this file
EXTRA_DIST = gnulib_m4/gnulib-cache.m4
FONTFILES =
PREFIXMAN1 =
man1_MANS =
man5_MANS =
man7_MANS =

# for lex/yacc
AM_YFLAGS = -d -v

SUFFIXES =

# tests launched by make check
check_SCRIPTS =
check_PROGRAMS =
TESTS =
XFAIL_TESTS =
AM_TESTS_ENVIRONMENT = \
  abs_top_srcdir=$(abs_top_srcdir) \
  abs_top_builddir=$(abs_top_builddir) \
  export abs_top_srcdir abs_top_builddir;

# We use Automake's Uniform Naming Scheme.
#
# prefixexecbin_PROGRAMS is the list of programs that may have a
# NAMEPREFIX if an existing non-GNU troff system was detected.
#
# This is done by the m4 macro GROFF_G that checks for the presence of
# GNU Troff built-in \n[.g] register.  If a Unix Troff binary was
# detected, the following programs and scripts names will have a 'g'
# prefix: chem, tbl, eqn, neqn, pic, soelim, grn, refer, lookbib,
# troff, nroff.  Also, PROG_PREFIX will be set to 'g' in the generated
# header defs.h so that these programs could be called correctly.
#
# prefixexecbindir is used to temporary install these programs.  They
# are then moved to bindir during the install-exec-hook.  Same thing
# for the scripts.
NAMEPREFIX=$(g)
prefixexecbin_PROGRAMS =
prefixexecbin_SCRIPTS =
if USEPROGRAMPREFIX
# We use datadir because the uninstall rule for the PROGRAMS attempts
# to cd into prefixexecbindir, and thus if prefixexecbindir is removed
# two consecutive uninstalls would fail
prefixexecbindir = $(datadir)
else
prefixexecbindir = $(bindir)
endif

# Path to binaries and flags used by contribs and doc to generated doc.
# These may be overridden if cross-compiling.
GROFFBIN = $(abs_top_builddir)/groff
GROFF_BIN_PATH = $(abs_top_builddir)
PDFMOMBIN = $(abs_top_builddir)/pdfmom
# Search for macro files and device/font description directories in
# source _and_ build trees; some of each are generated.
FFLAG=-F$(abs_top_builddir)/font -F$(abs_top_srcdir)/font
MFLAG=-M$(abs_top_builddir)/tmac -M$(abs_top_srcdir)/tmac

if !MAKE_DEFINES_RM
RM=rm -f
endif

# 'VERSION' is generated by gnulib script git-version-gen, using the
# command 'git describe':
#
# - From a git repository: if the current commit corresponds to a
#   tag, then 'VERSION' is simply the tag (e.g. 1.22.3).  Otherwise
#   'VERSION' has the following format:
#     <tag>-<nb_commits>-<commit>
#
#   With:
#     . tag: the most recent tag reachable from the current commit
#     . nb_commits: number of commits between the most recent tag and
#       the current commit.
#     . current commit, abbreviated.
#   For example: 1.22.3.real.434-5aafd
#   The version is stored in .version.
#
# - From a tarball, the version is taken from .tarball-version
#
# REVISION is the full revision given by git-version-gen, which can
# have non-alphanumeric symbols.
#
# Also see configure.ac for the related SHORT_VERSION macro.

MAJOR_VERSION = `echo $(VERSION) | cut -d . -f 1`
MINOR_VERSION = `echo $(VERSION) | cut -d . -f 2`
REVISION      = `echo $(VERSION) | cut -d . -f 3`

# Non-recursive makefile system.  See Automake manual '7.3 An
# Alternative Approach to Subdirectories'.  We use a single Makefile.am
# that includes other .am files, rather than using SUBDIRS.  Note that
# relative paths in the following .am files are relative to the top
# source directory.

# Initialize Automake variables we haven't already handled; see section
# "3.8 Modifying the build rules of a Gnulib import directory" in the
# Gnulib manual.
AUTOMAKE_OPTIONS =
SUBDIRS =
noinst_HEADERS =
noinst_LTLIBRARIES =
pkgdata_DATA =
MOSTLYCLEANDIRS =
AM_CFLAGS =

include $(top_srcdir)/lib/gnulib.mk
include $(top_srcdir)/arch/mingw/mingw.am
include $(top_srcdir)/arch/misc/misc.am
include $(top_srcdir)/contrib/chem/chem.am
include $(top_srcdir)/contrib/eqn2graph/eqn2graph.am
include $(top_srcdir)/contrib/gdiffmk/gdiffmk.am
include $(top_srcdir)/contrib/glilypond/glilypond.am
include $(top_srcdir)/contrib/gperl/gperl.am
include $(top_srcdir)/contrib/gpinyin/gpinyin.am
include $(top_srcdir)/contrib/grap2graph/grap2graph.am
include $(top_srcdir)/contrib/hdtbl/hdtbl.am
include $(top_srcdir)/contrib/mm/mm.am
include $(top_srcdir)/contrib/mom/mom.am
include $(top_srcdir)/contrib/pdfmark/pdfmark.am
include $(top_srcdir)/contrib/pic2graph/pic2graph.am
include $(top_srcdir)/contrib/rfc1345/rfc1345.am
include $(top_srcdir)/contrib/sboxes/sboxes.am
include $(top_srcdir)/doc/doc.am
include $(top_srcdir)/font/devX100/devX100.am
include $(top_srcdir)/font/devX100-12/devX100-12.am
include $(top_srcdir)/font/devX75/devX75.am
include $(top_srcdir)/font/devX75-12/devX75-12.am
include $(top_srcdir)/font/devascii/devascii.am
include $(top_srcdir)/font/devdvi/devdvi.am
include $(top_srcdir)/font/devhtml/devhtml.am
include $(top_srcdir)/font/devlatin1/devlatin1.am
include $(top_srcdir)/font/devlbp/devlbp.am
include $(top_srcdir)/font/devlj4/devlj4.am
include $(top_srcdir)/font/devpdf/devpdf.am
include $(top_srcdir)/font/devps/devps.am
include $(top_srcdir)/font/devutf8/devutf8.am
include $(top_srcdir)/font/scripts/scripts.am
include $(top_srcdir)/man/man.am
include $(top_srcdir)/src/include/include.am
include $(top_srcdir)/src/libs/libbib/libbib.am
include $(top_srcdir)/src/libs/libdriver/libdriver.am
include $(top_srcdir)/src/libs/libgroff/libgroff.am
include $(top_srcdir)/src/libs/libxutil/libxutil.am
include $(top_srcdir)/src/devices/grodvi/grodvi.am
include $(top_srcdir)/src/devices/grohtml/grohtml.am
include $(top_srcdir)/src/devices/grolbp/grolbp.am
include $(top_srcdir)/src/devices/grolj4/grolj4.am
include $(top_srcdir)/src/devices/gropdf/gropdf.am
include $(top_srcdir)/src/devices/grops/grops.am
include $(top_srcdir)/src/devices/grotty/grotty.am
include $(top_srcdir)/src/devices/xditview/xditview.am
include $(top_srcdir)/src/preproc/eqn/eqn.am
include $(top_srcdir)/src/preproc/grn/grn.am
include $(top_srcdir)/src/preproc/html/html.am
include $(top_srcdir)/src/preproc/pic/pic.am
include $(top_srcdir)/src/preproc/preconv/preconv.am
include $(top_srcdir)/src/preproc/refer/refer.am
include $(top_srcdir)/src/preproc/soelim/soelim.am
include $(top_srcdir)/src/preproc/tbl/tbl.am
include $(top_srcdir)/src/roff/groff/groff.am
include $(top_srcdir)/src/roff/nroff/nroff.am
include $(top_srcdir)/src/roff/troff/troff.am
include $(top_srcdir)/src/utils/addftinfo/addftinfo.am
include $(top_srcdir)/src/utils/afmtodit/afmtodit.am
include $(top_srcdir)/src/utils/grog/grog.am
include $(top_srcdir)/src/utils/hpftodit/hpftodit.am
include $(top_srcdir)/src/utils/indxbib/indxbib.am
include $(top_srcdir)/src/utils/lkbib/lkbib.am
include $(top_srcdir)/src/utils/lookbib/lookbib.am
include $(top_srcdir)/src/utils/pfbtops/pfbtops.am
include $(top_srcdir)/src/utils/tfmtodit/tfmtodit.am
include $(top_srcdir)/src/utils/xtotroff/xtotroff.am
include $(top_srcdir)/tmac/tmac.am

# Adding defs.h to 'BUILT_SOURCES' ensures that it is built by "make
# all" or "make check" before all other targets.  However, if another
# target is built from a clean build tree, for example "make groff", the
# files in 'BUILT_SOURCES' are not built first.  That is why additional
# dependencies were added in the .am files that list objects that use
# defs (for example in groff.am:
# src/roff/groff/groff.$(OBJEXT): defs.h
# ).
BUILT_SOURCES += defs.h

.DELETE_ON_ERROR:
.PRECIOUS: $(TEST_SUITE_LOG)

# if there is a name prefix we install the man pages by hand
all: generate_man_files
install-data-hook: install-prefix-man
uninstall-hook: uninstall-prefix-man
if USEPROGRAMPREFIX
generate_man_files: $(PREFIXMAN1)
install-prefix-man:
	for f in $(PREFIXMAN1); do \
	  cp -f $$f $(DESTDIR)$(man1dir)/$(NAMEPREFIX)`basename $$f`; \
	done
uninstall-prefix-man:
	for f in $(PREFIXMAN1); do \
	  rm -f $(DESTDIR)$(man1dir)/$(NAMEPREFIX)`basename $$f`; \
	done
else
man1_MANS += $(PREFIXMAN1)
install-prefix-man:
uninstall-prefix-man:
generate_man_files:
endif

# Hook to create the 'current' symlink
install-data-hook: create_current_symlink
create_current_symlink:
	cd $(DESTDIR)$(dataprogramdir); \
	  rm -f current; \
	  $(LN_S) $(SHORT_VERSION) current

# Hook to move the binaries that potentially have a prefix from
# prefixexecbindir to bindir.
install-exec-hook: move_binaries_with_prefix
move_binaries_with_prefix:
if USEPROGRAMPREFIX
	if test -n "$(NAMEPREFIX)"; then \
	   for f in $(prefixexecbin_PROGRAMS) $(prefixexecbin_SCRIPTS); do \
	      mv -f $(DESTDIR)$(prefixexecbindir)/$$f$(EXEEXT) \
	         $(DESTDIR)$(bindir)/$(NAMEPREFIX)$$f$(EXEEXT); \
	   done \
	fi
endif

# Always create the site-font directory as a guide to the user.
install-data-hook: install_site_font
install_site_font:
	-test -d $(DESTDIR)$(localfontdir) \
	  || $(mkinstalldirs) $(DESTDIR)$(localfontdir)

# Create systemtmacdir if not present
install-data-hook: install_tmacdir
install_tmacdir:
	-test -d $(DESTDIR)$(systemtmacdir) \
	  || $(mkinstalldirs) $(DESTDIR)$(systemtmacdir)

# directories specific to groff
uninstall-hook: uninstall_groffdirs
uninstall_groffdirs:
	if test -d $(DESTDIR)$(datasubdir); then \
	  rm -rf $(DESTDIR)$(fontdir); \
	  rm -rf $(DESTDIR)$(oldfontdir); \
	  rmdir $(DESTDIR)$(datasubdir); \
	fi
	if test -d $(DESTDIR)$(dataprogramdir); then \
	  if test -h $(DESTDIR)$(dataprogramdir)/current; then \
	    rm -f $(DESTDIR)$(dataprogramdir)/current; \
	  fi; \
	  if test -d $(DESTDIR)$(localfontdir); then \
	    rm -f $(DESTDIR)$(localfontdir)/*; \
	    rmdir $(DESTDIR)$(localfontdir); \
	  fi; \
	  rmdir $(DESTDIR)$(dataprogramdir); \
	fi
	if test -d $(DESTDIR)$(libprogramdir); then \
	  if test -d $(DESTDIR)$(systemtmacdir); then \
	    rm -f $(DESTDIR)$(systemtmacdir)/*; \
	    rmdir $(DESTDIR)$(systemtmacdir); \
	  fi; \
	  rmdir $(DESTDIR)$(libprogramdir); \
	fi
	if test -d $(DESTDIR)$(docdir); then \
	  if test -d $(DESTDIR)$(exampledir); then \
	    rmdir $(DESTDIR)$(exampledir); \
	  fi; \
	  if test -d $(DESTDIR)$(htmldocdir); then \
	    if test -d $(DESTDIR)$(htmldocdir)/groff.html.mono; then \
	      rmdir $(DESTDIR)$(htmldocdir)/groff.html.mono; \
	    fi; \
	    if test -d $(DESTDIR)$(htmldocdir)/groff.html.node; then \
	      rmdir $(DESTDIR)$(htmldocdir)/groff.html.node; \
	    fi; \
	    rmdir $(DESTDIR)$(htmldocdir); \
	  fi; \
	  rmdir $(DESTDIR)$(docdir); \
	fi

# Uninstall programs that have a 'g' prefix.
uninstall-hook: uninstall_binaries_with_prefix
uninstall_binaries_with_prefix:
if USEPROGRAMPREFIX
	if test -n "$(NAMEPREFIX)"; then \
	   for f in $(prefixexecbin_PROGRAMS) $(prefixexecbin_SCRIPTS); do \
	      rm -f $(DESTDIR)$(bindir)/$(NAMEPREFIX)$$f$(EXEEXT); \
	   done; \
	fi
endif

# Other files that should be present in the distribution archive.
EXTRA_DIST += \
  bootstrap \
  bootstrap.conf \
  BUG-REPORT \
  ChangeLog.old \
  ChangeLog.111 \
  ChangeLog.112 \
  ChangeLog.113 \
  ChangeLog.114 \
  ChangeLog.115 \
  ChangeLog.116 \
  ChangeLog.117 \
  ChangeLog.118 \
  ChangeLog.119 \
  ChangeLog.120 \
  ChangeLog.121 \
  ChangeLog.122 \
  FDL \
  gendef.sh \
  HACKING \
  INSTALL.REPO \
  INSTALL.extra \
  LICENSES \
  MANIFEST \
  mdate.pl \
  MORE.STUFF \
  PROBLEMS \
  PROJECTS \
  README.MinGW \
  arch/djgpp \
  font/util/make-Rproto \
  makevarescape.sed

MOSTLYCLEANFILES += $(prefixexecbin_SCRIPTS) $(bin_SCRIPTS) \
  $(man1_MANS) $(man5_MANS) $(man7_MANS) \
  $(PREFIXMAN1)

# Maintainer-mode rule to set man page time stamps to date of last
# Git commit thereto (rebasing can screw up their mtimes).
.PHONY: set-man-page-time-stamps
set-man-page-time-stamps:
	for f in $(man1_MANS) $(man5_MANS) $(man7_MANS); do \
	  g=$(top_srcdir)/$$f.man; \
	  if [ -f $$g ]; then \
	    h=$$g.in; \
	    if [ -f $$h ]; then \
	      page=$$h; \
	    else \
	      page=$$g; \
	    fi; \
	  fi; \
	  ts=`git log -1 --pretty="format:%aI" $$page`; \
	  touch -d $$ts $$page; \
	done

# Suffix rule to build .1, .5 and .7 files from .1.man, .5.man and
# .7.man files.  The brackets around the @ are used to prevent the
# substitution of the variable by Automake.
#
# The sed script transforms - to \-, ~ to \(ti, and so forth, (with an
# extra layer of backslashes--see below) so that Makefile variables
# containing ASCII characters that do not represent themselves literally
# in *roff (see groff_char(7)) are correctly interpolated into man page
# text.
#
# Note that while the script, combined with this target, will transform
# ` to \(ga, including grave accents in Makefile variables is likely to
# fail when the shell lexes the argument to echo within the old-style
# command substitution `` below.  Testing shows that grave accents
# should work* if POSIX-style command substitution $() is done instead.
# However, $() is less portable.  (Triple-escaping grave accents \\\`
# inside the interpolated Makefile variable will work, but is not
# attempted here.)
#
# Note also that the amount of backslash-escaping in the sed script is
# excessive (and incorrect) for normal purposes, but required here
# because a command substitution is being nested inside yet another
# invocation of sed.
#
# * For this target, no assurances about the good behavior of unusual
#   characters in Makefile variables in other aspects of the groff build
#   are offered.
#
# COMPATIBILITY_WRAPPERS is intended only for use in conditional
# expressions in man pages (to see if the legacy macro package wrappers
# feature is enabled) and not for expansion as formatted text.
makevarescape=$(top_srcdir)/makevarescape.sed

SUFFIXES += .man
.man:
	$(AM_V_GEN)$(MKDIR_P) `dirname $@` \
	&& LC_ALL=C \
	 sed -e "s|[@]APPDEFDIR[@]|`echo $(appdefdir) | sed -f $(makevarescape)`|g" \
	     -e "s|[@]BINDIR[@]|`echo $(bindir) | sed -f $(makevarescape)`|g" \
	     -e "s|[@]COMMON_WORDS_FILE[@]|`echo $(common_words_file) | sed -f $(makevarescape)`|g" \
	     -e "s|[@]COMPATIBILITY_WRAPPERS[@]|$(compatibility_wrappers)|g" \
	     -e "s|[@]DATASUBDIR[@]|`echo $(datasubdir) | sed -f $(makevarescape)`|g" \
	     -e "s|[@]DEFAULT_INDEX[@]|`echo $(indexdir)/$(indexname) | sed -f $(makevarescape)`|g" \
	     -e "s|[@]DEFAULT_INDEX_NAME[@]|`echo $(indexname) | sed -f $(makevarescape)`|g" \
	     -e "s|[@]DEVICE[@]|\\\\%$(DEVICE)|g" \
	     -e "s|[@]DOCDIR[@]|`echo $(docdir) | sed -f $(makevarescape)`|g" \
	     -e "s|[@]EXAMPLEDIR[@]|`echo $(exampledir) | sed -f $(makevarescape)`|g" \
	     -e "s|[@]FONTDIR[@]|`echo $(fontdir) | sed -f $(makevarescape)`|g" \
	     -e "s|[@]g[@]|\\\\%$(g)|g" \
	     -e "s|[@]HTMLDOCDIR[@]|`echo $(htmldocdir) | sed -f $(makevarescape)`|g" \
	     -e "s|[@]INDEX_SUFFIX[@]|\\\\%$(indexext)|g" \
	     -e "s|[@]LEGACYFONTDIR[@]|`echo $(legacyfontdir) | sed -f $(makevarescape)`|g" \
	     -e "s|[@]LOCALFONTDIR[@]|`echo $(localfontdir) | sed -f $(makevarescape)`|g" \
	     -e "s|[@]LOCALMACRODIR[@]|`echo $(localtmacdir) | sed -f $(makevarescape)`|g" \
	     -e "s|[@]MACRODIR[@]|`echo $(tmacdir) | sed -f $(makevarescape)`|g" \
	     -e "s|[@]MAN1EXT[@]|$(man1ext)|g" \
	     -e "s|[@]MAN5EXT[@]|$(man5ext)|g" \
	     -e "s|[@]MAN7EXT[@]|$(man7ext)|g" \
	     -e "s|[@]MDATE[@]|`$(PERL) $(top_srcdir)/mdate.pl $<`|g" \
	     -e "s|[@]OLDFONTDIR[@]|`echo $(oldfontdir) | sed -f $(makevarescape)`|g" \
	     -e "s|[@]PAGE[@]|\\\\%$(PAGE)|g" \
	     -e "s|[@]PDFDOCDIR[@]|`echo $(pdfdocdir) | sed -f $(makevarescape)`|g" \
	     -e "s|[@]PSPRINT[@]|`echo $(PSPRINT) | sed -f $(makevarescape)`|g" \
	     -e "s|[@]SYSTEMMACRODIR[@]|`echo $(systemtmacdir) | sed -f $(makevarescape)`|g" \
	     -e "s|[@]TMAC_AN_PREFIX[@]|\\\\%$(tmac_an_prefix)|g" \
	     -e "s|[@]TMAC_M_PREFIX[@]|\\\\%$(tmac_m_prefix)|g" \
	     -e "s|[@]TMAC_MDIR[@]|`echo $(tmacdir) | sed -f $(makevarescape)`/mm|g" \
	     -e "s|[@]TMAC_S_PREFIX[@]|\\\\%$(tmac_s_prefix)|g" \
	     -e "s|[@]VERSION[@]|$(VERSION)|g" \
	     $< \
	     >$@

# Version files - see script "build-aux/git-version-gen".
EXTRA_DIST += $(top_srcdir)/.version
BUILT_SOURCES += $(top_srcdir)/.version
$(top_srcdir)/.version:
	echo $(VERSION) > $@-t && mv $@-t $@
dist-hook:
	echo $(VERSION) > $(distdir)/.tarball-version

# Local Variables:
# fill-column: 72
# mode: makefile-automake
# End:
# vim: set autoindent filetype=automake textwidth=72:
