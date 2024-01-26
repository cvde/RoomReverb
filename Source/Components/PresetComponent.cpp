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

PresetComponent::PresetComponent(ReverbAudioProcessor& audioProcessor)
        : mParameters(audioProcessor.getParameters()),
          mPresetManager(audioProcessor.getPresetManager())
{
    // add default preset (index is 0) to the combobox
    mPresetSelector.addItem(mPresetManager.getPresetName(0), 1);
    // add other presets (index > 0) to their category
    for (int i = 1; i < mPresetManager.getNumPresets(); ++i)
    {
        if (mPresetCategorySubmenus.empty() || mPresetManager.getPresetCategory(i) != mPresetCategorySubmenus.back().name)
        {
            mPresetCategorySubmenus.push_back({mPresetManager.getPresetCategory(i), juce::PopupMenu()});
        }
        mPresetCategorySubmenus.back().popupMenu.addItem(i + 1, mPresetManager.getPresetName(i));
    }
    // add submenus to the combobox
    for (const auto& submenu : mPresetCategorySubmenus)
    {
        mPresetSelector.getRootMenu()->addSubMenu(submenu.name, submenu.popupMenu);
    }
    mPresetSelector.setTooltip("Select a preset (overrides your current plugin settings).");
    mPresetSelector.setJustificationType(juce::Justification::centred);
    const int comboBoxPresetId = mPresetManager.getCurrentPreset() + 1;
    mPresetSelector.setSelectedId(comboBoxPresetId, juce::dontSendNotification);
    mPresetSelector.onChange = [this] { loadNewPreset(); };
    addAndMakeVisible(mPresetSelector);

    mParameters.state.addListener(this);
}

PresetComponent::~PresetComponent()
{
    mParameters.state.removeListener(this);
}

void PresetComponent::paint(juce::Graphics& g)
{
    juce::ignoreUnused(g);
}

void PresetComponent::resized()
{
    auto area = getLocalBounds();

    mPresetSelector.setBounds(area);
}

void PresetComponent::loadNewPreset()
{
    const int valueTreePresetId = mPresetSelector.getSelectedId() - 1;
    mPresetManager.setCurrentPreset(valueTreePresetId);
}

void PresetComponent::valueTreePropertyChanged(
    juce::ValueTree& treeWhosePropertyHasChanged,
    const juce::Identifier& property)
{
    juce::ignoreUnused(treeWhosePropertyHasChanged);
    if (property.toString() == "GuiNeedsUpdate")
    {
        const int comboBoxPresetId = mPresetManager.getCurrentPreset() + 1;
        mPresetSelector.setSelectedId(comboBoxPresetId, juce::dontSendNotification);
    }
}
