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

ReverbAudioProcessorEditor::ReverbAudioProcessorEditor(ReverbAudioProcessor& audioProcessor)
        : AudioProcessorEditor(&audioProcessor),
          applicationState(audioProcessor.getApplicationState()),
          main(audioProcessor)
{
    juce::LookAndFeel::setDefaultLookAndFeel(&customLookAndFeel);
    setLookAndFeel(&customLookAndFeel);
    main.setLookAndFeel(&customLookAndFeel);

    addAndMakeVisible(main);

    // set default or stored previous GUI size
    const auto& storedGuiWidth = applicationState.getChildWithName("editor").getProperty("width");
    const auto& storedGuiHeight = applicationState.getChildWithName("editor").getProperty("height");
    if (storedGuiWidth.isVoid() || storedGuiHeight.isVoid())
    {
        setSize(idealWidth, idealHeight);
    }
    else
    {
        setSize(storedGuiWidth, storedGuiHeight);
    }
    setResizable(true, true);
}

ReverbAudioProcessorEditor::~ReverbAudioProcessorEditor()
{
    setLookAndFeel(nullptr);
    main.setLookAndFeel(nullptr);

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
    const int currentWidth = getWidth();
    const int currentHeight = getHeight();

    // scale GUI down if the available space is below the minimum size
    if (currentWidth < minWidth || currentHeight < minHeight)
    {
        const float scaleX = (float)currentWidth / (float)minWidth;
        const float scaleY = (float)currentHeight / (float)minHeight;
        const float scale = juce::jmin(scaleX, scaleY);
        main.setTransform(juce::AffineTransform::scale(scale));
        main.centreWithSize(minWidth, minHeight);
    }
    else
    {
        main.setTransform(juce::AffineTransform::scale(1));
        main.setBounds(getLocalBounds());
    }
}
