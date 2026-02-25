//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// MrShifty_types.h
//
// Code generation for function 'onParamChangeCImpl'
//

#ifndef MRSHIFTY_TYPES_H
#define MRSHIFTY_TYPES_H

// Include files
#include "MrShifty.h"
#include "rtwtypes.h"

// Type Definitions
struct b_processEntryPoint {
  double clipped_data[4096];
};

struct MrShiftyPersistentData {
  derivedAudioPlugin plugin;
  boolean_T plugin_not_empty;
};

struct MrShiftyStackData {
  b_processEntryPoint f0;
  MrShiftyPersistentData *pd;
};

#endif
// End of code generation (MrShifty_types.h)
