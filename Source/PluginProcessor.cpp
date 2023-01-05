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
          parameters(*this, &undo, "parameters", createParameterLayout()),
          presetManager(parameters),
          processorABStateManager(parameters, applicationState)
{
    // add a listener for each parameter
    parameters.addParameterListener("dryLevel", this);
    parameters.addParameterListener("earlyLevel", this);
    parameters.addParameterListener("earlySendLevel", this);
    parameters.addParameterListener("lateLevel", this);
    parameters.addParameterListener("stereoWidth", this);
    parameters.addParameterListener("earlyDamping", this);
    parameters.addParameterListener("earlyRoomSize", this);
    parameters.addParameterListener("lateDamping", this);
    parameters.addParameterListener("lateDiffusion", this);
    parameters.addParameterListener("latePredelay", this);
    parameters.addParameterListener("lateRoomSize", this);
    parameters.addParameterListener("lateDecay", this);
    parameters.addParameterListener("lateSpin", this);
    parameters.addParameterListener("lateWander", this);

    // load the default preset
    presetManager.setDefaultPreset();

    // the inactive processor state is initially a copy of the active processor state
    processorABStateManager.copyActiveToInactiveProcessorState();
}

ReverbAudioProcessor::~ReverbAudioProcessor()
{
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
    reverb.setSampleRate(sampleRate);
}

void ReverbAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
    reverb.mute();
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
        return false;

        // This checks if the input layout matches the output layout
#if !JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
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
        reverb.process(buffer.getReadPointer(0), buffer.getReadPointer(0),
                       buffer.getWritePointer(0), buffer.getWritePointer(1),
                       buffer.getNumSamples());
    }
    else if (totalNumInputChannels == 2 && totalNumOutputChannels == 2)
    {
        // stereo in, stereo out
        reverb.process(buffer.getReadPointer(0), buffer.getReadPointer(1),
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
    return new ReverbAudioProcessorEditor(*this, parameters, applicationState);
}

//==============================================================================
void ReverbAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    juce::ValueTree pluginState{"pluginState"};
    pluginState.appendChild(parameters.copyState(), nullptr);
    pluginState.appendChild(processorABStateManager.getInactiveProcessorState(), nullptr);
    pluginState.appendChild(applicationState, nullptr);

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
            if (subTree->hasTagName(parameters.state.getType()))
            {
                // there are two trees with the same tag
                // the first one is the active parameter tree
                if (isActiveParameterTree)
                {
                    parameters.replaceState(juce::ValueTree::fromXml(*subTree));
                    isActiveParameterTree = false;
                }
                else
                {
                    processorABStateManager.setInactiveProcessorState(juce::ValueTree::fromXml(*subTree));
                }
            }
            // set applicationState
            if (subTree->hasTagName(applicationState.getType()))
            {
                applicationState = juce::ValueTree::fromXml(*subTree);
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

juce::AudioProcessorValueTreeState::ParameterLayout
ReverbAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout params;
    using Range = juce::NormalisableRange<float>;

    params.add(std::make_unique<juce::AudioParameterFloat>("dryLevel", "Dry Level", Range{0.0f, 100.0f, 1.0f}, 80.0f, " %"));
    params.add(std::make_unique<juce::AudioParameterFloat>("earlyLevel", "Early Reflections Level", Range{0.0f, 100.0f, 1.0f}, 10.0f, " %"));
    params.add(std::make_unique<juce::AudioParameterFloat>("earlySendLevel", "Early Reflections Send Level", Range{0.0f, 100.0f, 1.0f}, 20.0f, " %"));
    params.add(std::make_unique<juce::AudioParameterFloat>("lateLevel", "Late Reverb Level", Range{0.0f, 100.0f, 1.0f}, 20.0f, " %"));
    params.add(std::make_unique<juce::AudioParameterFloat>("stereoWidth", "Stereo Width", Range{0.0f, 100.0f, 1.0f}, 100.0f, " %"));
    params.add(std::make_unique<juce::AudioParameterFloat>("earlyDamping", "Damping", Range{100.0f, 16000.0f, 1.0f}, 16000.0f, " Hz"));
    params.add(std::make_unique<juce::AudioParameterFloat>("earlyRoomSize", "Room Size", Range{0.0f, 100.0f, 1.0f}, 25.0f, " %"));
    params.add(std::make_unique<juce::AudioParameterFloat>("lateDamping", "Damping", Range{100.0f, 16000.0f, 1.0f}, 14000.0f, " Hz"));
    params.add(std::make_unique<juce::AudioParameterFloat>("lateDiffusion", "Diffusion", Range{0.0f, 100.0f, 1.0f}, 50.0f, " %"));
    params.add(std::make_unique<juce::AudioParameterFloat>("latePredelay", "Predelay", Range{0.0f, 200.0f, 0.1f}, 8.0f, " ms"));
    params.add(std::make_unique<juce::AudioParameterFloat>("lateRoomSize", "Room Size", Range{0.0f, 100.0f, 1.0f}, 25.0f, " %"));
    params.add(std::make_unique<juce::AudioParameterFloat>("lateDecay", "Decay", Range{0.1f, 30.0f, 0.01f}, 0.4f, " s"));
    params.add(std::make_unique<juce::AudioParameterFloat>("lateSpin", "Spin", Range{0.0f, 5.0f, 0.1f}, 0.4f, " Hz"));
    params.add(std::make_unique<juce::AudioParameterFloat>("lateWander", "Wander", Range{0.0f, 100.0f, 1.0f}, 30.0f, " %"));

    return params;
}

void ReverbAudioProcessor::parameterChanged(const juce::String& parameter, float newValue)
{
    // DBG("Parameter " + parameter + " was changed to " + juce::String(newValue));

    if (parameter == "dryLevel")
    {
        reverb.setDryLevel(newValue);
    }
    else if (parameter == "earlyLevel")
    {
        reverb.setEarlyLevel(newValue);
    }
    else if (parameter == "earlySendLevel")
    {
        reverb.setEarlySendLevel(newValue);
    }
    else if (parameter == "lateLevel")
    {
        reverb.setLateLevel(newValue);
    }
    else if (parameter == "earlyDamping")
    {
        reverb.setEarlyDamping(newValue);
    }
    else if (parameter == "earlyRoomSize")
    {
        reverb.setEarlyRoomSize(newValue);
    }
    else if (parameter == "lateDamping")
    {
        reverb.setLateDamping(newValue);
    }
    else if (parameter == "lateDiffusion")
    {
        reverb.setLateDiffusion(newValue);
    }
    else if (parameter == "latePredelay")
    {
        reverb.setLatePredelay(newValue);
    }
    else if (parameter == "lateRoomSize")
    {
        reverb.setLateRoomSize(newValue);
    }
    else if (parameter == "lateDecay")
    {
        reverb.setLateDecay(newValue);
    }
    else if (parameter == "lateSpin")
    {
        reverb.setLateSpin(newValue);
    }
    else if (parameter == "lateWander")
    {
        reverb.setLateWander(newValue);
    }
    else if (parameter == "stereoWidth")
    {
        reverb.setStereoWidth(newValue);
    }
    else
    {
        jassertfalse; // unknown parameter ...
    }
}

PresetManager& ReverbAudioProcessor::getPresetManager()
{
    return presetManager;
}

ProcessorABStateManager& ReverbAudioProcessor::getProcessorABStateManager()
{
    return processorABStateManager;
}
