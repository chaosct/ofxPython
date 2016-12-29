/*
    SWIG (http://www.swig.org) interface wrapper for the additional OpenFrameworks Operations

    (Python) Creates an "openframeworks_extra" module
    
    2015 Carles F. Julià <carles@protopixel.net>
*/

//workaround when compiling Python in MinGW
%begin %{
#if defined( __WIN32__ ) || defined( _WIN32 )
#include <cmath>
#endif
%}

%module openframeworks_extra

// ----- CALLBACKS -------------------------------------------------------------

%{

#include "ofxPythonCallBackBase.h"

%}

%include "ofxPythonCallBackBase.h"

%{

    CallBack * global_CallBack = NULL;

    void _setCallBackPointer(CallBack* c)
    {
        global_CallBack = c;
    }

%}

%inline %{
    CallBack * _getCallBackPointer()
    {
        return global_CallBack;
    }

%}

%extend CallBack{
%pythoncode %{
    def call(self,*args,**kwargs):
        CallBack._args = args
        CallBack._kwargs = kwargs
        self._call()
        CallBack._args = None
        CallBack._kwargs = None
%}
}
