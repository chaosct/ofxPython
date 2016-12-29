# All variables and this file are optional, if they are not present the PG and the
# makefiles will try to parse the correct values from the file system.
#
# Variables that specify exclusions can use % as a wildcard to specify that anything in
# that position will match. A partial path can also be specified to, for example, exclude
# a whole folder from the parsed paths from the file system
#
# Variables can be specified using = or +=
# = will clear the contents of that variable both specified from the file or the ones parsed
# from the file system
# += will add the values to the previous ones in the file or the ones parsed from the file 
# system
# 
# The PG can be used to detect errors in this file, just create a new project with this addon 
# and the PG will write to the console the kind of error and in which line it is

meta:
	ADDON_NAME = ofxPython
	ADDON_DESCRIPTION = Addon to embed Python into openFrameworks
	ADDON_AUTHOR = Carles F. Julia
	ADDON_TAGS = "Scripting" "embedding" "Python"
	ADDON_URL = http://github.com/chaosct/ofxPython

common:
	# dependencies with other addons, a list of them separated by spaces 
	# or use += in several lines
	# ADDON_DEPENDENCIES =
	
	# include search paths, this will be usually parsed from the file system
	# but if the addon or addon libraries need special search paths they can be
	# specified here separated by spaces or one per line using +=
	ADDON_INCLUDES += src
	
	# any special flag that should be passed to the compiler when using this
	# addon
	# ADDON_CFLAGS =
	
	# any special flag that should be passed to the linker when using this
	# addon, also used for system libraries with -lname
	# ADDON_LDFLAGS =
	
	# linux only, any library that should be included in the project using
	# pkg-config
	ADDON_PKG_CONFIG_LIBRARIES = python
	
	# osx/iOS only, any framework that should be included in the project
	# ADDON_FRAMEWORKS =
	
	# source files, these will be usually parsed from the file system looking
	# in the src folders in libs and the root of the addon. if your addon needs
	# to include files in different places or a different set of files per platform
	# they can be specified here
	# ADDON_SOURCES = 
	
	# some addons need resources to be copied to the bin/data folder of the project
	# specify here any files that need to be copied, you can use wildcards like * and ?
	ADDON_DATA = lib/openframeworks.py
	ADDON_DATA += lib/openframeworks_extra.py
	
	# when parsing the file system looking for libraries exclude this for all or
	# a specific platform
	# ADDON_LIBS_EXCLUDE =
	
linux64:
	ADDON_DATA = lib/openframeworks.py
	ADDON_DATA += lib/openframeworks_extra.py
	ADDON_CFLAGS += -I/usr/include/python2.7
	ADDON_LDFLAGS += -lpython2.7
    ADDON_SOURCES_EXCLUDE += src/bindings/linuxarm/%
    ADDON_INCLUDES_EXCLUDE += src/bindings/linuxarm/%

linux:
	ADDON_DATA = lib/openframeworks.py
	ADDON_DATA += lib/openframeworks_extra.py
	ADDON_CFLAGS += -I/usr/include/python2.7
	ADDON_LDFLAGS += -lpython2.7
    ADDON_SOURCES_EXCLUDE += src/bindings/linuxarm/%
    ADDON_INCLUDES_EXCLUDE += src/bindings/linuxarm/%

linuxarmv6l:
    ADDON_DATA = lib/openframeworks.py
    ADDON_DATA += lib/openframeworks_extra.py
    ADDON_CFLAGS += -I/usr/include/python2.7
    ADDON_LDFLAGS += -lpython2.7
    ADDON_SOURCES_EXCLUDE += src/bindings/desktop/%
    ADDON_INCLUDES_EXCLUDE += src/bindings/desktop/%
    ADDON_LIBS_EXCLUDE += libs

linuxarmv7l:
    ADDON_DATA = lib/openframeworks.py
    ADDON_DATA += lib/openframeworks_extra.py
    ADDON_CFLAGS += -I/usr/include/python2.7
    ADDON_LDFLAGS += -lpython2.7
    ADDON_SOURCES_EXCLUDE += src/bindings/desktop/%
    ADDON_INCLUDES_EXCLUDE += src/bindings/desktop/%
    ADDON_LIBS_EXCLUDE += libs

win_cb:
	ADDON_SOURCES += src/ofxPython.cpp src/ofxPythonCallBack.cpp
	ADDON_SOURCES += src/bindings/desktop/openFrameworks_wrap.cpp
	ADDON_SOURCES += src/bindings/openFrameworks_extra_wrap.cpp
	ADDON_DATA = lib/openframeworks.py
	ADDON_DATA += lib/openframeworks_extra.py
	ADDON_CFLAGS += -IC:/Python27/include
	ADDON_LDFLAGS += -LC:/Python27/libs -lpython
    ADDON_SOURCES_EXCLUDE += src/bindings/linuxarm/%
    ADDON_INCLUDES_EXCLUDE += src/bindings/linuxarm/%

android/armeabi:
	
android/armeabi-v7a:	

osx:
	ADDON_CFLAGS += -I/System/Library/Frameworks/Python.framework/Versions/2.7/include/python2.7
	ADDON_LDFLAGS += -lpython2.7 -L/System/Library/Frameworks/Python.framework/Versions/2.7/lib/python2.7/config
    ADDON_SOURCES_EXCLUDE += src/bindings/linuxarm/%
    ADDON_INCLUDES_EXCLUDE += src/bindings/linuxarm/%