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

#include "PresetManager.h"
#include <BinaryData.h>

PresetManager::PresetManager(juce::AudioProcessorValueTreeState& parameterTree, juce::ValueTree& appStateTree)
        : parameters(parameterTree),
          applicationState(appStateTree)
{
    presets = {
        {"Default (Medium Room)", "Default", juce::ValueTree::fromXml(BinaryData::MediumRoom_xml)},
        {"Small Room", "Small Rooms", juce::ValueTree::fromXml(BinaryData::SmallRoom_xml)},
        {"Small Room (Bright)", "Small Rooms", juce::ValueTree::fromXml(BinaryData::SmallRoomBright_xml)},
        {"Small Room (Dark)", "Small Rooms", juce::ValueTree::fromXml(BinaryData::SmallRoomDark_xml)},
        {"Small Room (Drum)", "Small Rooms", juce::ValueTree::fromXml(BinaryData::SmallRoomDrum_xml)},
        {"Small Room (Vocal)", "Small Rooms", juce::ValueTree::fromXml(BinaryData::SmallRoomVocal_xml)},
        {"Small Room (Tiled)", "Small Rooms", juce::ValueTree::fromXml(BinaryData::SmallRoomTiled_xml)},
        {"Small Room (Wooden)", "Small Rooms", juce::ValueTree::fromXml(BinaryData::SmallRoomWooden_xml)},
        {"Medium Room", "Medium Rooms", juce::ValueTree::fromXml(BinaryData::MediumRoom_xml)},
        {"Medium Room (Bright)", "Medium Rooms", juce::ValueTree::fromXml(BinaryData::MediumRoomBright_xml)},
        {"Medium Room (Dark)", "Medium Rooms", juce::ValueTree::fromXml(BinaryData::MediumRoomDark_xml)},
        {"Medium Room (Drum)", "Medium Rooms", juce::ValueTree::fromXml(BinaryData::MediumRoomDrum_xml)},
        {"Medium Room (Vocal)", "Medium Rooms", juce::ValueTree::fromXml(BinaryData::MediumRoomVocal_xml)},
        {"Medium Room (Tiled)", "Medium Rooms", juce::ValueTree::fromXml(BinaryData::MediumRoomTiled_xml)},
        {"Medium Room (Wooden)", "Medium Rooms", juce::ValueTree::fromXml(BinaryData::MediumRoomWooden_xml)},
        {"Large Room", "Large Rooms", juce::ValueTree::fromXml(BinaryData::LargeRoom_xml)},
        {"Large Room (Bright)", "Large Rooms", juce::ValueTree::fromXml(BinaryData::LargeRoomBright_xml)},
        {"Large Room (Dark)", "Large Rooms", juce::ValueTree::fromXml(BinaryData::LargeRoomDark_xml)},
        {"Large Room (Drum)", "Large Rooms", juce::ValueTree::fromXml(BinaryData::LargeRoomDrum_xml)},
        {"Large Room (Vocal)", "Large Rooms", juce::ValueTree::fromXml(BinaryData::LargeRoomVocal_xml)},
        {"Large Room (Tiled)", "Large Rooms", juce::ValueTree::fromXml(BinaryData::LargeRoomTiled_xml)},
        {"Large Room (Wooden)", "Large Rooms", juce::ValueTree::fromXml(BinaryData::LargeRoomWooden_xml)},
        {"Very Large Room", "Very Large Rooms", juce::ValueTree::fromXml(BinaryData::VeryLargeRoom_xml)},
        {"Very Large Room (Bright)", "Very Large Rooms", juce::ValueTree::fromXml(BinaryData::VeryLargeRoomBright_xml)},
        {"Very Large Room (Dark)", "Very Large Rooms", juce::ValueTree::fromXml(BinaryData::VeryLargeRoomDark_xml)},
        {"Very Large Room (Drum)", "Very Large Rooms", juce::ValueTree::fromXml(BinaryData::VeryLargeRoomDrum_xml)},
        {"Very Large Room (Vocal)", "Very Large Rooms", juce::ValueTree::fromXml(BinaryData::VeryLargeRoomVocal_xml)},
        {"Very Large Room (Tiled)", "Very Large Rooms", juce::ValueTree::fromXml(BinaryData::VeryLargeRoomTiled_xml)},
        {"Very Large Room (Wooden)", "Very Large Rooms", juce::ValueTree::fromXml(BinaryData::VeryLargeRoomWooden_xml)},
        {"Shimmer", "Effects", juce::ValueTree::fromXml(BinaryData::Shimmer_xml)},
        {"Live", "Effects", juce::ValueTree::fromXml(BinaryData::Live_xml)},
        {"Echo Chamber", "Effects", juce::ValueTree::fromXml(BinaryData::EchoChamber_xml)},
        {"Tunnel", "Effects", juce::ValueTree::fromXml(BinaryData::Tunnel_xml)},
        {"Long Reverb (12s)", "Effects", juce::ValueTree::fromXml(BinaryData::LongReverb12s_xml)},
        {"Long Reverb (30s)", "Effects", juce::ValueTree::fromXml(BinaryData::LongReverb30s_xml)}};

    // create a property in the parameters.state value tree and store the default preset
    parameters.state.getOrCreateChildWithName("preset", nullptr).setProperty("selected", defaultPreset, nullptr);
}

int PresetManager::getNumPresets() const
{
    return (int)presets.size();
}

int PresetManager::getCurrentPreset() const
{
    return parameters.state.getChildWithName("preset").getProperty("selected");
}

void PresetManager::setCurrentPreset(int presetIndex)
{
    if (presetIndex >= 0 && presetIndex <= (int)presets.size())
    {
        auto newParameters = presets.at((size_t)presetIndex).presetXml.createCopy();

        // keep values of locked parameters
        for (int i = 0; i < newParameters.getNumChildren(); ++i)
        {
            const auto& parameterName = newParameters.getChild(i).getProperty("id").toString();
            if (isParameterLocked(parameterName))
            {
                newParameters.getChild(i).setProperty("value", parameters.getParameterAsValue(parameterName), nullptr);
            }
        }

        parameters.replaceState(newParameters);
        parameters.state.getOrCreateChildWithName("preset", nullptr).setProperty("selected", presetIndex, nullptr);
        parameters.undoManager->clearUndoHistory();
    }
    else
    {
        jassertfalse; // there is no such preset ...
    }
}

void PresetManager::setDefaultPreset()
{
    setCurrentPreset(defaultPreset);
}

juce::String PresetManager::getPresetName(int presetIndex) const
{
    if (presetIndex >= 0 && presetIndex <= (int)presets.size())
    {
        return presets.at((size_t)presetIndex).presetName;
    }
    jassertfalse; // there is no such preset ...
    return {};
}

juce::String PresetManager::getPresetCategory(int presetIndex) const
{
    if (presetIndex >= 0 && presetIndex <= (int)presets.size())
    {
        return presets.at((size_t)presetIndex).presetCategory;
    }
    jassertfalse; // there is no such preset ...
    return {};
}

bool PresetManager::isParameterLocked(const juce::String& parameterID)
{
    return (bool)applicationState.getChildWithName("activeParameterLocks").getProperty(parameterID, false);
}

void PresetManager::lockParameter(const juce::String& parameterID)
{
    applicationState.getOrCreateChildWithName("activeParameterLocks", nullptr).setProperty(parameterID, true, nullptr);
}

void PresetManager::unlockParameter(const juce::String& parameterID)
{
    applicationState.getOrCreateChildWithName("activeParameterLocks", nullptr).setProperty(parameterID, false, nullptr);
}
