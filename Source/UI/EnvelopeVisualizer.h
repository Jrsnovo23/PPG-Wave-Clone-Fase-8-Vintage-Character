#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>

/**
    Dibuja la forma ADSR (Attack/Decay/Sustain/Release) de una envolvente,
    leyendo en vivo sus 4 parámetros del APVTS — pedido en la sección 18
    del documento de diseño ("Visualización gráfica ADSR").

    Los segmentos horizontales son proporciones fijas del ancho (no una
    escala de tiempo real) — así una envolvente con Attack de 1ms y Release
    de 2s se ve siempre como una forma clara y legible, en vez de que un
    segmento desaparezca comprimido junto al otro. Solo la altura del
    tramo de Sustain refleja su valor real.
*/
class EnvelopeVisualizer : public juce::Component,
                            private juce::Timer
{
public:
    EnvelopeVisualizer (juce::AudioProcessorValueTreeState& apvtsIn,
                         juce::String attackParamID, juce::String decayParamID,
                         juce::String sustainParamID, juce::String releaseParamID);
    ~EnvelopeVisualizer() override;

    void paint (juce::Graphics&) override;

private:
    void timerCallback() override;

    juce::AudioProcessorValueTreeState& apvts;
    const juce::String attackID, decayID, sustainID, releaseID;

    float lastAttack = -1.0f, lastDecay = -1.0f, lastSustain = -1.0f, lastRelease = -1.0f;
};
