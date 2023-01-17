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
#include "DonationSelector.h"
#include <BinaryData.h>

AboutDialog::AboutDialog()
{
    closeButton.setShape(getLookAndFeel().getCrossShape(1.0f), false, true, false);
    closeButton.onClick = [this] { setVisible(false); };
    addAndMakeVisible(closeButton);

    logo = juce::Drawable::createFromImageData(BinaryData::logo_svg, BinaryData::logo_svgSize);

    const juce::String pluginInfoText = juce::String("Room Reverb is free software distributed under the GNU General Public License version 3. For more information, contact options, and the source code, visit www.ElephantDSP.com\n\n")
                                        + "If you like this audio plugin, please consider donating a few spare coins. Thank you very much! :)";
    pluginInfo.setText(pluginInfoText, juce::dontSendNotification);
    pluginInfo.setFont(juce::Font(22.0f));
    pluginInfo.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(pluginInfo);

#if JUCE_IOS
    donateCTA = std::make_unique<DonationSelector>();
#else
    donateCTA = std::make_unique<juce::HyperlinkButton>();
    juce::HyperlinkButton* donateButton = static_cast<juce::HyperlinkButton*>(donateCTA.get());
    donateButton->setButtonText("Donate with PayPal");
    donateButton->setFont(juce::Font(22.0f, juce::Font::underlined), false, juce::Justification::centred);
    donateButton->setURL(juce::URL("https://www.paypal.com/donate/?hosted_button_id=Z3RSDGUEPVY52"));
    donateButton->setTooltip("");
#endif
    addAndMakeVisible(donateCTA.get());

    const juce::String versionInfoText = "Version: " + juce::String(PROJECT_VERSION) + "\n" +
                                         "Format: " + juce::String(juce::AudioProcessor::getWrapperTypeDescription(pluginHostType.getPluginLoadedAs())) + "\n" +
                                         "Host: " + juce::String(pluginHostType.getHostDescription()) + "\n" +
                                         "OS: " + juce::SystemStats::getOperatingSystemName();
    versionInfo.setText(versionInfoText, juce::dontSendNotification);
    versionInfo.setFont(juce::Font(18.0f));
    versionInfo.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(versionInfo);

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

    // header
    const int titleBarHeight = 40;
    auto titleBar = area.removeFromTop(titleBarHeight);
    closeButton.setBounds(titleBar.removeFromRight(titleBarHeight).reduced(10));
    if (logo != nullptr)
        logoBounds = area.removeFromTop(40).reduced(5).toFloat();

    // footer
    auto footer = area.removeFromBottom(150);
    if (vstLogo != nullptr)
    {
        vstLogoBounds = footer.removeFromRight(footer.getWidth() / 2).reduced(10).toFloat();
    }
    versionInfo.setBounds(footer.reduced(10));

    // content
    pluginInfo.setBounds(area.removeFromTop(180));
    donateCTA->setBounds(area.removeFromTop(50).withSizeKeepingCentre(350, 50));
}
