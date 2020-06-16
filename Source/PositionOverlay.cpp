/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.4.7

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2017 - ROLI Ltd.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "PositionOverlay.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
PositionOverlay::PositionOverlay (LoopProcessor* loopProcessor)
    : loopProcessor(loopProcessor)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    startTimer (40);
    //[/Constructor]
}

PositionOverlay::~PositionOverlay()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]



    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void PositionOverlay::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    //[UserPaint] Add your own custom painting code here..
    auto duration = loopProcessor->GetLoopDuration();

    if (duration > 0.0)
    {
        auto loopPositionSample = loopProcessor->GetLoopPosition();
        auto sampleRate = loopProcessor->GetSampleRate();
        auto audioPosition = (float) loopPositionSample * 1000.0 / sampleRate;
        auto drawPosition = (audioPosition / duration) * getWidth();

        g.setColour (Colours::white);
        g.drawLine (drawPosition, 25.0f, drawPosition, (float) getHeight(), 2.0f);
    }
    //[/UserPaint]
}

void PositionOverlay::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void PositionOverlay::timerCallback()
{
    repaint();
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="PositionOverlay" componentName=""
                 parentClasses="public Component, public Timer" constructorParams="LoopProcessor* loopProcessor"
                 variableInitialisers="loopProcessor(loopProcessor)" snapPixels="8"
                 snapActive="1" snapShown="1" overlayOpacity="0.330" fixedSize="0"
                 initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

