#
# Makefile for Project usbwall in .
#
# Made by David Fernandes
# Login   <dfernand@ece.fr>
#
# Started on  Tue Jan  10 14:49:06 2012 David FERNANDES
# 

#############################################################################
## basic macros
#############################################################################
CC           = gcc
AR           = ar
CFLAGS       = -Wall -W -Werror
LD           = ld
LDFLAGS      = -lusbwall
ARFLAGS      = cr
RM           = rm
RMFLAGS      = -rf
TAR          = tar
TARFLAGS     = -cjvf
MAKE        ?= make
INSTALL      = install
CTAGS        = ctags -x > tags
DEPEND       = makedepend $(CFLAGS)
MAKE         = make

#############################################################################
## lint -- static code mistakes detector
#############################################################################
LINT         = splint
LARCH_PATH   = /usr/local/lib
LCLIMPORTDIR = /usr/local/share/splint/imports

#############################################################################
## dist target file
#############################################################################
DISTTARGET   = .tar.bz2

#############################################################################
## project's source and generated files
#############################################################################
TARGET_CREATE    = create_white_list
SOURCE_CREATE    = create_white_list.c
OBJS_CREATE         = $(SOURCE_CREATE:.c=.o)

TARGET_LOAD       = load_white_list
SOURCE_LOAD       =  load_white_list.c
OBJS_LOAD         = $(SOURCE_LOAD:.c=.o)

TODEL        = tags *~ .*.swp *.o *~ 

#############################################################################
## rules
#############################################################################

all : $(TARGET_CREATE) $(TARGET_LOAD)

$(TARGET_CREATE) : $(OBJS_CREATE)
	$(CC) -o $@ $(OBJS_CREATE) $(LDFLAGS)

$(TARGET_LOAD) : $(OBJS_LOAD)
	$(CC) -o $@ $(OBJS_LOAD) $(LDFLAGS)

.c.o :
	$(CC) $(CFLAGS) -c $<

doc :
	$(MAKE) -C doc all

lint :
	$(LINT) $(CFLAGS) $(SOURCE)

tags : $(SOURCE)
	$(CTAGS) $(SOURCE)

depend : $(SOURCE)
	$(DEPEND) $(SOURCE)

.PHONY : clean doc

clean : 
	$(RM) $(RMFLAGS) $(OBJS_LOAD) $(OBJS_CREATE) $(TODEL)

distclean : clean
	$(RM) $(RMFLAGS) $(TARGET)

dist : distclean
	$(TAR) $(TARFLAGS) $(DISTTARGET) .

check : $(TARGET)
	cd check; $(MAKE) all

install : $(TARGET)
	@echo you must be root to install
