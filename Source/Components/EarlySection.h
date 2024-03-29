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
#include "InfoButtonComponent.h"
#include "SliderComponent.h"
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_basics/juce_gui_basics.h>

class EarlySection : public juce::Component
{
public:
    EarlySection(ReverbAudioProcessor& audioProcessor, juce::BubbleMessageComponent& bubbleTooltip);

    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    juce::Label mSectionTitle;
    InfoButtonComponent mSectionInfoButton;
    SliderComponent mEarlyDamping;
    SliderComponent mEarlyRoomSize;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EarlySection)
};
