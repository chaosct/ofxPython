
%module openframeworks
%{
//#define SWIG_FILE_WITH_INIT
#include "ofMain.h"
#include "ofBitmapFont.h"
%}


//MISSING DEFINITIONS

%include "std_string.i"
%{
bool ofDirectory::canRead() const{return true;}
bool ofDirectory::canWrite() const{return true;}
bool ofDirectory::canExecute() const{return true;}
int ofPath::getCurveResolution() const{return 0;}
void of3dPrimitive::clear(){}
void ofVec4f::set(float){}
void ofGLProgrammableRenderer::setSphereResolution(int){}
void of3dPrimitive::removeTexture(int){}
void of3dPrimitive::removeMesh(int){}
vector<int> & ofMesh::getFace(int){static vector<int> v; return v;}
void ofRectRounded(float, float, float, float, float, float){}

void TO_OF_LOG(const char * s)
{
	ofLog() << s;
}
%}
void TO_OF_LOG(const char * s);

%ignore OF_DEPRECATED_MSG;
%include "../../../../libs/openFrameworks/ofMain.h"
%include "../../../../libs/openFrameworks/utils/ofXml.h"
%include "../../../../libs/openFrameworks/utils/ofMatrixStack.h"
%include "../../../../libs/openFrameworks/utils/ofURLFileLoader.h"
//%include "../../../../libs/openFrameworks/utils/ofUtils.h"
//%include "../../../../libs/openFrameworks/utils/ofNoise.h"
//%include "../../../../libs/openFrameworks/utils/ofLog.h"
%include "../../../../libs/openFrameworks/utils/ofFileUtils.h"
//%include "../../../../libs/openFrameworks/utils/ofThread.h"
%include "../../../../libs/openFrameworks/utils/ofConstants.h"
%include "../../../../libs/openFrameworks/utils/ofSystemUtils.h"
%include "../../../../libs/openFrameworks/math/ofVec3f.h"
%include "../../../../libs/openFrameworks/math/ofVec4f.h"
%include "../../../../libs/openFrameworks/math/ofMatrix3x3.h"
%include "../../../../libs/openFrameworks/math/ofMath.h"
%include "../../../../libs/openFrameworks/math/ofVectorMath.h"
%include "../../../../libs/openFrameworks/math/ofVec2f.h"
%include "../../../../libs/openFrameworks/math/ofMatrix4x4.h"
%include "../../../../libs/openFrameworks/math/ofQuaternion.h"
%include "../../../../libs/openFrameworks/gl/ofShader.h"
%include "../../../../libs/openFrameworks/gl/ofVbo.h"
%include "../../../../libs/openFrameworks/gl/ofFbo.h"
%include "../../../../libs/openFrameworks/gl/ofTexture.h"
%include "../../../../libs/openFrameworks/gl/ofLight.h"
%include "../../../../libs/openFrameworks/gl/ofGLProgrammableRenderer.h"
%include "../../../../libs/openFrameworks/gl/ofGLUtils.h"
%include "../../../../libs/openFrameworks/gl/ofGLRenderer.h"
%include "../../../../libs/openFrameworks/gl/ofMaterial.h"
%include "../../../../libs/openFrameworks/gl/ofVboMesh.h"
%include "../../../../libs/openFrameworks/app/ofAppRunner.h"
%include "../../../../libs/openFrameworks/app/ofAppBaseWindow.h"
//%include "../../../../libs/openFrameworks/app/ofAppEGLWindow.h"
//%include "../../../../libs/openFrameworks/app/ofAppGLFWWindow.h"
%include "../../../../libs/openFrameworks/app/ofBaseApp.h"
//%include "../../../../libs/openFrameworks/app/ofAppGlutWindow.h"
//%include "../../../../libs/openFrameworks/app/ofAppNoWindow.h"
//%include "../../../../libs/openFrameworks/app/ofIcon.h"
%include "../../../../libs/openFrameworks/graphics/ofPath.h"
%include "../../../../libs/openFrameworks/graphics/ofPixels.h"
%include "../../../../libs/openFrameworks/graphics/ofCairoRenderer.h"
%include "../../../../libs/openFrameworks/graphics/ofImage.h"
%include "../../../../libs/openFrameworks/graphics/ofBitmapFont.h"
%include "../../../../libs/openFrameworks/graphics/of3dGraphics.h"
%include "../../../../libs/openFrameworks/graphics/ofGraphics.h"
%include "../../../../libs/openFrameworks/graphics/ofTessellator.h"
//%include "../../../../libs/openFrameworks/graphics/ofTrueTypeFont.h"
%include "../../../../libs/openFrameworks/graphics/ofPolyline.h"
%include "../../../../libs/openFrameworks/graphics/ofRendererCollection.h"
%include "../../../../libs/openFrameworks/3d/ofMesh.h"
%include "../../../../libs/openFrameworks/3d/ofEasyCam.h"
%include "../../../../libs/openFrameworks/3d/ofNode.h"
%include "../../../../libs/openFrameworks/3d/of3dUtils.h"
%include "../../../../libs/openFrameworks/3d/ofCamera.h"
%include "../../../../libs/openFrameworks/3d/of3dPrimitives.h"
%include "../../../../libs/openFrameworks/communication/ofSerial.h"
%include "../../../../libs/openFrameworks/communication/ofArduino.h"
//%include "../../../../libs/openFrameworks/video/ofDirectShowGrabber.h"
//%include "../../../../libs/openFrameworks/video/ofGstUtils.h"
//%include "../../../../libs/openFrameworks/video/ofGstVideoPlayer.h"
//%include "../../../../libs/openFrameworks/video/ofQTKitMovieRenderer.h"
//%include "../../../../libs/openFrameworks/video/ofQTKitGrabber.h"
//%include "../../../../libs/openFrameworks/video/ofQTKitPlayer.h"
//%include "../../../../libs/openFrameworks/video/ofQuickTimeGrabber.h"
%include "../../../../libs/openFrameworks/video/ofVideoGrabber.h"
%include "../../../../libs/openFrameworks/video/ofVideoPlayer.h"
//%include "../../../../libs/openFrameworks/video/ofGstVideoGrabber.h"
//%include "../../../../libs/openFrameworks/video/ofQuickTimePlayer.h"
//%include "../../../../libs/openFrameworks/video/ofQtUtils.h"
%include "../../../../libs/openFrameworks/sound/ofBaseSoundStream.h"
%include "../../../../libs/openFrameworks/sound/ofBaseSoundPlayer.h"
%include "../../../../libs/openFrameworks/sound/ofSoundStream.h"
//%include "../../../../libs/openFrameworks/sound/ofFmodSoundPlayer.h"
%include "../../../../libs/openFrameworks/sound/ofSoundPlayer.h"
%include "../../../../libs/openFrameworks/sound/ofRtAudioSoundStream.h"
%include "../../../../libs/openFrameworks/sound/ofOpenALSoundPlayer.h"
%include "../../../../libs/openFrameworks/types/ofColor.h"
%include "../../../../libs/openFrameworks/types/ofRectangle.h"
%include "../../../../libs/openFrameworks/types/ofParameterGroup.h"
%include "../../../../libs/openFrameworks/types/ofPoint.h"
%include "../../../../libs/openFrameworks/types/ofTypes.h"
//%include "../../../../libs/openFrameworks/types/ofBaseTypes.h"
%include "../../../../libs/openFrameworks/types/ofParameter.h"
%include "../../../../libs/openFrameworks/events/ofDelegate.h"
%include "../../../../libs/openFrameworks/events/ofEvents.h"
%include "../../../../libs/openFrameworks/events/ofEventUtils.h"

