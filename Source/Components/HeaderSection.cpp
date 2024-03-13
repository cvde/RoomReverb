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

HeaderSection::HeaderSection(
    ReverbAudioProcessor& audioProcessor,
    AboutDialog& aboutDialog)
        : mUndo(audioProcessor),
          mPreset(audioProcessor),
          mAbToggle(audioProcessor),
          mAboutDialog(aboutDialog)
{
    addAndMakeVisible(mLogo);
    addAndMakeVisible(mUndo);
    addAndMakeVisible(mAbToggle);
    addAndMakeVisible(mPreset);

    mAboutButton.setButtonText("?");
    mAboutButton.setTooltip("About this plugin.");
    mAboutButton.onClick = [this] { mAboutDialog.setVisible(true); };
    addAndMakeVisible(mAboutButton);
}

void HeaderSection::paint(juce::Graphics& g)
{
    g.setColour(juce::Colour(0xff303030));
    g.fillRoundedRectangle(getLocalBounds().reduced(2).toFloat(), 10.0f);
}

void HeaderSection::resized()
{
    auto area = getLocalBounds().reduced(2);
    const auto logoWidth = static_cast<int>(area.getWidth() * 0.34f);
    const auto undoWidth = static_cast<int>(area.getWidth() * 0.12f);
    const auto presetWidth = static_cast<int>(area.getWidth() * 0.3f);
    const auto abToggleWidth = static_cast<int>(area.getWidth() * 0.18f);
    const auto aboutButtonWidth = static_cast<int>(area.getWidth() * 0.06f);

    mLogo.setBounds(area.removeFromLeft(logoWidth).reduced(5));
    mUndo.setBounds(area.removeFromLeft(undoWidth).reduced(5));
    mPreset.setBounds(area.removeFromLeft(presetWidth).reduced(5));
    mAbToggle.setBounds(area.removeFromLeft(abToggleWidth).reduced(5));
    mAboutButton.setBounds(area.removeFromLeft(aboutButtonWidth).reduced(5));
}
