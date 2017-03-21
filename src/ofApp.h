#pragma once

#include "simplicialComplex.h"
#include "ofMain.h"

using namespace std;

class ofApp : public ofBaseApp{
	private:
		simplicialComplex comp;
		float radius;
		Matrix<bool, Dynamic, Dynamic> prox;
		vector<ofVec2f> points;
		vector<vector<vector<int>>> simplices;
		vector<ofPath> shapes;
	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		ofVec2f rescale(float x, float y);
		ofVec2f rescale(ofVec2f old);


		
};
