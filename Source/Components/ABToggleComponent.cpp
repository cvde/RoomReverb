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

#include "ABToggleComponent.h"

ABToggleComponent::ABToggleComponent(ReverbAudioProcessor& audioProcessor)
        : mProcessorABStateManager(audioProcessor.getProcessorABStateManager())
{
    mAButton.setName("A");
    mAButton.setButtonText("A");
    mAButton.setTooltip("The A/B buttons are useful for comparing two different plugin settings.");
    mAButton.setClickingTogglesState(true);
    mAButton.setRadioGroupId(4711, juce::dontSendNotification);
    mAButton.onClick = [this] { updateToggleState(mAButton); };
    addAndMakeVisible(mAButton);

    mBButton.setName("B");
    mBButton.setButtonText("B");
    mBButton.setTooltip("The A/B buttons are useful for comparing two different plugin settings.");
    mBButton.setClickingTogglesState(true);
    mBButton.setRadioGroupId(4711, juce::dontSendNotification);
    mBButton.onClick = [this] { updateToggleState(mBButton); };
    addAndMakeVisible(mBButton);

    // decide which button is currently active
    if (mProcessorABStateManager.getCurrentProcessorState() == mAButton.getName())
    {
        mAButton.setToggleState(true, juce::dontSendNotification);
    }
    else
    {
        mBButton.setToggleState(true, juce::dontSendNotification);
    }

    mCopyButton.setButtonText("Copy");
    mCopyButton.setTooltip("Copy plugin setting A to plugin setting B or vice versa (depending on which setting is currently active).");
    mCopyButton.onClick = [this] { mProcessorABStateManager.copyActiveToInactiveProcessorState(); };
    addAndMakeVisible(mCopyButton);
}

void ABToggleComponent::paint(juce::Graphics& g)
{
    juce::ignoreUnused(g);
}

void ABToggleComponent::resized()
{
    auto area = getLocalBounds();
    const int elementWidth = area.getWidth() / 3;

    mAButton.setBounds(area.removeFromLeft(elementWidth).reduced(1));
    mBButton.setBounds(area.removeFromLeft(elementWidth).reduced(1));
    mCopyButton.setBounds(area.removeFromLeft(elementWidth).reduced(1));
}

void ABToggleComponent::updateToggleState(const juce::Button& clickedButton)
{
    // process clicks only if the state of the buttons has really changed
    if (clickedButton.getToggleState() &&
        clickedButton.getName() != mProcessorABStateManager.getCurrentProcessorState().toString())
    {
        mProcessorABStateManager.switchProcessorState();
    }
}
