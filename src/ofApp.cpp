#include "ofApp.h"


ofVec2f ofApp::rescale(float x, float y)
{
	int nx = (int)(ofGetWindowWidth() * (x + 1.5) / 3.0);
	int ny = (int)(ofGetWindowHeight() * (y + 1.5) / 3.0);
	return ofVec2f(nx, ny);
}

ofVec2f ofApp::rescale(ofVec2f old)
{
	return rescale(old.x, old.y);
}

//--------------------------------------------------------------
void ofApp::setup() {
	//comp = simplicialComplex(100, 10);
	//comp = simplicialComplex();
	points = comp.get2d();
	radius = 0.5;

}

//--------------------------------------------------------------
void ofApp::update(){
	
	prox = comp.getProximityGraph(radius);
	simplices = comp.getSimplices(radius);
	//radius += 0.01;
}

//--------------------------------------------------------------
void ofApp::draw(){
	
	ofClear(ofColor::gray);
	

	shapes.clear();
	for (int  size = simplices.size() - 1; size > 1; size--)
	{
		vector<vector<int>> simplicesOfSize = simplices[size];
		ofPath shapesOfSize;
		for (int simplexIndex = 2; simplexIndex < simplicesOfSize.size(); simplexIndex++)
		{
			vector<int> thisSimplex = simplicesOfSize[simplexIndex];
			//ofSetColor(1.0,0, 0, 0.2*size);
			//ofBeginShape();
			shapesOfSize.moveTo(rescale(points[thisSimplex[0]]));
			for (int pointIndex = 1; pointIndex < thisSimplex.size(); pointIndex++)
			{
				//ofVertex(rescale(points[thisSimplex[pointIndex]]));
				shapesOfSize.lineTo(rescale(points[thisSimplex[pointIndex]]));
			}
			//ofEndShape();
			shapesOfSize.close();
			shapesOfSize.setFillColor(ofColor(255, 5 * size));
			shapesOfSize.setStrokeColor(ofColor(255, 7 * size));
			shapesOfSize.setStrokeWidth(3);
			shapesOfSize.setFilled(true);
		}
		shapes.push_back(shapesOfSize);
	}

	for (int revSize = 0; revSize < shapes.size(); revSize++)
	{
		shapes[revSize].draw();
	}

	
	for (int i = 0; i < points.size(); i++)
	{
		ofSetColor(ofColor::red);
		ofDrawCircle(rescale(points[i]), 5);
		ofSetColor(ofColor::black);
		ofDrawBitmapString(to_string(i), rescale(points[i]) + ofVec2f(5, 5));
	}

	vector<vector<int>> lines = simplices[1];
	for (auto it = lines.begin(); it < lines.end(); it++)
	{
		ofSetColor(255, 0, 0, 128);
		ofDrawLine(rescale(points[(*it)[0]]), rescale(points[(*it)[1]]));
	}

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch(key)
	{
	case 'r': 
		radius = 0;
		break;
	case 'u':
		radius += 0.1;
		break;
	case 'j':
		radius -= 0.1;
		break;
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
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

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
