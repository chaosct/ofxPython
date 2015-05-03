#include "ofxPython.h"
#include "ofMain.h"

extern "C"{
void init_openframeworks();
void init_openframeworks_extra();
}

unsigned int ofxPython::instances = 0;

ofxPythonObject make_object_owned(PyObject * obj, bool errcheck);


void PythonErrorCheck()
{
	PyObject * ptype, * pvalue, * ptraceback;
	PyErr_Fetch(&ptype, &pvalue, &ptraceback);
	if(ptype)
	{
		PyErr_NormalizeException(&ptype, &pvalue, &ptraceback);
		ofxPythonObject optype = make_object_owned(ptype, false);
		ofxPythonObject opvalue = make_object_owned(pvalue, false);
		ofxPythonObject optraceback = make_object_owned(ptraceback, false);
		ofLog() << "Python Error: ";
		if(optraceback)
		{
			vector<ofxPythonObject> tb = ofxPython::getObject("format_tb","traceback")(optraceback).asVector();
			for(unsigned int i = 0; i < tb.size(); ++i)
			{
				vector<ofxPythonObject> lines = tb[i].attr("splitlines")().asVector();
				for (unsigned int j=0; j < lines.size(); ++j)
				{
					ofLog() << lines[j].asString();
				}
			}
		}
		ofLog() << "\t" << "Error: " << opvalue.repr();
	}
}

ofxPythonObject make_object_owned(PyObject * obj, bool errcheck= true)
{
	if (obj==NULL)
		ofLog()<< "WARNING! make_object_owned: creating ofxPythonObject with NULL"; 
	if(errcheck)
		PythonErrorCheck();
	ofxPythonObject o;
	o.insert_owned(obj);
	return o;
}

ofxPythonObject make_object_borrowed(PyObject * obj, bool errcheck= true)
{
	if (obj==NULL)
		ofLog()<< "WARNING! make_object_borrowed: creating ofxPythonObject with NULL"; 
	if(errcheck)
		PythonErrorCheck();
	ofxPythonObject o;
	o.insert_borrowed(obj);
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


ofxPython::ofxPython():initialized(false)
{
}

ofxPython::~ofxPython()
{
	if(initialized)
	{
		instances--;
		if(instances == 0)
		{
			Py_Finalize();
		}
	}
}

int ofxPython::init()
{
	if (!initialized)
	{
		initialized = true;
		if(instances == 0)
		{
			Py_Initialize();
			init_openframeworks();
			init_openframeworks_extra();
			//this seems to be the easiest way to add '.' to python path
#ifndef TARGET_OSX
			PyRun_SimpleString(
				"import sys\n"
				"sys.path.append('.')\n"
				"sys.path.append('data')\n"
				);
#else
			PyRun_SimpleString(
				"import sys\n"
				"sys.path.append('../../..')\n"
				"sys.path.append('../../../data')\n"
				);
#endif
			PyRun_SimpleString(
				"import sys\n"
				"class StdoutCatcher:\n"
				"	#this class redirect stdout to ofLog()\n"
				"	def __init__(self):\n"
				"		self.message = []\n"
				"	def write(self,s):\n"
				"		from openframeworks import ofLog, OF_LOG_NOTICE\n"
				"		if s.endswith('\\n'):\n"
				"			self.message.append(s[:-1])\n"
				"			ofLog(OF_LOG_NOTICE,''.join(self.message))\n"
				"			self.message = []\n"
				"		else:\n"
				"			self.message.append(s)\n"
				"catcher = StdoutCatcher()\n"
				"sys.stdout = catcher\n"
				);
			PythonErrorCheck();
		}
		instances++;
	}
	reset();
	return instances;
}

void ofxPython::reset()
{
	// globals = make_object_owned(PyDict_New());
	locals = make_object_owned(PyDict_New());
	//insert builtins
	locals["__builtins__"]=make_object_borrowed(PyEval_GetBuiltins());
}

void ofxPython::executeScript(const string& path)
{
	executeString(ofBufferFromFile(path).getText());
	PythonErrorCheck();
}

void ofxPython::executeString(const string& script)
{
	make_object_owned(PyRun_String(script.c_str(),Py_file_input,locals->obj,locals->obj));
	PythonErrorCheck();
}

ofxPythonObject ofxPython::evalString(const string& expression)
{
	return  make_object_owned(PyRun_String(expression.c_str(),Py_eval_input,locals->obj,locals->obj));
}

ofxPythonObject ofxPython::getObject(const string& name, const string& module)
{

	ofxPythonObject pmodule = make_object_owned(
		PyImport_Import(ofxPythonObject::fromString(module)->obj));
	if(pmodule)
		return pmodule.attr(name);
	return ofxPythonObject();
}

ofxPythonObject ofxPython::getObject(const string& name)
{
	return locals[name];
}

void ofxPython::setObject(const string& name, ofxPythonObject o)
{
	locals[name]=o;
}

void ofxPythonObject::insert_owned(PyObject * obj)
{
	reset(new ofxPythonObjectManaged(obj));
}

void ofxPythonObject::insert_borrowed(PyObject * obj)
{
	Py_XINCREF(obj);
	reset(new ofxPythonObjectManaged(obj));
}

ofxPythonObjectManaged::ofxPythonObjectManaged(PyObject * o):obj(o)
{
}
ofxPythonObjectManaged::~ofxPythonObjectManaged()
{
	if(ofxPython::instances > 0)
		Py_XDECREF(obj);
}

ofxPythonObject ofxPythonObject::method(const string &method_name)
{
	return make_object_owned(
		PyObject_CallMethod(get()->obj,noconststring(method_name),NULL));
}

ofxPythonObject ofxPythonObject::operator ()()
{
	return make_object_owned(PyObject_CallObject(get()->obj,NULL));
}

ofxPythonObject ofxPythonObject::operator ()(ofxPythonObject o1)
{
	if(o1->obj == NULL) o1=ofxPythonObject::_None();
	return make_object_owned(PyObject_CallFunctionObjArgs(get()->obj,o1->obj,NULL));
}

ofxPythonObject ofxPythonObject::operator ()(ofxPythonObject o1, ofxPythonObject o2)
{
	if(o1->obj == NULL) o1=ofxPythonObject::_None();
	if(o2->obj == NULL) o2=ofxPythonObject::_None();
	return make_object_owned(PyObject_CallFunctionObjArgs(get()->obj,o1->obj,o2->obj,NULL));
}

ofxPythonObject ofxPythonObject::operator ()(ofxPythonObject o1, ofxPythonObject o2, ofxPythonObject o3)
{
	if(o1->obj == NULL) o1=ofxPythonObject::_None();
	if(o2->obj == NULL) o2=ofxPythonObject::_None();
	if(o3->obj == NULL) o3=ofxPythonObject::_None();
	return make_object_owned(PyObject_CallFunctionObjArgs(get()->obj,o1->obj,o2->obj,o3->obj,NULL));
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
	ofxPythonObject objectsRepresentation = make_object_owned(
		PyObject_Repr(get()->obj));
	string s = string(PyString_AsString(objectsRepresentation->obj));
	PythonErrorCheck();
	return s;
}

ofxPythonObject::operator bool() const
{
	return get() && get()->obj && !isNone(); //TODO: check if evaluates to false (0,(,),[])
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

bool ofxPythonObject::isList() const
{
	return get() && PyList_Check(get()->obj);
}

bool ofxPythonObject::isTuple() const
{
	return get() && PyTuple_Check(get()->obj);
}

bool ofxPythonObject::isDict() const
{
	return get() && PyDict_Check(get()->obj);
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

vector<ofxPythonObject> ofxPythonObject::asVector() const
{
	std::vector<ofxPythonObject> v;
	if(isList())
	{
		int len = PyList_Size(get()->obj);
		for (int i = 0; i<len; ++i)
		{
			v.push_back(make_object_borrowed(PyList_GetItem(get()->obj,i)));
		}
	}
	else if(isTuple())
	{
		int len = PyTuple_Size(get()->obj);
		for (int i = 0; i<len; ++i)
		{
			v.push_back(make_object_borrowed(PyTuple_GetItem(get()->obj,i)));
		}
	}
	return v;
}

std::map<ofxPythonObject,ofxPythonObject> ofxPythonObject::asMap() const
{
	std::map<ofxPythonObject,ofxPythonObject> m;
	if(isDict())
	{
		PyObject *key, *value;
		Py_ssize_t pos = 0;

		while (PyDict_Next(get()->obj, &pos, &key, &value)) {
		    m[make_object_borrowed(key)] = make_object_borrowed(value);
		}
	}
	return m;
}


ofxPythonObject ofxPythonObject::fromInt(long int i)
{
	return make_object_owned(PyInt_FromLong(i));
}

ofxPythonObject ofxPythonObject::fromBool(bool b)
{
	if(b)
		return make_object_borrowed(Py_True);
	return make_object_borrowed(Py_False);
}

ofxPythonObject ofxPythonObject::_None()
{
	return make_object_borrowed(Py_None);
}

ofxPythonObject ofxPythonObject::fromFloat(double d)
{
	return make_object_owned(PyFloat_FromDouble(d));
}

ofxPythonObject ofxPythonObject::fromString(const string& s)
{
	return make_object_owned(PyString_FromString(s.c_str()));
}

ofxPythonTupleMaker ofxPythonObject::makeTuple()
{
	ofxPythonTupleMaker t;
	return t;
}

ofxPythonMappingValue::ofxPythonMappingValue(ofxPythonObject o, const string& k)
:object(o), key(k){}

ofxPythonMappingValue::operator ofxPythonObject()
{
	return make_object_owned(
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
	return make_object_owned(
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

ofxPythonObject ofxPythonObjectLike::method(const string &method_name)
{
	ofxPythonObject PO = *this;
	return PO.method(method_name);
}
ofxPythonObject ofxPythonObjectLike::operator ()()
{
	ofxPythonObject PO = *this;
	return PO();
}
ofxPythonObject ofxPythonObjectLike::operator ()(ofxPythonObject o)
{
	ofxPythonObject PO = *this;
	return PO(o);
}
ofxPythonObject ofxPythonObjectLike::operator ()(ofxPythonObject o1, ofxPythonObject o2)
{
	ofxPythonObject PO = *this;
	return PO(o1,o2);
}
ofxPythonObject ofxPythonObjectLike::operator ()(ofxPythonObject o1, ofxPythonObject o2, ofxPythonObject o3)
{
	ofxPythonObject PO = *this;
	return PO(o1,o2,o3);
}
ofxPythonAttrValue ofxPythonObjectLike::attr(const string& attribute)
{
	ofxPythonObject PO = *this;
	return PO.attr(attribute);
}
ofxPythonMappingValue ofxPythonObjectLike::operator [](const string& key)
{
	ofxPythonObject PO = *this;
	return PO[key];
}
ofxPythonMappingValue ofxPythonObjectLike::operator [](const char * key)
{
	ofxPythonObject PO = *this;
	return PO[key];
}
bool ofxPythonObjectLike::isNone()
{
	ofxPythonObject PO = *this;
	return PO.isNone();
}
bool ofxPythonObjectLike::isBool()
{
	ofxPythonObject PO = *this;
	return PO.isBool();
}
bool ofxPythonObjectLike::isInt()
{
	ofxPythonObject PO = *this;
	return PO.isInt();
}
bool ofxPythonObjectLike::isFloat()
{
	ofxPythonObject PO = *this;
	return PO.isFloat();
}
bool ofxPythonObjectLike::isString()
{
	ofxPythonObject PO = *this;
	return PO.isString();
}
bool ofxPythonObjectLike::isList()
{
	ofxPythonObject PO = *this;
	return PO.isList();
}
bool ofxPythonObjectLike::isDict()
{
	ofxPythonObject PO = *this;
	return PO.isDict();
}
bool ofxPythonObjectLike::isTuple()
{
	ofxPythonObject PO = *this;
	return PO.isTuple();
}
bool ofxPythonObjectLike::asBool()
{
	ofxPythonObject PO = *this;
	return PO.asBool();
}
long int ofxPythonObjectLike::asInt()
{
	ofxPythonObject PO = *this;
	return PO.asInt();
}
double ofxPythonObjectLike::asFloat()
{
	ofxPythonObject PO = *this;
	return PO.asFloat();
}
string ofxPythonObjectLike::asString()
{
	ofxPythonObject PO = *this;
	return PO.asString();
}
vector<ofxPythonObject> ofxPythonObjectLike::asVector()
{
	ofxPythonObject PO = *this;
	return PO.asVector();
}

std::map<ofxPythonObject,ofxPythonObject> ofxPythonObjectLike::asMap()
{
	ofxPythonObject PO = *this;
	return PO.asMap();
}

ofxPythonObjectLike::operator bool()
{
	ofxPythonObject PO = *this;
	return (bool)PO;
}
const string ofxPythonObjectLike::repr()
{
	ofxPythonObject PO = *this;
	return PO.repr();
}

ofxPythonTupleMaker& ofxPythonTupleMaker::operator<<(ofxPythonObject o)
{
	contents.push_back(o);
	return *this;
}

ofxPythonTupleMaker::operator ofxPythonObject()
{
	ofxPythonObject tuple = make_object_owned(
		PyTuple_New(contents.size()));
	for (unsigned int i = 0; i < contents.size(); ++i)
	{
		PyTuple_SetItem(tuple->obj,i,contents[i]->obj);
		Py_XINCREF(contents[i]->obj); //finding this one was tricky!
	}
	PythonErrorCheck();
	return tuple;
}