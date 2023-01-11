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

#include "Main.h"

Main::Main(ReverbAudioProcessor& processor)
        : headerSection(processor, aboutDialog),
          outputSection(processor),
          earlySection(processor),
          lateSection(processor)
{
    // background of tooltips (rounded corners) should be transparent
    tooltipWindow.setOpaque(false);

    addAndMakeVisible(headerSection);
    addAndMakeVisible(outputSection);
    addAndMakeVisible(earlySection);
    addAndMakeVisible(lateSection);
    addChildComponent(aboutDialog); // initially not visible
}

void Main::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void Main::resized()
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
