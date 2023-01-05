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

#include "PresetComponent.h"

PresetComponent::PresetComponent(
    ReverbAudioProcessor& processor,
    juce::AudioProcessorValueTreeState& parameterTree)
        : parameters(parameterTree),
          presetManager(processor.getPresetManager())
{
    // add all presets to the combobox
    for (int i = 0; i < presetManager.getNumPresets(); ++i)
    {
        presetSelector.addItem(presetManager.getPresetName(i), i + 1);
    }
    presetSelector.setTooltip("Select a preset (overrides your current plugin settings).");
    presetSelector.setJustificationType(juce::Justification::centred);
    const int comboBoxPresetId = presetManager.getCurrentPreset() + 1;
    presetSelector.setSelectedId(comboBoxPresetId, juce::dontSendNotification);
    presetSelector.onChange = [this] { loadNewPreset(); };
    addAndMakeVisible(presetSelector);

    parameters.state.addListener(this);
}

PresetComponent::~PresetComponent()
{
    parameters.state.removeListener(this);
}

void PresetComponent::paint(juce::Graphics& g)
{
    juce::ignoreUnused(g);
}

void PresetComponent::resized()
{
    auto area = getLocalBounds();

    presetSelector.setBounds(area);
}

void PresetComponent::loadNewPreset()
{
    const int valueTreePresetId = presetSelector.getSelectedId() - 1;
    presetManager.setCurrentPreset(valueTreePresetId);
}

void PresetComponent::valueTreePropertyChanged(
    juce::ValueTree& treeWhosePropertyHasChanged,
    const juce::Identifier& property)
{
    juce::ignoreUnused(treeWhosePropertyHasChanged);
    if (property.toString() == "GuiNeedsUpdate")
    {
        const int comboBoxPresetId = presetManager.getCurrentPreset() + 1;
        presetSelector.setSelectedId(comboBoxPresetId, juce::dontSendNotification);
    }
}
