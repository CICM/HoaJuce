/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#ifndef PLUGINPROCESSOR_H_INCLUDED
#define PLUGINPROCESSOR_H_INCLUDED

#include "../Hoa/Sources/Hoa.hpp"
#include "../JuceLibraryCode/JuceHeader.h"


//==============================================================================
/**
*/
class HoaAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    HoaAudioProcessor();
    ~HoaAudioProcessor();

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    void processBlock (AudioSampleBuffer&, MidiBuffer&) override;

    AudioProcessorEditor* createEditor() override   { return nullptr; }
    bool hasEditor() const override                 { return false; }
    const String getName() const override           { return JucePlugin_Name; }
    bool acceptsMidi() const override               { return JucePlugin_WantsMidiInput; }
    bool producesMidi() const override              { return JucePlugin_ProducesMidiOutput; }
    double getTailLengthSeconds() const override    { return 0.; }
    int getNumPrograms() override                   { return 1; }
    int getCurrentProgram() override                { return 0; }
    void setCurrentProgram (int index) override     { }
    const String getProgramName (int index) override{ return String(); }
    void changeProgramName (int index, const String& newName) override { }

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

private:
    
    hoa::MultiEncoder<hoa::Hoa3d, float>        m_encoder;
    hoa::Optim<hoa::Hoa3d, float>               m_optim;
    hoa::DecoderRegular<hoa::Hoa3d, float>      m_decoder;
    
    float*                                      m_inputs;
    float*                                      m_harmonics;
    float*                                      m_outputs;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HoaAudioProcessor)
};


#endif  // PLUGINPROCESSOR_H_INCLUDED
