# PPG Wave Clone — Fase 8: Vintage Character

Implementación original de un instrumento virtual VST3 de síntesis wavetable
inspirado conceptualmente en el PPG Wave 3.3. No contiene ROMs, muestras,
gráficos ni código propietario de terceros.

## Estado de esta entrega

**Fases 1-7**: listo y confirmado sonando en hardware real (arquitectura,
osciladores wavetable, filtro + envolventes, LFOs + matriz de modulación,
efectos, gestión de presets, interfaz profesional).

**Fase 8** (esta entrega): carácter digital vintage, configurable y opcional.

- `VintageCharacter` (instancia por voz): reducción de bit depth, reducción
  de sample rate (decimación tipo sample & hold), ruido digital, drift de
  pitch (paseo aleatorio suavizado, no saltos), y variación fija por nota
  de filtro/voz (simula tolerancia de componentes entre voces).
- `Oscillator::setAliasingAmount()`: reducción deliberada de la resolución
  de lectura de la wavetable — aliasing como carácter, no como límite
  técnico (complementa el aliasing "de fábrica" por interpolación simple
  que ya existía desde la Fase 2).
- **Control global Vintage/Modern**: todo lo anterior se escala por este
  único knob (0 = Modern, sin carácter añadido; 1 = la cantidad completa
  configurada en cada parámetro individual). Vive junto a Master Volume,
  arriba de todo.
- Por defecto Vintage/Modern está en 0 — el plugin suena exactamente igual
  que en la Fase 7 hasta que subas ese knob a propósito.
- Nuevo panel "VINTAGE CHARACTER" con los 7 controles individuales: Bit
  Depth, Sample Rate Reduce, Noise, Drift, Filter Var, Voice Var, Aliasing.

**Lo que sigue**: Fase 9, pruebas generales (sample rates, buffers,
polifonía máxima, automatización, cambio rápido de presets) — no agrega
funcionalidad nueva, es la fase de validación final.

## Cómo probarlo

Con Vintage/Modern en 0 no deberías notar ningún cambio de sonido respecto
a la Fase 7. Súbelo gradualmente y prueba:
- Toca la misma nota varias veces seguidas — con Drift y Voice Variation
  activos, cada repetición debería sonar *ligeramente* distinta (no
  idéntica), como en hardware analógico/digital real.
- Sube Bit Depth (bájalo en número de bits) y Sample Rate Reduce para un
  carácter tipo "8-bit"/lo-fi evidente.
- Sube Aliasing con un Wave Position en movimiento (LFO → Wave Position,
  como en la Fase 4) para oír cómo el timbre se vuelve más áspero/digital.

## Problemas conocidos

- El drift de pitch se aplica igual a OSC1 y OSC2 (deriva la voz completa
  junta, no cada oscilador por separado) — decisión de diseño para que
  suene a "toda la voz se mueve", no a desafinación entre los dos
  osciladores, que ya tienen su propio control de Fine Tune independiente.
- La variación de filtro y de voz se recalculan una vez por nota (al
  presionarla), no continuamente — es intencional, simula tolerancia fija
  de componentes, no una modulación en vivo.
- Mismo aliasing "de fábrica" en notas agudas de fases anteriores sigue
  presente incluso con Vintage/Modern en 0 (viene de la interpolación
  simple de la Fase 2, no de este módulo) — ver README de esa fase.

## Compilación sin instalar nada (recomendado)

Este proyecto incluye `.github/workflows/build.yml`: compila el VST3 en un
Mac con Xcode ya instalado, en la nube, gratis, vía GitHub Actions. Sube la
carpeta a un repositorio (con GitHub Desktop, para no perder `.github` por
ser una carpeta oculta), espera la palomita verde en la pestaña "Actions", y
descarga el `.vst3` desde "Artifacts".

## Compilación local (requiere CMake + compilador C++20)

1. Clona JUCE dentro de esta carpeta:
   ```
   git clone --depth 1 --branch 7.0.12 https://github.com/juce-framework/JUCE.git
   ```
   Debe quedar como `PPGWaveClone/JUCE/`.

2. Configura y compila:

   **macOS** (Xcode instalado):
   ```
   cmake -B build -G Xcode
   cmake --build build --config Release
   ```
   El `.vst3` queda en `build/PPGWaveClone_artefacts/Release/VST3/`.
   Cópialo a `~/Library/Audio/Plug-Ins/VST3/` (tu usuario) o
   `/Library/Audio/Plug-Ins/VST3/` (todo el sistema — recomendado si Ableton
   corre en otra sesión de usuario). Si lo bajaste de un navegador:
   ```
   xattr -dr com.apple.quarantine "/ruta/al/PPG Wave Clone.vst3"
   ```

   **Windows** (Visual Studio 2022):
   ```
   cmake -B build -G "Visual Studio 17 2022"
   cmake --build build --config Release
   ```
   El `.vst3` queda en `build/PPGWaveClone_artefacts/Release/VST3/`.
   Cópialo a `C:\Program Files\Common Files\VST3\`.

3. Rescanea plugins en Ableton Live (Preferences → Plug-ins → Rescan).

## Estructura

```
PPGWaveClone/
  CMakeLists.txt
  .github/workflows/build.yml   — compila el VST3 en la nube (macOS, x86_64)
  Source/
    PluginProcessor.h/.cpp      — AudioProcessor, APVTS, Synthesiser, efectos, presets, actividad MIDI
    PluginEditor.h/.cpp         — GUI: + panel Vintage Character
    UI/      ...                 (sin cambios de fondo esta fase)
    Params/
      ...
      VintageParameterPointers.h  — nuevo
      SynthVoiceParameters.h      — ahora incluye VintageParameterPointers
    DSP/
      ...
      VintageCharacter.h/.cpp     — nuevo
      Oscillator.h                 — + setAliasingAmount()
    Synth/
      SynthVoice.h/.cpp            — integra VintageCharacter por voz
    Presets/ ...                  (sin cambios de fondo esta fase)
```

## Próxima fase (Fase 9 — Pruebas)

- Verificación en Ableton Live: distintos sample rates (44.1/48/88.2/96
  kHz), distintos tamaños de buffer, polifonía máxima (8 voces
  simultáneas), automatización de parámetros clave, cambio rápido de
  presets sin clicks ni cuelgues.
- No agrega parámetros ni funcionalidad nueva — es la fase de validación
  antes de considerar el instrumento "terminado" según los criterios de
  éxito del documento de diseño original.

Dime cuándo avanzamos a la Fase 9.
