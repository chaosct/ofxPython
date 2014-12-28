#pragma once
#include "ofTypes.h"

extern "C"{
#include <Python.h>
}

class ofxPythonObjectManaged;
class ofxPythonMappingValue;
class ofxPythonAttrValue;

class ofxPythonObject: public ofPtr<ofxPythonObjectManaged>
{
public:
	ofxPythonObject method(const string &method_name); //call method without arguments
	ofxPythonObject operator ()(); //call objects without arguments
	ofxPythonAttrValue attr(const string& attribute);
	ofxPythonMappingValue operator [](const string& key);
	const string repr();
protected:
	void insert(PyObject *);
	void insert_noaddref(PyObject *);
	friend ofxPythonObject make_object_noaddref(PyObject * obj);
	friend ofxPythonObject make_object_addref(PyObject * obj);
	friend class ofxPython;
};

class ofxPython
{
public:
	ofxPython();
	virtual ~ofxPython();
	void init();
	void reset();
	void executeScript(const string& path);
	void executeString(const string& script);
	ofxPythonObject evalString(const string& expression);
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

class ofxPythonMappingValue
{
	ofxPythonObject object;
	string key;
public:
	ofxPythonMappingValue(ofxPythonObject o, const string& k);
	operator ofxPythonObject();
	ofxPythonMappingValue & operator =(ofxPythonObject);
	ofxPythonMappingValue & operator =(ofxPythonMappingValue & o);
};

class ofxPythonAttrValue
{
	ofxPythonObject object;
	string attribute;
public:
	ofxPythonAttrValue(ofxPythonObject o, const string& attr);
	operator ofxPythonObject();
	ofxPythonAttrValue & operator =(ofxPythonObject);
	ofxPythonAttrValue & operator =(ofxPythonAttrValue & o);
};