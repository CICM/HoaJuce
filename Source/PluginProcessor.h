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

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioSampleBuffer&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

private:
    size_t m_order     = 3;
    size_t m_nsources  = 16;
    size_t m_nhps      = 16;
    size_t m_nharmos   = 16; // (3+1)^2
    
    ScopedPointer<hoa::Encoder<hoa::Hoa3d, float>::Multi>   m_encoder;
    ScopedPointer<hoa::Optim<hoa::Hoa3d, float>::InPhase>   m_optim;
    ScopedPointer<hoa::Decoder<hoa::Hoa3d, float>::Regular> m_decoder;
    float*                                      m_inputs;
    float*                                      m_harmonics;
    float*                                      m_outputs;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HoaAudioProcessor)
};


#endif  // PLUGINPROCESSOR_H_INCLUDED
