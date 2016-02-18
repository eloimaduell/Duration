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

#include "ofxTLUIHeaderBangs.h"
#include "ofxTLColorTrack.h"
#include <locale>


//--------------------------------------------------------------------------------
ofxTLUIHeaderBangs::ofxTLUIHeaderBangs()
{

    ofxTLUIHeader::ofxTLUIHeader();
    
    guiOscIn = new ofxDatGuiToggle("OSC IN",false);

    guiOscIn->onButtonEvent(this, &ofxTLUIHeaderBangs::onButtonEvent);
    
    guiOscIn->setStripeColor(oscCol);
    
    headerGuiComponents.push_back(guiOscIn);
    
    for(int i=0;i<headerGuiComponents.size();i++)
    {
        headerGuiComponents[i]->setWidth(headerGuiComponentWidth,headerGuiComponentWidth/2);
        headerGuiComponents[i]->setLabelColor(ofColor(180,180,180));
        if(i!=0)
        {
            headerGuiComponents[i]->setBackgroundColor(ofColor(35,35,35,255));
        }
        else
        {
            headerGuiComponents[i]->setLabelColor(ofColor(0,0,0));
        }
    }

    rearrangeComponents();
    //reorderGuiComponents();


}

//--------------------------------------------------------------------------------
ofxTLUIHeaderBangs::~ofxTLUIHeaderBangs()
{
}


//--------------------------------------------------------------------------------
void ofxTLUIHeaderBangs::onButtonEvent(ofxDatGuiButtonEvent e)
{
    // base class
    ofxTLUIHeader::onButtonEvent(e);
}

//--------------------------------------------------------------------------------
void ofxTLUIHeaderBangs::onTextInputEvent(ofxDatGuiTextInputEvent e)
{
    cout << "ofxTLUIHeaderBangs :: textInput event from : " << e.target->getLabel() <<endl;
    // base class
    ofxTLUIHeader::onTextInputEvent(e);
}


