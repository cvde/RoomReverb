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

#include "CustomLookAndFeel.h"
#include <BinaryData.h>

namespace juce
{
namespace LookAndFeelHelpers
{
    static juce::TextLayout layoutTooltipText (const juce::String& text, juce::Colour colour) noexcept
    {
        const float tooltipFontSize = 20.0f; // modified
        const int maxToolTipWidth = 400;

        juce::AttributedString s;
        s.setJustification (juce::Justification::centred);
        s.append (text, juce::Font (tooltipFontSize, juce::Font::bold), colour);

        juce::TextLayout tl;
        tl.createLayoutWithBalancedLineLengths (s, (float) maxToolTipWidth);
        return tl;
    }
}
}

CustomLookAndFeel::CustomLookAndFeel()
{
    defaultFont = juce::Typeface::createSystemTypefaceFor(BinaryData::OpenSansCondensedBold_ttf, BinaryData::OpenSansCondensedBold_ttfSize);
    LookAndFeel::getDefaultLookAndFeel().setDefaultSansSerifTypeface(defaultFont.getTypefacePtr());

    setColour(juce::ResizableWindow::ColourIds::backgroundColourId, juce::Colour(0xff5e5e5e));
    setColour(juce::DocumentWindow::textColourId, juce::Colours::white);

    setColour(juce::TextButton::buttonColourId, juce::Colour(0xff303030));
    setColour(juce::TextButton::buttonOnColourId, juce::Colour(0xff5e5e5e));

    setColour(juce::ComboBox::backgroundColourId, juce::Colour(0xff303030));
    setColour(juce::PopupMenu::backgroundColourId, juce::Colour(0xff5e5e5e));
    setColour(juce::PopupMenu::highlightedBackgroundColourId, juce::Colour(0xff303030));

    setColour(juce::Slider::thumbColourId, juce::Colour(0xffdc0000));
    setColour(juce::Slider::trackColourId, juce::Colour(0xffdc0000));
    setColour(juce::Slider::backgroundColourId, juce::Colour(0xffc6c6c6));

    setColour(juce::TooltipWindow::backgroundColourId, juce::Colour(0xff5e5e5e));
    setColour(juce::TooltipWindow::outlineColourId, juce::Colour(0xffc6c6c6));

    setColour(juce::TextEditor::focusedOutlineColourId, juce::Colour(0xffc6c6c6));
    setColour(juce::TextEditor::backgroundColourId, juce::Colour(0xff5e5e5e));
}

// change size of slider textbox
juce::Slider::SliderLayout CustomLookAndFeel::getSliderLayout (juce::Slider& slider)
{
    // 1. compute the actually visible textBox size from the slider textBox size and some additional constraints
    auto bounds = slider.getLocalBounds();
    auto textBoxWidth  = juce::jmax (slider.getTextBoxWidth(),  (int) (bounds.getWidth() * 0.1f));
    auto textBoxHeight = juce::jmax (slider.getTextBoxHeight(), (int) (bounds.getHeight() * 0.8f));
    juce::Slider::SliderLayout layout;

    // 2. set the textBox bounds
    layout.textBoxBounds.setWidth (textBoxWidth);
    layout.textBoxBounds.setHeight (textBoxHeight);
    layout.textBoxBounds.setX (bounds.getWidth() - textBoxWidth);
    layout.textBoxBounds.setY ((bounds.getHeight() - textBoxHeight) / 2);

    // 3. set the slider bounds
    layout.sliderBounds = bounds;
    layout.sliderBounds.removeFromRight (textBoxWidth);
    const int thumbIndent = getSliderThumbRadius (slider);
    layout.sliderBounds.reduce (thumbIndent, 0);

    return layout;
}

// change slider track and thumb appearance
void CustomLookAndFeel::drawLinearSlider (juce::Graphics& g, int x, int y, int width, int height,
                                       float sliderPos,
                                       float minSliderPos,
                                       float maxSliderPos,
                                       const juce::Slider::SliderStyle style, juce::Slider& slider)
{
    juce::ignoreUnused(style);
    juce::ignoreUnused(minSliderPos);
    juce::ignoreUnused(maxSliderPos);
    auto trackWidth = juce::jmin (2.0f, (float) height * 0.25f);

    juce::Point<float> startPoint((float) x, (float) y + (float) height * 0.5f);
    juce::Point<float> endPoint((float) (width + x), startPoint.y);

    juce::Path backgroundTrack;
    backgroundTrack.startNewSubPath(startPoint);
    backgroundTrack.lineTo(endPoint);
    g.setColour(slider.findColour(juce::Slider::backgroundColourId));
    g.strokePath(backgroundTrack, { trackWidth, juce::PathStrokeType::curved, juce::PathStrokeType::rounded });

    juce::Path valueTrack;
    juce::Point<float> minPoint, maxPoint, thumbPoint;

    auto kx = sliderPos;
    auto ky = ((float) y + (float) height * 0.5f);

    minPoint = startPoint;
    maxPoint = { kx, ky };

    auto thumbWidth = juce::jmin(15, static_cast<int> ((float) slider.getHeight() * 0.5f));

    valueTrack.startNewSubPath(minPoint);
    valueTrack.lineTo(maxPoint);
    g.setColour(slider.findColour(juce::Slider::trackColourId));
    g.strokePath(valueTrack, { trackWidth, juce::PathStrokeType::curved, juce::PathStrokeType::rounded });

    g.setColour (slider.findColour (juce::Slider::thumbColourId));
    g.fillEllipse (juce::Rectangle<float> (static_cast<float> (thumbWidth), static_cast<float> (thumbWidth)).withCentre(maxPoint));
}

// change slider textbox font size
juce::Label* CustomLookAndFeel::createSliderTextBox (juce::Slider& slider)
{
    auto* l = LookAndFeel_V2::createSliderTextBox (slider);
    l->setFont(juce::Font(20.0f)); // modified

    if (getCurrentColourScheme() == LookAndFeel_V4::getGreyColourScheme() && (slider.getSliderStyle() == juce::Slider::LinearBar
                                                                               || slider.getSliderStyle() == juce::Slider::LinearBarVertical))
    {
        l->setColour (juce::Label::textColourId, juce::Colours::black.withAlpha (0.7f));
    }

    return l;
}

// change slider textbox border to rounded corners
void CustomLookAndFeel::drawLabel (juce::Graphics& g, juce::Label& label)
{
    auto cornerSize = 10.0f;
    auto bounds = label.getLocalBounds().toFloat().reduced (0.5f, 0.5f);;

    g.setColour(label.findColour (juce::Label::backgroundColourId));
    g.fillRoundedRectangle (bounds.toFloat(), cornerSize);

    if (! label.isBeingEdited())
    {
        auto alpha = label.isEnabled() ? 1.0f : 0.5f;
        const juce::Font font (getLabelFont (label));

        g.setColour (label.findColour (juce::Label::textColourId).withMultipliedAlpha (alpha));
        g.setFont (font);

        auto textArea = getLabelBorderSize (label).subtractedFrom (label.getLocalBounds());

        g.drawFittedText (label.getText(), textArea, label.getJustificationType(),
                          juce::jmax (1, (int) ((float) textArea.getHeight() / font.getHeight())),
                          label.getMinimumHorizontalScale());

        g.setColour (label.findColour (juce::Label::outlineColourId).withMultipliedAlpha (alpha));
    }
    else if (label.isEnabled())
    {
        g.setColour (label.findColour (juce::Label::outlineColourId));
    }

    g.drawRoundedRectangle(bounds, cornerSize, 1.0f);
}

// change button border to rounded corners and remove hover effect
void CustomLookAndFeel::drawButtonBackground (juce::Graphics& g,
                                           juce::Button& button,
                                           const juce::Colour& backgroundColour,
                                           bool shouldDrawButtonAsHighlighted,
                                           bool shouldDrawButtonAsDown)
{
    auto cornerSize = 10.0f; // modified
    auto bounds = button.getLocalBounds().toFloat().reduced (0.5f, 0.5f);

    auto baseColour = backgroundColour.withMultipliedSaturation (button.hasKeyboardFocus (true) ? 1.3f : 0.9f)
                                      .withMultipliedAlpha (button.isEnabled() ? 1.0f : 0.5f);

    juce::ignoreUnused(shouldDrawButtonAsHighlighted);
    if (shouldDrawButtonAsDown) // modified
        baseColour = baseColour.contrasting (shouldDrawButtonAsDown ? 0.2f : 0.05f);

    g.setColour (baseColour);

    auto flatOnLeft   = button.isConnectedOnLeft();
    auto flatOnRight  = button.isConnectedOnRight();
    auto flatOnTop    = button.isConnectedOnTop();
    auto flatOnBottom = button.isConnectedOnBottom();

    if (flatOnLeft || flatOnRight || flatOnTop || flatOnBottom)
    {
        juce::Path path;
        path.addRoundedRectangle (bounds.getX(), bounds.getY(),
                                  bounds.getWidth(), bounds.getHeight(),
                                  cornerSize, cornerSize,
                                  ! (flatOnLeft  || flatOnTop),
                                  ! (flatOnRight || flatOnTop),
                                  ! (flatOnLeft  || flatOnBottom),
                                  ! (flatOnRight || flatOnBottom));

        g.fillPath (path);

        g.setColour (button.findColour (juce::ComboBox::outlineColourId));
        g.strokePath (path, juce::PathStrokeType (1.0f));
    }
    else
    {
        g.fillRoundedRectangle (bounds, cornerSize);

        g.setColour (button.findColour (juce::ComboBox::outlineColourId));
        g.drawRoundedRectangle (bounds, cornerSize, 1.0f);
    }
}

// change botton font size
juce::Font CustomLookAndFeel::getTextButtonFont(juce::TextButton&, int buttonHeight)
{
    return { juce::jmin (20.0f, (float) buttonHeight * 0.6f) };
}

// change combobox border to rounded corners
void CustomLookAndFeel::drawComboBox (juce::Graphics& g, int width, int height, bool,
                                   int, int, int, int, juce::ComboBox& box)
{
    auto cornerSize = 10.0f; // modified
    juce::Rectangle<int> boxBounds (0, 0, width, height);

    g.setColour (box.findColour (juce::ComboBox::backgroundColourId));
    g.fillRoundedRectangle (boxBounds.toFloat(), cornerSize);

    g.setColour (box.findColour (juce::ComboBox::outlineColourId));
    g.drawRoundedRectangle (boxBounds.toFloat().reduced (0.5f, 0.5f), cornerSize, 1.0f);

    juce::Rectangle<int> arrowZone (width - 30, 0, 20, height);
    juce::Path path;
    path.startNewSubPath ((float) arrowZone.getX() + 3.0f, (float) arrowZone.getCentreY() - 2.0f);
    path.lineTo ((float) arrowZone.getCentreX(), (float) arrowZone.getCentreY() + 3.0f);
    path.lineTo ((float) arrowZone.getRight() - 3.0f, (float) arrowZone.getCentreY() - 2.0f);

    g.setColour (box.findColour (juce::ComboBox::arrowColourId).withAlpha ((box.isEnabled() ? 0.9f : 0.2f)));
    g.strokePath (path, juce::PathStrokeType (2.0f));
}

// change combobox font size
juce::Font CustomLookAndFeel::getComboBoxFont(juce::ComboBox& box)
{
    return { juce::jmin (20.0f, (float) box.getHeight() * 0.85f) };
}

// change font size of unfolded comboboxes
juce::Font CustomLookAndFeel::getPopupMenuFont()
{
    return { 20.0f };
}

// change opaque to false to enable rounded corners on unfolded comboboxes
void CustomLookAndFeel::preparePopupMenuWindow (juce::Component& component)
{
    component.setOpaque(false);
}

// change unfolded comboboxes to rounded corners
void CustomLookAndFeel::drawPopupMenuBackground (juce::Graphics& g, int width, int height)
{
    juce::Rectangle<int> bounds (width, height);
    auto cornerSize = 10.0f;

    g.setColour(findColour(juce::PopupMenu::backgroundColourId));
    g.fillRoundedRectangle(bounds.toFloat(), cornerSize);

    g.setColour (findColour (juce::PopupMenu::textColourId).withAlpha (0.6f));
    g.drawRoundedRectangle (bounds.toFloat().reduced (0.5f, 0.5f), cornerSize, 1.0f);
}

// hide border of unfolded comboboxes
int CustomLookAndFeel::getPopupMenuBorderSize()
{
    return 0;
}

// also the up and down arrow section needs rounded corners
void CustomLookAndFeel::drawPopupMenuUpDownArrow (juce::Graphics& g, int width, int height, bool isScrollUpArrow)
{
    auto background = findColour (juce::PopupMenu::backgroundColourId);

    g.setGradientFill (juce::ColourGradient (background, 0.0f, (float) height * 0.5f,
                                       background.withAlpha (0.0f),
                                       0.0f, isScrollUpArrow ? ((float) height) : 0.0f,
                                       false));

    auto cornerSize = 10.0f;
    g.fillRoundedRectangle(1, 1, width - 2, height - 2, cornerSize);

    auto hw = (float) width * 0.5f;
    auto arrowW = (float) height * 0.3f;
    auto y1 = (float) height * (isScrollUpArrow ? 0.6f : 0.3f);
    auto y2 = (float) height * (isScrollUpArrow ? 0.3f : 0.6f);

    juce::Path p;
    p.addTriangle (hw - arrowW, y1,
                   hw + arrowW, y1,
                   hw, y2);

    g.setColour (findColour (juce::PopupMenu::textColourId).withAlpha (0.5f));
    g.fillPath (p);
}

// change highlighting of elements in unfolded comboboxes
void CustomLookAndFeel::drawPopupMenuItem (juce::Graphics& g, const juce::Rectangle<int>& area,
                                        const bool isSeparator, const bool isActive,
                                        const bool isHighlighted, const bool isTicked,
                                        const bool hasSubMenu, const juce::String& text,
                                        const juce::String& shortcutKeyText,
                                        const juce::Drawable* icon, const juce::Colour* const textColourToUse)
{
    if (isSeparator)
    {
        auto r  = area.reduced (5, 0);
        r.removeFromTop (juce::roundToInt (((float) r.getHeight() * 0.5f) - 0.5f));

        g.setColour (findColour (juce::PopupMenu::textColourId).withAlpha (0.3f));
        g.fillRect (r.removeFromTop (1));
    }
    else
    {
        auto textColour = (textColourToUse == nullptr ? findColour (juce::PopupMenu::textColourId)
                                                      : *textColourToUse);

        auto r  = area.reduced (1);

        if (isHighlighted && isActive)
        {
            g.setColour (findColour (juce::PopupMenu::highlightedBackgroundColourId));
            auto cornerSize = 10.0f; // modified
            g.fillRoundedRectangle(r.toFloat(), cornerSize); // modified

            g.setColour (findColour (juce::PopupMenu::highlightedTextColourId));
        }
        else
        {
            g.setColour (textColour.withMultipliedAlpha (isActive ? 1.0f : 0.5f));
        }

        r.reduce (juce::jmin (5, area.getWidth() / 20), 0);

        auto font = getPopupMenuFont();

        auto maxFontHeight = (float) r.getHeight() / 1.3f;

        if (font.getHeight() > maxFontHeight)
            font.setHeight (maxFontHeight);

        g.setFont (font);

        auto iconArea = r.removeFromLeft (juce::roundToInt (maxFontHeight)).toFloat();

        if (icon != nullptr)
        {
            icon->drawWithin (g, iconArea, juce::RectanglePlacement::centred | juce::RectanglePlacement::onlyReduceInSize, 1.0f);
            r.removeFromLeft (juce::roundToInt (maxFontHeight * 0.5f));
        }
        else if (isTicked)
        {
            auto tick = getTickShape (1.0f);
            g.fillPath (tick, tick.getTransformToScaleToFit (iconArea.reduced (iconArea.getWidth() / 5, 0).toFloat(), true));
        }

        if (hasSubMenu)
        {
            auto arrowH = 0.6f * getPopupMenuFont().getAscent();

            auto x = static_cast<float> (r.removeFromRight ((int) arrowH).getX());
            auto halfH = static_cast<float> (r.getCentreY());

            juce::Path path;
            path.startNewSubPath (x, halfH - arrowH * 0.5f);
            path.lineTo (x + arrowH * 0.6f, halfH);
            path.lineTo (x, halfH + arrowH * 0.5f);

            g.strokePath (path, juce::PathStrokeType (2.0f));
        }

        r.removeFromRight (3);
        g.drawFittedText (text, r, juce::Justification::centredLeft, 1);

        if (shortcutKeyText.isNotEmpty())
        {
            auto f2 = font;
            f2.setHeight (f2.getHeight() * 0.75f);
            f2.setHorizontalScale (0.95f);
            g.setFont (f2);

            g.drawText (shortcutKeyText, r, juce::Justification::centredRight, true);
        }
    }
}

// change size of tooltips
juce::Rectangle<int> CustomLookAndFeel::getTooltipBounds (const juce::String& tipText, juce::Point<int> screenPos, juce::Rectangle<int> parentArea)
{
    const juce::TextLayout tl (juce::LookAndFeelHelpers::layoutTooltipText (tipText, juce::Colours::black));

    auto w = (int) (tl.getWidth() + 28.0f); // modified
    auto h = (int) (tl.getHeight() + 12.0f); // modified

    return juce::Rectangle<int> (screenPos.x > parentArea.getCentreX() ? screenPos.x - (w + 12) : screenPos.x + 24,
                           screenPos.y > parentArea.getCentreY() ? screenPos.y - (h + 6)  : screenPos.y + 6,
                           w, h)
             .constrainedWithin (parentArea);
}

// change tooltip border to rounded corners
void CustomLookAndFeel::drawTooltip (juce::Graphics& g, const juce::String& text, int width, int height)
{
    juce::Rectangle<int> bounds (width, height);
    auto cornerSize = 10.0f; // modified

    g.setColour (findColour (juce::TooltipWindow::backgroundColourId));
    g.fillRoundedRectangle (bounds.toFloat(), cornerSize);

    g.setColour (findColour (juce::TooltipWindow::outlineColourId));
    g.drawRoundedRectangle (bounds.toFloat().reduced (0.5f, 0.5f), cornerSize, 1.0f);

    juce::LookAndFeelHelpers::layoutTooltipText (text, findColour (juce::TooltipWindow::textColourId))
                       .draw (g, { static_cast<float> (width), static_cast<float> (height) });
}

// change texteditor background
void CustomLookAndFeel::fillTextEditorBackground (juce::Graphics& g, int width, int height, juce::TextEditor& textEditor)
{
    juce::ignoreUnused(textEditor);

    juce::Rectangle<int> bounds (width, height);
    auto cornerSize = 10.0f;

    g.setColour(findColour(juce::TextEditor::backgroundColourId));
    g.fillRoundedRectangle(bounds.toFloat(), cornerSize);
}

// change texteditor to rounded corners
void CustomLookAndFeel::drawTextEditorOutline (juce::Graphics& g, int width, int height, juce::TextEditor& textEditor)
{
    juce::Rectangle<int> bounds (width, height); // modified
    auto cornerSize = 10.0f; // modified

    if (dynamic_cast<juce::AlertWindow*> (textEditor.getParentComponent()) == nullptr)
    {
        if (textEditor.isEnabled())
        {
            if (textEditor.hasKeyboardFocus (true) && ! textEditor.isReadOnly())
            {
                g.setColour (textEditor.findColour (juce::TextEditor::focusedOutlineColourId));
                g.drawRoundedRectangle(bounds.toFloat().reduced(0.5f, 0.5f), cornerSize, 1);  // modified
            }
            else
            {
                g.setColour (textEditor.findColour (juce::TextEditor::outlineColourId));
                g.drawRect (0, 0, width, height);
            }
        }
    }
}
