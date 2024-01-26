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

#include "LogoComponent.h"
#include <BinaryData.h>

LogoComponent::LogoComponent()
{
    mLogo = juce::Drawable::createFromImageData(BinaryData::logo_svg, BinaryData::logo_svgSize);
}

void LogoComponent::paint(juce::Graphics& g)
{
    if (mLogo != nullptr)
    {
        mLogo->drawWithin(g, mLogoBounds, juce::RectanglePlacement::xLeft, 1.0f);
    }
}

void LogoComponent::resized()
{
    mLogoBounds = getLocalBounds().reduced(5).toFloat();
}
