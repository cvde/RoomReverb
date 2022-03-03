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

#include "Components/AboutDialog.h"
#include "Components/EarlySection.h"
#include "Components/HeaderSection.h"
#include "Components/LateSection.h"
#include "Components/OutputSection.h"
#include "LookAndFeel/CustomLookAndFeel.h"
#include "PluginProcessor.h"
#include <juce_audio_processors/juce_audio_processors.h>

class ReverbAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    ReverbAudioProcessorEditor(ReverbAudioProcessor& processor, juce::AudioProcessorValueTreeState& parameterTree, juce::ValueTree& appStateTree);
    ~ReverbAudioProcessorEditor() override;

    //==============================================================================
    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    juce::ValueTree& applicationState;
    CustomLookAndFeel customLookAndFeel;
    HeaderSection headerSection;
    OutputSection outputSection;
    EarlySection earlySection;
    LateSection lateSection;
    AboutDialog aboutDialog;

    // make tooltips visible
    juce::TooltipWindow tooltipWindow{nullptr, 300};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ReverbAudioProcessorEditor)
};
