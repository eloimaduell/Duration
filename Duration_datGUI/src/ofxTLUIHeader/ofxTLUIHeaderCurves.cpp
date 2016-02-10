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

#include "ofxTLUIHeaderCurves.h"
#include "ofxTLColorTrack.h"
#include <locale>


//--------------------------------------------------------------------------------
ofxTLUIHeaderCurves::ofxTLUIHeaderCurves()
{

    ofxTLUIHeader::ofxTLUIHeader();
    
    guiOscIn = new ofxDatGuiToggle("Osc In",false);
    guiMin = new ofxDatGuiTextInput("Min","0.000");
    guiMax = new ofxDatGuiTextInput("Max","1.000");

    guiOscIn->onButtonEvent(this, &ofxTLUIHeaderCurves::onButtonEvent);
    guiMin->onTextInputEvent(this,&ofxTLUIHeaderCurves::onTextInputEvent);
    guiMax->onTextInputEvent(this,&ofxTLUIHeaderCurves::onTextInputEvent);

    //colors
    guiOscIn->setStripeColor(oscCol);
    guiMin->setStripeColor(valueCol);
    guiMax->setStripeColor(valueCol);

    headerGuiComponents.push_back(guiOscIn);
    headerGuiComponents.push_back(guiMin);
    headerGuiComponents.push_back(guiMax);
    
    for(int i=0;i<headerGuiComponents.size();i++)
    {
        if(i!=0) headerGuiComponents[i]->setBackgroundColor(ofColor(35,35,35,255));
        headerGuiComponents[i]->setWidth(headerGuiComponentWidth,headerGuiComponentWidth/2);
    }


    reorderGuiComponents();

    

}

//--------------------------------------------------------------------------------
ofxTLUIHeaderCurves::~ofxTLUIHeaderCurves()
{
}


//--------------------------------------------------------------------------------
void ofxTLUIHeaderCurves::onButtonEvent(ofxDatGuiButtonEvent e)
{
    // base class
    ofxTLUIHeader::onButtonEvent(e);
}

//--------------------------------------------------------------------------------
void ofxTLUIHeaderCurves::onTextInputEvent(ofxDatGuiTextInputEvent e)
{
    cout << "ofxTLUIHeaderCurves :: textInput event from : " << e.target->getLabel() <<endl;
    
    bool isRange=false;
    
    if (e.target->getLabel() == "MIN")
    {
        isRange = true;
    }
    else if (e.target->getLabel() == "MAX")
    {
        isRange = true;
    }
    
    
    if(isRange)
    {
        float min = ofToFloat(guiMin->getText());
        float max = ofToFloat(guiMax->getText());
        
        cout << "ofxTLUIHeaderCurves :: onTextInput :: min = " << min << " ... max =  " << max << endl;
        ofRange newValueRange = ofRange(min,max);
        
        ofxTLKeyframes* track = (ofxTLKeyframes*)trackHeader->getTrack();
        track->setValueRange(newValueRange);
    }
    
}

//--------------------------------------------------------------------------------
void ofxTLUIHeaderCurves::setMinMax(float _min, float _max)
{
    guiMin->setText(ofToString(_min));
    guiMax->setText(ofToString(_max));
    
}

//--------------------------------------------------------------------------------
void ofxTLUIHeaderCurves::setMin(float _min)
{
    guiMin->setText(ofToString(_min));
}
//--------------------------------------------------------------------------------
void ofxTLUIHeaderCurves::setMax(float _max)
{
    guiMax->setText(ofToString(_max));
}

//--------------------------------------------------------------------------------
void ofxTLUIHeaderCurves::reorderGuiComponents()
{
    ofxTLUIHeader::reorderGuiComponents();
    
    ofxDatGuiComponent* aux;
    int whereIsOscIn=-1;
    int whereIsMin=-1;
    int whereIsMax=-1;
    
    // look where is ...
    for(int i=0;i<headerGuiComponents.size();i++)
    {
        if(headerGuiComponents[i]->getLabel()=="Osc In")
        {
            whereIsOscIn = i;
        }

        else if(headerGuiComponents[i]->getLabel()=="Min")
        {
            whereIsMin = i;
        }
        else if(headerGuiComponents[i]->getLabel()=="Max")
        {
            whereIsMax = i;
        }
    }
    
    aux = headerGuiComponents[whereIsMax];

    headerGuiComponents[whereIsMax] = headerGuiComponents[whereIsOscIn];
    headerGuiComponents[whereIsOscIn] = headerGuiComponents[whereIsMin];
    headerGuiComponents[whereIsMin] = aux;
    
//    headerGuiComponents[whereIsOscIn] = headerGuiComponents[headerGuiComponents.size()-1];
//    headerGuiComponents[headerGuiComponents.size()-1] = aux;

//    //headerGuiComponents[whereIsMax] = headerGuiComponents[whereIsOscIn];
//    //headerGuiComponents[whereIsOscIn] = headerGuiComponents[whereIsMin];
//    headerGuiComponents[whereIsMax] = headerGuiComponents[whereIsMin];
//    headerGuiComponents[whereIsMin] = aux;
    
}
