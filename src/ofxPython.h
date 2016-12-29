#pragma once
#include "ofTypes.h"

extern "C"{
#include <Python.h>
}

#include <map>

class ofxPythonMappingValue;
class ofxPythonAttrValue;
class ofxPythonTupleMaker;
class ofxPythonListMaker;

class ofxPythonObject
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
	bool isList() const;
	bool isTuple() const;
	bool isDict() const;
	bool asBool( ) const;
	long int asInt() const;
	double asFloat() const;
	string asString() const;
	vector<ofxPythonObject> asVector() const;
	std::map<ofxPythonObject,ofxPythonObject> asMap() const;
	static ofxPythonObject _None();
	static ofxPythonObject fromBool(bool);
	static ofxPythonObject fromInt(long int);
	static ofxPythonObject fromFloat(double);
	static ofxPythonObject fromString(const string&);
	// use makeTuple like this:
	// ofxPythonObject tuple = ofxPythonObject::makeTuple() << obj1 << obj2 << obj3;
	static ofxPythonTupleMaker makeTuple();
    static ofxPythonListMaker makeList();
    static ofxPythonObject makeDict();
	operator bool() const;
	const string repr();
	const string str();
protected:
	void insert_borrowed(PyObject *);
	void insert_owned(PyObject *);
    class ofxPythonObjectManaged
    {
    public:
        ofxPythonObjectManaged(PyObject*);
        ~ofxPythonObjectManaged();
        PyObject * obj;
    };
    ofPtr<ofxPythonObjectManaged> data;
    friend class ofxPython;
    friend class ofxPythonMappingValue;
    friend class ofxPythonAttrValue;
    friend class ofxPythonListMaker;
    friend class ofxPythonTupleMaker;
    friend ofxPythonObject make_object_owned(PyObject * obj, bool);
    friend ofxPythonObject make_object_borrowed(PyObject * obj, bool);
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
    ofxPythonObject executeStatement(const string& script);
	ofxPythonObject evalString(const string& expression);
	static ofxPythonObject getObject(const string& name, const string& module);
	ofxPythonObject getObject(const string& name);
	ofxPythonObject getObjectOrNone(const string& name);
	void setObject(const string& name, ofxPythonObject o);
    ofxPythonObject getLocals();
    void setVirtualEnv(const string & path);
    void addPath(const string & path);
protected:
    ofxPythonObject locals;
	// ofxPythonObject globals;
	static unsigned int instances;
	bool initialized;
    friend class ofxPythonObject::ofxPythonObjectManaged;
};

class ofxPythonOperation
{
public:
    static PyThreadState * pstate;
    static unsigned int instances;
    ofxPythonOperation();
    ~ofxPythonOperation();
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
	bool isList();
	bool isTuple();
	bool isDict();
	bool asBool();
	long int asInt();
	double asFloat();
	string asString();
	vector<ofxPythonObject> asVector();
	std::map<ofxPythonObject,ofxPythonObject> asMap();
	operator bool();
	const string repr();
	const string str();
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

class ofxPythonTupleMaker: public ofxPythonObjectLike
{
	vector<ofxPythonObject> contents;
public:
	ofxPythonTupleMaker& operator<<(ofxPythonObject);
	operator ofxPythonObject();
};

class ofxPythonListMaker: public ofxPythonObjectLike
{
    vector<ofxPythonObject> contents;
public:
    ofxPythonListMaker& operator<<(ofxPythonObject);
    operator ofxPythonObject();
};