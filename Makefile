#------------------------------------------------------------------------------
#    getw Makefile
#
#    (2011)  Alexandre Dantas (kure) <alex.dantas92@gmail.com>
#
#    Commandlines:
#        V       Verbose mode, off by default.
#                To turn on for the current command, add 'V=1' on the
#                commandline OR uncomment the line specified below for
#                verbose on every command.
#        DESTDIR Installs the package on a custom root directory (other than /)
#        PREFIX  Installs the package on a custom directory (overwrites root)
#        CC      Changes the C flags used on compilation
#        CDEBUG  If you wish to build on debug mode, add CDEBUG=-g
#        
#        Targets:
#                all:        Compiles the binary
#                clean:      Removes the binary and the *.o files
#                install:    Installs the package
#                uninstall:  Uninstalls the package
#                dist:       Creates the 'tarball' for distribution
#
#                newversion: Creates a new directory above with a new specified
#                            version. For example: 'make newversion VERSION=1.2'
#                run:        Compiles and runs the binary
#                dox:        Generates doxygen documentation
#                doxclean:   Removes the doxygen documentation
#------------------------------------------------------------------------------

#	Uncomment to tun on the verbose mode
#V	= 1;

SHELL	= /$(LBIN)/sh

#-------General Info------------------------------------------------------------
PACKAGE = getw
VERSION = 1.0
DATE	= Dec2011

#-------Local build information-------------------------------------------------
LBIN    = bin
LOBJ    = obj
LDOC    = doc
LSRC    = src
LFILES  = ChangeLog COPYING Doxyfile INSTALL Makefile README TODO

#-------Install folders---------------------------------------------------------
DESTDIR =
PREFIX	= $(DESTDIR)/usr/local

DATAROOTDIR = $(PREFIX)/share
EXEC_PREFIX = $(PREFIX)

BINDIR	= $(EXEC_PREFIX)/$(LBIN)
DOCDIR	= $(DATAROOTDIR)/$(LDOC)/$(PACKAGE)

#-------Configure---------------------------------------------------------------
LOCALSTATEDIR	=

#-------Compile-----------------------------------------------------------------
CC	= gcc
EXE	= getw
DEBUG	=
CFLAGS	= $(DEBUG) -Wall -Wextra -O2
LIBS	=
OBJ	= $(LOBJ)/main.o      \
      $(LOBJ)/arguments.o \
      $(LOBJ)/network.o   \
      $(LOBJ)/text.o      \
      $(LOBJ)/file.o

DEFINES	= -DVERSION=\"$(VERSION)\"  \
          -DPACKAGE=\"$(PACKAGE)\"  \
          -DDATE=\"$(DATE)\"

INSTALL = install -s

#-------Distribute--------------------------------------------------------------
DISTDIR = $(PACKAGE)-$(VERSION)
TARNAME = $(DISTDIR).tar.gz

#-------Verbose Mode------------------------------------------------------------

ifdef V
MUTE =
VTAG = -v
else
MUTE = @
endif

#-------Standard Makes----------------------------------------------------------

all: $(EXE)
	@echo "* Build completed!"

install: all
	@echo "* Installing..."
	$(MUTE)install -d --mode=755 $(BINDIR)
	$(MUTE)install --mode=755 $(LBIN)/$(EXE) $(BINDIR)
	@echo "**********************************"
	@echo "* Program successfuly installed! *"
	@echo "**********************************"

uninstall:
	@echo "* Uninstalling..."
	$(MUTE)rm -f $(BINDIR)/$(EXE)

#	To make the executable file
$(EXE): $(OBJ)
	@echo "* Linking..."
	$(MUTE)$(CC) $(OBJ) -o $(LBIN)/$(EXE) $(LIBS)

#	All the object files
$(LOBJ)/%.o: $(LSRC)/%.c
	@echo "* Compiling $<..."
	$(MUTE)$(CC) $(CFLAGS) $< -c -o $@ $(DEFINES)

#	Make the 'tarball'
dist: $(TARNAME)


$(TARNAME): $(DISTDIR)
	$(MUTE)tar czf $(DISTDIR).tar.gz $(DISTDIR)
	$(MUTE)rm -rf $(DISTDIR)
	$(MUTE)cp $(DISTDIR).tar.gz ..
	$(MUTE)rm -f $(DISTDIR).tar.gz

$(DISTDIR):
	$(MUTE)mkdir -p $(DISTDIR)/$(LSRC) $(DISTDIR)/$(LDOC)
	$(MUTE)mkdir -p $(DISTDIR)/$(LBIN) $(DISTDIR)/$(LOBJ)
	-$(MUTE)cp $(LFILES) -t $(DISTDIR)
	-$(MUTE)cp -r $(LSRC)/* $(DISTDIR)/$(LSRC)
	-$(MUTE)cp -r $(LBIN)/* $(DISTDIR)/$(LBIN)
	-$(MUTE)rm -f $(LDOC)/$(PACKAGE)\ documentation
	-$(MUTE)cp -r $(LDOC)/* $(DISTDIR)/$(LDOC)

# Creates a new directory above with a new specified version.
# Remember to change the version manually on the new
# Makefile after doing this!
newversion: dist
	$(MUTE)tar $(VTAG) -xzf ../$(TARNAME)
	mv $(DESTDIR) ../

#-------Phonys-----------------------------------------------------------------
run: all
	@echo "* Running..."
	$(MUTE)./$(LBIN)/$(EXE)

clean:
	@echo "* Cleaning..."
	$(MUTE)rm $(VTAG) -f $(LOBJ)/*.o
	$(MUTE)rm $(VTAG) -f $(LBIN)/*

# Documents and creates a soft-link to ease documentation viewing
dox:
	@echo "* Documenting..."
	$(MUTE)doxygen Doxyfile
	-$(MUTE)ln $(VTAG) -s html/index.html $(LDOC)/$(PACKAGE)\ documentation

doxclean:
	@echo "* Removing documentation..."
	$(MUTE)rm $(VTAG) -rf $(LDOC)/html
	$(MUTE)rm $(VTAG) -rf $(LDOC)/latex
	$(MUTE)rm $(VTAG) -rf $(LDOC)/$(PACKAGE)\ documentation

.PHONY: clean doxy backup uninstall

#------------------------------------------------------------------------------

