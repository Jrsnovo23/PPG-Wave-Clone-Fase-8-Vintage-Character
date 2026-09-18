#include "EnvelopeVisualizer.h"
#include "PPGLookAndFeel.h"

EnvelopeVisualizer::EnvelopeVisualizer (juce::AudioProcessorValueTreeState& apvtsIn,
                                         juce::String attackParamID, juce::String decayParamID,
                                         juce::String sustainParamID, juce::String releaseParamID)
    : apvts (apvtsIn),
      attackID (std::move (attackParamID)), decayID (std::move (decayParamID)),
      sustainID (std::move (sustainParamID)), releaseID (std::move (releaseParamID))
{
    startTimerHz (15);
}

EnvelopeVisualizer::~EnvelopeVisualizer()
{
    stopTimer();
}

void EnvelopeVisualizer::timerCallback()
{
    const float a = apvts.getRawParameterValue (attackID)->load();
    const float d = apvts.getRawParameterValue (decayID)->load();
    const float s = apvts.getRawParameterValue (sustainID)->load();
    const float r = apvts.getRawParameterValue (releaseID)->load();

    if (a != lastAttack || d != lastDecay || s != lastSustain || r != lastRelease)
    {
        lastAttack = a; lastDecay = d; lastSustain = s; lastRelease = r;
        repaint();
    }
}

void EnvelopeVisualizer::paint (juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat().reduced (1.0f);
    g.setColour (PPGLookAndFeel::panelBackground.darker (0.15f));
    g.fillRoundedRectangle (bounds, 3.0f);

    const float sustain = juce::jlimit (0.0f, 1.0f, apvts.getRawParameterValue (sustainID)->load());

    const float w = bounds.getWidth();
    const float baseY = bounds.getBottom() - 3.0f;
    const float topY = bounds.getY() + 3.0f;
    const float sustainY = juce::jmap (sustain, 0.0f, 1.0f, baseY, topY);

    const float attackW  = w * 0.22f;
    const float decayW   = w * 0.20f;
    const float sustainW = w * 0.28f;
    const float releaseW = w * 0.30f;

    juce::Path path;
    float x = bounds.getX();
    path.startNewSubPath (x, baseY);
    x += attackW;
    path.lineTo (x, topY);
    x += decayW;
    path.lineTo (x, sustainY);
    x += sustainW;
    path.lineTo (x, sustainY);
    x += releaseW;
    path.lineTo (x, baseY);

    g.setColour (PPGLookAndFeel::accent);
    g.strokePath (path, juce::PathStrokeType (1.6f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
}
