/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


HoaAudioProcessor::HoaAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    m_encoder = new hoa::Encoder<hoa::Hoa3d, float>::Multi(m_order, m_nsources);
    m_optim   = new hoa::Optim<hoa::Hoa3d, float>::InPhase(m_order);
    m_decoder = new hoa::Decoder<hoa::Hoa3d, float>::Regular(m_order, m_nhps);
    
    // 0 0 45 0 90 0 135 0 180 0 225 0 270 0 315 0 335 38 47 38 119 38 191 38 263 38 287 75 47 75 167 75
    m_decoder->setPlanewaveAzimuth(0, 0.);
    m_decoder->setPlanewaveAzimuth(1, 45.);
    m_decoder->setPlanewaveAzimuth(2, 90.);
    m_decoder->setPlanewaveAzimuth(3, 135);
    m_decoder->setPlanewaveAzimuth(4, 180.);
    m_decoder->setPlanewaveAzimuth(5, 225.);
    m_decoder->setPlanewaveAzimuth(6, 270.);
    m_decoder->setPlanewaveAzimuth(7, 315.);
    m_decoder->setPlanewaveAzimuth(8, 335.);
    m_decoder->setPlanewaveAzimuth(9, 47.);
    m_decoder->setPlanewaveAzimuth(10, 119.);
    m_decoder->setPlanewaveAzimuth(11, 191.);
    m_decoder->setPlanewaveAzimuth(12, 263.);
    m_decoder->setPlanewaveAzimuth(13, 287.);
    m_decoder->setPlanewaveAzimuth(14, 47.);
    m_decoder->setPlanewaveAzimuth(15, 167.);
    
    m_decoder->setPlanewaveElevation(0, 0.);
    m_decoder->setPlanewaveElevation(1, 0.);
    m_decoder->setPlanewaveElevation(2, 0.);
    m_decoder->setPlanewaveElevation(3, 0.);
    m_decoder->setPlanewaveElevation(4, 0.);
    m_decoder->setPlanewaveElevation(5, 0.);
    m_decoder->setPlanewaveElevation(6, 0.);
    m_decoder->setPlanewaveElevation(7, 0.);
    m_decoder->setPlanewaveElevation(8, 38.);
    m_decoder->setPlanewaveElevation(9, 38.);
    m_decoder->setPlanewaveElevation(10, 38.);
    m_decoder->setPlanewaveElevation(11, 38.);
    m_decoder->setPlanewaveElevation(12, 38.);
    m_decoder->setPlanewaveElevation(13, 75.);
    m_decoder->setPlanewaveElevation(14, 75.);
    m_decoder->setPlanewaveElevation(15, 75.);
}

HoaAudioProcessor::~HoaAudioProcessor()
{
}

//==============================================================================
const String HoaAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool HoaAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool HoaAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

double HoaAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int HoaAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int HoaAudioProcessor::getCurrentProgram()
{
    return 0;
}

void HoaAudioProcessor::setCurrentProgram (int index)
{
}

const String HoaAudioProcessor::getProgramName (int index)
{
    return String();
}

void HoaAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void HoaAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    m_inputs    = new float[samplesPerBlock * m_nsources];
    m_harmonics = new float[samplesPerBlock * m_nharmos];
    m_outputs   = new float[samplesPerBlock * m_nhps];
}

void HoaAudioProcessor::releaseResources()
{
    delete [] m_inputs;
    delete [] m_harmonics;
    delete [] m_outputs;
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool HoaAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void HoaAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    const int nins  = getTotalNumInputChannels();
    const int nouts = getTotalNumOutputChannels();
    const int nharmos = m_nharmos;
    const int nsamples = buffer.getNumSamples();

    
    for(int i = 0; i < nins; ++i)
    {
        hoa::Signal<float>::copy(nsamples, buffer.getReadPointer(i), 1, m_inputs+i, nins);
    }
    for(int i = 0; i < nsamples; i++)
    {
        for(int j = 0; j < nins; ++j)
        {
            // Here you can change the radius, the azimuth and the elevation for each source j
            m_encoder->setRadius(j, 1.);
            m_encoder->setAzimuth(j, 0.);
            m_encoder->setElevation(j, 0.);
        }
        m_encoder->process(m_inputs+nins*i, m_harmonics+nharmos*i);
        m_optim->process(m_harmonics+nharmos*i, m_harmonics+nharmos*i);
        m_decoder->process(m_harmonics+nharmos*i, m_outputs+nouts*i);
    }
    
    for(long i = 0; i < nouts; i++)
    {
        hoa::Signal<float>::copy(nsamples, m_outputs+i, nouts, buffer.getWritePointer(i), 1);
    }
    
}

//==============================================================================
bool HoaAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* HoaAudioProcessor::createEditor()
{
    return new HoaAudioProcessorEditor (*this);
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
