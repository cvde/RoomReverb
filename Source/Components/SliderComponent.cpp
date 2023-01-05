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

#include "SliderComponent.h"
#include <BinaryData.h>

SliderComponent::SliderComponent(
    juce::AudioProcessorValueTreeState& parameterTree,
    const juce::String& parameterID,
    const char* binaryIcon,
    const int binaryIconSize,
    const juce::String& parameterInfoText)
        : parameterSliderAttachment(parameterTree, parameterID, parameterSlider)
{
    parameterSlider.setTextValueSuffix(parameterTree.getParameter(parameterID)->getLabel());
    parameterSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 62, 22);
    addAndMakeVisible(parameterSlider);

    parameterLabel.setText(parameterTree.getParameter(parameterID)->getName(40), juce::dontSendNotification);
    parameterLabel.setFont(juce::Font(20.0f));
    addAndMakeVisible(parameterLabel);

    parameterInfoButton.setImages(juce::Drawable::createFromImageData(BinaryData::info_svg, BinaryData::info_svgSize).get());
    parameterInfoButton.setTooltip(parameterInfoText);
    addAndMakeVisible(parameterInfoButton);

    parameterIcon = juce::Drawable::createFromImageData(binaryIcon, binaryIconSize);
    addAndMakeVisible(parameterIcon.get());
}

void SliderComponent::paint(juce::Graphics& g)
{
    juce::ignoreUnused(g);
}

void SliderComponent::resized()
{
    auto area = getLocalBounds();

    auto parameterIconArea = area.removeFromLeft(area.getHeight() * 0.5f);
    parameterIcon->setTransformToFit(parameterIconArea.toFloat().reduced(5), juce::RectanglePlacement::centred);
    parameterIcon->setBounds(parameterIconArea);

    auto labelAndInfo = area.removeFromTop(area.getHeight() / 2);
    // for some reason the string width calculation is not reliable... therefore + 10
    const int labelTextWidth = parameterLabel.getFont().getStringWidth(parameterLabel.getText()) + 10;
    parameterLabel.setBounds(labelAndInfo.removeFromLeft(labelTextWidth));
    // placing the info button
    parameterInfoButton.setBounds(labelAndInfo.getX(), labelAndInfo.getCentreY() - 12, 24, 24);

    area.removeFromRight(5);
    parameterSlider.setBounds(area);
}
