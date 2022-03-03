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

#include "HeaderSection.h"
#include <BinaryData.h>

HeaderSection::HeaderSection(
    ReverbAudioProcessor& processor,
    juce::AudioProcessorValueTreeState& parameterTree,
    AboutDialog& aboutDialogComponent)
        : undo(parameterTree),
          preset(processor, parameterTree),
          abToggle(processor),
          aboutDialog(aboutDialogComponent)
{
    addAndMakeVisible(logo);
    addAndMakeVisible(undo);
    addAndMakeVisible(abToggle);
    addAndMakeVisible(preset);

    aboutButton.setButtonText("?");
    aboutButton.setTooltip("About this plugin.");
    aboutButton.onClick = [this] { this->aboutDialog.setVisible(true); };
    addAndMakeVisible(aboutButton);
}

void HeaderSection::paint(juce::Graphics& g)
{
    g.setColour(juce::Colour(0xff303030));
    g.fillRoundedRectangle(getLocalBounds().reduced(2).toFloat(), 10.0f);
}

void HeaderSection::resized()
{
    auto area = getLocalBounds().reduced(2);
    const int logoWidth = area.getWidth() * 0.34f;
    const int undoWidth = area.getWidth() * 0.12f;
    const int presetWidth = area.getWidth() * 0.3f;
    const int abToggleWidth = area.getWidth() * 0.18f;
    const int aboutButtonWidth = area.getWidth() * 0.06f;

    logo.setBounds(area.removeFromLeft(logoWidth).reduced(5));
    undo.setBounds(area.removeFromLeft(undoWidth).reduced(5));
    preset.setBounds(area.removeFromLeft(presetWidth).reduced(5));
    abToggle.setBounds(area.removeFromLeft(abToggleWidth).reduced(5));
    aboutButton.setBounds(area.removeFromLeft(aboutButtonWidth).reduced(5));
}
