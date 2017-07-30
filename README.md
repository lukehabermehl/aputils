# aputils

A C++ library for easy audio processing.

This library is currently under development and may have broken / missing functionality. Please check the issues tab to see what's in progress.

**libaputils** provides a framework for audio software and audio plugin development. The library aims to be as simple and easy-to-use as possible while supporting powerful functionality.

With the APU framework, an audio plugin can be written in just a few lines. Below is an example unit that reduces the gain of the input signal by 50%.

      class GainUnit
      : public AudioProcessingUnit
      {
      public:
          void processAudio(float *inputBuffer, float *outputBuffer,
                            unsigned numInputChannels, unsigned numOutputChannels)
          {
              outputBuffer[0] = inputBuffer[0] * 0.5;
              if (numOutputChannels == 2) {
                  if (numInputChannels == 2) {
                      outputBuffer[1] = inputBuffer[1] * 0.5;
                  } else {
                      outputBuffer[1] = outputBuffer[0];
                  }
              }
          }
      };
 
 Of course, APUs also support parameters that can be manipulated in real time with just a little more code:
 
      class GainUnit
      : public AudioProcessingUnit
      {
          APUPtr<APUParameter> m_gainParameter; // Reference-counted pointer to an APUParameter instance
      public:
          GainUnit() : AudioProcessingUnit() 
          {
              m_gainParameter = new APUParameter("Gain",
                                                 APUNUM_FLOAT, // number type of the parameter
                                                 APUNUM_FLOAT(0), // minimum value
                                                 APUNUM_FLOAT(1.2), // maximum value
                                                 APUNUM_FLOAT(0.7)); // default value
              
              addParameter(m_gainParameter); // Expose the parameter to the world
          }
                                                 
          void processAudio(float *inputBuffer, float *outputBuffer,
                            unsigned numInputChannels, unsigned numOutputChannels)
          {
              float gain = m_gainParameter->getCurrent().getFloatValue(); // Get the parameter value
              outputBuffer[0] = inputBuffer[0] * gain;
              if (numOutputChannels == 2) {
                  if (numInputChannels == 2) {
                      outputBuffer[1] = inputBuffer[1] * gain;
                  } else {
                      outputBuffer[1] = outputBuffer[0];
                  }
              }
          }
      };
 
 The framework is built to allow the creation of virtually any kind of audio effect or synthesizer. The complete documentation of the API can be generated using Doxygen.
