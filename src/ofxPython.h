#pragma once
#include "ofTypes.h"

extern "C"{
#include <Python.h>
}

class ofxPythonObject;

class ofxPython
{
public:
	ofxPython();
	virtual ~ofxPython();
	void init();
	ofxPythonObject getObject(const string& name, const string& module);
protected:

};

class ofxPythonObjectManaged;

class ofxPythonObject: public ofPtr<ofxPythonObjectManaged>
{
public:
	ofxPythonObject method(const string &method_name); //call method without arguments
	ofxPythonObject operator ()(); //call no arguments
protected:
	void insert(PyObject *);
	void insert_noaddref(PyObject *);
	friend class ofxPython;
};

class ofxPythonObjectManaged
{
public:
	ofxPythonObjectManaged(PyObject*);
	~ofxPythonObjectManaged();
	PyObject * obj;
};