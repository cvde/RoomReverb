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
#include <juce_gui_basics/juce_gui_basics.h>

class SliderComponent : public juce::Component
{
public:
    SliderComponent(
        juce::AudioProcessorValueTreeState& parameterTree,
        const juce::String& parameterID,
        const char* binaryIcon,
        const int binaryIconSize,
        const juce::String& parameterInfoText);

    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    std::unique_ptr<juce::Drawable> parameterIcon;
    juce::Label parameterLabel;
    juce::DrawableButton parameterInfoButton{"parameterInfoButton", juce::DrawableButton::ImageFitted};
    juce::Slider parameterSlider;
    juce::AudioProcessorValueTreeState::SliderAttachment parameterSliderAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SliderComponent)
};
