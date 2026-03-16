#
# Makefile to generate extra OF bindings using SWIG
#
# 2015 Carles F. Julià <carles@protopixel.net>
# 2014 Dan Wilcox <danomatika@gmail.com>
#
# running `make` generates bindings and puts them in ../src,
#
# override any of the following variables using make,
# i.e. to generate bindings with a different
# dest location:
#
#     make DEST_DIR=../src/bindings
#

# swig command
SWIG = swig

# generated bindings filename
NAME = openFrameworks_extra_wrap

# where to copy the generated bindings
DEST_DIR = ../src/bindings

# where to copy the generated specific language files
DEST_LANG_DIR = ../lib

# path to OF headers
OF_HEADERS = ../../../libs/openFrameworks

# C & C++ flags
CFLAGS = -I$(OF_HEADERS) -I../src
CXXFLAGS = $(CFLAGS)

.PHONY: all debug clean

desktop: bindings move

# generates bindings
bindings:

	@echo Generating extra bindings:
	@echo CXXFLAGS = $(CXXFLAGS)
	@echo NAME = $(NAME)
	@echo DEST_DIR = $(DEST_DIR)
	
	$(SWIG) -c++ -python -fcompact -fvirtual $(CXXFLAGS) -outdir $(DEST_LANG_DIR) openframeworks_extra.i
	mv openframeworks_extra_wrap.cxx $(NAME).cpp

	$(SWIG) -c++ -python -external-runtime $(NAME).h
 
# move generated files to DEST_DIR
move:

	mkdir -p $(DEST_DIR)
	mv *.h $(DEST_DIR)
	mv *.cpp $(DEST_DIR)

# outputs debug symbols
debug:
	$(SWIG) -c++ -$(LANG) -debug-lsymbols $(CXXFLAGS) openframeworks_extra.i > debug.txt
	rm -f *.cxx

clean:
	rm -f $(DEST_DIR)/$(NAME).h
	rm -rf $(DEST_DIR)/$(NAME).cpp
	rm -f debug.txt




