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

#pragma once

#include "AboutDialog.h"
#include "EarlySection.h"
#include "HeaderSection.h"
#include "LateSection.h"
#include "OutputSection.h"
#include <juce_gui_extra/juce_gui_extra.h>

class Main : public juce::Component
{
public:
    Main(ReverbAudioProcessor& audioProcessor);

    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    HeaderSection mHeaderSection;
    OutputSection mOutputSection;
    EarlySection mEarlySection;
    LateSection mLateSection;
    AboutDialog mAboutDialog;

    // make tooltips visible
    juce::TooltipWindow mTooltipWindow{this, 300};

    juce::BubbleMessageComponent mBubbleTooltip;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Main)
};
