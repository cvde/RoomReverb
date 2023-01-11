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

#include "AboutDialog.h"
#include "Config.h"
#include <BinaryData.h>

AboutDialog::AboutDialog()
{
    closeButton.setShape(getLookAndFeel().getCrossShape(1.0f), false, true, false);
    closeButton.onClick = [this] { setVisible(false); };
    addAndMakeVisible(closeButton);

    logo = juce::Drawable::createFromImageData(BinaryData::logo_svg, BinaryData::logo_svgSize);

    const juce::String pluginInfoText = juce::String("ElephantDSP.com Room Reverb is free software distributed under the GNU General Public License version 3. For more information, contact options, and the source code, visit www.ElephantDSP.com\n\n") +
                                        "Version: " + juce::String(PROJECT_VERSION) + "\n" +
                                        "Format: " + juce::String(juce::AudioProcessor::getWrapperTypeDescription(pluginHostType.getPluginLoadedAs())) + "\n" +
                                        "Host: " + juce::String(pluginHostType.getHostDescription()) + "\n" +
                                        "OS: " + juce::SystemStats::getOperatingSystemName();
    pluginInfo.setText(pluginInfoText, juce::dontSendNotification);
    pluginInfo.setFont(juce::Font(22.0f));
    pluginInfo.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(pluginInfo);

    if (pluginHostType.getPluginLoadedAs() == juce::AudioProcessor::wrapperType_VST3)
        vstLogo = juce::Drawable::createFromImageData(BinaryData::VST_Compatible_Logo_Steinberg_with_TM_negative_svg, BinaryData::VST_Compatible_Logo_Steinberg_with_TM_negative_svgSize);

    setAlwaysOnTop(true);
}

void AboutDialog::paint(juce::Graphics& g)
{
    auto cornerSize = 10.0f;

    g.setColour(juce::Colour(0xff303030));
    g.fillRoundedRectangle(getLocalBounds().toFloat(), cornerSize);

    g.setColour(juce::Colour(0xffc6c6c6));
    g.drawRoundedRectangle(getLocalBounds().toFloat().reduced(0.5f, 0.5f), cornerSize, 1.0f);

    if (vstLogo != nullptr)
        vstLogo->drawWithin(g, vstLogoBounds, juce::RectanglePlacement::centred, 1.0);

    if (logo != nullptr)
        logo->drawWithin(g, logoBounds, juce::RectanglePlacement::centred, 1.0);
}

void AboutDialog::resized()
{
    auto area = getLocalBounds().reduced(2);

    const int titleBarHeight = 35;
    auto titleBar = area.removeFromTop(titleBarHeight);
    closeButton.setBounds(titleBar.removeFromRight(titleBarHeight).reduced(10));

    if (logo != nullptr)
        logoBounds = area.removeFromTop(40).reduced(5).toFloat();

    if (vstLogo != nullptr)
        vstLogoBounds = area.removeFromBottom(150).reduced(5).toFloat();

    pluginInfo.setBounds(area);
}
