#include "ToolWindow.h"

PulpLookAndFeel::PulpLookAndFeel() {
    using namespace juce;
    setDefaultSansSerifTypeface(theme::face(0));
    setColour(ResizableWindow::backgroundColourId, theme::panel);
    setColour(PopupMenu::backgroundColourId, theme::panel);
    setColour(PopupMenu::textColourId, theme::text);
    setColour(PopupMenu::headerTextColourId, theme::accent);
    setColour(PopupMenu::highlightedBackgroundColourId, theme::accent.withAlpha(0.18f));
    setColour(PopupMenu::highlightedTextColourId, theme::accent.brighter(0.3f));
    setColour(AlertWindow::backgroundColourId, theme::panel);
    setColour(AlertWindow::textColourId, theme::text);
    setColour(AlertWindow::outlineColourId, theme::line);
    setColour(TextEditor::backgroundColourId, theme::inset);
    setColour(TextEditor::textColourId, theme::text);
    setColour(TextEditor::outlineColourId, theme::line);
    setColour(TextEditor::focusedOutlineColourId, theme::accent);
    setColour(TextEditor::highlightColourId, theme::accent.withAlpha(0.4f));
    setColour(CaretComponent::caretColourId, theme::accent);
    setColour(TextButton::buttonColourId, theme::panelHi);
    setColour(TextButton::buttonOnColourId, theme::accent);
    setColour(TextButton::textColourOffId, theme::text);
    setColour(TextButton::textColourOnId, Colours::black);
    setColour(ComboBox::backgroundColourId, theme::inset);
    setColour(ComboBox::textColourId, theme::text);
    setColour(ComboBox::outlineColourId, theme::line);
    setColour(ComboBox::arrowColourId, theme::textDim);
    setColour(Label::textColourId, theme::text);
    setColour(ScrollBar::thumbColourId, theme::panelHi.brighter(0.2f));
    setColour(ResizableWindow::backgroundColourId, theme::panel);
    setColour(TreeView::backgroundColourId, theme::panelLo);
    setColour(TreeView::linesColourId, theme::line);
    setColour(ListBox::backgroundColourId, theme::panelLo);
    setColour(TooltipWindow::backgroundColourId, theme::panel);
    setColour(TooltipWindow::textColourId, theme::text);
}

void PulpLookAndFeel::drawScrollbar(juce::Graphics& g, juce::ScrollBar&, int x, int y, int w, int h, bool vertical,
                                    int thumbStart, int thumbSize, bool over, bool down) {
    g.setColour(theme::panelLo);
    g.fillRect(x, y, w, h);
    if (thumbSize <= 0) return;
    auto r = vertical ? juce::Rectangle<int>(x + 3, thumbStart + 1, w - 6, thumbSize - 2) : juce::Rectangle<int>(thumbStart + 1, y + 3, thumbSize - 2, h - 6);
    g.setColour(down ? theme::accent.withAlpha(0.7f) : theme::text.withAlpha(over ? 0.28f : 0.14f));
    g.fillRoundedRectangle(r.toFloat(), juce::jmin(r.getWidth(), r.getHeight()) * 0.5f);
}

ToolWindow::ToolWindow(const juce::String& title, juce::Component* content, bool ownsContent)
    : title_(title), content_(content), owns_(ownsContent) {
    setOpaque(true);
    addAndMakeVisible(content_);
    limits_.setMinimumSize(220, 120);
    limits_.setMinimumOnscreenAmounts(kTitle, 60, 0, 60);
    border_ = std::make_unique<juce::ResizableBorderComponent>(this, &limits_);
    border_->setBorderThickness(juce::BorderSize<int>(4));
    addAndMakeVisible(*border_);
    setWantsKeyboardFocus(true);
    setMouseClickGrabsKeyboardFocus(true);
}

ToolWindow::~ToolWindow() {
    removeChildComponent(content_);
    if (owns_) delete content_;
}

void ToolWindow::resized() {
    border_->setBounds(getLocalBounds());
    content_->setBounds(getLocalBounds().reduced(4).withTrimmedTop(kTitle - 4));
}

void ToolWindow::paint(juce::Graphics& g) {
    g.fillAll(theme::panel);
    auto bar = getLocalBounds().removeFromTop(kTitle);
    const bool front = hasKeyboardFocus(true);
    const auto top = front ? theme::panelHi.brighter(0.05f) : theme::panelHi;
    g.setGradientFill(juce::ColourGradient(top, 0.0f, 0.0f, theme::panel, 0.0f, (float)kTitle, false));
    g.fillRect(bar);
    g.setColour(theme::sheen);
    g.drawHorizontalLine(1, 1.0f, (float)getWidth() - 1.0f);
    // a gold hairline under the title of the window that has the focus
    g.setColour(front ? theme::accent.withAlpha(0.55f) : theme::line);
    g.drawHorizontalLine(kTitle - 1, 0.0f, (float)getWidth());
    // status jewel
    auto jewel = juce::Rectangle<float>(6.0f, 6.0f).withCentre({11.0f, kTitle * 0.5f});
    if (front) { g.setColour(theme::accent.withAlpha(0.25f)); g.fillEllipse(jewel.expanded(3.0f)); }
    g.setColour(front ? theme::accent : theme::textDim.withAlpha(0.6f));
    g.fillEllipse(jewel);
    g.setColour(front ? theme::text : theme::textDim);
    g.setFont(theme::font(12.5f, true));
    g.drawText(title_, bar.withTrimmedLeft(22).withTrimmedRight(46), juce::Justification::centredLeft, true);
    // detach / dock and close boxes
    auto cb = closeBox().toFloat(), db = detachBox().toFloat();
    if (hover_ == 1) { g.setColour(theme::bad.withAlpha(0.18f)); g.fillRoundedRectangle(cb, 4.0f); }
    if (hover_ == 2) { g.setColour(theme::accent.withAlpha(0.14f)); g.fillRoundedRectangle(db, 4.0f); }
    g.setColour(hover_ == 1 ? theme::bad : theme::textDim);
    g.drawLine(cb.getX() + 4.5f, cb.getY() + 4.5f, cb.getRight() - 4.5f, cb.getBottom() - 4.5f, 1.4f);
    g.drawLine(cb.getRight() - 4.5f, cb.getY() + 4.5f, cb.getX() + 4.5f, cb.getBottom() - 4.5f, 1.4f);
    g.setColour(hover_ == 2 ? theme::accent : theme::textDim);
    if (isDetached()) g.drawRoundedRectangle(db.reduced(3.5f), 1.5f, 1.3f);
    else { g.drawRoundedRectangle(db.reduced(4.0f).translated(1.5f, -1.5f), 1.5f, 1.1f); g.drawRoundedRectangle(db.reduced(4.0f).translated(-1.5f, 1.5f), 1.5f, 1.1f); }
    g.setColour(front ? theme::accent.withAlpha(0.22f) : juce::Colours::black.withAlpha(0.7f));
    g.drawRect(getLocalBounds(), 1);
}

void ToolWindow::mouseMove(const juce::MouseEvent& e) {
    const int h = closeBox().contains(e.getPosition()) ? 1 : (detachBox().contains(e.getPosition()) ? 2 : 0);
    if (h != hover_) { hover_ = h; repaint(0, 0, getWidth(), kTitle); }
}

void ToolWindow::mouseDown(const juce::MouseEvent& e) {
    toFront(true);
    draggingTitle_ = false;
    if (closeBox().contains(e.getPosition())) { setVisible(false); return; }
    if (detachBox().contains(e.getPosition())) { setDetached(!isDetached()); return; }
    if (e.y < kTitle) { draggingTitle_ = true; dragger_.startDraggingComponent(this, e); }
}

void ToolWindow::mouseDrag(const juce::MouseEvent& e) {
    if (draggingTitle_) dragger_.dragComponent(this, e, isDetached() ? nullptr : &limits_);
}

void ToolWindow::mouseUp(const juce::MouseEvent&) { draggingTitle_ = false; }

void ToolWindow::mouseDoubleClick(const juce::MouseEvent& e) {
    if (e.y >= kTitle || isDetached() || !getParentComponent()) return;
    // double-click the title: fill the workspace, or go back to the previous size
    auto full = getParentComponent()->getLocalBounds();
    if (auto* p = getParentComponent()) {
        full = p->getLocalBounds();
        if (auto v = p->getProperties()["workTop"]; !v.isVoid()) full.removeFromTop((int)v);
        if (auto v = p->getProperties()["workLeft"]; !v.isVoid()) full.removeFromLeft((int)v);
    }
    if (getBounds() == full) setBounds(homeBounds_.isEmpty() ? full.reduced(60) : homeBounds_);
    else { homeBounds_ = getBounds(); setBounds(full); }
}

void ToolWindow::setDetached(bool detached) {
    if (detached == isDetached()) return;
    const bool vis = isVisible();
    if (detached) {
        home_ = getParentComponent();
        homeBounds_ = getBounds();
        const auto screenPos = getScreenPosition();
        addToDesktop(juce::ComponentPeer::windowAppearsOnTaskbar | juce::ComponentPeer::windowHasDropShadow);
        setTopLeftPosition(screenPos);
    } else {
        removeFromDesktop();
        if (home_) { home_->addChildComponent(this); setBounds(homeBounds_); }
    }
    setVisible(vis);
    toFront(true);
    repaint();
}

void ToolWindow::showAndRaise() {
    setVisible(true);
    toFront(true);
    grabKeyboardFocus();
}

void ToolWindow::toggle() {
    if (isVisible() && hasKeyboardFocus(true)) { setVisible(false); if (home_) home_->grabKeyboardFocus(); else if (auto* p = getParentComponent()) p->grabKeyboardFocus(); }
    else showAndRaise();
}
