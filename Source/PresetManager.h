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

#include <juce_audio_processors/juce_audio_processors.h>
#include <vector>

struct Preset
{
    juce::String presetName;
    juce::String presetCategory;
    juce::ValueTree presetXml;
};

class PresetManager
{
public:
    PresetManager(juce::AudioProcessorValueTreeState& parameterTree, juce::ValueTree& appStateTree);

    int getNumPresets() const;
    int getCurrentPreset() const;
    void setCurrentPreset(int presetIndex);
    void setDefaultPreset();
    juce::String getPresetName(int presetIndex) const;
    juce::String getPresetCategory(int presetIndex) const;

    bool isParameterLocked(const juce::String& parameterID);
    void lockParameter(const juce::String& parameterID);
    void unlockParameter(const juce::String& parameterID);

private:
    juce::AudioProcessorValueTreeState& parameters;
    juce::ValueTree& applicationState;
    std::vector<Preset> presets;

    static constexpr int defaultPreset = 0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PresetManager)
};
