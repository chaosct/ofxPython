#pragma once
#include "ofTypes.h"

extern "C"{
#include <Python.h>
}

class ofxPythonObjectManaged;

class ofxPythonObject: public ofPtr<ofxPythonObjectManaged>
{
public:
	ofxPythonObject method(const string &method_name); //call method without arguments
	ofxPythonObject operator ()(); //call no arguments
	ofxPythonObject operator [](const string& key);

protected:
	void insert(PyObject *);
	void insert_noaddref(PyObject *);
	friend ofxPythonObject make_object_noaddref(PyObject * obj);
	friend class ofxPython;
};

class ofxPython
{
public:
	ofxPython();
	virtual ~ofxPython();
	void init();
	void reset();
	ofxPythonObject executeScript(const string& path);
	ofxPythonObject executeString(const string& script);
	ofxPythonObject getObject(const string& name, const string& module);
	ofxPythonObject getObject(const string& name);
protected:
	// ofxPythonObject globals;
	ofxPythonObject locals;
};



class ofxPythonObjectManaged
{
public:
	ofxPythonObjectManaged(PyObject*);
	~ofxPythonObjectManaged();
	PyObject * obj;
};