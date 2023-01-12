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
        : processorABStateManager(audioProcessor.getProcessorABStateManager())
{
    aButton.setName("A");
    aButton.setButtonText("A");
    aButton.setTooltip("The A/B buttons are useful for comparing two different plugin settings.");
    aButton.setClickingTogglesState(true);
    aButton.setRadioGroupId(4711, juce::dontSendNotification);
    aButton.onClick = [this] { updateToggleState(aButton); };
    addAndMakeVisible(aButton);

    bButton.setName("B");
    bButton.setButtonText("B");
    bButton.setTooltip("The A/B buttons are useful for comparing two different plugin settings.");
    bButton.setClickingTogglesState(true);
    bButton.setRadioGroupId(4711, juce::dontSendNotification);
    bButton.onClick = [this] { updateToggleState(bButton); };
    addAndMakeVisible(bButton);

    // decide which button is currently active
    if (processorABStateManager.getCurrentProcessorState() == aButton.getName())
    {
        aButton.setToggleState(true, juce::dontSendNotification);
    }
    else
    {
        bButton.setToggleState(true, juce::dontSendNotification);
    }

    copyButton.setButtonText("Copy");
    copyButton.setTooltip("Copy plugin setting A to plugin setting B or vice versa (depending on which setting is currently active).");
    copyButton.onClick = [this] { processorABStateManager.copyActiveToInactiveProcessorState(); };
    addAndMakeVisible(copyButton);
}

void ABToggleComponent::paint(juce::Graphics& g)
{
    juce::ignoreUnused(g);
}

void ABToggleComponent::resized()
{
    auto area = getLocalBounds();
    const int elementWidth = area.getWidth() / 3;

    aButton.setBounds(area.removeFromLeft(elementWidth).reduced(1));
    bButton.setBounds(area.removeFromLeft(elementWidth).reduced(1));
    copyButton.setBounds(area.removeFromLeft(elementWidth).reduced(1));
}

void ABToggleComponent::updateToggleState(const juce::Button& clickedButton)
{
    // process clicks only if the state of the buttons has really changed
    if (clickedButton.getToggleState() &&
        clickedButton.getName() != processorABStateManager.getCurrentProcessorState().toString())
    {
        processorABStateManager.switchProcessorState();
    }
}
