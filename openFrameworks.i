/*
	SWIG (http://www.swig.org) interface wrapper for the OpenFrameworks core API

	(Lua) Creates an "of" module and renames functions, classes, constants, & enums

		* function: ofBackground -> of.background
		* class: ofColor -> of.Color
		* constant: OF_LOG_VERBOSE -> of.LOG_VERBOSE
		* enum: ofShader::POSITION_ATTRIBUTE -> of.Shader.POSITION_ATTRIBUTE

	(Python) Creates an "openframeworks" module
	
	Deprecations are ignored (aka not wrapped)

	2014 Dan Wilcox <danomatika@gmail.com>
*/

// workaround when compiling Python in MinGW
#ifdef SWIGPYTHON
%begin %{
	#if defined( __WIN32__ ) || defined( _WIN32 )
		#include <cmath>
	#endif
%}
#endif

%module MODULE_NAME
%{
	#include "ofMain.h"
	#undef check
%}

%include <attribute.i>
%include <typemaps.i>

// custom attribute wrapper for nested union var access
%define %attributeVar(Class, AttributeType, AttributeName, GetVar, SetVar...)
	#if #SetVar != ""
		%attribute_custom(%arg(Class), %arg(AttributeType), AttributeName, GetVar, SetVar, self_->GetVar, self_->SetVar = val_)
	#else
		%attribute_readonly(%arg(Class), %arg(AttributeType), AttributeName, GetVar, self_->GetVar)
	#endif
%enddef

// ----- C++ -----

// SWIG doesn't understand C++ streams
%ignore operator <<;
%ignore operator >>;

// ----- STL types -----

%include <stl.i>
%include <std_string.i>
%include <std_vector.i>

// needed for functions and return types
namespace std {
	%template(IntVector) std::vector<int>;
	%template(FloatVector) std::vector<float>;
	%template(StringVector) std::vector<std::string>;
};

// ----- Renaming -----

#ifdef OF_SWIG_RENAME

	// strip "of" prefix from classes
	%rename("%(strip:[of])s", %$isclass) "";

	// strip "of" prefix from global functions & make first char lowercase
	%rename("%(regex:/of(.*)/\\l\\1/)s", %$isfunction) "";

	// strip "OF_" from constants & enums
	%rename("%(strip:[OF_])s", %$isconstant) "";
	%rename("%(strip:[OF_])s", %$isenumitem) "";

#endif

// ----- Python -----

// overloading operators
#ifdef SWIGPYTHON
	%rename(__getitem__) *::operator[];
	%rename(__mul__) *::operator*;
	%rename(__div__) *::operator/;
	%rename(__add__) *::operator+;
	%rename(__sub__) *::operator-;
#endif

////////////////////////////////////////////////////////////////////////////////
// ----- BINDINGS --------------------------------------------------------------
//
// Look for the follow notes:
//     TODO: something that might need to be updated or fixed in the future
//     DIFF: an important difference as compared to the OF C++ API
//
// Make sure to %rename & %ignore *before* %including a header.
//
// The order of class declarations is important! This is why a number of
// classes are %included at the beginning before ofBaseTypes.h.
//
// If a header forward declares a class like (AnotherClass.h):
//
//     class SomeClass;
//     class AnotherClass {
//     ...
//
// and this header is %included before the actual class implementation
// (SomeClass.h), SWIG will wrap the empty declaration and *not* the actual
// class! This basically allows you to create the class in the bound language,
// but it does nothing and has no attributes or functions. Bummer.
//
// What needs to be done is either %include SomeClass.h before AnotherClass.h
//
//    %include "SomeClass.h"
//    %include "AnotherClas.h"
//
// or make a forward declaration before %inluding SomeClass.h:
//
//    class SomeClass {};
//    %include AnotherClass.h
//
// This forward delcaration is then overridden by the actual implentation after
// %include "SomeClass.h" later on.

// ----- ofConstants.h -----

// GL types used as OF arguments, etc so swig needs to know about them
typedef int GLint;
typedef float GLfloat;

// ofIndexType only defined in ofConstants.h as a TESSIndex,
// so we do it unambiguously here
#if TARGET_OS_IPHONE || ANDROID || __ARMEL__
	typedef unsigned short ofIndexType;
#else
	typedef unsigned int ofIndexType;
#endif

%include "utils/ofConstants.h"

// ----- ofFbo.h -----

// need to forward declare these for ofFbo
%ignore ofBaseDraws;
class ofBaseDraws {};

%ignore ofBaseHasTexture;
class ofBaseHasTexture {};

%ignore ofBaseHasPixels;
class ofBaseHasPixels {};

// TODO: ofFbo.h: SWIG Warning 325 due to Settings nested struct
%ignore ofFbo::Settings; // doesn't seem to silence warning

// DIFF: (Lua) ofFbo.h: beginFbo() & endFbo() since "end" is a Lua keyword
#ifdef SWIGLUA
	%rename(beginFbo) ofFbo::begin;
	%rename(endFbo) ofFbo::end;
#endif

%include "gl/ofFbo.h"

// ----- ofTexture.h -----

// TODO: ofTexture.h: ignore GL specific global functions, need to wrap the GL enums?
%ignore ofSetTextureWrap;
%ignore ofGetUsingCustomTextureWrap;
%ignore ofRestoreTextureWrap;
%ignore ofSetMinMagFilters;
%ignore ofGetUsingCustomMinMagFilters;
%ignore ofRestoreMinMagFilters;

%ignore ofTexture::getMeshForSubsection(float, float, float, float, float, float, float, float, float) const;

%include "gl/ofTexture.h"

// ----- ofImage.h -----

// forward declare needed types
%ignore ofBaseImage_;
template<typename T> class ofBaseImage_ {};

// forward declare base template classes
%ignore ofBaseImage;
%ignore ofBaseFloatImage;
%ignore ofBaseShortImage;
#ifdef OF_SWIG_RENAME
	%template(BaseImage) ofBaseImage_<unsigned char>;
	%template(BaseFloatImage) ofBaseImage_<float>;
	%template(BaseShortImage) ofBaseImage_<unsigned short>;
#else
	%template(ofBaseImage) ofBaseImage_<unsigned char>;
	%template(ofBaseFloatImage) ofBaseImage_<float>;
	%template(ofBaseShortImage) ofBaseImage_<unsigned short>;
#endif
// DIFF: ofImage.h: ignore global helper functions
%ignore ofLoadImage;
%ignore ofSaveImage;
%ignore ofCloseFreeImage;

// TODO: ofImage.h: SWIG Warning 503: due to operator ofImage_::operator pixels
%include "graphics/ofImage.h"

// handle template implementations
#ifdef OF_SWIG_RENAME
	%template(Image) ofImage_<unsigned char>;
	%template(FloatImage) ofImage_<float>;
	%template(ShortImage) ofImage_<unsigned short>;
#else
	%template(ofImage) ofImage_<unsigned char>;
	%template(ofFloatImage) ofImage_<float>;
	%template(ofShortImage) ofImage_<unsigned short>;
#endif

// DIFF: defined GLint texture types for convenience: OF_TEXTURE_LUMINENCE, etc
#define OF_TEXTURE_LUMINENCE = GL_LUMINENCE
#define OF_TEXTURE_RGB = GL_RGB
#define OF_TEXTURE_RGBA = GL_RGBA

// DIFF: defined GLfloat texture wrap defines for convenience: OF_TEXTURE_REPEAT, etc
#define OF_CLAMP_TO_EDGE GL_CLAMP_TO_EDGE
#ifndef TARGET_OPENGLES
	#define OF_CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER
#endif
#define OF_MIRRORED_REPEAT = GL_MIRRORED_REPEAT
#define OF_REPEAT = GL_REPEAT

// ----- SOUND -----------------------------------------------------------------

// ----- ofSoundStream.h -----

%include "sound/ofSoundStream.h"

// ----- ofSoundPlayer.h -----

%ignore ofBaseSoundPlayer;
class ofBaseSoundPlayer {};

// DIFF: ofSoundPlayer.h: warnings say "FIX THIS SHIT", so leaving out fmod global functions
%ignore ofSoundStopAll;
%ignore ofSoundSetVolume;
%ignore ofSoundUpdate;
%ignore ofSoundGetSpectrum;
%ignore ofSoundShutdown;

%include "sound/ofSoundPlayer.h"

// ----- TYPES -----------------------------------------------------------------

// ----- ofColor.h -----

%include "types/ofColor.h"

// TODO: ofColor.h: SWIG Warning 312 ignores nested union that provides r, g, b, & a access
// DIFF: ofColor.h: added ofColor_ pixel channel getters getR(), getG(), getB(), getA()
// DIFF:            added ofColor_ pixel channel setters setR(), setG(), setB(), setA()
// DIFF: ofColor.h: added target language tostring wrapper for ofColor_::operator <<
%extend ofColor_ {

	// pixel channel getters
	PixelType getR() {return $self->r;}
	PixelType getG() {return $self->g;}
	PixelType getB() {return $self->b;}
	PixelType getA() {return $self->a;}

	// pixel channel setters
	void setR(PixelType r) {$self->r = r;}
	void setG(PixelType g) {$self->g = g;}
	void setB(PixelType b) {$self->b = b;}
	void setA(PixelType a) {$self->a = a;}

	const char* __str__() {
		stringstream str;
		str << (*$self);
		return str.str().c_str();
	}
};

%attributeVar(ofColor_<unsigned char>, unsigned char, r, r, r);
%attributeVar(ofColor_<unsigned char>, unsigned char, g, g, g);
%attributeVar(ofColor_<unsigned char>, unsigned char, b, b, b);
%attributeVar(ofColor_<unsigned char>, unsigned char, a, a, a);

%attributeVar(ofColor_<float>, float, r, r, r);
%attributeVar(ofColor_<float>, float, g, g, g);
%attributeVar(ofColor_<float>, float, b, b, b);
%attributeVar(ofColor_<float>, float, a, a, a);

%attributeVar(ofColor_<unsigned short>, unsigned short, r, r, r);
%attributeVar(ofColor_<unsigned short>, unsigned short, g, g, g);
%attributeVar(ofColor_<unsigned short>, unsigned short, b, b, b);
%attributeVar(ofColor_<unsigned short>, unsigned short, a, a, a);

// tell SWIG about template classes
#ifdef OF_SWIG_RENAME
	%template(Color) ofColor_<unsigned char>;
	%template(FloatColor) ofColor_<float>;
	%template(ShortColor) ofColor_<unsigned short>;
#else
	%template(ofColor) ofColor_<unsigned char>;
	%template(ofFloatColor) ofColor_<float>;
	%template(ofShortColor) ofColor_<unsigned short>;
#endif

// ----- ofBaseTypes.h -----

// DIFF: ofBaseTypes.h: ignore all abstract amd base types
%ignore ofAbstractParameter;
%ignore ofBaseDraws;
%ignore ofBaseUpdates;
%ignore ofBaseHasTexture;

%ignore ofAbstractHasPixels;
%ignore ofBaseHasPixels_;
%ignore ofBaseHasPixels;
%ignore ofBaseHasFloatPixels;
%ignore ofBaseHasShortPixels;

%ignore ofAbstractImage;
%ignore ofBaseImage_;
%ignore ofBaseImage;
%ignore ofBaseFloatImage;
%ignore ofBaseShortImage;

%ignore ofBaseSoundInput;
%ignore ofBaseSoundOutput;

%ignore ofBaseVideo;
%ignore ofBaseVideoDraws;
%ignore ofBaseVideoGrabber;
%ignore ofBaseVideoPlayer;

%ignore ofBaseRenderer;
%ignore ofBaseGLRenderer;
%ignore ofBaseSerializer;
%ignore ofBaseFileSerializer;

// still include header for derived classes
%include "types/ofBaseTypes.h"

// ----- ofTypes.h -----

// TODO: ofTypes.h: not sure if video format and vidoe device classes are needed
%ignore ofVideoFormat;
%ignore ofVideoDevice;

// DIFF: ofTypes.h: mutex, scoped lock, ptr are probably too low level
%ignore ofMutex;
%ignore ofScopedLock;
%ignore ofPtr;

// TODO: ofTypes.h: SWIG Warning 315 due to std::tr1::shared_ptr, etc
%include "types/ofTypes.h"

// ----- ofUtils.h -----

// DIFF: ofUtils.h: ignoring ofFromString as templating results in too much overloading
%ignore ofFromString;

// DIFF: ofUtils.h: variable argument support is painful, safer to ignore
// see http://www.swig.org/Doc2.0/Varargs.html
%ignore ofVAArgsToString;

%include "utils/ofUtils.h"

// ----- ofFileUtils.h -----

/*
// forward declare fstream for ofBuffer
%ignore fstream;
class fstream {};

// TODO: ofFileUtils.h: ignoring ofDirectory::canRead(), ofDirectory::canWrite(), and
// TODO:                ofDirectory::canExecute(), defined but not implemented in OF 0.8.4
%ignore ofDirectory::canRead() const;
%ignore ofDirectory::canWrite() const;
%ignore ofDirectory::canExecute() const;

// TODO: SWIG Warning 503: due to operator ofBuffer::operator string
%include "utils/ofFileUtils.h"
*/

// ----- ofLog.h -----

// function wrapper for ofLog class
%inline %{
	void log(ofLogLevel level, const string & message) {
		ofLog(level, message);
	}
%}

// DIFF: ofLog.h: ignore stream based log classes since target languages won't support it
%ignore ofLog;
%ignore ofLogVerbose;
%ignore ofLogNotice;
%ignore ofLogWarning;
%ignore ofLogError;
%ignore ofLogFatalError;

// DIFF: ofLog.h: ignore logger channel classes
%ignore ofBaseLoggerChannel;
%ignore ofSetLoggerChannel;
%ignore ofConsoleLoggerChannel;
%ignore ofFileLoggerChannel;

%include "utils/ofLog.h"

// ----- ofSystemUtils.h -----

%include "utils/ofSystemUtils.h"

// ----- ofThread.h -----

// DIFF: ofThread.h: ignore ofThread, not useful if you can't inherit it in the target language
%ignore ofThread;
%include "utils/ofThread.h"

// ----- ofURLFileLoader.h -----

// TODO: ofURLFileLoader.h: SWIG Warning 503: due to operator ofURLFileLoader::buffer&
%include "utils/ofURLFileLoader.h"

// ----- ofPoint.h -----

// this file is just a typedef which swig cannot wrap, so the types need to be 
// handled in the scripting language, see the Lua, Python, etc code at the end
%include "types/ofPoint.h"

// ----- ofRectangle.h -----

// DIFF: ofRectangle.h: renamed functions due to ambiguous overloading:
// DIFF:                scaleToScaleMode() & scaleToAspectRatio()
%rename(scaleToScaleMode) ofRectangle::scaleTo(ofRectangle const &, ofScaleMode);
%rename(scaleToAspectRatio) ofRectangle::scaleTo(ofRectangle const &, ofAspectRatioMode);

// TODO: ofRectangle.h: SWIG Warning 302: due to manual override of x & y attributes
%extend ofRectangle {

	// override these since they are float references in the orig file and we
	// want to access them as floats
	float x;
	float y;

	const char* __str__() {
		stringstream str;
		str << (*$self);
		return str.str().c_str();
	}
};

%include "types/ofRectangle.h"

// SWIG converts the x & y float& types into pointers,
// so specify x & y as attributes using the get & set functions
%attribute(ofRectangle, float, x, getX, setX);
%attribute(ofRectangle, float, y, getY, setY);

// ----- 3D --------------------------------------------------------------------

// ----- ofNode.h -----

// process ofNode first since it's a base class
%include "3d/ofNode.h"

// ----- of3dUtils.h -----

%include "3d/of3dUtils.h"

// ----- ofCamera.h -----

// DIFF: (Lua) ofCamera.h: beginCamera() & endCamera() since "end" is a Lua keyword
#ifdef SWIGLUA
	%rename(beginCamera) ofCamera::begin;
	%rename(endCamera) ofCamera::end;
#endif

%include "3d/ofCamera.h"

// ----- ofEasyCam.h -----

%include "3d/ofEasyCam.h"

// ----- ofMesh.h -----

// TODO: ofMesh.h: ignoring getFace(i), defined but not implemented in OF 0.8.4
%ignore ofMesh::getFace(int);

%include "3d/ofMesh.h"

// ----- of3dPrimitives.h -----

// DIFF: of3dPrimitives.h: ignore of3DPrimitive base class
%ignore of3dPrimitive;

%include "3d/of3dPrimitives.h"

// ----- ofConstants.h -----

%include "3d/of3dUtils.h"

// ----- APP -------------------------------------------------------------------

// ----- ofAppRunner.h -----

// deprecated
%ignore ofSetupOpenGL(ofAppBaseWindow *, int, int, int);
%ignore ofRunApp(ofBaseApp *);

// TODO: ofAppRunner.h: is ofSetAppPtr applicable in a target language?
%ignore ofSetAppPtr;

// DIFF: ofAppRunner.h: ignoring api-specific window objects: display, window, context, surface
%ignore ofGetX11Display;
%ignore ofGetX11Window;
%ignore ofGetGLXContext;
%ignore ofGetEGLDisplay;
%ignore ofGetEGLContext;
%ignore ofGetEGLSurface;
%ignore ofGetNSGLContext;
%ignore ofGetCocoaWindow;
%ignore ofGetWGLContext;
%ignore ofGetWin32Window;

%include "app/ofAppRunner.h"

// ----- COMMUNICATION ---------------------------------------------------------

// conditional compilation for iOS and Android
#if !defined(TARGET_IOS) && !defined(TARGET_ANDROID)

	// ----- ofArduino.h -----

	%include "communication/ofArduino.h"

	// ----- ofSerial.h -----

	%include "communication/ofSerial.h"

#endif

// ----- GL --------------------------------------------------------------------

// ----- ofLight.h -----

%include "gl/ofLight.h"

// ----- ofMaterial.h -----

// DIFF: (Lua) ofMaterial.h: beginMaterial() & endMaterial() since "end" is a Lua keyword
#ifdef SWIGLUA
	%rename(beginMaterial) ofMaterial::begin;
	%rename(endMaterial) ofMaterial::end;
#endif

%include "gl/ofMaterial.h"

// ----- ofShader.h -----

// DIFF: (Lua) ofShader.h: beginShader() & endShader() since "end" is a Lua keyword
#ifdef SWIGLUA
	%rename(beginShader) ofShader::begin;
	%rename(endShader) ofShader::end;
#endif

%include "gl/ofShader.h"

// ----- ofVbo.h -----

%include "gl/ofVbo.h"

// ----- ofVboMesh.h -----

%include "gl/ofVboMesh.h"

// ----- GRAPHICS --------------------------------------------------------------

// ----- ofPixels.h -----

// DIFF: ofPixels.h: fixed ambiguous ofPixels function overloads since enums = int in SWIG
// DIFF:             by renaming to allocatePixelFormat, allocateimageType, & setFromPixelsImageType
%rename(allocatePixelFormat) ofPixels_<unsigned char>::allocate(int,int,ofPixelFormat);
%rename(allocateImageType) ofPixels_<unsigned char>::allocate(int,int,ofImageType);
%rename(setFromPixelsImageType) ofPixels_<unsigned char>::setFromPixels(unsigned char const *,int,int,ofImageType);

%rename(allocatePixelFormat) ofPixels_<float>::allocate(int,int,ofPixelFormat);
%rename(allocateImageType) ofPixels_<float>::allocate(int,int,ofImageType);
%rename(setFromPixelsImageType) ofPixels_<float>::setFromPixels(float const *,int,int,ofImageType);

%rename(allocatePixelFormat) ofPixels_<unsigned short>::allocate(int,int,ofPixelFormat);
%rename(allocateImageType) ofPixels_<unsigned short>::allocate(int,int,ofImageType);
%rename(setFromPixelsImageType) ofPixels_<unsigned short>::setFromPixels(unsigned short const *,int,int,ofImageType);

%ignore ofPixels_<unsigned short>::getLine(int line);
%ignore ofPixels_<unsigned short>::getLines();
%ignore ofPixels_<unsigned short>::getLines(int first, int numLines);
%ignore ofPixels_<unsigned short>::getPixelsIter();
%ignore ofPixels_<unsigned short>::getConstLine(int line) const;
%ignore ofPixels_<unsigned short>::getConstLines() const;
%ignore ofPixels_<unsigned short>::getConstLines(int first, int numLines) const;
%ignore ofPixels_<unsigned short>::getConstPixelsIter() const;

%ignore ofPixels_<unsigned char>::getLine(int line);
%ignore ofPixels_<unsigned char>::getLines();
%ignore ofPixels_<unsigned char>::getLines(int first, int numLines);
%ignore ofPixels_<unsigned char>::getPixelsIter();
%ignore ofPixels_<unsigned char>::getConstLine(int line) const;
%ignore ofPixels_<unsigned char>::getConstLines() const;
%ignore ofPixels_<unsigned char>::getConstLines(int first, int numLines) const;
%ignore ofPixels_<unsigned char>::getConstPixelsIter() const;

%ignore ofPixels_<float>::getLine(int line);
%ignore ofPixels_<float>::getLines();
%ignore ofPixels_<float>::getLines(int first, int numLines);
%ignore ofPixels_<float>::getPixelsIter();
%ignore ofPixels_<float>::getConstLine(int line) const;
%ignore ofPixels_<float>::getConstLines() const;
%ignore ofPixels_<float>::getConstLines(int first, int numLines) const;
%ignore ofPixels_<float>::getConstPixelsIter() const;

%include "graphics/ofPixels.h"

// tell SWIG about template classes
#ifdef OF_SWIG_RENAME
	%template(Pixels) ofPixels_<unsigned char>;
	%template(FloatPixels) ofPixels_<float>;
	%template(ShortPixels) ofPixels_<unsigned short>;
#else
	%template(ofPixels) ofPixels_<unsigned char>;
	%template(ofFloatPixels) ofPixels_<float>;
	%template(ofShortPixels) ofPixels_<unsigned short>;
#endif

// ----- ofPath.h -----

// deprecated
%ignore ofPath::getArcResolution;
%ignore ofPath::setArcResolution;

// TODO: ofPath.h: ignoring getCurveResolution(), defined but not implemented in OF 0.8.4
%ignore ofPath::getCurveResolution() const;

// TODO: ofPath.h: SWIG Warning 325 due to ofPath::Command nested struct
%include "graphics/ofPath.h"

// ----- ofPolyline.h -----

// ignored due to default variable overload
%ignore ofPolyline::arc(float,float,float,float,float,float,float);
%ignore ofPolyline::arcNegative(float,float,float,float,float,float,float);

%include "graphics/ofPolyline.h"

// ----- ofGraphics.h -----

// no PDF export support on mobile
#if defined(TARGET_IOS) || defined(TARGET_ANDROID)
	%ignore ofBeginSaveScreenAsPDF;
	%ignore ofEndSaveScreenAsPDF();
#endif

// DIFF: ofGraphics.h: get/set current renderer not applicable to target language
%ignore ofSetCurrentRenderer;
%ignore ofGetCurrentRenderer;

// deprecated (all overloaded funcs too ...)
%ignore ofSetupScreenPerspective(float, float, ofOrientation);
%ignore ofSetupScreenPerspective(float, float, ofOrientation, bool);
%ignore ofSetupScreenPerspective(float, float, ofOrientation, bool, float);
%ignore ofSetupScreenPerspective(float, float, ofOrientation, bool, float, float);
%ignore ofSetupScreenPerspective(float, float, ofOrientation, bool, float, float, float);
%ignore ofSetupScreenOrtho(float, float, ofOrientation);
%ignore ofSetupScreenOrtho(float, float, ofOrientation, bool);
%ignore ofSetupScreenOrtho(float, float, ofOrientation, bool, float);
%ignore ofSetupScreenOrtho(float, float, ofOrientation, bool, float, float);

// TODO: ofGraphics.h: ignoring ofBgColorPtr &ofbClearBg, needed in target language?
%ignore ofBgColorPtr;
%ignore ofbClearBg;

// TODO: ofGraphics.h: ignoring ofRectRounded(f,f,f,f,f,f), defined but not implemented in OF 0.8.4
%ignore ofRectRounded(float, float, float, float, float, float);

%include "graphics/ofGraphics.h"

// ----- of3dGraphics.h -----

// deprecated
%ignore ofSphere;
%ignore ofCone;
%ignore ofBox;

%include "graphics/of3dGraphics.h"

// ----- ofTrueTypeFont.h -----

// TODO: ofTrueTypeFont.h: SWIGS fails on the static strings when including ofTrueTypeFont.h,
// TODO:                   so define the class to be wrapped here for now
class ofTrueTypeFont{

public:
	ofTrueTypeFont();
	virtual ~ofTrueTypeFont();
	bool load(const std::string& filename,
                  int fontsize,
                  bool _bAntiAliased=true,
                  bool _bFullCharacterSet=true,
                  bool makeContours=false,
                  float simplifyAmt=0.3f,
                  int dpi=0);
	
	bool isLoaded() const;
	static void setGlobalDpi(int newDpi);
	
	bool isAntiAliased() const;

	bool hasFullCharacterSet() const;
	
	int	getNumCharacters() const;


	int getSize() const;
	
	float getLineHeight() const;

	void setLineHeight(float height);

	float getAscenderHeight() const;

	float getDescenderHeight() const;

    const ofRectangle & getGlyphBBox() const;

	float getLetterSpacing() const;

	void setLetterSpacing(float spacing);

	float getSpaceSize() const;

	void setSpaceSize(float size);

	float stringWidth(const std::string& s) const;

	float stringHeight(const std::string& s) const;

	ofRectangle getStringBoundingBox(const std::string& s, float x, float y, bool vflip=true) const;


	void drawString(const std::string& s, float x, float y) const;

	void drawStringAsShapes(const std::string& s, float x, float y) const;

	ofTTFCharacter getCharacterAsPoints(int character, bool vflip=true, bool filled=true) const;
	vector<ofTTFCharacter> getStringAsPoints(const std::string& str, bool vflip=true, bool filled=true) const;
	const ofMesh & getStringMesh(const std::string& s, float x, float y, bool vflip=true) const;
	const ofTexture & getFontTexture() const;

};

// DIFF: ofTrueTypeFont.h: added attributes: lineHeight, letterSpacing, & spaceSize
%attribute(ofTrueTypeFont, float, lineHeight, getLineHeight, setLineHeight);
%attribute(ofTrueTypeFont, float, letterSpacing, getLetterSpacing, setLetterSpacing);
%attribute(ofTrueTypeFont, float, spaceSize, getSpaceSize, setSpaceSize);

// ----- MATH ------------------------------------------------------------------

// ----- ofMatrix3x3.h -----

#ifdef OF_SWIG_RENAME
	%rename(Matrix3x3) ofMatrix3x3;
#endif

%include "math/ofMatrix3x3.h"

%extend ofMatrix3x3 {
	const char* __str__() {
		stringstream str;
		str << (*$self);
		return str.str().c_str();
	}
};

// ----- ofMatrix4x4.h -----

%ignore ofMatrix4x4::operator()(int,int) const;

%include "math/ofMatrix4x4.h"

%extend ofMatrix4x4 {
	const char* __str__() {
		stringstream str;
		str << (*$self);
		return str.str().c_str();
	}
};

// ----- ofQuaternion.h -----

// silence warning as SWIG ignores these anyway
// since it uses the non-const versions
%ignore ofQuaternion::x() const;
%ignore ofQuaternion::y() const;
%ignore ofQuaternion::z() const;
%ignore ofQuaternion::w() const;

%include "math/ofQuaternion.h"

%extend ofQuaternion {
	const char* __str__() {
		stringstream str;
		str << (*$self);
		return str.str().c_str();
	}
};

// ----- ofVecs -----

// TODO: ofVec4f.h: ignoring ofVec4f::set(f), defined but not implemented in OF 0.8.4
%ignore ofVec4f::set(float);
%ignore ofVec2f::getLimited;

%include "math/ofVec2f.h"

%extend ofVec2f {
	const char* __str__() {
		stringstream str;
		str << (*$self);
		return str.str().c_str();
	}
};

%include "math/ofVec3f.h"

%extend ofVec3f {
	const char* __str__() {
		stringstream str;
		str << (*$self);
		return str.str().c_str();
	}
};

%include "math/ofVec4f.h"

%extend ofVec4f {
	const char* __str__() {
		stringstream str;
		str << (*$self);
		return str.str().c_str();
	}
};

// ----- ofMath.h -----

// ignore the template functions
%ignore ofInterpolateCosine;
%ignore ofInterpolateCubic;
%ignore ofInterpolateCatmullRom;
%ignore ofInterpolateHermite;

// declare float functions
ofInterpolateCosine(float y1, float y2, float pct);
ofInterpolateCubic(float y1, float y2, float pct);
ofInterpolateCatmullRom(float y1, float y2, float pct);
ofInterpolateHermite(float y1, float y2, float pct);

%include "math/ofMath.h"

// tell SWIG about template functions
#ifdef OF_SWIG_RENAME
	%template(interpolateCosine) ofInterpolateCosine<float>;
	%template(interpolateCubic) ofInterpolateCubic<float>;
	%template(interpolateCatmullRom) ofInterpolateCatmullRom<float>;
	%template(interpolateHermite) ofInterpolateHermite<float>;
#else
	%template(ofInterpolateCosine) ofInterpolateCosine<float>;
	%template(ofInterpolateCubic) ofInterpolateCubic<float>;
	%template(ofInterpolateCatmullRom) ofInterpolateCatmullRom<float>;
	%template(ofInterpolateHermite) ofInterpolateHermite<float>;
#endif

// ----- UTILS -----------------------------------------------------------------

// ----- ofXml.h -----

%include "utils/ofXml.h"

// ----- ofMatrixStack.h -----

%include "utils/ofMatrixStack.h"

// ----- VIDEO -----------------------------------------------------------------

// ----- ofVideoGrabber.h -----

%include "video/ofVideoGrabber.h"

// ----- ofVideoPlayer.h -----

%include "video/ofVideoPlayer.h"

// ----- EVENTS ----------------------------------------------------------------

// ----- ofEvents.h -----

// DIFF: ofEvents.h: ignore event classes, event args, and registration functions
%ignore ofEventArgs;
%ignore ofEntryEventArgs;
%ignore ofKeyEventArgs;
%ignore ofMouseEventArgs;
// need ofTouchEventArgs
%ignore ofAudioEventArgs;
%ignore ofResizeEventArgs;
%ignore ofMessage;

%ignore ofCoreEvents;
%ignore ofEvents;

%ignore ofRegisterMouseEvents;
%ignore ofRegisterKeyEvents;
%ignore ofRegisterTouchEvents;
%ignore ofRegisterGetMessages;
%ignore ofRegisterDragEvents;
%ignore ofUnregisterMouseEvents;
%ignore ofUnregisterKeyEvents;
%ignore ofUnregisterTouchEvents;
%ignore ofUnregisterGetMessages;
%ignore ofUnregisterDragEvents;

%ignore ofNotifySetup;
%ignore ofNotifyUpdate;
%ignore ofNotifyDraw;
%ignore ofNotifyKeyPressed;
%ignore ofNotifyKeyReleased;
%ignore ofNotifyKeyEvent;
%ignore ofNotifyMousePressed;
%ignore ofNotifyMouseReleased;
%ignore ofNotifyMouseDragged;
%ignore ofNotifyMouseMoved;
%ignore ofNotifyMouseEvent;
%ignore ofNotifyExit;
%ignore ofNotifyWindowResized;
%ignore ofNotifyWindowEntry;
%ignore ofNotifyDragEvent;

%include "events/ofEvents.h"

////////////////////////////////////////////////////////////////////////////////
// ----- LUA -------------------------------------------------------------------

// support for simple classes from http://lua-users.org/wiki/SimpleLuaClasses
//
// example usage:
//
// -- class declaration
// MyClass = class()
//
// -- constructor & attributes
// function MyClass:__init(x, y)
//     self.x = x
//     self.y = y
//     self.bBeingDragged = false
//     self.bOver = false
//     self.radius = 4
// end
//
// -- create instance & access attribute
// myclass = MyClass(10, 10)
// myclass.x = 100

#ifdef SWIGLUA

%luacode %{

-- this isnt wrapped correctly, so set it here
of.CLOSE = true

-- handle typedefs which swig doesnt wrap
of.Point = of.Vec3f

-- class.lua
-- Compatible with Lua 5.1 (not 5.0).
function class(base, __init)
   local c = {}    -- a new class instance
   if not __init and type(base) == 'function' then
	  __init = base
	  base = nil
   elseif type(base) == 'table' then
	-- our new class is a shallow copy of the base class!
	  for i,v in pairs(base) do
		 c[i] = v
	  end
	  c._base = base
   end
   -- the class will be the metatable for all its objects,
   -- and they will look up their methods in it.
   c.__index = c

   -- expose a constructor which can be called by <classname>(<args>)
   local mt = {}
   mt.__call = function(class_tbl, ...)
   local obj = {}
   setmetatable(obj,c)
   if class_tbl.__init then
	  class_tbl.__init(obj,...)
   else
	  -- make sure that any stuff from the base class is initialized!
	  if base and base.__init then
	  base.__init(obj, ...)
	  end
   end
   return obj
   end
   c.__init = __init
   c.is_a = function(self, klass)
	  local m = getmetatable(self)
	  while m do
		 if m == klass then return true end
		 m = m._base
	  end
	  return false
   end
   setmetatable(c, mt)
   return c
end

%}

#endif

////////////////////////////////////////////////////////////////////////////////
// ----- Python ----------------------------------------------------------------

#ifdef SWIGPYTHON
#ifndef OF_SWIG_RENAME

%pythoncode %{

# handle typedefs which swig doesnt wrap
ofPoint = ofVec3f

# renaming log -> ofLog
ofLog = log
del log

%}

#endif
#endif
