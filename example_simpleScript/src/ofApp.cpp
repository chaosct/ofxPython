#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	python.init();
	python.executeScript("mytest.py");
	ofxPythonObject klass = python.getObject("myApp");
	if(klass)
		python_program = klass();
	if(python_program)
		drawmethod = python_program.attr("draw");
}

//--------------------------------------------------------------
void ofApp::update(){
	if(python_program)
		python_program.method("update");
}

//--------------------------------------------------------------
void ofApp::draw(){
	if(drawmethod)
		drawmethod();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	python.executeString("myApp.color = [random.randint(0,255) for _ in range(3)]");
	if(python_program)
	{
		ofxPythonObject x = python.evalString("random.randint(100,200)");
		ofLog() << x.repr();
		python_program.attr("x") = x;
		python_program.attr("y") = python_program.attr("x");
		python_program.attr("d") = ofxPythonObject::fromInt(x.asInt()/4);
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
