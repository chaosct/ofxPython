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
	ofxPythonObject operator ()(ofxPythonObject); //call objects 1 argument
	ofxPythonObject operator ()(ofxPythonObject, ofxPythonObject); //call objects 2 arguments
	ofxPythonObject operator ()(ofxPythonObject, ofxPythonObject, ofxPythonObject); //call objects 3 arguments
	ofxPythonAttrValue attr(const string& attribute);
	ofxPythonMappingValue operator [](const string& key);
	ofxPythonMappingValue operator [](const char * key);
	bool isNone() const;
	bool isBool() const;
	bool isInt() const;
	bool isFloat() const;
	bool isString() const;
	bool asBool( ) const;
	long int asInt() const;
	double asFloat() const;
	string asString() const;
	static ofxPythonObject _None();
	static ofxPythonObject fromBool(bool);
	static ofxPythonObject fromInt(long int);
	static ofxPythonObject fromFloat(double);
	static ofxPythonObject fromString(const string&);
	operator bool() const;
	const string repr();
protected:
	void insert_borrowed(PyObject *);
	void insert_owned(PyObject *);
	friend ofxPythonObject make_object_owned(PyObject * obj, bool);
	friend ofxPythonObject make_object_borrowed(PyObject * obj, bool);
	friend class ofxPython;
};

class ofxPython
{
public:
	ofxPython();
	virtual ~ofxPython();
	int init(); //returns the number of ofxPython instances,
				//if it's 1 you should initialize your custom bindings if any
	void reset();
	void executeScript(const string& path);
	void executeString(const string& script);
	ofxPythonObject evalString(const string& expression);
	ofxPythonObject getObject(const string& name, const string& module);
	ofxPythonObject getObject(const string& name);
protected:
	// ofxPythonObject globals;
	ofxPythonObject locals;
	static unsigned int instances;
	bool initialized;
};



class ofxPythonObjectManaged
{
public:
	ofxPythonObjectManaged(PyObject*);
	~ofxPythonObjectManaged();
	PyObject * obj;
};

class ofxPythonObjectLike
{
public:
	ofxPythonObject method(const string &method_name); //call method without arguments
	ofxPythonObject operator ()(); //call objects without arguments
	ofxPythonObject operator ()(ofxPythonObject); //call objects 1 argument
	ofxPythonObject operator ()(ofxPythonObject, ofxPythonObject); //call objects 2 arguments
	ofxPythonObject operator ()(ofxPythonObject, ofxPythonObject, ofxPythonObject); //call objects 3 arguments
	ofxPythonAttrValue attr(const string& attribute);
	ofxPythonMappingValue operator [](const string& key);
	ofxPythonMappingValue operator [](const char * key);
	bool isNone();
	bool isBool();
	bool isInt();
	bool isFloat();
	bool isString();
	bool asBool();
	long int asInt();
	double asFloat();
	string asString();
	operator bool();
	const string repr();
	virtual operator ofxPythonObject()=0;
};

class ofxPythonMappingValue: public ofxPythonObjectLike
{
	ofxPythonObject object;
	string key;
public:
	ofxPythonMappingValue(ofxPythonObject o, const string& k);
	operator ofxPythonObject();
	ofxPythonMappingValue & operator =(ofxPythonObject);
	ofxPythonMappingValue & operator =(ofxPythonMappingValue & o);
};

class ofxPythonAttrValue: public ofxPythonObjectLike
{
	ofxPythonObject object;
	string attribute;
public:
	ofxPythonAttrValue(ofxPythonObject o, const string& attr);
	operator ofxPythonObject();
	ofxPythonAttrValue & operator =(ofxPythonObject);
	ofxPythonAttrValue & operator =(ofxPythonAttrValue & o);
};
