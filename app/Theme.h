#pragma once
// Colours, fonts and small drawing helpers.
// NoDAW edition look: obsidian surfaces, hairline edges, champagne-gold accent, Inter type.
#include <juce_gui_basics/juce_gui_basics.h>
#include "BinaryData.h"

namespace theme {
using juce::Colour;

const Colour bg{0xff0a0b0e};          // workspace behind the windows
const Colour panel{0xff15171c};       // window and toolbar body
const Colour panelHi{0xff1f2229};     // raised parts
const Colour panelLo{0xff101216};     // recessed parts
const Colour inset{0xff08090c};       // displays, grids
const Colour line{0xff24272e};        // hairline edges
const Colour text{0xffefeae2};        // warm ivory
const Colour textDim{0xff8a8f98};
const Colour accent{0xffd9b878};      // champagne gold
const Colour accent2{0xff7fd6cb};     // soft aqua
const Colour good{0xff8fd18a};
const Colour warn{0xffe9c46a};
const Colour bad{0xffe5675f};
const Colour stepOff{0xff23262d};
const Colour stepOffAlt{0xff2c2822};
const Colour stepOn{0xffefe6d4};
const Colour sheen{0x12ffffff};       // top-edge highlight on raised parts

inline juce::Typeface::Ptr face(int which) {
    static juce::Typeface::Ptr regular = juce::Typeface::createSystemTypefaceFor(BinaryData::InterRegular_ttf, BinaryData::InterRegular_ttfSize);
    static juce::Typeface::Ptr semibold = juce::Typeface::createSystemTypefaceFor(BinaryData::InterSemiBold_ttf, BinaryData::InterSemiBold_ttfSize);
    static juce::Typeface::Ptr monoFace = juce::Typeface::createSystemTypefaceFor(BinaryData::JetBrainsMonoMedium_ttf, BinaryData::JetBrainsMonoMedium_ttfSize);
    return which == 1 ? semibold : (which == 2 ? monoFace : regular);
}

// Inter sits a little larger than Segoe UI at the same point size; 0.94 keeps the old layouts fitting.
inline juce::Font font(float size, bool bold = false) {
    return juce::Font(juce::FontOptions(face(bold ? 1 : 0)).withHeight(size * 0.94f));
}
inline juce::Font mono(float size) {
    return juce::Font(juce::FontOptions(face(2)).withHeight(size * 0.92f));
}

// A raised surface: soft vertical gradient, a faint top sheen and a hairline edge.
inline void bevel(juce::Graphics& g, juce::Rectangle<float> r, Colour fill, float radius = 4.0f) {
    g.setGradientFill(juce::ColourGradient(fill.brighter(0.07f), r.getX(), r.getY(), fill.darker(0.10f), r.getX(), r.getBottom(), false));
    g.fillRoundedRectangle(r, radius);
    g.setColour(sheen);
    g.drawHorizontalLine((int)r.getY() + 1, r.getX() + radius, r.getRight() - radius);
    g.setColour(juce::Colours::black.withAlpha(0.55f));
    g.drawRoundedRectangle(r.reduced(0.5f), radius, 1.0f);
}

// A recessed display: near-black well with an inner top shadow and a faint lit lower edge.
inline void insetBox(juce::Graphics& g, juce::Rectangle<float> r, float radius = 4.0f) {
    g.setColour(inset);
    g.fillRoundedRectangle(r, radius);
    g.setGradientFill(juce::ColourGradient(juce::Colours::black.withAlpha(0.45f), r.getX(), r.getY(), juce::Colours::transparentBlack, r.getX(), r.getY() + 5.0f, false));
    g.fillRoundedRectangle(r.withHeight(juce::jmin(6.0f, r.getHeight())), radius);
    g.setColour(juce::Colours::white.withAlpha(0.06f));
    g.drawRoundedRectangle(r.reduced(0.5f), radius, 1.0f);
}

// A subtle glow behind a lit element (accent arcs, active buttons).
inline void glow(juce::Graphics& g, const juce::Path& p, Colour c, float width) {
    g.setColour(c.withAlpha(0.16f));
    g.strokePath(p, juce::PathStrokeType(width * 3.2f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
    g.setColour(c.withAlpha(0.28f));
    g.strokePath(p, juce::PathStrokeType(width * 1.8f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
}

// Sixteen channel / clip colours: muted jewel tones that sit well on obsidian.
inline Colour palette(int i) {
    static const juce::uint32 c[16] = {
        0xff7d9bb5, 0xffc98d6e, 0xff8fb487, 0xffc9ab72, 0xff9c8cc4, 0xff6fb8ad, 0xffc283a0, 0xffa7acb3,
        0xff5d8fbf, 0xffd9a15a, 0xff6daa69, 0xffd6bf63, 0xff8673c9, 0xff55b8b0, 0xffcf6b8c, 0xff8a939d};
    return Colour(c[((i % 16) + 16) % 16]);
}

} // namespace theme
