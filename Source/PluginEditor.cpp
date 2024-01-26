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
          mApplicationState(audioProcessor.getApplicationState()),
          mMainView(audioProcessor)
{
    juce::LookAndFeel::setDefaultLookAndFeel(&mCustomLookAndFeel);
    setLookAndFeel(&mCustomLookAndFeel);
    mMainView.setLookAndFeel(&mCustomLookAndFeel);

    addAndMakeVisible(mMainView);

    // set default or stored previous GUI size
    const auto& storedGuiWidth = mApplicationState.getChildWithName("editor").getProperty("width");
    const auto& storedGuiHeight = mApplicationState.getChildWithName("editor").getProperty("height");
    if (storedGuiWidth.isVoid() || storedGuiHeight.isVoid())
    {
        setSize(IDEAL_WIDTH, IDEAL_HEIGHT);
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
    mMainView.setLookAndFeel(nullptr);

    // store last GUI size
    mApplicationState.getOrCreateChildWithName("editor", nullptr).setProperty("width", getWidth(), nullptr);
    mApplicationState.getOrCreateChildWithName("editor", nullptr).setProperty("height", getHeight(), nullptr);
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
    if (currentWidth < MIN_WIDTH || currentHeight < MIN_HEIGHT)
    {
        const float scaleX = static_cast<float>(currentWidth) / MIN_WIDTH;
        const float scaleY = static_cast<float>(currentHeight) / MIN_HEIGHT;
        const float scale = juce::jmin(scaleX, scaleY);
        mMainView.setTransform(juce::AffineTransform::scale(scale));
        mMainView.centreWithSize(MIN_WIDTH, MIN_HEIGHT);
    }
    else
    {
        mMainView.setTransform(juce::AffineTransform::scale(1));
        mMainView.setBounds(getLocalBounds());
    }
}
