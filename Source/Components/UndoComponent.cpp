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

#include "UndoComponent.h"
#include <BinaryData.h>

UndoComponent::UndoComponent(ReverbAudioProcessor& audioProcessor)
        : mParameters(audioProcessor.getParameters())
{
    mUndoButton.setImages(juce::Drawable::createFromImageData(BinaryData::undo_svg, BinaryData::undo_svgSize).get());
    mUndoButton.onClick = [this] { mParameters.undoManager->undo(); };
    addAndMakeVisible(mUndoButton);

    mRedoButton.setImages(juce::Drawable::createFromImageData(BinaryData::redo_svg, BinaryData::redo_svgSize).get());
    mRedoButton.onClick = [this] { mParameters.undoManager->redo(); };
    addAndMakeVisible(mRedoButton);

    updateButtonState();
    mParameters.undoManager->addChangeListener(this);
}

UndoComponent::~UndoComponent()
{
    mParameters.undoManager->removeChangeListener(this);
}

void UndoComponent::paint(juce::Graphics& g)
{
    juce::ignoreUnused(g);
}

void UndoComponent::resized()
{
    auto area = getLocalBounds();

    mUndoButton.setBounds(area.removeFromLeft(area.getWidth() / 2).reduced(1));
    mRedoButton.setBounds(area.reduced(1));
}

void UndoComponent::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    if (source == mParameters.undoManager)
    {
        updateButtonState();
    }
}

void UndoComponent::updateButtonState()
{
    if (mParameters.undoManager->canUndo())
    {
        mUndoButton.setEnabled(true);
        mUndoButton.setTooltip("Undo last change.");
    }
    else
    {
        mUndoButton.setEnabled(false);
        mUndoButton.setTooltip("");
    }
    if (mParameters.undoManager->canRedo())
    {
        mRedoButton.setEnabled(true);
        mRedoButton.setTooltip("Redo changes which were undone.");
    }
    else
    {
        mRedoButton.setEnabled(false);
        mRedoButton.setTooltip("");
    }
}
