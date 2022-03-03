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

PresetManager::PresetManager(juce::AudioProcessorValueTreeState& parameterTree)
        : parameters(parameterTree)
{
    presets = {
        {"Default (Medium Room)", juce::ValueTree::fromXml(BinaryData::MediumRoom_xml)},
        {"Small Room", juce::ValueTree::fromXml(BinaryData::SmallRoom_xml)},
        {"Small Room (Bright)", juce::ValueTree::fromXml(BinaryData::SmallRoomBright_xml)},
        {"Small Room (Dark)", juce::ValueTree::fromXml(BinaryData::SmallRoomDark_xml)},
        {"Small Room (Drum)", juce::ValueTree::fromXml(BinaryData::SmallRoomDrum_xml)},
        {"Small Room (Vocal)", juce::ValueTree::fromXml(BinaryData::SmallRoomVocal_xml)},
        {"Small Room (Tiled)", juce::ValueTree::fromXml(BinaryData::SmallRoomTiled_xml)},
        {"Small Room (Wooden)", juce::ValueTree::fromXml(BinaryData::SmallRoomWooden_xml)},
        {"Medium Room", juce::ValueTree::fromXml(BinaryData::MediumRoom_xml)},
        {"Medium Room (Bright)", juce::ValueTree::fromXml(BinaryData::MediumRoomBright_xml)},
        {"Medium Room (Dark)", juce::ValueTree::fromXml(BinaryData::MediumRoomDark_xml)},
        {"Medium Room (Drum)", juce::ValueTree::fromXml(BinaryData::MediumRoomDrum_xml)},
        {"Medium Room (Vocal)", juce::ValueTree::fromXml(BinaryData::MediumRoomVocal_xml)},
        {"Medium Room (Tiled)", juce::ValueTree::fromXml(BinaryData::MediumRoomTiled_xml)},
        {"Medium Room (Wooden)", juce::ValueTree::fromXml(BinaryData::MediumRoomWooden_xml)},
        {"Large Room", juce::ValueTree::fromXml(BinaryData::LargeRoom_xml)},
        {"Large Room (Bright)", juce::ValueTree::fromXml(BinaryData::LargeRoomBright_xml)},
        {"Large Room (Dark)", juce::ValueTree::fromXml(BinaryData::LargeRoomDark_xml)},
        {"Large Room (Drum)", juce::ValueTree::fromXml(BinaryData::LargeRoomDrum_xml)},
        {"Large Room (Vocal)", juce::ValueTree::fromXml(BinaryData::LargeRoomVocal_xml)},
        {"Large Room (Tiled)", juce::ValueTree::fromXml(BinaryData::LargeRoomTiled_xml)},
        {"Large Room (Wooden)", juce::ValueTree::fromXml(BinaryData::LargeRoomWooden_xml)},
        {"Very Large Room", juce::ValueTree::fromXml(BinaryData::VeryLargeRoom_xml)},
        {"Very Large Room (Bright)", juce::ValueTree::fromXml(BinaryData::VeryLargeRoomBright_xml)},
        {"Very Large Room (Dark)", juce::ValueTree::fromXml(BinaryData::VeryLargeRoomDark_xml)},
        {"Very Large Room (Drum)", juce::ValueTree::fromXml(BinaryData::VeryLargeRoomDrum_xml)},
        {"Very Large Room (Vocal)", juce::ValueTree::fromXml(BinaryData::VeryLargeRoomVocal_xml)},
        {"Very Large Room (Tiled)", juce::ValueTree::fromXml(BinaryData::VeryLargeRoomTiled_xml)},
        {"Very Large Room (Wooden)", juce::ValueTree::fromXml(BinaryData::VeryLargeRoomWooden_xml)},
        {"Shimmer", juce::ValueTree::fromXml(BinaryData::Shimmer_xml)},
        {"Live", juce::ValueTree::fromXml(BinaryData::Live_xml)},
        {"Echo Chamber", juce::ValueTree::fromXml(BinaryData::EchoChamber_xml)},
        {"Tunnel", juce::ValueTree::fromXml(BinaryData::Tunnel_xml)},
        {"Long Reverb (12s)", juce::ValueTree::fromXml(BinaryData::LongReverb12s_xml)},
        {"Long Reverb (30s)", juce::ValueTree::fromXml(BinaryData::LongReverb30s_xml)}};

    // create a property in the parameters.state value tree and store the default preset
    parameters.state.getOrCreateChildWithName("preset", nullptr).setProperty("selected", defaultPreset, nullptr);
}

int PresetManager::getNumPresets() const
{
    return presets.size();
}

int PresetManager::getCurrentPreset() const
{
    return parameters.state.getChildWithName("preset").getProperty("selected");
}

void PresetManager::setCurrentPreset(int presetIndex)
{
    if (presetIndex >= 0 && presetIndex <= (int)presets.size())
    {
        parameters.replaceState(presets.at((size_t)presetIndex).presetXml.createCopy());
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
