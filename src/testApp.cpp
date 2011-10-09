#include "testApp.h"
#include <string.h>


//--------------------------------------------------------------
void testApp::setup(){	 

	// load in sounds:
	beat.loadSound("sounds/jdee_beat.mp3");
	beat.setVolume(1.0f);
	beat.setLoop(true);
	beat.play();
	
	// the fft needs to be smoothed out, so we create an array of floats
	// for that purpose:
	fftSmoothed = new float[8192];
	for (int i = 0; i < 8192; i++){
		fftSmoothed[i] = 0;
	}
	
	nBandsToGet = 4;
	minUpdatesBeforeRetrigger = 100;
	updatesUntilTriggerable = (int *) malloc(nBandsToGet * sizeof(int));
	for (int i=0; i < nBandsToGet; i++)
		updatesUntilTriggerable[i] = 0;

	oscPeakTriggerThresh = 0.17f;
}


//--------------------------------------------------------------
void testApp::update(){
	
	ofBackground(80, 20, 80);

	// update the sound playing system:
	ofSoundUpdate();	
	
	// grab the fft, and put in into a "smoothed" array,
	//		by taking maximums, as peaks and then smoothing downward
	float * val = ofSoundGetSpectrum(nBandsToGet);
	for (int i = 0;i < nBandsToGet; i++){
		
		// let the smoothed vcalue sink to zero:
		fftSmoothed[i] *= 0.96f;
		
		// take the max, either the smoothed or the incoming:
		if (fftSmoothed[i] < val[i]) fftSmoothed[i] = val[i];

		if (updatesUntilTriggerable[i] != 0)
			updatesUntilTriggerable[i] -= 1;
		
		//check to see if OSC peak-hit message should be fired
		//fired if above threshold and past retrigger time
		if (val[i] >= oscPeakTriggerThresh && updatesUntilTriggerable[i] == 0) {
			updatesUntilTriggerable[i] = minUpdatesBeforeRetrigger;
		}
			
	}
}

//--------------------------------------------------------------
void testApp::draw(){

	//rectangle where fft results go
	ofEnableAlphaBlending();
	ofSetColor(255,255,255,100);
	int fftRectHeight = 200;
	int fftRectX = 100;
	int fftRectY = ofGetHeight() - 100;
	int fftRectWidth = 5 * 128;
	ofRect(fftRectX, ofGetHeight() - 300, 5 * 128, fftRectHeight);
	ofDisableAlphaBlending();

	// draw the fft results:
	ofSetColor(255, 255, 255, 255);

	float bandWidth = (float)(fftRectWidth) / nBandsToGet;
	for (int i = 0; i < nBandsToGet; i++){
		// (we use negative height here, because we want to flip them
		// because the top corner is 0,0)
		ofRect(fftRectX + i * bandWidth, fftRectY, bandWidth, -(fftSmoothed[i] * fftRectHeight));
	}

	//draw the current threshold line
	ofSetColor(5, 255, 5, 255);
	ofRect(fftRectX, fftRectY - (oscPeakTriggerThresh * fftRectHeight), fftRectWidth, 5);

	//for OSC trigger thresh
	ofSetColor(ofColor::white);
	string outstring = "peak detect threshold is ";
	char converted[10] = ""; 
	sprintf(converted, "%.2f", oscPeakTriggerThresh);
	outstring += converted;
	outstring += ". Press +/- to change it.";
	ofDrawBitmapString(outstring, 100, 20);
	//for num bands
	outstring = "Number of bands is ";
	sprintf(converted, "%3d", nBandsToGet);
	outstring += converted;
	outstring += ". Press q/a to raise or lower it.";
	ofDrawBitmapString(outstring, 100, 40);


	//rectangle to indicate if OSC is sent
	ofEnableAlphaBlending();
	ofSetColor(255, 255, 255, 100);
	ofRect(100, 100, 5 * 128, 200);
	ofDisableAlphaBlending();



	//indicate if OSC peak-hit message has been sent
	for (int i = 0; i < nBandsToGet; i++){
		if (updatesUntilTriggerable[i] > 0) {
			//ofDrawBitmapString(outstring, 280, 20 );
		}
		
	}

}


//--------------------------------------------------------------
void testApp::keyPressed  (int key){ 
	if( key == '+' ) {
		oscPeakTriggerThresh += 0.01f;
		if (oscPeakTriggerThresh > 1.0f)
			oscPeakTriggerThresh = 1.0f;
	}
	else if( key == '-' ) {
		oscPeakTriggerThresh -= 0.01f;
		if (oscPeakTriggerThresh < 0.0f)
			oscPeakTriggerThresh = 0.0f;
	}

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){ 
	
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}
 
//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}

