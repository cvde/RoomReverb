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

#include "EarlySection.h"
#include <BinaryData.h>

EarlySection::EarlySection(ReverbAudioProcessor& audioProcessor, juce::BubbleMessageComponent& bubbleTooltip)
        : mSectionInfoButton(bubbleTooltip, "Early reflections are produced by the dry signal bouncing off a wall and directly returning to the listener's ear."),
          mEarlyDamping(audioProcessor, "earlyDamping", BinaryData::parameter_damping_svg, BinaryData::parameter_damping_svgSize, bubbleTooltip,
                        "Damping determines the frequency at which higher frequencies of the early reflections are cut off. Higher values produce a brighter sound. Lower values produce a darker sound."),
          mEarlyRoomSize(audioProcessor, "earlyRoomSize", BinaryData::parameter_room_size_svg, BinaryData::parameter_room_size_svgSize, bubbleTooltip,
                         "Room Size determines the size of the virtual room in which the early reflections appear. Higher values give the impression of a larger room.")
{
    mSectionTitle.setText("Early Reflections", juce::dontSendNotification);
    mSectionTitle.setFont(juce::FontOptions(26.0f));
    addAndMakeVisible(mSectionTitle);
    addAndMakeVisible(mSectionInfoButton);
    addAndMakeVisible(mEarlyDamping);
    addAndMakeVisible(mEarlyRoomSize);
}

void EarlySection::paint(juce::Graphics& g)
{
    g.setColour(juce::Colour(0xff303030));
    g.fillRoundedRectangle(getLocalBounds().reduced(2).toFloat(), 10.0f);
}

void EarlySection::resized()
{
    auto area = getLocalBounds().reduced(5, 0);
    const int elementHeight = (area.getHeight() - 5) / 3; // remove 5 pixels from the bottom

    auto sectionTitleAndInfo = area.removeFromTop(elementHeight);
    // for some reason the string width calculation is not reliable... therefore + 10
    const int sectionTitleTextWidth = mSectionTitle.getFont().getStringWidth(mSectionTitle.getText()) + 10;
    mSectionTitle.setBounds(sectionTitleAndInfo.removeFromLeft(sectionTitleTextWidth));
    mSectionInfoButton.setBounds(sectionTitleAndInfo.getX(), sectionTitleAndInfo.getCentreY() - 12, 24, 24);

    mEarlyRoomSize.setBounds(area.removeFromTop(elementHeight));
    mEarlyDamping.setBounds(area.removeFromTop(elementHeight));
}
