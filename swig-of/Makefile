#
# Makefile to generate OF bindings using SWIG
#
# 2014 Dan Wilcox <danomatika@gmail.com>
#
# running `make` generates desktop os lua bindings and puts them in ../src,
# running `make ios` generates ios lua bindings, etc
#
# override any of the following variables using make,
# i.e. to generate Python bindings with a different
# filename and dest location:
#
#     make LANG=python NAME=ofxPythonBindings DEST_DIR=../src/bindings
#

# swig command
SWIG = swig

# default output language, see swig -h for more
LANG = lua

# module name
MODULE_NAME = of

# strip "of" from function, class, & define/enum names?
RENAME = true

# default platform target, available targets are:
#   * desktop: win, linux, & mac osx
#   * ios: apple iOS using OpenGL ES
#   * linuxarm: embedded linux using OpenGL ES
TARGET = desktop

# generated bindings filename
NAME = openFrameworks_wrap

# where to copy the generated bindings
DEST_DIR = ../src/bindings

# where to copy the generated specific language files
DEST_LANG_DIR = .

# path to OF headers
OF_HEADERS = ../../../libs/openFrameworks

# Python specific preferences
# typically, long names are used in Python,
# and function names remain unaltered (see pyOpenGL for instance)
ifeq ($(LANG), python)
	MODULE_NAME = openframeworks
	RENAME = false
endif

ifeq ($(RENAME), true)
	RENAME_CFLAGS = -DOF_SWIG_RENAME
else
	RENAME_CFLAGS = 
endif

CFLAGS = -I$(OF_HEADERS) -DMODULE_NAME=$(MODULE_NAME) $(RENAME_CFLAGS) -DTARGET_OSX

.PHONY: all debug clean desktop ios linuxarm

# generates bindings
bindings:

	@echo Generating for: $(TARGET)
	@echo LANG = $(LANG)
	@echo CFLAGS = $(CFLAGS)
	@echo NAME = $(NAME)
	@echo DEST_DIR = $(DEST_DIR)
	
	$(SWIG) -c++ -$(LANG) -fcompact -fvirtual $(CFLAGS) -outdir $(DEST_LANG_DIR) openFrameworks.i
	mv openFrameworks_wrap.cxx $(NAME).cpp

	$(SWIG) -c++ -$(LANG) -external-runtime $(NAME).h

# move generated files to DEST_DIR
move:
	mkdir -p $(DEST_DIR)/$(TARGET)
	mv *.h $(DEST_DIR)
	mv *.cpp $(DEST_DIR)/$(TARGET)

# outputs debug symbols
debug:
	@echo Debugging
	$(SWIG) -c++ -$(LANG) -debug-lsymbols $(CFLAGS) openFrameworks.i > debug.txt
	rm -f *.cxx
	if [ $(LANG) == "python" ]; then rm -f *.py; fi 

clean:
	rm -f $(DEST_DIR)/$(FILENAME).h
	rm -rf $(DEST_DIR)/desktop
	rm -rf $(DEST_DIR)/ios
	rm -f debug.txt

# desktop OS generation
desktop-prepare:
	$(eval TARGET := desktop)
	$(eval CFLAGS := $(CFLAGS))

desktop: desktop-prepare bindings move

# iOS specific generation
ios-prepare:
	$(eval TARGET := ios)
	$(eval CFLAGS := $(CFLAGS) -DTARGET_OPENGLES -DTARGET_IOS)

ios: ios-prepare bindings move

# embedded linux specific generation
linuxarm-prepare:
	$(eval TARGET := linuxarm)
	$(eval CFLAGS := $(CFLAGS) -DTARGET_OPENGLES)

linuxarm: linuxarm-prepare bindings move
