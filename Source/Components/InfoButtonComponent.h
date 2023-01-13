/**
 *  ElephantDSP.com Room Reverb
 *
 *  Copyright (C) 2023 Christian Voigt
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

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_gui_extra/juce_gui_extra.h>

class InfoButtonComponent : public juce::Component
{
public:
    InfoButtonComponent(juce::BubbleMessageComponent& bubbleTooltip, const juce::String& text);

    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    void showBubbleMessageTooltip();

    juce::DrawableButton infoButton{"infoButton", juce::DrawableButton::ImageFitted};
    juce::BubbleMessageComponent& bubbleMessageTooltip;
    juce::AttributedString infoText;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(InfoButtonComponent)
};
