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

PresetManager::PresetManager(juce::AudioProcessorValueTreeState& parameters, juce::ValueTree& applicationState)
        : mParameters(parameters),
          mApplicationState(applicationState)
{
    mPresets = juce::Array<Preset>({Preset{"Default (Medium Room)", "Default", juce::ValueTree::fromXml(BinaryData::MediumRoom_xml)},
                                    Preset{"Small Room", "Small Rooms", juce::ValueTree::fromXml(BinaryData::SmallRoom_xml)},
                                    Preset{"Small Room (Bright)", "Small Rooms", juce::ValueTree::fromXml(BinaryData::SmallRoomBright_xml)},
                                    Preset{"Small Room (Dark)", "Small Rooms", juce::ValueTree::fromXml(BinaryData::SmallRoomDark_xml)},
                                    Preset{"Small Room (Drum)", "Small Rooms", juce::ValueTree::fromXml(BinaryData::SmallRoomDrum_xml)},
                                    Preset{"Small Room (Vocal)", "Small Rooms", juce::ValueTree::fromXml(BinaryData::SmallRoomVocal_xml)},
                                    Preset{"Small Room (Tiled)", "Small Rooms", juce::ValueTree::fromXml(BinaryData::SmallRoomTiled_xml)},
                                    Preset{"Small Room (Wooden)", "Small Rooms", juce::ValueTree::fromXml(BinaryData::SmallRoomWooden_xml)},
                                    Preset{"Medium Room", "Medium Rooms", juce::ValueTree::fromXml(BinaryData::MediumRoom_xml)},
                                    Preset{"Medium Room (Bright)", "Medium Rooms", juce::ValueTree::fromXml(BinaryData::MediumRoomBright_xml)},
                                    Preset{"Medium Room (Dark)", "Medium Rooms", juce::ValueTree::fromXml(BinaryData::MediumRoomDark_xml)},
                                    Preset{"Medium Room (Drum)", "Medium Rooms", juce::ValueTree::fromXml(BinaryData::MediumRoomDrum_xml)},
                                    Preset{"Medium Room (Vocal)", "Medium Rooms", juce::ValueTree::fromXml(BinaryData::MediumRoomVocal_xml)},
                                    Preset{"Medium Room (Tiled)", "Medium Rooms", juce::ValueTree::fromXml(BinaryData::MediumRoomTiled_xml)},
                                    Preset{"Medium Room (Wooden)", "Medium Rooms", juce::ValueTree::fromXml(BinaryData::MediumRoomWooden_xml)},
                                    Preset{"Large Room", "Large Rooms", juce::ValueTree::fromXml(BinaryData::LargeRoom_xml)},
                                    Preset{"Large Room (Bright)", "Large Rooms", juce::ValueTree::fromXml(BinaryData::LargeRoomBright_xml)},
                                    Preset{"Large Room (Dark)", "Large Rooms", juce::ValueTree::fromXml(BinaryData::LargeRoomDark_xml)},
                                    Preset{"Large Room (Drum)", "Large Rooms", juce::ValueTree::fromXml(BinaryData::LargeRoomDrum_xml)},
                                    Preset{"Large Room (Vocal)", "Large Rooms", juce::ValueTree::fromXml(BinaryData::LargeRoomVocal_xml)},
                                    Preset{"Large Room (Tiled)", "Large Rooms", juce::ValueTree::fromXml(BinaryData::LargeRoomTiled_xml)},
                                    Preset{"Large Room (Wooden)", "Large Rooms", juce::ValueTree::fromXml(BinaryData::LargeRoomWooden_xml)},
                                    Preset{"Very Large Room", "Very Large Rooms", juce::ValueTree::fromXml(BinaryData::VeryLargeRoom_xml)},
                                    Preset{"Very Large Room (Bright)", "Very Large Rooms", juce::ValueTree::fromXml(BinaryData::VeryLargeRoomBright_xml)},
                                    Preset{"Very Large Room (Dark)", "Very Large Rooms", juce::ValueTree::fromXml(BinaryData::VeryLargeRoomDark_xml)},
                                    Preset{"Very Large Room (Drum)", "Very Large Rooms", juce::ValueTree::fromXml(BinaryData::VeryLargeRoomDrum_xml)},
                                    Preset{"Very Large Room (Vocal)", "Very Large Rooms", juce::ValueTree::fromXml(BinaryData::VeryLargeRoomVocal_xml)},
                                    Preset{"Very Large Room (Tiled)", "Very Large Rooms", juce::ValueTree::fromXml(BinaryData::VeryLargeRoomTiled_xml)},
                                    Preset{"Very Large Room (Wooden)", "Very Large Rooms", juce::ValueTree::fromXml(BinaryData::VeryLargeRoomWooden_xml)},
                                    Preset{"Shimmer", "Effects", juce::ValueTree::fromXml(BinaryData::Shimmer_xml)},
                                    Preset{"Live", "Effects", juce::ValueTree::fromXml(BinaryData::Live_xml)},
                                    Preset{"Echo Chamber", "Effects", juce::ValueTree::fromXml(BinaryData::EchoChamber_xml)},
                                    Preset{"Tunnel", "Effects", juce::ValueTree::fromXml(BinaryData::Tunnel_xml)},
                                    Preset{"Long Reverb (12s)", "Effects", juce::ValueTree::fromXml(BinaryData::LongReverb12s_xml)},
                                    Preset{"Long Reverb (30s)", "Effects", juce::ValueTree::fromXml(BinaryData::LongReverb30s_xml)}});

    // create a property in the mParameters.state value tree and store the default preset
    mParameters.state.getOrCreateChildWithName("preset", nullptr).setProperty("selected", DEFAULT_PRESET, nullptr);
}

int PresetManager::getNumPresets() const
{
    return mPresets.size();
}

int PresetManager::getCurrentPreset() const
{
    return mParameters.state.getChildWithName("preset").getProperty("selected");
}

void PresetManager::setCurrentPreset(int presetIndex)
{
    if (presetIndex >= 0 && presetIndex <= mPresets.size())
    {
        auto newParameters = mPresets[presetIndex].presetXml.createCopy();

        // keep values of locked parameters
        for (int i = 0; i < newParameters.getNumChildren(); ++i)
        {
            const auto& parameterName = newParameters.getChild(i).getProperty("id").toString();
            if (isParameterLocked(parameterName))
            {
                newParameters.getChild(i).setProperty("value", mParameters.getParameterAsValue(parameterName), nullptr);
            }
        }

        mParameters.replaceState(newParameters);
        mParameters.state.getOrCreateChildWithName("preset", nullptr).setProperty("selected", presetIndex, nullptr);
        mParameters.undoManager->clearUndoHistory();
    }
    else
    {
        jassertfalse; // there is no such preset ...
    }
}

void PresetManager::setDefaultPreset()
{
    setCurrentPreset(DEFAULT_PRESET);
}

juce::String PresetManager::getPresetName(int presetIndex) const
{
    if (presetIndex >= 0 && presetIndex <= mPresets.size())
    {
        return mPresets[presetIndex].presetName;
    }
    jassertfalse; // there is no such preset ...
    return {};
}

juce::String PresetManager::getPresetCategory(int presetIndex) const
{
    if (presetIndex >= 0 && presetIndex <= mPresets.size())
    {
        return mPresets[presetIndex].presetCategory;
    }
    jassertfalse; // there is no such preset ...
    return {};
}

bool PresetManager::isParameterLocked(const juce::String& parameterID) const
{
    return static_cast<bool>(mApplicationState.getChildWithName("activeParameterLocks").getProperty(parameterID, false));
}

void PresetManager::lockParameter(const juce::String& parameterID)
{
    mApplicationState.getOrCreateChildWithName("activeParameterLocks", nullptr).setProperty(parameterID, true, nullptr);
}

void PresetManager::unlockParameter(const juce::String& parameterID)
{
    mApplicationState.getOrCreateChildWithName("activeParameterLocks", nullptr).setProperty(parameterID, false, nullptr);
}
