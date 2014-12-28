#include "ofxPython.h"
#include "ofMain.h"

extern "C"{
void init_openframeworks();
}


ofxPythonObject make_object_noaddref(PyObject * obj)
{
	ofxPythonObject o;
	o.insert_noaddref(obj);
	return o;
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
	reset();
}

void ofxPython::reset()
{
	// globals = make_object_noaddref(PyDict_New());
	locals = make_object_noaddref(PyDict_New());
	//insert builtins
	PyDict_SetItemString(locals->obj, "__builtins__", PyEval_GetBuiltins());
}

ofxPythonObject ofxPython::executeScript(const string& path)
{
	return executeString(ofBufferFromFile(path).getText());
}

ofxPythonObject ofxPython::executeString(const string& script)
{
	return make_object_noaddref(PyRun_String(script.c_str(),Py_file_input,locals->obj,locals->obj));
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
	//So it seems that PyObject_CallMethod needs a non-const char * ¬¬
	std::vector<char> chars(method_name.c_str(), method_name.c_str() + method_name.size() + 1u);
	return make_object_noaddref(PyObject_CallMethod(get()->obj,&chars[0],NULL));
}

ofxPythonObject ofxPythonObject::operator ()()
{
	return make_object_noaddref(PyObject_CallObject(get()->obj,NULL));
}

ofxPythonObject ofxPythonObject::operator [](const string& key)
{
	//So it seems that also PyMapping_GetItemString needs a non-const char * ¬¬
	std::vector<char> chars(key.c_str(), key.c_str() + key.size() + 1u);
	return make_object_noaddref(PyMapping_GetItemString(get()->obj, &chars[0]));
}
