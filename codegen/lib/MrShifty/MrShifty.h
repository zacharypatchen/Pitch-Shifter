//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// MrShifty.h
//
// Code generation for function 'MrShifty'
//

#ifndef MRSHIFTY_H
#define MRSHIFTY_H

// Include files
#include "MrShifty_types1.h"
#include "rtwtypes.h"
#include "coder_array.h"
#include <cstddef>
#include <cstdlib>

// Type Declarations
struct MrShiftyStackData;

// Type Definitions
namespace coder {
namespace dspcodegen {
class VariableFractionalDelay {
public:
  void outputImpl(const array<double, 2U> &varargin_1,
                  const array<double, 3U> &varargin_2,
                  array<double, 3U> &varargout_1);
  VariableFractionalDelay();
  ~VariableFractionalDelay();
  int isInitialized;
  dsp_VariableFractionalDelay_0 cSFunObject;
};

} // namespace dspcodegen
namespace audiopluginexample {
class PitchShifter {
public:
  void stepImpl(const array<double, 2U> &u, array<double, 2U> &y);
  PitchShifter();
  ~PitchShifter();
  double PrivateSampleRate;
  int isInitialized;
  boolean_T TunablePropsChanged;
  double PitchShift;
  double Overlap;
  double Phase1State;
  double Phase2State;
  double pRate;
  double pSampsDelay;
  dspcodegen::VariableFractionalDelay pShifter;
  double pPhaseStep;
  double pFaderGain;
};

} // namespace audiopluginexample
} // namespace coder
class ParameterSmoother {
public:
  ParameterSmoother();
  ~ParameterSmoother();
  int isInitialized;
  double smoothingFactor;
  double currentValue;
  double targetValue;
};

class derivedAudioPlugin {
public:
  void updateLowFreqFilter();
  void updateMidFreqFilter();
  void updateHighFreqFilter();
  derivedAudioPlugin();
  ~derivedAudioPlugin();
  boolean_T matlabCodegenIsDeleted;
  double PrivateSampleRate;
  int PrivateLatency;
  double LF_FREQ;
  double MF_FREQ;
  double HF_FREQ;
  double LF_GAIN;
  double MF_GAIN;
  double HF_GAIN;
  double LF_Q;
  double MF_Q;
  double HF_Q;
  double PITCHSHIFT;
  double OVERLAY;
  double PITCHSHIFT_MIX;
  double DRIVE;
  double FS;
  coder::audiopluginexample::PitchShifter b_PitchShifter;
  ParameterSmoother pitchMixSmoother;
  ParameterSmoother pitchSmoother;
  ParameterSmoother overlaySmoother;
  double Vt;
  double eta;
  double Is;
  ParameterSmoother driveSmoother;
  struct_T lowFreqFilter;
  struct_T midFreqFilter;
  struct_T highFreqFilter;
  ParameterSmoother lowFreqSmoother;
  ParameterSmoother lowFreqGainSmoother;
  ParameterSmoother lowFreqQSmoother;
  ParameterSmoother midFreqSmoother;
  ParameterSmoother midFreqGainSmoother;
  ParameterSmoother midFreqQSmoother;
  ParameterSmoother highFreqSmoother;
  ParameterSmoother highFreqGainSmoother;
  ParameterSmoother highFreqQSmoother;
};

// Function Declarations
extern void MrShifty_initialize(MrShiftyStackData *SD);

extern void MrShifty_terminate(MrShiftyStackData *SD);

extern void createPluginInstance(MrShiftyStackData *SD, unsigned long thisPtr);

extern int getLatencyInSamplesCImpl(MrShiftyStackData *SD);

extern void onParamChangeCImpl(MrShiftyStackData *SD, int paramIdx,
                               double b_value);

extern void processEntryPoint(MrShiftyStackData *SD, double samplesPerFrame,
                              const double i1_data[], const int i1_size[1],
                              const double i2_data[], const int i2_size[1],
                              double o1_data[], int o1_size[1],
                              double o2_data[], int o2_size[1]);

extern void resetCImpl(MrShiftyStackData *SD, double rate);

#endif
// End of code generation (MrShifty.h)
