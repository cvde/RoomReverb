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

#include "InfoButtonComponent.h"
#include <BinaryData.h>

InfoButtonComponent::InfoButtonComponent(juce::BubbleMessageComponent& bubbleTooltip, const juce::String& text)
        : bubbleMessageTooltip(bubbleTooltip),
          infoText(text)
{
    infoText.setColour(findColour(juce::DocumentWindow::textColourId));
    infoText.setFont(juce::Font(20, juce::Font::bold));
    
    infoButton.setImages(juce::Drawable::createFromImageData(BinaryData::info_svg, BinaryData::info_svgSize).get());
    infoButton.setTooltip(infoText.getText());
#if JUCE_IOS // tooltips don't work on touch screens -> show a bubble message instead
    infoButton.onClick = [this] { showBubbleMessageTooltip(); };
#endif
    addAndMakeVisible(infoButton);
}

void InfoButtonComponent::paint(juce::Graphics& g)
{
    juce::ignoreUnused(g);
}

void InfoButtonComponent::resized()
{
    infoButton.setBounds(getLocalBounds());
}

void InfoButtonComponent::showBubbleMessageTooltip()
{
    bubbleMessageTooltip.showAt(&infoButton, infoText, 4000);
}
