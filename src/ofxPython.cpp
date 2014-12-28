#include "ofxPython.h"
#include "ofMain.h"

extern "C"{
void init_openframeworks();
}

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
							"sys.path.append('.')\n"
							"sys.path.append('data')\n");
	}
	~ofxPythonDestructor()
	{
		Py_Finalize();
	}
};

void ofxPython::init()
{
	static ofxPythonDestructor d;
}

ofxPythonObject ofxPython::getObject(const string& name, const string& module)
{

	ofxPythonObject o;
	PyObject *pName = PyString_FromString(module.c_str());
	PyObject *pModule = PyImport_Import(pName);
	Py_DECREF(pName);
	if(pModule)
	{
		PyObject * f = PyObject_GetAttrString(pModule, name.c_str());
		o.insert_noaddref(f);
	}
	else
	{
		ofLog() << "Python module not found:(";
	}
	Py_XDECREF(pModule);
	return o;
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
	//So it seems that PyObject_CallMethod needs a non-const char * ¬¬
	std::vector<char> chars(method_name.c_str(), method_name.c_str() + method_name.size() + 1u);
	PyObject* r = PyObject_CallMethod(get()->obj,&chars[0],NULL);
	ofxPythonObject o;
	o.insert_noaddref(r);
	return o;
}

ofxPythonObject ofxPythonObject::operator ()()
{
	PyObject* r = PyObject_CallObject(get()->obj,NULL);
	ofxPythonObject o;
	o.insert_noaddref(r);
	return o;
}