//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// MrShifty_types1.h
//
// Code generation for function 'MrShifty_types1'
//

#ifndef MRSHIFTY_TYPES1_H
#define MRSHIFTY_TYPES1_H

// Include files
#include "rtwtypes.h"
#include <cstddef>
#include <cstdlib>

// Type Definitions
struct dsp_VariableFractionalDelay_0 {
  int S0_isInitialized;
  int W0_BUFF_OFFSET;
  double W1_BUFF[11526];
  double W2_PolyCoeffs[4];
  int W3_PrevNumTaps;
  int W4_PrevNumChan;
  double P0_IC;
  double P1_FilterArgs[16];
};

struct struct_T {
  double w[4];
  double a0;
  double a1;
  double a2;
  double b0;
  double b1;
  double b2;
  double x1;
  double x2;
  double y1;
  double y2;
};

#endif
// End of code generation (MrShifty_types1.h)
