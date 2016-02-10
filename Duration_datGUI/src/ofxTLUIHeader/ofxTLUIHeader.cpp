/**
 * Duration
 * Standalone timeline for Creative Code
 *
 * Copyright (c) 2012 James George
 * Development Supported by YCAM InterLab http://interlab.ycam.jp/en/
 * http://jamesgeorge.org + http://flightphase.com
 * http://github.com/obviousjim + http://github.com/flightphase
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#include "ofxTLUIHeader.h"
#include "ofxTLAudioTrack.h"
#include <locale>


//--------------------------------------------------------------------------------
bool isNumber(const string& s){
	locale loc;
	std::string::const_iterator it = s.begin();
    while (it != s.end() && (std::isdigit(*it, loc))) ++it;
	return !s.empty() && it == s.end();
}

//--------------------------------------------------------------------------------
ofxTLUIHeader::ofxTLUIHeader(){
    trackHeader = NULL;
    shouldDelete = false;
	lastInputReceivedTime = -1000;

	hasReceivedValue = false;
	hasSentValue = false;
	lastFloatSent = 0;
	lastBoolSent = false;
	lastColorSent = ofColor(0,0,0);
	lastValueReceived = 0;
	audioNumberOfBins = 256;

//    sendOSCEnable = NULL;
//	receiveOSCEnable = NULL;
	modified = false;
    
    // DAT GUI
    guiIsEnabled = true;
    
    headerGuiComponentWidth = 120;
    guiTrackName = new ofxDatGuiButton("DEFAULT_NAME");
    guiTrackName->setBackgroundColor(ofColor(200,200,200,255));
    guiTrackName->setLabelColor(ofColor(0,0,0,255));
    guiPlaySolo = new ofxDatGuiButton("PLAY SOLO");
    guiDelay = new ofxDatGuiTextInput("DELAY MS.","0.000");
    guiOscOut = new ofxDatGuiToggle("OSC OUT",true);
    guiDeleteTrack = new ofxDatGuiButton("DELETE");
    guiOscAddressButton = new ofxDatGuiButton("OSC ADDRESS     ::");
    guiOscAddressLabel = new ofxDatGuiLabel("/default");
    //guiOscAddressLabel->setLabelColor(ofColor(0,255,0));
    guiOscAddressLabel->setBackgroundColor(ofColor(80,80,80));
    
    // DAT GUI events
    guiTrackName->onButtonEvent(this, &ofxTLUIHeader::onButtonEvent);
    guiPlaySolo->onButtonEvent(this, &ofxTLUIHeader::onButtonEvent);
    guiOscOut->onButtonEvent(this, &ofxTLUIHeader::onButtonEvent);
    guiDeleteTrack->onButtonEvent(this, &ofxTLUIHeader::onButtonEvent);
    guiOscAddressButton->onButtonEvent(this,&ofxTLUIHeader::onButtonEvent);

    // STRIPE colors
    trackCol = ofColor(255,125,0);
    guiTrackName->setStripeWidth(5);
    guiTrackName->setStripeColor(trackCol);
    guiPlaySolo->setStripeColor(trackCol);
    guiDelay->setStripeColor(trackCol);
    
//    oscCol = ofColor(0,120,255);
    oscCol = ofColor(128,128,128);
    guiOscOut->setStripeColor(oscCol);
    guiOscAddressButton->setStripeColor(oscCol);
    guiOscAddressLabel->setStripeColor(oscCol);
    
//    deleteCol = ofColor(255,0,0);
    deleteCol = ofColor(128,128,128);
    guiDeleteTrack->setStripeColor(deleteCol);

    //valueCol = ofColor(0,255,120);
    valueCol = ofColor(128,128,128);
    
    // array of gui components
    headerGuiComponents.push_back(guiTrackName);
    headerGuiComponents.push_back(guiPlaySolo);
    headerGuiComponents.push_back(guiDelay);
    headerGuiComponents.push_back(guiOscOut);
    headerGuiComponents.push_back(guiOscAddressButton);
    headerGuiComponents.push_back(guiOscAddressLabel);
    headerGuiComponents.push_back(guiDeleteTrack);
    
    for(int i=0;i<headerGuiComponents.size();i++)
    {
        if(i!=0) headerGuiComponents[i]->setBackgroundColor(ofColor(35,35,35,255));

        headerGuiComponents[i]->setWidth(headerGuiComponentWidth,headerGuiComponentWidth/2);
    }
}

//--------------------------------------------------------------------------------
ofxTLUIHeader::~ofxTLUIHeader()
{
    // ofxDatGui delete
    for(int i=0;i<headerGuiComponents.size();i++)
    {
        delete headerGuiComponents[i];
    }

    if(trackHeader != NULL){
        ofRemoveListener(trackHeader->events().viewWasResized, this, &ofxTLUIHeader::viewWasResized);
    }

}

//--------------------------------------------------------------------------------
void ofxTLUIHeader::draw()
{
    for(int i=0;i<headerGuiComponents.size();i++)
    {
        headerGuiComponents[i]->draw();
    }
}


//--------------------------------------------------------------------------------
void ofxTLUIHeader::update()
{
    for(int i=0;i<headerGuiComponents.size();i++)
    {
        if(!getTrack()->isDragging) headerGuiComponents[i]->update();
    }
}

//--------------------------------------------------------------------------------
void ofxTLUIHeader::setTrackHeader(ofxTLTrackHeader* header){
    trackHeader = header;

    //create gui
    ofRectangle headerRect = trackHeader->getDrawRect();
    headerRect.width = 600;
    headerRect.x = trackHeader->getTimeline()->getTopRight().x - (headerRect.width + 50);


    trackType = trackHeader->getTrack()->getTrackType();
//	if(trackType != "Audio" && trackType != "Video"){
//		ofxUILabelButton* playSolo = new ofxUILabelButton(">", false,0,0,0, OFX_UI_FONT_SMALL);
//		playSolo->setPadding(0);
//		gui->addWidgetRight(playSolo);
//	}

//	if(trackType == "Curves" || trackType == "LFO"){
//        //SET THE RANGE
//        ofxTLKeyframes* tweenTrack = (ofxTLKeyframes*)trackHeader->getTrack();
//        minDialer = new ofxUINumberDialer(-9999., 9999., tweenTrack->getValueRange().min, 2, "min", OFX_UI_FONT_SMALL);
//        minDialer->setPadding(0);
//        gui->addWidgetRight( minDialer );
//
//        maxDialer = new ofxUINumberDialer(-9999., 9999, tweenTrack->getValueRange().max, 2, "max", OFX_UI_FONT_SMALL);
//        maxDialer->setPadding(0);
//        gui->addWidgetRight( maxDialer );
//
//		resetRange = new ofxUILabelButton(translation->translateKey("reset"), false, 0,0,0,0,OFX_UI_FONT_SMALL);
//		resetRange->setPadding(0);
//		gui->addWidgetRight(resetRange);
//    }
//	else if(trackType == "Colors"){
//		palette = new ofxUILabelButton(translation->translateKey("change palette"), false,0,0,0,0, OFX_UI_FONT_SMALL);
//		palette->setPadding(0);
//		gui->addWidgetRight(palette);
//	}
//	else if(trackType == "Audio"){
//		audioClip = new ofxUILabelButton(translation->translateKey("select audio"), false,0,0,0,0, OFX_UI_FONT_SMALL);
//		audioClip->setPadding(0);
//		gui->addWidgetRight(audioClip);
//
//        //REMOVING BINS
////		ofxUILabel* binLabel = new ofxUILabel(0, 0, "bins", translation->translateKey("bins"), OFX_UI_FONT_SMALL);
////		binLabel->setPadding(0);
////		gui->addWidgetRight(binLabel);
//
////		bins = new ofxUITextInput("bins", "256", 50, 0,0,0, OFX_UI_FONT_SMALL);
////		bins->setAutoClear(false);
////		bins->setPadding(0);
////		gui->addWidgetRight(bins);
//	}

//	if(trackType == "Bangs" || trackType == "Curves")
//    {
//		receiveOSCEnable = new ofxUIToggle(translation->translateKey("receive osc"), true, 17, 17, 0, 0, OFX_UI_FONT_SMALL);
//		receiveOSCEnable->setPadding(1);
//		gui->addWidgetRight(receiveOSCEnable);
//	}

//////	if(trackType != "Audio"){ //TODO: audio should send some nice FFT OSC
//		sendOSCEnable = new ofxUIToggle(translation->translateKey("send osc"), true, 17, 17, 0, 0, OFX_UI_FONT_SMALL);
//		sendOSCEnable->setPadding(1);
//		gui->addWidgetRight(sendOSCEnable);
//	}

    ofAddListener(trackHeader->events().viewWasResized, this, &ofxTLUIHeader::viewWasResized);
    
    // DAT GUI
    guiTrackName->setLabel(getTrack()->getDisplayName());
    
    for(int i=0;i<headerGuiComponents.size();i++)
    {
        headerGuiComponents[i]->setPosition(i*headerGuiComponentWidth,trackHeader->getDrawRect().y);
    }

}

//--------------------------------------------------------------------------------
void ofxTLUIHeader::viewWasResized(ofEventArgs& args)
{
     // DAT GUI
    for(int i=0;i<headerGuiComponents.size();i++)
    {
        headerGuiComponents[i]->setPosition(i*headerGuiComponentWidth,trackHeader->getDrawRect().y);
    }

}

//void ofxTLUIHeader::setMinFrequency(int frequency){
//
//}

//int ofxTLUIHeader::getMinFrequency(){
//	if(getTrackType() == "Audio"){
//		return ((ofxTLAudioTrack*)getTrack())->getFFTBinCount();
//	}
//	return 0;
//}
//
//void ofxTLUIHeader::setBandsPerOctave(int bands){
//
//}
//
//int ofxTLUIHeader::getBandsPerOctave(){
//
//}


//int ofxTLUIHeader::getNumberOfBins(){
//	if(getTrackType() == "Audio"){
//		return ((ofxTLAudioTrack*)getTrack())->getFFTBinCount();
//	}
//	return 0;
//}
//
//void ofxTLUIHeader::setNumberOfbins(int binCount){
//
//	if(getTrackType() == "Audio"){
//		((ofxTLAudioTrack*)getTrack())->getFFTSpectrum(binCount);
//		bins->setTextString(ofToString(binCount));
//	}
//}

//--------------------------------------------------------------------------------
void ofxTLUIHeader::setShouldDelete(bool del)
{
	shouldDelete = del;
	if(shouldDelete){
		ofRemoveListener(trackHeader->events().viewWasResized, this, &ofxTLUIHeader::viewWasResized);
		trackHeader = NULL; //this is needed to circumvent the problem in the destructor
	}
}

//--------------------------------------------------------------------------------
bool ofxTLUIHeader::getShouldDelete(){
    return shouldDelete;
}

//--------------------------------------------------------------------------------
bool ofxTLUIHeader::getModified(){
	bool b = modified;
	modified = false;
	return b;
}

//--------------------------------------------------------------------------------
ofxTLTrack* ofxTLUIHeader::getTrack(){
	return trackHeader->getTrack();
}

//--------------------------------------------------------------------------------
ofxTLTrackHeader* ofxTLUIHeader::getTrackHeader(){
	return trackHeader;
}

//--------------------------------------------------------------------------------
string ofxTLUIHeader::getTrackType(){
	return trackType;
}

//--------------------------------------------------------------------------------
void ofxTLUIHeader::onButtonEvent(ofxDatGuiButtonEvent e)
{
    // we have a couple ways to figure out which button was clicked //
    
    // we can compare our button pointer to the target of the event //
//    if (e.target == guiTrackName)
//    {
//        
//        
//    }

    // else // or we can check against the label of the event target //
    cout << "ofxTLUIHeader :: button event from : " << e.target->getLabel() <<endl;

    if(e.target == guiTrackName)
    {
        string newTrackName = ofSystemTextBoxDialog("Track name ?");
        if(newTrackName!="")
        {
            guiTrackName->setLabel(newTrackName);
            getTrack()->setDisplayName(newTrackName);
        }
    }
    
    
    
    if (e.target->getLabel() == "OSC OUT")
    {
        modified = true;

    }
    else if(e.target->getLabel() == "OSC IN")
    {
        modified = true;
        
    }
    else if(e.target->getLabel() == "PLAY SOLO")
    {
        //if(e.target->getEnabled())
        {
            cout << "PLAY SOLO PRESSED" << endl;
            modified = true;
            getTrack()->togglePlay();
        }
    }
    else if(e.target->getLabel() == "DELETE")
    {
        setShouldDelete(true);
        modified = true;
    }
    else if(e.target->getLabel()=="OSC ADDRESS     ::")
    {
        cout << "TLUHeader:: OSC Folder Event";
        string add = ofSystemTextBoxDialog("OSC address ?");
        if(add!="") guiOscAddressLabel->setLabel(add);
    }

    
}

//--------------------------------------------------------------------------------
void ofxTLUIHeader::setShowGui(bool b)
{
    for(int i=0;i<headerGuiComponents.size();i++)
    {
        headerGuiComponents[i]->setVisible(b);
    }
}

//--------------------------------------------------------------------------------
void ofxTLUIHeader::setEnabledGui(bool b)
{
    for(int i=0;i<headerGuiComponents.size();i++)
    {
        headerGuiComponents[i]->setEnabled(b);
    }
    guiIsEnabled = b;
}

//--------------------------------------------------------------------------------
bool ofxTLUIHeader::getEnabledGui()
{
    return guiIsEnabled;
}


//--------------------------------------------------------------------------------
void ofxTLUIHeader::reorderGuiComponents()
{
    ofxDatGuiComponent* aux;
    int whereIsDelete;
    
    // look where is "DELETE"
    for(int i=0;i<headerGuiComponents.size();i++)
    {
        if(headerGuiComponents[i]->getLabel()=="DELETE")
        {
            aux = headerGuiComponents[i];
            whereIsDelete = i;
        }
    }
    
    headerGuiComponents[whereIsDelete] = headerGuiComponents[headerGuiComponents.size()-1];
    headerGuiComponents[headerGuiComponents.size()-1] = aux;
    
    
}

//--------------------------------------------------------------------------------
void ofxTLUIHeader:: onTextInputEvent(ofxDatGuiTextInputEvent e)
{
}
