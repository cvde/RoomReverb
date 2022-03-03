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

UndoComponent::UndoComponent(juce::AudioProcessorValueTreeState& parameterTree)
        : parameters(parameterTree)
{
    undoButton.setImages(juce::Drawable::createFromImageData(BinaryData::undo_svg, BinaryData::undo_svgSize).get());
    undoButton.onClick = [this] { parameters.undoManager->undo(); };
    addAndMakeVisible(undoButton);

    redoButton.setImages(juce::Drawable::createFromImageData(BinaryData::redo_svg, BinaryData::redo_svgSize).get());
    redoButton.onClick = [this] { parameters.undoManager->redo(); };
    addAndMakeVisible(redoButton);

    updateButtonState();
    changeListenerCallback(parameters.undoManager);
    parameters.undoManager->addChangeListener(this);
}

UndoComponent::~UndoComponent()
{
    parameters.undoManager->removeChangeListener(this);
}

void UndoComponent::paint(juce::Graphics& g)
{
    juce::ignoreUnused(g);
}

void UndoComponent::resized()
{
    auto area = getLocalBounds();

    undoButton.setBounds(area.removeFromLeft(area.getWidth() / 2).reduced(1));
    redoButton.setBounds(area.reduced(1));
}

void UndoComponent::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    if (source == parameters.undoManager)
        updateButtonState();
}

void UndoComponent::updateButtonState()
{
    if (parameters.undoManager->canUndo())
    {
        undoButton.setEnabled(true);
        undoButton.setTooltip("Undo last change.");
    }
    else
    {
        undoButton.setEnabled(false);
        undoButton.setTooltip("");
    }
    if (parameters.undoManager->canRedo())
    {
        redoButton.setEnabled(true);
        redoButton.setTooltip("Redo changes which were undone.");
    }
    else
    {
        redoButton.setEnabled(false);
        redoButton.setTooltip("");
    }
}
