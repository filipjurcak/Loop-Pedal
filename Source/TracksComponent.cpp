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
#include <typeinfo>
//[/Headers]

#include "TracksComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
TracksComponent::TracksComponent (Looper* looper)
    : looper(looper)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]


    //[UserPreSize]
    //[/UserPreSize]

    setSize (1200, 800);


    //[Constructor] You can add your own custom stuff here..
    this->loopState = looper->GetMode();
    looper->addChangeListener(this);
    int numberOfTracks = looper->GetNumberOfTracks();
    auto centralArea = getLocalBounds().toFloat();
    int trackWidth = centralArea.getWidth() / numberOfTracks;
    for (int i = 0; i < numberOfTracks; i++) {
        TrackComponent* trackComponent = new TrackComponent(looper->GetTrack(i), looper->GetMode());
        trackComponent->setBounds(i * trackWidth, 0, trackWidth, int(centralArea.getHeight()));
        trackComponents.add(trackComponent);
        addAndMakeVisible(trackComponent);
    }
    //[/Constructor]
}

TracksComponent::~TracksComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]



    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void TracksComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colours::black);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void TracksComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void TracksComponent::changeListenerCallback(ChangeBroadcaster* source)
{
    if (source == looper) {
        if (this->loopState != looper->GetMode()) {
            for (auto trackComponent: this->trackComponents) {
                trackComponent->changeLoopState(looper->GetMode());
            }
            this->loopState = looper->GetMode();
        }
    }
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="TracksComponent" componentName=""
                 parentClasses="public Component, public ChangeListener" constructorParams="Looper* looper"
                 variableInitialisers="looper(looper)" snapPixels="8" snapActive="1"
                 snapShown="1" overlayOpacity="0.330" fixedSize="1" initialWidth="1200"
                 initialHeight="800">
  <BACKGROUND backgroundColour="ff000000"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

