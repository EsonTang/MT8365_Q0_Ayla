$(error Makefile disabled because config might not support this example)
# Customer ID=13943; Build=0x75f5e; Copyright (c) 2005-2013 Tensilica Inc.  ALL RIGHTS RESERVED.

# These coded instructions, statements, and computer programs are the
# copyrighted works and confidential proprietary information of
# Tensilica Inc.  They may be adapted and modified by bona fide
# purchasers for internal use, but neither the original nor any adapted
# or modified version may be disclosed or distributed to third parties
# in any manner, medium, or form, in whole or in part, without the prior
# written consent of Tensilica Inc.

EXAMPLE_NAME = pif2sb_bridge$(EXE)
HOST_OBJECTS = \
	sc_main.$(OBJ) \
	pif2sb_bridge.$(OBJ) \
	simple_bus_types.$(OBJ) \
	simple_bus.$(OBJ) \
	simple_bus_arbiter.$(OBJ)

TARGET_OBJECTS = target/hello.out
NO_TIE = 1
EXAMPLE_DIR = $(CURDIR)

xtsc-run: xtsc-run.not.supported

include $(EXAMPLE_DIR)/../Makefile.include
include $(EXAMPLE_DIR)/../Makefile.common

VPATH += $(EXAMPLE_DIR)/simple_bus

HOST_SOURCES = \
	sc_main.cpp \
	pif2sb_bridge.cpp \
	simple_bus/simple_bus_types.cpp \
	simple_bus/simple_bus.cpp \
	simple_bus/simple_bus_arbiter.cpp

