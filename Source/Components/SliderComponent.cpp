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
    ReverbAudioProcessor& audioProcessor,
    const juce::String& parameterID,
    const char* binaryIcon,
    const int binaryIconSize,
    juce::BubbleMessageComponent& bubbleTooltip,
    const juce::String& infoText)
        : mParameters(audioProcessor.getParameters()),
          mPresetManager(audioProcessor.getPresetManager()),
          mParameterInfoButton(bubbleTooltip, infoText),
          mParameterSliderAttachment(audioProcessor.getParameters(), parameterID, mParameterSlider)
{
    mParameterIcon = juce::Drawable::createFromImageData(binaryIcon, (size_t)binaryIconSize);
    addAndMakeVisible(mParameterIcon.get());

    mParameterLabel.setText(mParameters.getParameter(parameterID)->getName(40), juce::dontSendNotification);
    mParameterLabel.setFont(juce::Font(20.0f));
    addAndMakeVisible(mParameterLabel);

    addAndMakeVisible(mParameterInfoButton);

    mParameterSlider.setTextValueSuffix(mParameters.getParameter(parameterID)->getLabel());
    mParameterSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 62, 22);
    addAndMakeVisible(mParameterSlider);

    mParameterLockButton.setImages(juce::Drawable::createFromImageData(BinaryData::lock_open_svg, BinaryData::lock_open_svgSize).get(), nullptr, nullptr, nullptr, juce::Drawable::createFromImageData(BinaryData::lock_closed_svg, BinaryData::lock_closed_svgSize).get(), nullptr, nullptr, nullptr);
    mParameterLockButton.setTooltip("Lock this parameter to keep it when selecting another preset.");
    mParameterLockButton.setClickingTogglesState(true);
    mParameterLockButton.setToggleState(mPresetManager.isParameterLocked(parameterID), juce::dontSendNotification);
    mParameterLockButton.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colours::transparentBlack);
    mParameterLockButton.onClick = [this, parameterID] { mParameterLockButton.getToggleState() ? mPresetManager.lockParameter(parameterID) : mPresetManager.unlockParameter(parameterID); };

    addAndMakeVisible(mParameterLockButton);
}

void SliderComponent::paint(juce::Graphics& g)
{
    juce::ignoreUnused(g);
}

void SliderComponent::resized()
{
    auto area = getLocalBounds();

    auto parameterIconArea = area.removeFromLeft(area.getHeight() / 2);
    mParameterIcon->setTransformToFit(parameterIconArea.toFloat().reduced(5), juce::RectanglePlacement::centred);
    mParameterIcon->setBounds(parameterIconArea);

    auto labelAndInfo = area.removeFromTop(area.getHeight() / 2);
    // for some reason the string width calculation is not reliable... therefore + 10
    const int labelTextWidth = mParameterLabel.getFont().getStringWidth(mParameterLabel.getText()) + 10;
    mParameterLabel.setBounds(labelAndInfo.removeFromLeft(labelTextWidth));
    mParameterInfoButton.setBounds(labelAndInfo.getX(), labelAndInfo.getCentreY() - 12, 24, 24);

    auto parameterLockButtonSize = area.removeFromRight(30).reduced(3);
    mParameterLockButton.setBounds(parameterLockButtonSize.getX(), parameterLockButtonSize.getCentreY() - 12, 24, 24);

    mParameterSlider.setBounds(area);
}
