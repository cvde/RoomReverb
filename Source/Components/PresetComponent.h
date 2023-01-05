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

#pragma once

#include "../PluginProcessor.h"
#include "../PresetManager.h"
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_basics/juce_gui_basics.h>

class PresetComponent : public juce::Component, public juce::ValueTree::Listener
{
public:
    PresetComponent(ReverbAudioProcessor& processor);
    ~PresetComponent() override;

    void paint(juce::Graphics& g) override;
    void resized() override;

    void valueTreePropertyChanged(juce::ValueTree& treeWhosePropertyHasChanged, const juce::Identifier& property) override;

private:
    void loadNewPreset();
    juce::AudioProcessorValueTreeState& parameters;
    PresetManager& presetManager;
    juce::ComboBox presetSelector;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PresetComponent)
};
