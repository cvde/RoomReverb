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

PresetComponent::PresetComponent(ReverbAudioProcessor& processor)
        : parameters(processor.getParameters()),
          presetManager(processor.getPresetManager())
{
    // add default preset (index is 0) to the combobox
    presetSelector.addItem(presetManager.getPresetName(0), 1);
    // add other presets (index > 0) to their category
    for (int i = 1; i < presetManager.getNumPresets(); ++i)
    {
        if (presetCategorySubmenus.empty() || presetManager.getPresetCategory(i) != presetCategorySubmenus.back().name)
        {
            presetCategorySubmenus.push_back({presetManager.getPresetCategory(i), juce::PopupMenu()});
        }
        presetCategorySubmenus.back().popupMenu.addItem(i + 1, presetManager.getPresetName(i));
    }
    // add submenus to the combobox
    for (const auto& submenu : presetCategorySubmenus)
    {
        presetSelector.getRootMenu()->addSubMenu(submenu.name, submenu.popupMenu);
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
