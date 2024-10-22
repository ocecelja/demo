#-------------------------------------------------------------------------------
#
#  Name:
#    common.mak
#
#  Description:
#    Makefile to build the pkgs/comon package 
#
#   The following make targets are available in this makefile:
#
#     all           - make .o and .a image files (default)
#                     Test programs are also built when present
#     clean         - delete object directory and image files
#     amtest        - builds the test program if available
#
#   The above targets can be made with the following command:
#
#     make -f common.mak [CPU={i386, arm9}] [target]
#       defaults to i386 if CPU is omitted
#
# Copyright (c) 2011 by Sierra Wireless, Incorporated.  All Rights Reserved.
#-------------------------------------------------------------------------------

#-------------------------------------------------------------------------------
# Local Settings-
#
#-------------------------------------------------------------------------------
PKG  = common
SRCS = switype_256bit.c dev_util.c str_util.c
OBJS = $(OBJSDIR)/switype_256bit.o $(OBJSDIR)/dev_util.o $(OBJSDIR)/str_util.o
HDRS = switype_256bit.h dev_util.h str_util.h

COMMONOBJ = $(OBJSDIR)/switype_256bit.o $(OBJSDIR)/dev_util.o $(OBJSDIR)/str_util.o
SDKOBJ = $(OBJSDIR)/switype_256bit.o $(OBJSDIR)/dev_util.o $(OBJSDIR)/str_util.o
SLQSAPIOBJ  = $(OBJSDIR)/switype_256bit.o $(OBJSDIR)/dev_util.o $(OBJSDIR)/str_util.o

# LIB and OBJ files required by $(PKG) test program
TESTLIBS = $(OBJS) $(CORELIB) $(SDKLIB)
#TESTFLAGS = -lrt -lpthread

#-------------------------------------------------------------------------------
# Common Rules & Settings
#-------------------------------------------------------------------------------
include ../gen.mak
