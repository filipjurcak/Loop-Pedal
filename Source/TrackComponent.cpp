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

#include "TrackComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
TrackComponent::TrackComponent (Track* track, const LoopStates loopState)
    : track(track), loopState(loopState)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]


    //[UserPreSize]
    //[/UserPreSize]

    setSize (400, 800);


    //[Constructor] You can add your own custom stuff here..
    this->track->addChangeListener(this);
    this->label.setText(this->track->GetName(), NotificationType::dontSendNotification);
    this->label.setJustificationType(Justification::centred);
    this->label.setFont(Font (30.0f, Font::bold));
    addAndMakeVisible(this->label);
//    addAndMakeVisible(viewport);
    //[/Constructor]
}

TrackComponent::~TrackComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]



    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void TrackComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colours::black);

    //[UserPaint] Add your own custom painting code here..
    if ((this->loopState == LoopStates::Play && !this->track->IsMuted()) ||
        (this->loopState != LoopStates::Play && this->track->IsSelected())) {
        if (this->loopState == LoopStates::Play && !this->track->IsMuted()) {
            g.setColour(Colours::green);
        } else if (this->loopState == LoopStates::Record || this->loopState == LoopStates::Overdub) {
            g.setColour(Colours::red);
        } else if (this->loopState == LoopStates::PlayInRecord) {
            g.setColour(Colours::orange);
        }
        g.fillEllipse((getWidth()) / 2 - ledSize / 2, getHeight() - this->labelSize - this->ledSize - 20, this->ledSize, this->ledSize);
    }

    if (this->track->IsSelected()) {
        g.setColour (Colours::white);
        auto centralArea = getLocalBounds().toFloat().reduced(10.0f);
        g.drawRoundedRectangle (centralArea, 5.0f, 7.0f);
    }
    //[/UserPaint]
}

void TrackComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    this->label.setBounds(0, int(getHeight() - labelSize), int(getWidth()), labelSize);
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void TrackComponent::changeListenerCallback(ChangeBroadcaster* source)
{
//    Logger::writeToLog("TrackComponent sa chce updatnut!!!");
    this->repaint();
}

void TrackComponent::changeLoopState(const LoopStates loopState) {
    this->loopState = loopState;
    this->repaint();
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="TrackComponent" componentName=""
                 parentClasses="public Component" constructorParams="Track* track"
                 variableInitialisers="track(track)" snapPixels="8" snapActive="1"
                 snapShown="1" overlayOpacity="0.330" fixedSize="0" initialWidth="400"
                 initialHeight="800">
  <BACKGROUND backgroundColour="ff000000"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

