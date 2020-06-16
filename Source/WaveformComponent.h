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

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include "Looper.h"
#include <JuceHeader.h>
#include "PositionOverlay.h"
#include "ThumbnailComponent.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class WaveformComponent  : public Component,
                           public ChangeListener
{
public:
    //==============================================================================
    WaveformComponent (Looper* looper);
    ~WaveformComponent() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void changeListenerCallback(ChangeBroadcaster* source) override;
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    Looper* looper;

    Track* track;

    Label label;

    int labelSize = 25;

    AudioThumbnailCache thumbnailCache;

    AudioFormatManager formatManager;

    ThumbnailComponent thumbnailComp;

    PositionOverlay positionOverlay;
    //[/UserVariables]

    //==============================================================================


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

