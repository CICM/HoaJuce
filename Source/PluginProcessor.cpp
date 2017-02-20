/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"


HoaAudioProcessor::HoaAudioProcessor() :
m_encoder(JucePlugin_HoaOrder, JucePlugin_HoaNumSources),
m_optim(JucePlugin_HoaOrder),
m_decoder(JucePlugin_HoaOrder, JucePlugin_HoaNumChannels)
{
    // 0 0 45 0 90 0 135 0 180 0 225 0 270 0 315 0 335 38 47 38 119 38 191 38 263 38 287 75 47 75 167 75
    m_decoder.setPlanewaveAzimuth(0, 0.);
    m_decoder.setPlanewaveAzimuth(1, 45.);
    m_decoder.setPlanewaveAzimuth(2, 90.);
    m_decoder.setPlanewaveAzimuth(3, 135);
    m_decoder.setPlanewaveAzimuth(4, 180.);
    m_decoder.setPlanewaveAzimuth(5, 225.);
    m_decoder.setPlanewaveAzimuth(6, 270.);
    m_decoder.setPlanewaveAzimuth(7, 315.);
    m_decoder.setPlanewaveAzimuth(8, 335.);
    m_decoder.setPlanewaveAzimuth(9, 47.);
    m_decoder.setPlanewaveAzimuth(10, 119.);
    m_decoder.setPlanewaveAzimuth(11, 191.);
    m_decoder.setPlanewaveAzimuth(12, 263.);
    m_decoder.setPlanewaveAzimuth(13, 287.);
    m_decoder.setPlanewaveAzimuth(14, 47.);
    m_decoder.setPlanewaveAzimuth(15, 167.);
    
    m_decoder.setPlanewaveElevation(0, 0.);
    m_decoder.setPlanewaveElevation(1, 0.);
    m_decoder.setPlanewaveElevation(2, 0.);
    m_decoder.setPlanewaveElevation(3, 0.);
    m_decoder.setPlanewaveElevation(4, 0.);
    m_decoder.setPlanewaveElevation(5, 0.);
    m_decoder.setPlanewaveElevation(6, 0.);
    m_decoder.setPlanewaveElevation(7, 0.);
    m_decoder.setPlanewaveElevation(8, 38.);
    m_decoder.setPlanewaveElevation(9, 38.);
    m_decoder.setPlanewaveElevation(10, 38.);
    m_decoder.setPlanewaveElevation(11, 38.);
    m_decoder.setPlanewaveElevation(12, 38.);
    m_decoder.setPlanewaveElevation(13, 75.);
    m_decoder.setPlanewaveElevation(14, 75.);
    m_decoder.setPlanewaveElevation(15, 75.);
    m_decoder.prepare();
}

HoaAudioProcessor::~HoaAudioProcessor()
{
}


//==============================================================================
void HoaAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    m_inputs    = new float[samplesPerBlock * JucePlugin_HoaNumSources];
    m_harmonics = new float[samplesPerBlock * JucePlugin_HoaNumHarmonics];
    m_outputs   = new float[samplesPerBlock * JucePlugin_HoaNumChannels];
}

void HoaAudioProcessor::releaseResources()
{
    delete [] m_inputs;
    delete [] m_harmonics;
    delete [] m_outputs;
}

void HoaAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    const int nins  = getTotalNumInputChannels();
    const int nouts = getTotalNumOutputChannels();
    const int nsamples = buffer.getNumSamples();

    // The input audio buffers are [channels][samples] but we need [samples][channels]
    // so we "interleave" the buffer
    for(int i = 0; i < nins; ++i)
    {
        hoa::Signal<float>::copy(nsamples, buffer.getReadPointer(i), 1, m_inputs+i, JucePlugin_HoaNumSources);
    }
    for(int i = 0; i < nsamples; i++)
    {
        // For each sample
        // we encode the sources
        for(int j = 0; j < nins; ++j)
        {
            // Here you can change the radius,
            // the azimuth and the elevation
            // for each source j
            m_encoder.setRadius(j, 1.);
            m_encoder.setAzimuth(j, 0.);
            m_encoder.setElevation(j, 0.);
        }
        m_encoder.process(m_inputs+JucePlugin_HoaNumSources*i, m_harmonics+JucePlugin_HoaNumHarmonics*i);
        // we optimize the sound field
        m_optim.process(m_harmonics+JucePlugin_HoaNumHarmonics*i, m_harmonics+JucePlugin_HoaNumHarmonics*i);
        // we decode the sound field
        // for a set of loudspeakers
        m_decoder.process(m_harmonics+JucePlugin_HoaNumHarmonics*i, m_outputs+JucePlugin_HoaNumHarmonics*i);
    }
    // The output audio buffers are [channels][samples] but we offer [samples][channels]
    // so we "interleave" the buffer
    for(long i = 0; i < nouts; i++)
    {
        hoa::Signal<float>::copy(nsamples, m_outputs+i, JucePlugin_HoaNumChannels, buffer.getWritePointer(i), 1);
    }
    
}

//==============================================================================
void HoaAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void HoaAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new HoaAudioProcessor();
}
