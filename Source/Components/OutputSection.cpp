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

#include "OutputSection.h"
#include <BinaryData.h>

OutputSection::OutputSection(ReverbAudioProcessor& audioProcessor, juce::BubbleMessageComponent& bubbleTooltip)
        : mSectionInfoButton(bubbleTooltip, "Levels of the different parts of the output reverb signal."),
          mDryLevel(audioProcessor, "dryLevel", BinaryData::parameter_level_svg, BinaryData::parameter_level_svgSize, bubbleTooltip,
                    "Dry Level is the proportion of the clean, unprocessed input signal sent to the output. Higher values give the impression of being closer to the sound source."),
          mEarlyLevel(audioProcessor, "earlyLevel", BinaryData::parameter_level_svg, BinaryData::parameter_level_svgSize, bubbleTooltip,
                      "Early Reflections Level is the proportion of early reflections sent to the output. Higher values give the impression of a smaller room."),
          mEarlySendLevel(audioProcessor, "earlySendLevel", BinaryData::parameter_level_svg, BinaryData::parameter_level_svgSize, bubbleTooltip,
                          "Early Reflections Send Level is the proportion of early reflections sent to the late reverberation. Higher values produce more dispersion and a smoother sound in the late reverberation."),
          mLateLevel(audioProcessor, "lateLevel", BinaryData::parameter_level_svg, BinaryData::parameter_level_svgSize, bubbleTooltip,
                     "Late Reverb Level is the proportion of late reverberation sent to the output. Higher values give the impression of a larger room."),
          mStereoWidth(audioProcessor, "stereoWidth", BinaryData::parameter_stereo_svg, BinaryData::parameter_stereo_svgSize, bubbleTooltip,
                       "Stereo Width determines the perceived width of the early reflections and late reverberation stereo field. Higher values produce a wider stereo field.")
{
    mSectionTitle.setText("Output", juce::dontSendNotification);
    mSectionTitle.setFont(juce::FontOptions(26.0f));
    addAndMakeVisible(mSectionTitle);
    addAndMakeVisible(mSectionInfoButton);
    addAndMakeVisible(mDryLevel);
    addAndMakeVisible(mEarlyLevel);
    addAndMakeVisible(mEarlySendLevel);
    addAndMakeVisible(mLateLevel);
    addAndMakeVisible(mStereoWidth);
}

void OutputSection::paint(juce::Graphics& g)
{
    g.setColour(juce::Colour(0xff303030));
    g.fillRoundedRectangle(getLocalBounds().reduced(2).toFloat(), 10.0f);
}

void OutputSection::resized()
{
    auto area = getLocalBounds().reduced(5, 0);
    const int elementHeight = (area.getHeight() - 5) / 6; // remove 5 pixels from the bottom

    auto sectionTitleAndInfo = area.removeFromTop(elementHeight);
    // for some reason the string width calculation is not reliable... therefore + 10
    const int sectionTitleTextWidth = mSectionTitle.getFont().getStringWidth(mSectionTitle.getText()) + 10;
    mSectionTitle.setBounds(sectionTitleAndInfo.removeFromLeft(sectionTitleTextWidth));
    mSectionInfoButton.setBounds(sectionTitleAndInfo.getX(), sectionTitleAndInfo.getCentreY() - 12, 24, 24);

    mDryLevel.setBounds(area.removeFromTop(elementHeight));
    mEarlyLevel.setBounds(area.removeFromTop(elementHeight));
    mEarlySendLevel.setBounds(area.removeFromTop(elementHeight));
    mLateLevel.setBounds(area.removeFromTop(elementHeight));
    mStereoWidth.setBounds(area.removeFromTop(elementHeight));
}
