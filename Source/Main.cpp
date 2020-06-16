#include <JuceHeader.h>
#include "WaveformComponent.h"
#include "TracksComponent.h"
#include "Looper.h"

//==============================================================================
class LoopPedalApplication: public JUCEApplication
{
public:
    //==============================================================================
    LoopPedalApplication() {
        AudioDeviceSelectorComponent audioConfig(deviceManager,
          1, 2,     // at least one input channel, no more than 2
          1, 2,     // at least one output channel, no more than 2
          true,     // show midi input options
          true,     // show midi output options
          true,     // show channels as stereo pairs.
          false     // don't hide the advanced options.
          );

        audioConfig.setSize(500, 450);

        DialogWindow::LaunchOptions(o);
        o.content.setNonOwned(&audioConfig);
        o.dialogTitle = "Configure Audio";
        o.dialogBackgroundColour = Colours::grey;
        o.escapeKeyTriggersCloseButton = true;
        o.useNativeTitleBar = true;
        o.resizable = false;

        o.runModal();

        looper = std::make_unique<Looper>(deviceManager);
    }

    const String getApplicationName() override       { return ProjectInfo::projectName; }
    const String getApplicationVersion() override    { return ProjectInfo::versionString; }
    bool moreThanOneInstanceAllowed() override       { return true; }

    //==============================================================================
    void initialise (const String& commandLine) override
    {
        tracksWindow.reset(new TracksWindow(looper.get()));
        waveformWindow.reset(new WaveformWindow(looper.get()));
    }

    void shutdown() override
    {
        tracksWindow = nullptr;
        waveformWindow = nullptr;
        looper = nullptr;
    }

    //==============================================================================
    void systemRequestedQuit() override
    {
        quit();
    }

    void anotherInstanceStarted (const String& commandLine) override {}
    
    class TracksWindow: public DocumentWindow
    {
    public:
        TracksWindow(Looper* looper): DocumentWindow ("Tracks",
                                         Desktop::getInstance().getDefaultLookAndFeel().findColour (ResizableWindow::backgroundColourId),
                                         DocumentWindow::allButtons)
        {
            setUsingNativeTitleBar (true);
            setContentOwned (new TracksComponent(looper), true);

           #if JUCE_IOS || JUCE_ANDROID
            setFullScreen (true);
           #else
            centreWithSize (getWidth(), getHeight());
           #endif

            setVisible (true);
        }

        void closeButtonPressed() override
        {
            JUCEApplication::getInstance()->systemRequestedQuit();
        }

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TracksWindow)
    };
    
    class WaveformWindow: public DocumentWindow
    {
    public:
        WaveformWindow(Looper* looper): DocumentWindow ("Waveform",
                                           Desktop::getInstance().getDefaultLookAndFeel().findColour (ResizableWindow::backgroundColourId),
                                           DocumentWindow::allButtons)
        {
            setUsingNativeTitleBar (true);
            setContentOwned (new WaveformComponent(looper), true);

           #if JUCE_IOS || JUCE_ANDROID
            setFullScreen (true);
           #else
            centreWithSize (getWidth(), getHeight());
           #endif

            setVisible (true);
        }

        void closeButtonPressed() override
        {
            JUCEApplication::getInstance()->systemRequestedQuit();
        }

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformWindow)
    };

private:
    // windows for tracks view and waveform view
    std::unique_ptr<TracksWindow> tracksWindow;
    std::unique_ptr<WaveformWindow> waveformWindow;
    AudioDeviceManager deviceManager;
    std::unique_ptr<Looper> looper;
};

//==============================================================================
// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION (LoopPedalApplication)
