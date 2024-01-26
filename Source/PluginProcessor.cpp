/**
 *  ElephantDSP.com Room Reverb
 *
 *  Copyright (C) 2021 Christian Voigt
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "PluginProcessor.h"
#include "PluginEditor.h"

ReverbAudioProcessor::ReverbAudioProcessor()
        :
#ifndef JucePlugin_PreferredChannelConfigurations
          AudioProcessor(
              BusesProperties()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
                  .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
                  .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
                  ),
#endif
          mParameters(*this, &mUndoManager, "parameters", createParameterLayout()),
          mPresetManager(mParameters, mApplicationState),
          mProcessorABStateManager(mParameters, mApplicationState)
{
    mParameters.addParameterListener("dryLevel", this);
    mParameters.addParameterListener("earlyLevel", this);
    mParameters.addParameterListener("earlySendLevel", this);
    mParameters.addParameterListener("lateLevel", this);
    mParameters.addParameterListener("stereoWidth", this);
    mParameters.addParameterListener("earlyDamping", this);
    mParameters.addParameterListener("earlyRoomSize", this);
    mParameters.addParameterListener("lateDamping", this);
    mParameters.addParameterListener("lateDiffusion", this);
    mParameters.addParameterListener("latePredelay", this);
    mParameters.addParameterListener("lateRoomSize", this);
    mParameters.addParameterListener("lateDecay", this);
    mParameters.addParameterListener("lateSpin", this);
    mParameters.addParameterListener("lateWander", this);

    mPresetManager.setDefaultPreset();

    // the inactive processor state is initially a copy of the active processor state
    mProcessorABStateManager.copyActiveToInactiveProcessorState();
}

//==============================================================================
const juce::String ReverbAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ReverbAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool ReverbAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool ReverbAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double ReverbAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ReverbAudioProcessor::getNumPrograms()
{
    return 1; // NB: some hosts don't cope very well if you tell them there are
              // 0 programs, so this should be at least 1, even if you're not
              // really implementing programs.
}

int ReverbAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ReverbAudioProcessor::setCurrentProgram(int index)
{
    juce::ignoreUnused(index);
}

const juce::String ReverbAudioProcessor::getProgramName(int index)
{
    juce::ignoreUnused(index);
    return "None";
}

void ReverbAudioProcessor::changeProgramName(int index,
                                             const juce::String& newName)
{
    juce::ignoreUnused(index);
    juce::ignoreUnused(newName);
}

//==============================================================================
void ReverbAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    juce::ignoreUnused(samplesPerBlock);
    mRoomReverb.setSampleRate(static_cast<float>(sampleRate));
}

void ReverbAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
    mRoomReverb.mute();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ReverbAudioProcessor::isBusesLayoutSupported(
    const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono() &&
        layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
    {
        return false;
    }

    // This checks if the input layout matches the output layout
#if !JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
    {
        return false;
    }
#endif

    return true;
#endif
}
#endif

void ReverbAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer,
                                        juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused(midiMessages);

    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    if (totalNumInputChannels == 1 && totalNumOutputChannels == 2)
    {
        // mono in, stereo out
        mRoomReverb.process(buffer.getReadPointer(0), buffer.getReadPointer(0),
                            buffer.getWritePointer(0), buffer.getWritePointer(1),
                            buffer.getNumSamples());
    }
    else if (totalNumInputChannels == 2 && totalNumOutputChannels == 2)
    {
        // stereo in, stereo out
        mRoomReverb.process(buffer.getReadPointer(0), buffer.getReadPointer(1),
                            buffer.getWritePointer(0), buffer.getWritePointer(1),
                            buffer.getNumSamples());
    }
    else
    {
        jassertfalse; // channel layout not supported
    }
}

//==============================================================================
bool ReverbAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* ReverbAudioProcessor::createEditor()
{
    return new ReverbAudioProcessorEditor(*this);
}

//==============================================================================
void ReverbAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    juce::ValueTree pluginState("pluginState");
    pluginState.appendChild(mParameters.copyState(), nullptr);
    pluginState.appendChild(mProcessorABStateManager.getInactiveProcessorState(), nullptr);
    pluginState.appendChild(mApplicationState, nullptr);

    std::unique_ptr<juce::XmlElement> pluginStateXml(pluginState.createXml());
    copyXmlToBinary(*pluginStateXml, destData);

    // DBG("pluginState has been stored:");
    // DBG(pluginStateXml->toString() + "\n");
}

void ReverbAudioProcessor::setStateInformation(const void* data,
                                               int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory
    // block, whose contents will have been created by the getStateInformation()
    // call.
    std::unique_ptr<juce::XmlElement> pluginStateXml(getXmlFromBinary(data, sizeInBytes));
    if (pluginStateXml.get() != nullptr)
    {
        bool isActiveParameterTree = true;
        for (const auto* subTree : pluginStateXml->getChildIterator())
        {
            // set parameters & inactive Parameters
            if (subTree->hasTagName(mParameters.state.getType()))
            {
                // there are two trees with the same tag
                // the first one is the active parameter tree
                if (isActiveParameterTree)
                {
                    mParameters.replaceState(juce::ValueTree::fromXml(*subTree));
                    isActiveParameterTree = false;
                }
                else
                {
                    mProcessorABStateManager.setInactiveProcessorState(juce::ValueTree::fromXml(*subTree));
                }
            }
            // set applicationState
            if (subTree->hasTagName(mApplicationState.getType()))
            {
                mApplicationState = juce::ValueTree::fromXml(*subTree);
            }
        }
    }

    // DBG("pluginState has been restored:");
    // DBG(pluginStateXml->toString() + "\n");
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ReverbAudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout ReverbAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout params;
    using Range = juce::NormalisableRange<float>;

    params.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("dryLevel", 1), "Dry Level", Range{0.0f, 100.0f, 1.0f}, 80.0f, " %"));
    params.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("earlyLevel", 1), "Early Reflections Level", Range{0.0f, 100.0f, 1.0f}, 10.0f, " %"));
    params.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("earlySendLevel", 1), "Early Reflections Send Level", Range{0.0f, 100.0f, 1.0f}, 20.0f, " %"));
    params.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("lateLevel", 1), "Late Reverb Level", Range{0.0f, 100.0f, 1.0f}, 20.0f, " %"));
    params.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("stereoWidth", 1), "Stereo Width", Range{0.0f, 100.0f, 1.0f}, 100.0f, " %"));
    params.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("earlyDamping", 1), "Damping", Range{100.0f, 16000.0f, 1.0f}, 16000.0f, " Hz"));
    params.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("earlyRoomSize", 1), "Room Size", Range{0.0f, 100.0f, 1.0f}, 25.0f, " %"));
    params.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("lateDamping", 1), "Damping", Range{100.0f, 16000.0f, 1.0f}, 14000.0f, " Hz"));
    params.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("lateDiffusion", 1), "Diffusion", Range{0.0f, 100.0f, 1.0f}, 50.0f, " %"));
    params.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("latePredelay", 1), "Predelay", Range{0.0f, 200.0f, 0.1f}, 8.0f, " ms"));
    params.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("lateRoomSize", 1), "Room Size", Range{0.0f, 100.0f, 1.0f}, 25.0f, " %"));
    params.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("lateDecay", 1), "Decay", Range{0.1f, 30.0f, 0.01f}, 0.4f, " s"));
    params.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("lateSpin", 1), "Spin", Range{0.0f, 5.0f, 0.1f}, 0.4f, " Hz"));
    params.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("lateWander", 1), "Wander", Range{0.0f, 100.0f, 1.0f}, 30.0f, " %"));

    return params;
}

void ReverbAudioProcessor::parameterChanged(const juce::String& parameter, float newValue)
{
    // DBG("Parameter " + parameter + " was changed to " + juce::String(newValue));

    if (parameter == "dryLevel")
    {
        mRoomReverb.setDryLevel(newValue);
    }
    else if (parameter == "earlyLevel")
    {
        mRoomReverb.setEarlyLevel(newValue);
    }
    else if (parameter == "earlySendLevel")
    {
        mRoomReverb.setEarlySendLevel(newValue);
    }
    else if (parameter == "lateLevel")
    {
        mRoomReverb.setLateLevel(newValue);
    }
    else if (parameter == "earlyDamping")
    {
        mRoomReverb.setEarlyDamping(newValue);
    }
    else if (parameter == "earlyRoomSize")
    {
        mRoomReverb.setEarlyRoomSize(newValue);
    }
    else if (parameter == "lateDamping")
    {
        mRoomReverb.setLateDamping(newValue);
    }
    else if (parameter == "lateDiffusion")
    {
        mRoomReverb.setLateDiffusion(newValue);
    }
    else if (parameter == "latePredelay")
    {
        mRoomReverb.setLatePredelay(newValue);
    }
    else if (parameter == "lateRoomSize")
    {
        mRoomReverb.setLateRoomSize(newValue);
    }
    else if (parameter == "lateDecay")
    {
        mRoomReverb.setLateDecay(newValue);
    }
    else if (parameter == "lateSpin")
    {
        mRoomReverb.setLateSpin(newValue);
    }
    else if (parameter == "lateWander")
    {
        mRoomReverb.setLateWander(newValue);
    }
    else if (parameter == "stereoWidth")
    {
        mRoomReverb.setStereoWidth(newValue);
    }
    else
    {
        jassertfalse; // unknown parameter ...
    }
}

juce::ValueTree& ReverbAudioProcessor::getApplicationState()
{
    return mApplicationState;
}

juce::AudioProcessorValueTreeState& ReverbAudioProcessor::getParameters()
{
    return mParameters;
}

PresetManager& ReverbAudioProcessor::getPresetManager()
{
    return mPresetManager;
}

ProcessorABStateManager& ReverbAudioProcessor::getProcessorABStateManager()
{
    return mProcessorABStateManager;
}
