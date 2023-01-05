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

#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

class ProcessorABStateManager
{
public:
    ProcessorABStateManager(juce::AudioProcessorValueTreeState& parameterTree, juce::ValueTree& appStateTree);

    const juce::var& getCurrentProcessorState() const;
    void switchProcessorState();
    void copyActiveToInactiveProcessorState();

    const juce::ValueTree& getInactiveProcessorState() const;
    void setInactiveProcessorState(const juce::ValueTree& newState);

private:
    juce::AudioProcessorValueTreeState& parameters;
    juce::ValueTree& applicationState;
    juce::ValueTree inactiveParameters;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ProcessorABStateManager)
};
