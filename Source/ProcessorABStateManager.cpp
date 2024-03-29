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

#include "ProcessorABStateManager.h"

ProcessorABStateManager::ProcessorABStateManager(
    juce::AudioProcessorValueTreeState& parameters,
    juce::ValueTree& applicationState)
        : mParameters(parameters),
          mApplicationState(applicationState)
{
    // A is the initial state of the processor
    mApplicationState.getOrCreateChildWithName("processorABState", nullptr).setProperty("currentState", "A", nullptr);
}

juce::String ProcessorABStateManager::getCurrentProcessorState() const
{
    return mApplicationState.getChildWithName("processorABState").getProperty("currentState").toString();
}

void ProcessorABStateManager::switchProcessorState()
{
    // temporarily store the currently active parameters
    const auto oldState = mParameters.copyState();

    // switch to the inactive parameters
    mParameters.replaceState(mInactiveParameters);
    const juce::var processorABState = (getCurrentProcessorState() == "A") ? "B" : "A";
    mApplicationState.getChildWithName("processorABState").setProperty("currentState", processorABState, nullptr);

    // send a notification to the GUI that the parameters have changed
    // this is not needed for the real (audio) parameters but for the selected preset that is stored in mParameters.state
    mParameters.state.sendPropertyChangeMessage("GuiNeedsUpdate");

    // store the previously active parameters as inactive parameters
    mInactiveParameters = oldState;
}

void ProcessorABStateManager::copyActiveToInactiveProcessorState()
{
    mInactiveParameters = mParameters.copyState();
}

const juce::ValueTree& ProcessorABStateManager::getInactiveProcessorState() const
{
    return mInactiveParameters;
}

void ProcessorABStateManager::setInactiveProcessorState(const juce::ValueTree& newState)
{
    mInactiveParameters = newState;
}
