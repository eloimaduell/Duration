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

#include "ofxTLUIHeaderAudio.h"
#include "ofxTLAudioTrack.h"
#include <locale>


//--------------------------------------------------------------------------------
ofxTLUIHeaderAudio::ofxTLUIHeaderAudio()
{
    // base class constructor
    ofxTLUIHeader::ofxTLUIHeader();
    
    // specific gui for this class
    guiSelectAudio = new ofxDatGuiButton("SELECT AUDIO");
    guiSelectAudio->onButtonEvent(this, &ofxTLUIHeaderAudio::onButtonEvent);
    headerGuiComponents.push_back(guiSelectAudio);

    for(int i=0;i<headerGuiComponents.size();i++)
    {
        if(i!=0) headerGuiComponents[i]->setBackgroundColor(ofColor(35,35,35,255));

        headerGuiComponents[i]->setWidth(headerGuiComponentWidth,headerGuiComponentWidth/2);
    }

    reorderGuiComponents();

}

//--------------------------------------------------------------------------------
ofxTLUIHeaderAudio::~ofxTLUIHeaderAudio()
{
    // base class constructor
    //ofxTLUIHeader::~ofxTLUIHeader();
}

//--------------------------------------------------------------------------------
void ofxTLUIHeaderAudio::onButtonEvent(ofxDatGuiButtonEvent e)
{
    // base class
    ofxTLUIHeader::onButtonEvent(e);

    cout << "ofxTLUIHeaderAudio :: button event from : " << e.target->getLabel() <<endl;
    
    if (e.target->getLabel() == "SELECT AUDIO")
    {
        ofFileDialogResult r = ofSystemLoadDialog();
        if(r.bSuccess){
            ofxTLAudioTrack* audioTrack = (ofxTLAudioTrack*)trackHeader->getTrack();
            audioTrack->loadSoundfile(r.getPath());
            modified = true;
        }
    }
    

    
}

