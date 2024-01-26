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

InfoButtonComponent::InfoButtonComponent(juce::BubbleMessageComponent& bubbleTooltip, const juce::String& intoText)
        : mBubbleMessageTooltip(bubbleTooltip),
          mInfoText(intoText)
{
    mInfoText.setColour(findColour(juce::DocumentWindow::textColourId));
    mInfoText.setFont(juce::Font(20, juce::Font::bold));

    mInfoButton.setImages(juce::Drawable::createFromImageData(BinaryData::info_svg, BinaryData::info_svgSize).get());
    mInfoButton.setTooltip(mInfoText.getText());
#if JUCE_IOS // tooltips don't work on touch screens -> show a bubble message instead
    mInfoButton.onClick = [this] { showBubbleMessageTooltip(); };
#endif
    addAndMakeVisible(mInfoButton);
}

void InfoButtonComponent::paint(juce::Graphics& g)
{
    juce::ignoreUnused(g);
}

void InfoButtonComponent::resized()
{
    mInfoButton.setBounds(getLocalBounds());
}

void InfoButtonComponent::showBubbleMessageTooltip()
{
    mBubbleMessageTooltip.showAt(&mInfoButton, mInfoText, 4000);
}
