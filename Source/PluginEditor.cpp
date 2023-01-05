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

#include "PluginEditor.h"

ReverbAudioProcessorEditor::ReverbAudioProcessorEditor(
    ReverbAudioProcessor& processor,
    juce::AudioProcessorValueTreeState& parameterTree,
    juce::ValueTree& appStateTree)
        : AudioProcessorEditor(&processor),
          applicationState(appStateTree),
          customLookAndFeel(*this),
          headerSection(processor, parameterTree, aboutDialog),
          outputSection(parameterTree),
          earlySection(parameterTree),
          lateSection(parameterTree)
{
    juce::LookAndFeel::setDefaultLookAndFeel(&customLookAndFeel);
    setLookAndFeel(&customLookAndFeel);
    headerSection.setLookAndFeel(&customLookAndFeel);
    outputSection.setLookAndFeel(&customLookAndFeel);
    earlySection.setLookAndFeel(&customLookAndFeel);
    lateSection.setLookAndFeel(&customLookAndFeel);
    aboutDialog.setLookAndFeel(&customLookAndFeel);
    tooltipWindow.setLookAndFeel(&customLookAndFeel);
    tooltipWindow.setOpaque(false);

    addAndMakeVisible(headerSection);
    addAndMakeVisible(outputSection);
    addAndMakeVisible(earlySection);
    addAndMakeVisible(lateSection);
    addChildComponent(aboutDialog); // initially not visible

    // set default or stored previous GUI size
    const auto& storedGuiWidth = applicationState.getChildWithName("editor").getProperty("width");
    const auto& storedGuiHeight = applicationState.getChildWithName("editor").getProperty("height");
    if (storedGuiWidth.isVoid() || storedGuiHeight.isVoid())
    {
        setSize(1024, 768);
    }
    else
    {
        setSize(storedGuiWidth, storedGuiHeight);
    }
    setResizable(true, true);
    setResizeLimits(800, 600, 4096, 4096);
}

ReverbAudioProcessorEditor::~ReverbAudioProcessorEditor()
{
    setLookAndFeel(nullptr);
    tooltipWindow.setLookAndFeel(nullptr);
    // store last GUI size
    applicationState.getOrCreateChildWithName("editor", nullptr).setProperty("width", getWidth(), nullptr);
    applicationState.getOrCreateChildWithName("editor", nullptr).setProperty("height", getHeight(), nullptr);
}

//==============================================================================
void ReverbAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void ReverbAudioProcessorEditor::resized()
{
    auto area = getLocalBounds().reduced(2);

    const int aboutDialogHeight = area.getHeight() * 0.75f;
    const int aboutDialogWidth = area.getWidth() * 0.5f;
    aboutDialog.setBounds(area.getCentreX() - aboutDialogWidth / 2, area.getCentreY() - aboutDialogHeight / 2, aboutDialogWidth, aboutDialogHeight);

    const int headerSectionHeight = juce::jlimit(50, 60, (area.getHeight() / 15));
    headerSection.setBounds(area.removeFromTop(headerSectionHeight));

    const int mainContentHeight = area.getHeight();
    auto leftColumn = area.removeFromLeft(area.getWidth() / 2);
    outputSection.setBounds(leftColumn.removeFromTop(mainContentHeight * 2 / 3));
    earlySection.setBounds(leftColumn);

    lateSection.setBounds(area);
}
