#include "ofxPython.h"
#include "ofMain.h"

extern "C"{
void init_openframeworks();
}

void PythonErrorCheck()
{
	if(PyErr_Occurred())
	{
		PyErr_Print();
		PyErr_Clear();
	}
}

ofxPythonObject make_object_noaddref(PyObject * obj)
{
	PythonErrorCheck();
	ofxPythonObject o;
	o.insert_noaddref(obj);
	return o;
}

ofxPythonObject make_object_addref(PyObject * obj)
{
	PythonErrorCheck();
	ofxPythonObject o;
	o.insert(obj);
	return o;
}

//helper to transform a (const) std:string into a char *
class noconststring: vector<char>
{
public:
	noconststring(const string& source)
	:vector<char>(source.c_str(), source.c_str() + source.size() + 1u)
	{}
	operator char *()
	{
		return &(*this).at(0);
	}
};


ofxPython::ofxPython()
{
}

ofxPython::~ofxPython()
{
}

class ofxPythonDestructor
{
public:
	ofxPythonDestructor()
	{
		Py_Initialize();
		init_openframeworks();
		//this seems to be the easiest way to add '.' to python path
		PyRun_SimpleString( "import sys\n"
#ifndef TARGET_OSX
							"sys.path.append('.')\n"
							"sys.path.append('data')\n"
#else
							"sys.path.append('../../..')\n"
							"sys.path.append('../../../data')\n"
#endif
						   );
		PythonErrorCheck();
	}
	~ofxPythonDestructor()
	{
		Py_Finalize();
	}
};

void ofxPython::init()
{
	static ofxPythonDestructor d;
	reset();
}

void ofxPython::reset()
{
	// globals = make_object_noaddref(PyDict_New());
	locals = make_object_noaddref(PyDict_New());
	//insert builtins
	locals["__builtins__"]=make_object_addref(PyEval_GetBuiltins());
}

void ofxPython::executeScript(const string& path)
{
	executeString(ofBufferFromFile(path).getText());
	PythonErrorCheck();
}

void ofxPython::executeString(const string& script)
{
	make_object_noaddref(PyRun_String(script.c_str(),Py_file_input,locals->obj,locals->obj));
	PythonErrorCheck();
}

ofxPythonObject ofxPython::evalString(const string& expression)
{
	return  make_object_noaddref(PyRun_String(expression.c_str(),Py_eval_input,locals->obj,locals->obj));
}

ofxPythonObject ofxPython::getObject(const string& name, const string& module)
{

	ofxPythonObject pmodule = make_object_noaddref(
		PyImport_Import(ofxPythonObject::fromString(module)->obj));
	if(pmodule)
		return pmodule.attr(name);
	return ofxPythonObject();
}

ofxPythonObject ofxPython::getObject(const string& name)
{
	return locals[name];
}

void ofxPythonObject::insert_noaddref(PyObject * obj)
{
	reset(new ofxPythonObjectManaged(obj));
}

void ofxPythonObject::insert(PyObject * obj)
{
	Py_XINCREF(obj);
	reset(new ofxPythonObjectManaged(obj));
}

ofxPythonObjectManaged::ofxPythonObjectManaged(PyObject * o):obj(o)
{
}
ofxPythonObjectManaged::~ofxPythonObjectManaged()
{
	Py_XDECREF(obj);
}

ofxPythonObject ofxPythonObject::method(const string &method_name)
{
	return make_object_noaddref(
		PyObject_CallMethod(get()->obj,noconststring(method_name),NULL));
}

ofxPythonObject ofxPythonObject::operator ()()
{
	return make_object_noaddref(PyObject_CallObject(get()->obj,NULL));
}

ofxPythonObject ofxPythonObject::operator ()(ofxPythonObject o1)
{
	return make_object_noaddref(PyObject_CallFunctionObjArgs(get()->obj,o1->obj,NULL));
}

ofxPythonObject ofxPythonObject::operator ()(ofxPythonObject o1, ofxPythonObject o2)
{
	return make_object_noaddref(PyObject_CallFunctionObjArgs(get()->obj,o1->obj,o2->obj,NULL));
}

ofxPythonObject ofxPythonObject::operator ()(ofxPythonObject o1, ofxPythonObject o2, ofxPythonObject o3)
{
	return make_object_noaddref(PyObject_CallFunctionObjArgs(get()->obj,o1->obj,o2->obj,o3->obj,NULL));
}

ofxPythonMappingValue ofxPythonObject::operator [](const string& key)
{
	return ofxPythonMappingValue(*this,key);
}

ofxPythonMappingValue ofxPythonObject::operator [](const char * key)
{
	return ofxPythonMappingValue(*this,string(key));
}

ofxPythonAttrValue ofxPythonObject::attr(const string& attribute)
{
	return ofxPythonAttrValue(*this, attribute);
}

const string ofxPythonObject::repr()
{
	ofxPythonObject objectsRepresentation = make_object_noaddref(
		PyObject_Repr(get()->obj));
	string s = string(PyString_AsString(objectsRepresentation->obj));
	PythonErrorCheck();
	return s;
}

ofxPythonObject::operator bool() const
{
	return get() && get()->obj && !isNone();
}

bool ofxPythonObject::isNone() const
{
	return get() && get()->obj == Py_None;
}

bool ofxPythonObject::isBool() const
{
	return get() && PyBool_Check(get()->obj);
}

bool ofxPythonObject::isInt() const
{
	return get() && PyInt_Check(get()->obj);
}

bool ofxPythonObject::isFloat() const
{
	return get() && PyFloat_Check(get()->obj);
}

bool ofxPythonObject::isString() const
{
	return get() && PyString_Check(get()->obj);
}

long int ofxPythonObject::asInt() const
{
	if (isInt())
		return PyInt_AsLong(get()->obj);
	return 0;
}

bool ofxPythonObject::asBool() const
{
	if (isBool())
		return get()->obj == Py_True;
	return false;
}

double ofxPythonObject::asFloat() const
{
	if (isFloat())
		return PyFloat_AsDouble(get()->obj);
	return 0.0;
}

string ofxPythonObject::asString() const
{
	if(isString())
		return string(PyString_AsString(get()->obj));
	return string();
}


ofxPythonObject ofxPythonObject::fromInt(long int i)
{
	return make_object_noaddref(PyInt_FromLong(i));
}

ofxPythonObject ofxPythonObject::fromBool(bool b)
{
	if(b)
		return make_object_addref(Py_True);
	return make_object_addref(Py_False);
}

ofxPythonObject ofxPythonObject::_None()
{
	return make_object_addref(Py_None);
}

ofxPythonObject fromFloat(double d)
{
	return make_object_noaddref(PyFloat_FromDouble(d));
}

ofxPythonObject ofxPythonObject::fromString(const string& s)
{
	return make_object_noaddref(PyString_FromString(s.c_str()));
}

ofxPythonMappingValue::ofxPythonMappingValue(ofxPythonObject o, const string& k)
:object(o), key(k){}

ofxPythonMappingValue::operator ofxPythonObject()
{
	return make_object_noaddref(
		PyMapping_GetItemString(object->obj, noconststring(key)));
}

ofxPythonMappingValue& ofxPythonMappingValue::operator =(ofxPythonObject o)
{
	PyMapping_SetItemString(object->obj, noconststring(key) , o->obj);
	PythonErrorCheck();
	return *this;
}

ofxPythonAttrValue::ofxPythonAttrValue(ofxPythonObject o, const string& attr)
:object(o),attribute(attr){}

ofxPythonAttrValue::operator ofxPythonObject()
{
	return make_object_noaddref(
		PyObject_GetAttrString(object->obj,attribute.c_str()));
}

ofxPythonAttrValue & ofxPythonAttrValue::operator =(ofxPythonObject o)
{
	PyObject_SetAttrString(object->obj, attribute.c_str(), o->obj);
	PythonErrorCheck();
	return *this;
}

ofxPythonMappingValue & ofxPythonMappingValue::operator =(ofxPythonMappingValue & o)
{
	return *this = (ofxPythonObject) o;
}

ofxPythonAttrValue & ofxPythonAttrValue::operator =(ofxPythonAttrValue & o)
{
	return *this = (ofxPythonObject) o;
}
