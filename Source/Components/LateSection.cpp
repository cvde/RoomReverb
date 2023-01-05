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

#include "LateSection.h"
#include <BinaryData.h>

LateSection::LateSection(juce::AudioProcessorValueTreeState& parameterTree)
        : lateDamping(parameterTree, "lateDamping", BinaryData::parameter_damping_svg, BinaryData::parameter_damping_svgSize,
                      "Damping determines the frequency at which the higher frequencies of the late reverberation are cut off. Higher values produce a brighter sound. Lower values produce a darker sound."),
          lateDiffusion(parameterTree, "lateDiffusion", BinaryData::parameter_diffusion_svg, BinaryData::parameter_diffusion_svgSize,
                        "Diffusion determines the dispersion of the late reverberation. Higher values produce more dispersion and sound smoother."),
          latePredelay(parameterTree, "latePredelay", BinaryData::parameter_predelay_svg, BinaryData::parameter_predelay_svgSize,
                       "Predelay determines the time between the dry signal and the beginning of the late reverberation. Higher values increase the time and make the virtual room appear larger."),
          lateRoomSize(parameterTree, "lateRoomSize", BinaryData::parameter_room_size_svg, BinaryData::parameter_room_size_svgSize,
                       "Room Size determines the size of the virtual room in which the late reverberation appears. Higher values give the impression of a larger room."),
          lateDecay(parameterTree, "lateDecay", BinaryData::parameter_decay_svg, BinaryData::parameter_decay_svgSize,
                    "Decay is the time needed for the late reverberation to fade away. Higher values increase the time and make the virtual room appear larger."),
          lateSpin(parameterTree, "lateSpin", BinaryData::parameter_spin_svg, BinaryData::parameter_spin_svgSize,
                   "Spin determines the modulation rate of the late reverberation. Higher values produce more movement within the late reverberation."),
          lateWander(parameterTree, "lateWander", BinaryData::parameter_wander_svg, BinaryData::parameter_wander_svgSize,
                     "Wander determines the strength of the late reverberation modulation. Higher values make the movement more audible.")
{
    sectionTitle.setText("Late Reverb", juce::dontSendNotification);
    sectionTitle.setFont(juce::Font(26.0f));
    addAndMakeVisible(sectionTitle);

    parameterInfoButton.setImages(juce::Drawable::createFromImageData(BinaryData::info_svg, BinaryData::info_svgSize).get());
    parameterInfoButton.setTooltip("Late reverberation is produced by the sound bouncing around the room several times, creating a very high density of echoes before returning to the listener's ear.");
    addAndMakeVisible(parameterInfoButton);

    addAndMakeVisible(lateDamping);
    addAndMakeVisible(lateDiffusion);
    addAndMakeVisible(latePredelay);
    addAndMakeVisible(lateRoomSize);
    addAndMakeVisible(lateDecay);
    addAndMakeVisible(lateSpin);
    addAndMakeVisible(lateWander);
}

void LateSection::paint(juce::Graphics& g)
{
    g.setColour(juce::Colour(0xff303030));
    g.fillRoundedRectangle(getLocalBounds().reduced(2).toFloat(), 10.0f);
}

void LateSection::resized()
{
    auto area = getLocalBounds().reduced(5, 0);
    const int elementHeight = (area.getHeight() - 5) / 9; // remove 5 pixels from the bottom

    auto sectionTitleAndInfo = area.removeFromTop(elementHeight);
    // for some reason the string width calculation is not reliable... therefore + 10
    const int sectionTitleTextWidth = sectionTitle.getFont().getStringWidth(sectionTitle.getText()) + 10;
    sectionTitle.setBounds(sectionTitleAndInfo.removeFromLeft(sectionTitleTextWidth));
    // placing the info button
    parameterInfoButton.setBounds(sectionTitleAndInfo.getX(), sectionTitleAndInfo.getCentreY() - 12, 24, 24);

    lateRoomSize.setBounds(area.removeFromTop(elementHeight));
    lateDecay.setBounds(area.removeFromTop(elementHeight));
    latePredelay.setBounds(area.removeFromTop(elementHeight));
    lateDamping.setBounds(area.removeFromTop(elementHeight));
    lateDiffusion.setBounds(area.removeFromTop(elementHeight));
    lateSpin.setBounds(area.removeFromTop(elementHeight));
    lateWander.setBounds(area.removeFromTop(elementHeight));
}
