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

LateSection::LateSection(ReverbAudioProcessor& audioProcessor, juce::BubbleMessageComponent& bubbleTooltip)
        : mSectionInfoButton(bubbleTooltip, "Late reverberation is produced by the sound bouncing around the room several times, creating a very high density of echoes before returning to the listener's ear."),
          mLateDamping(audioProcessor, "lateDamping", BinaryData::parameter_damping_svg, BinaryData::parameter_damping_svgSize, bubbleTooltip,
                       "Damping determines the frequency at which the higher frequencies of the late reverberation are cut off. Higher values produce a brighter sound. Lower values produce a darker sound."),
          mLateDiffusion(audioProcessor, "lateDiffusion", BinaryData::parameter_diffusion_svg, BinaryData::parameter_diffusion_svgSize, bubbleTooltip,
                         "Diffusion determines the dispersion of the late reverberation. Higher values produce more dispersion and sound smoother."),
          mLatePredelay(audioProcessor, "latePredelay", BinaryData::parameter_predelay_svg, BinaryData::parameter_predelay_svgSize, bubbleTooltip,
                        "Predelay determines the time between the dry signal and the beginning of the late reverberation. Higher values increase the time and make the virtual room appear larger."),
          mLateRoomSize(audioProcessor, "lateRoomSize", BinaryData::parameter_room_size_svg, BinaryData::parameter_room_size_svgSize, bubbleTooltip,
                        "Room Size determines the size of the virtual room in which the late reverberation appears. Higher values give the impression of a larger room."),
          mLateDecay(audioProcessor, "lateDecay", BinaryData::parameter_decay_svg, BinaryData::parameter_decay_svgSize, bubbleTooltip,
                     "Decay is the time needed for the late reverberation to fade away. Higher values increase the time and make the virtual room appear larger."),
          mLateSpin(audioProcessor, "lateSpin", BinaryData::parameter_spin_svg, BinaryData::parameter_spin_svgSize, bubbleTooltip,
                    "Spin determines the modulation rate of the late reverberation. Higher values produce more movement within the late reverberation."),
          mLateWander(audioProcessor, "lateWander", BinaryData::parameter_wander_svg, BinaryData::parameter_wander_svgSize, bubbleTooltip,
                      "Wander determines the strength of the late reverberation modulation. Higher values make the movement more audible.")
{
    mSectionTitle.setText("Late Reverb", juce::dontSendNotification);
    mSectionTitle.setFont(juce::Font(26.0f));
    addAndMakeVisible(mSectionTitle);
    addAndMakeVisible(mSectionInfoButton);
    addAndMakeVisible(mLateDamping);
    addAndMakeVisible(mLateDiffusion);
    addAndMakeVisible(mLatePredelay);
    addAndMakeVisible(mLateRoomSize);
    addAndMakeVisible(mLateDecay);
    addAndMakeVisible(mLateSpin);
    addAndMakeVisible(mLateWander);
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
    const int sectionTitleTextWidth = mSectionTitle.getFont().getStringWidth(mSectionTitle.getText()) + 10;
    mSectionTitle.setBounds(sectionTitleAndInfo.removeFromLeft(sectionTitleTextWidth));
    mSectionInfoButton.setBounds(sectionTitleAndInfo.getX(), sectionTitleAndInfo.getCentreY() - 12, 24, 24);

    mLateRoomSize.setBounds(area.removeFromTop(elementHeight));
    mLateDecay.setBounds(area.removeFromTop(elementHeight));
    mLatePredelay.setBounds(area.removeFromTop(elementHeight));
    mLateDamping.setBounds(area.removeFromTop(elementHeight));
    mLateDiffusion.setBounds(area.removeFromTop(elementHeight));
    mLateSpin.setBounds(area.removeFromTop(elementHeight));
    mLateWander.setBounds(area.removeFromTop(elementHeight));
}
