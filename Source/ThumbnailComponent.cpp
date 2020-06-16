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
#include "Track.h"
//[/Headers]

#include "ThumbnailComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
ThumbnailComponent::ThumbnailComponent (int sourceSamplesPerThumbnailSample, AudioFormatManager& formatManager, AudioThumbnailCache& cache, Looper* looper)
    : thumbnail (sourceSamplesPerThumbnailSample, formatManager, cache), looper(looper)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    thumbnail.addChangeListener(this);
    track = looper->GetSelectedTrack();
    loopProcessor = track->GetLoopProcessor();
    this->fillThumbnail();
    looper->addChangeListener(this);
    loopProcessor->addChangeListener(this);
    //[/Constructor]
}

ThumbnailComponent::~ThumbnailComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]



    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void ThumbnailComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colours::black);

    //[UserPaint] Add your own custom painting code here..
    if (thumbnail.getNumChannels() != 0)
    {
        g.setColour (Colours::limegreen);
        thumbnail.drawChannels (g, getLocalBounds(), 0.0, thumbnail.getTotalLength(), 1.0f);
    }
    //[/UserPaint]
}

void ThumbnailComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void ThumbnailComponent::changeListenerCallback (ChangeBroadcaster* source)
{
    if (source == &thumbnail)
    {
        this->repaint();
    }
    if (source == looper and track != looper->GetSelectedTrack())
    {
        track = looper->GetSelectedTrack();
        loopProcessor = track->GetLoopProcessor();
        loopProcessor->addChangeListener(this);
        fillThumbnail();
    }
    else if (source == loopProcessor)
    {
        fillThumbnail();
    }
}

void ThumbnailComponent::fillThumbnail()
{
    auto loopBuffer = loopProcessor->GetLoopBuffer();
    auto sampleRate = loopProcessor->GetSampleRate();
    thumbnail.reset(1, sampleRate, loopBuffer->getNumSamples());
    thumbnail.addBlock(0, *loopBuffer, 0, loopBuffer->getNumSamples());
    this->repaint();
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="ThumbnailComponent" componentName=""
                 parentClasses="public Component, private ChangeListener" constructorParams="int sourceSamplesPerThumbnailSample, AudioFormatManager&amp; formatManager, AudioThumbnailCache&amp; cache, Looper* looper"
                 variableInitialisers="thumbnail (sourceSamplesPerThumbnailSample, formatManager, cache), looper(looper)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ff000000"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

