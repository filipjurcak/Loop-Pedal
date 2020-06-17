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

#include "WaveformComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
#include "Track.h"
//[/MiscUserDefs]

//==============================================================================
WaveformComponent::WaveformComponent (Looper* looper)
    : looper(looper), thumbnailCache (5), thumbnailComp (512, formatManager, thumbnailCache, looper), positionOverlay (looper->GetSelectedTrack()->GetLoopProcessor())
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 200);


    //[Constructor] You can add your own custom stuff here..
    looper->addChangeListener(this);
    track = looper->GetSelectedTrack();
    label.setText(track->GetName(), NotificationType::dontSendNotification);
    label.setJustificationType(Justification::centred);
    label.setFont(Font (15.0f, Font::bold));
    addAndMakeVisible(this->label);
    addAndMakeVisible(&thumbnailComp);
    addAndMakeVisible(&positionOverlay);
    //[/Constructor]
}

WaveformComponent::~WaveformComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]



    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void WaveformComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colours::black);

    //[UserPaint] Add your own custom painting code here..
    Rectangle<int> thumbnailBounds(0, labelSize, getWidth(), getHeight() - labelSize);
    thumbnailComp.setBounds(thumbnailBounds);
    //Rectangle<int> thumbnailBounds (0, labelSize, getWidth(), getHeight() - labelSize);
    //[/UserPaint]
}

void WaveformComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    this->label.setBounds(0, 0, int(getWidth()), labelSize);
    Rectangle<int> thumbnailBounds(0, labelSize, getWidth(), getHeight() - labelSize);
    thumbnailComp.setBounds(thumbnailBounds);
//    positionOverlay.setBounds(thumbnailBounds);
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void WaveformComponent::changeListenerCallback(ChangeBroadcaster* source)
{
    if (source == looper && looper->GetSelectedTrack() != track)
    {
        track = looper->GetSelectedTrack();
        label.setText(track->GetName(), NotificationType::dontSendNotification);
    }
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="WaveformComponent" componentName=""
                 parentClasses="public Component, public ChangeListener" constructorParams="Looper* looper"
                 variableInitialisers="looper(looper), thumbnailCache (5), thumbnailComp (512, formatManager, thumbnailCache, looper), positionOverlay (looper-&gt;GetSelectedTrack()-&gt;GetLoopProcessor())"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="600" initialHeight="200">
  <BACKGROUND backgroundColour="ff000000"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

