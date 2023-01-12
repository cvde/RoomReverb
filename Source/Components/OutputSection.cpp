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

OutputSection::OutputSection(ReverbAudioProcessor& audioProcessor)
        : dryLevel(audioProcessor, "dryLevel", BinaryData::parameter_level_svg, BinaryData::parameter_level_svgSize,
                   "Dry Level is the proportion of the clean, unprocessed input signal sent to the output. Higher values give the impression of being closer to the sound source."),
          earlyLevel(audioProcessor, "earlyLevel", BinaryData::parameter_level_svg, BinaryData::parameter_level_svgSize,
                     "Early Reflections Level is the proportion of early reflections sent to the output. Higher values give the impression of a smaller room."),
          earlySendLevel(audioProcessor, "earlySendLevel", BinaryData::parameter_level_svg, BinaryData::parameter_level_svgSize,
                         "Early Reflections Send Level is the proportion of early reflections sent to the late reverberation. Higher values produce more dispersion and a smoother sound in the late reverberation."),
          lateLevel(audioProcessor, "lateLevel", BinaryData::parameter_level_svg, BinaryData::parameter_level_svgSize,
                    "Late Reverb Level is the proportion of late reverberation sent to the output. Higher values give the impression of a larger room."),
          stereoWidth(audioProcessor, "stereoWidth", BinaryData::parameter_stereo_svg, BinaryData::parameter_stereo_svgSize,
                      "Stereo Width determines the perceived width of the early reflections and late reverberation stereo field. Higher values produce a wider stereo field.")
{
    sectionTitle.setText("Output", juce::dontSendNotification);
    sectionTitle.setFont(juce::Font(26.0f));
    addAndMakeVisible(sectionTitle);

    parameterInfoButton.setImages(juce::Drawable::createFromImageData(BinaryData::info_svg, BinaryData::info_svgSize).get());
    parameterInfoButton.setTooltip("Levels of the different parts of the output reverb signal.");
    addAndMakeVisible(parameterInfoButton);

    addAndMakeVisible(dryLevel);
    addAndMakeVisible(earlyLevel);
    addAndMakeVisible(earlySendLevel);
    addAndMakeVisible(lateLevel);
    addAndMakeVisible(stereoWidth);
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
    const int sectionTitleTextWidth = sectionTitle.getFont().getStringWidth(sectionTitle.getText()) + 10;
    sectionTitle.setBounds(sectionTitleAndInfo.removeFromLeft(sectionTitleTextWidth));
    // placing the info button
    parameterInfoButton.setBounds(sectionTitleAndInfo.getX(), sectionTitleAndInfo.getCentreY() - 12, 24, 24);

    dryLevel.setBounds(area.removeFromTop(elementHeight));
    earlyLevel.setBounds(area.removeFromTop(elementHeight));
    earlySendLevel.setBounds(area.removeFromTop(elementHeight));
    lateLevel.setBounds(area.removeFromTop(elementHeight));
    stereoWidth.setBounds(area.removeFromTop(elementHeight));
}
