//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// MrShifty.cpp
//
// Code generation for function 'MrShifty'
//

// Include files
#include "MrShifty.h"
#include "MrShifty_types.h"
#include "MrShifty_types1.h"
#include "rt_nonfinite.h"
#include "coder_array.h"
#include <algorithm>
#include <cmath>
#include <cstring>

// Function Declarations
static void binary_expand_op(coder::array<double, 2U> &in1, double in2,
                             const coder::array<double, 2U> &in3);

static void binary_expand_op_1(coder::array<double, 3U> &in1, int in2,
                               const double in3_data[], const int &in3_size);

static void binary_expand_op_2(coder::array<double, 3U> &in1, int in2,
                               const double in3_data[], const int &in3_size);

namespace coder {
static int filter(double b[3], double a[3], const double x_data[], int x_size,
                  const double zi[2], double y_data[], double zf[2]);

}
static derivedAudioPlugin *getPluginInstance(MrShiftyStackData *SD);

static void getPluginInstance_delete(MrShiftyStackData *SD);

static void getPluginInstance_init(MrShiftyStackData *SD);

static void getPluginInstance_new(MrShiftyStackData *SD);

static double rt_powd_snf(double u0, double u1);

// Function Definitions
namespace coder {
namespace audiopluginexample {
void PitchShifter::stepImpl(const array<double, 2U> &u, array<double, 2U> &y)
{
  array<double, 3U> delayedOut;
  array<double, 3U> dly;
  double delays1_data[4096];
  double delays2_data[4096];
  double gains1_data[4096];
  double gains2_data[4096];
  double fgain;
  double ovrlp;
  double ph1;
  double ph2;
  double pstep;
  double sd;
  int buffstart;
  int gains1_size;
  int gains2_size;
  int i;
  int loop_ub;
  loop_ub = u.size(0);
  gains1_size = u.size(0);
  gains2_size = u.size(0);
  ph1 = Phase1State;
  ph2 = Phase2State;
  pstep = pPhaseStep;
  ovrlp = Overlap;
  sd = pSampsDelay;
  fgain = pFaderGain;
  for (i = 0; i < loop_ub; i++) {
    double x;
    gains1_data[i] = 0.0;
    gains2_data[i] = 0.0;
    delays1_data[i] = 0.0;
    delays2_data[i] = 0.0;
    x = ph1 + pstep;
    if (std::isnan(x) || std::isinf(x)) {
      ph1 = rtNaN;
    } else if (x == 0.0) {
      ph1 = 0.0;
    } else {
      ph1 = std::fmod(x, 1.0);
      if (ph1 == 0.0) {
        ph1 = 0.0;
      } else if (x < 0.0) {
        ph1++;
      }
    }
    x = ph2 + pstep;
    if (std::isnan(x) || std::isinf(x)) {
      ph2 = rtNaN;
    } else if (x == 0.0) {
      ph2 = 0.0;
    } else {
      ph2 = std::fmod(x, 1.0);
      if (ph2 == 0.0) {
        ph2 = 0.0;
      } else if (x < 0.0) {
        ph2++;
      }
    }
    if ((ph1 < ovrlp) && (ph2 >= 1.0 - ovrlp)) {
      delays1_data[i] = sd * ph1;
      delays2_data[i] = sd * ph2;
      gains1_data[i] = std::cos((1.0 - ph1 * fgain) * 3.1415926535897931 / 2.0);
      gains2_data[i] =
          std::cos((ph2 - (1.0 - ovrlp)) * fgain * 3.1415926535897931 / 2.0);
    } else if ((ph1 > ovrlp) && (ph1 < 1.0 - ovrlp)) {
      ph2 = 0.0;
      delays1_data[i] = sd * ph1;
      gains1_data[i] = 1.0;
      gains2_data[i] = 0.0;
    } else if ((ph1 >= 1.0 - ovrlp) && (ph2 < ovrlp)) {
      delays1_data[i] = sd * ph1;
      delays2_data[i] = sd * ph2;
      gains1_data[i] =
          std::cos((ph1 - (1.0 - ovrlp)) * fgain * 3.1415926535897931 / 2.0);
      gains2_data[i] = std::cos((1.0 - ph2 * fgain) * 3.1415926535897931 / 2.0);
    } else if ((ph2 > ovrlp) && (ph2 < 1.0 - ovrlp)) {
      ph1 = 0.0;
      delays2_data[i] = sd * ph2;
      gains1_data[i] = 0.0;
      gains2_data[i] = 1.0;
    }
  }
  Phase1State = ph1;
  Phase2State = ph2;
  dly.set_size(u.size(0), 1, 2);
  i = u.size(0) << 1;
  for (buffstart = 0; buffstart < i; buffstart++) {
    dly[buffstart] = 0.0;
  }
  for (buffstart = 0; buffstart < loop_ub; buffstart++) {
    dly[buffstart] = delays1_data[buffstart];
  }
  for (buffstart = 0; buffstart < loop_ub; buffstart++) {
    dly[buffstart + dly.size(0)] = delays2_data[buffstart];
  }
  if (pShifter.isInitialized != 1) {
    pShifter.isInitialized = 1;
    // System object Initialization function: dsp.VariableFractionalDelay
    pShifter.cSFunObject.W0_BUFF_OFFSET = 5760;
    pShifter.cSFunObject.W0_BUFF_OFFSET += 2;
    for (i = 0; i < 11526; i++) {
      pShifter.cSFunObject.W1_BUFF[i] = pShifter.cSFunObject.P0_IC;
    }
    pShifter.cSFunObject.W4_PrevNumChan = -1;
    pShifter.cSFunObject.W3_PrevNumTaps = -1;
  }
  pShifter.outputImpl(u, dly, delayedOut);
  // System object Update function: dsp.VariableFractionalDelay
  if (pShifter.cSFunObject.W4_PrevNumChan == -1) {
    pShifter.cSFunObject.W4_PrevNumChan = 2;
  }
  if (pShifter.cSFunObject.W3_PrevNumTaps == -1) {
    pShifter.cSFunObject.W3_PrevNumTaps = 2;
  }
  if ((u.size(0) != 0) && (dly.size(0) != 0)) {
    buffstart = pShifter.cSFunObject.W0_BUFF_OFFSET;
    for (i = 0; i < loop_ub; i++) {
      pShifter.cSFunObject.W1_BUFF[buffstart] = u[i];
      if (buffstart < 5762) {
        buffstart++;
      } else {
        buffstart = 0;
      }
    }
    buffstart = pShifter.cSFunObject.W0_BUFF_OFFSET;
    for (i = 0; i < loop_ub; i++) {
      pShifter.cSFunObject.W1_BUFF[buffstart + 5763] = u[i + loop_ub];
      if (buffstart < 5762) {
        buffstart++;
      } else {
        buffstart = 0;
      }
    }
    pShifter.cSFunObject.W0_BUFF_OFFSET = buffstart;
  }
  for (i = 0; i < 2; i++) {
    if (delayedOut.size(0) == gains1_size) {
      loop_ub = delayedOut.size(0);
      for (buffstart = 0; buffstart < loop_ub; buffstart++) {
        delayedOut[buffstart + delayedOut.size(0) * i] =
            delayedOut[buffstart + delayedOut.size(0) * i] *
            gains1_data[buffstart];
      }
    } else {
      binary_expand_op_2(delayedOut, i, gains1_data, gains1_size);
    }
    if (delayedOut.size(0) == gains2_size) {
      loop_ub = delayedOut.size(0);
      for (buffstart = 0; buffstart < loop_ub; buffstart++) {
        delayedOut[(buffstart + delayedOut.size(0) * i) +
                   delayedOut.size(0) * delayedOut.size(1)] =
            delayedOut[(buffstart + delayedOut.size(0) * i) +
                       delayedOut.size(0) * delayedOut.size(1)] *
            gains2_data[buffstart];
      }
    } else {
      binary_expand_op_1(delayedOut, i, gains2_data, gains2_size);
    }
  }
  i = delayedOut.size(2);
  if ((delayedOut.size(0) == 0) || (delayedOut.size(1) == 0) ||
      (delayedOut.size(2) == 0)) {
    y.set_size(static_cast<int>(static_cast<short>(delayedOut.size(0))),
               static_cast<int>(static_cast<short>(delayedOut.size(1))));
    loop_ub = static_cast<short>(delayedOut.size(0)) *
              static_cast<short>(delayedOut.size(1));
    for (buffstart = 0; buffstart < loop_ub; buffstart++) {
      y[buffstart] = 0.0;
    }
  } else {
    buffstart = delayedOut.size(0) * delayedOut.size(1);
    y.set_size(static_cast<int>(static_cast<short>(delayedOut.size(0))),
               static_cast<int>(static_cast<short>(delayedOut.size(1))));
    for (loop_ub = 0; loop_ub < buffstart; loop_ub++) {
      y[loop_ub] = delayedOut[loop_ub];
    }
    for (gains1_size = 2; gains1_size <= i; gains1_size++) {
      for (loop_ub = 0; loop_ub < buffstart; loop_ub++) {
        y[loop_ub] = y[loop_ub] + delayedOut[buffstart + loop_ub];
      }
    }
  }
}

} // namespace audiopluginexample
namespace dspcodegen {
void VariableFractionalDelay::outputImpl(const array<double, 2U> &varargin_1,
                                         const array<double, 3U> &varargin_2,
                                         array<double, 3U> &varargout_1)
{
  double frac;
  int nDelayElements;
  int nSamps;
  int ti;
  boolean_T incrementDelayPerSample;
  // System object Outputs function: dsp.VariableFractionalDelay
  nSamps = varargin_1.size(0);
  varargout_1.set_size(varargin_1.size(0), 2, 2);
  ti = 0;
  frac = 0.0;
  if (cSFunObject.W4_PrevNumChan == -1) {
    cSFunObject.W4_PrevNumChan = 2;
  }
  if (cSFunObject.W3_PrevNumTaps == -1) {
    cSFunObject.W3_PrevNumTaps = 2;
  }
  nDelayElements = varargin_2.size(0) << 1;
  incrementDelayPerSample =
      ((nDelayElements != 1) && ((varargin_1.size(0) > 1) &&
                                 (varargin_2.size(0) == varargin_1.size(0))));
  nDelayElements /= 2;
  if ((varargin_1.size(0) != 0) && (varargin_2.size(0) != 0)) {
    for (int b_index{0}; b_index < 2; b_index++) {
      int dlyIdx;
      int outOffset;
      outOffset = (nSamps << 1) * b_index;
      dlyIdx = 0;
      if (!incrementDelayPerSample) {
        frac = varargin_2[nDelayElements * b_index];
        ti = static_cast<int>(std::floor(frac));
        if (ti < 1) {
          ti = 1;
          frac = 0.0;
        } else if (ti >= 5760) {
          ti = 5760;
          frac = 0.0;
        } else {
          frac -= static_cast<double>(ti);
        }
      }
      for (int k{0}; k < 2; k++) {
        int buffIdx;
        int buffstart;
        int ioIdx;
        buffstart = cSFunObject.W0_BUFF_OFFSET;
        ioIdx = k * nSamps;
        buffIdx = k * 5763;
        for (int i{0}; i < nSamps; i++) {
          int filtIdx;
          if (incrementDelayPerSample) {
            frac = varargin_2[dlyIdx + nDelayElements * b_index];
            ti = static_cast<int>(std::floor(frac));
            if (ti < 1) {
              ti = 1;
              frac = 0.0;
            } else if (ti >= 5760) {
              ti = 5760;
              frac = 0.0;
            } else {
              frac -= static_cast<double>(ti);
            }
            dlyIdx++;
          }
          filtIdx = 0;
          if (ti - 1 < 0) {
            for (int j{0}; j < 4; j++) {
              double accum;
              double prod2;
              int idx;
              if (i - 1 < 0) {
                idx = buffstart + -1;
                if (buffstart + -1 < 0) {
                  idx = buffstart + 5762;
                }
                prod2 = cSFunObject.W1_BUFF[idx + buffIdx] *
                        cSFunObject.P1_FilterArgs[filtIdx + 1];
              } else {
                prod2 = varargin_1[(i + ioIdx) - 1] *
                        cSFunObject.P1_FilterArgs[filtIdx + 1];
              }
              accum =
                  varargin_1[i + ioIdx] * cSFunObject.P1_FilterArgs[filtIdx] +
                  prod2;
              if (i - 2 < 0) {
                idx = (i + buffstart) - 2;
                if (idx < 0) {
                  idx += 5763;
                }
                prod2 = cSFunObject.W1_BUFF[idx + buffIdx] *
                        cSFunObject.P1_FilterArgs[filtIdx + 2];
              } else {
                prod2 = varargin_1[(i + ioIdx) - 2] *
                        cSFunObject.P1_FilterArgs[filtIdx + 2];
              }
              accum += prod2;
              if (i - 3 < 0) {
                idx = (i + buffstart) - 3;
                if (idx < 0) {
                  idx += 5763;
                }
                prod2 = cSFunObject.W1_BUFF[idx + buffIdx] *
                        cSFunObject.P1_FilterArgs[filtIdx + 3];
              } else {
                prod2 = varargin_1[(i + ioIdx) - 3] *
                        cSFunObject.P1_FilterArgs[filtIdx + 3];
              }
              accum += prod2;
              filtIdx += 4;
              cSFunObject.W2_PolyCoeffs[j] = accum;
            }
          } else {
            int idx_tmp;
            idx_tmp = i - ti;
            for (int j{0}; j < 4; j++) {
              double accum;
              double prod2;
              int idx;
              if (idx_tmp + 1 < 0) {
                idx = (idx_tmp + buffstart) + 1;
                if (idx < 0) {
                  idx += 5763;
                }
                prod2 = cSFunObject.W1_BUFF[idx + buffIdx] *
                        cSFunObject.P1_FilterArgs[filtIdx];
              } else {
                prod2 = varargin_1[(idx_tmp + ioIdx) + 1] *
                        cSFunObject.P1_FilterArgs[filtIdx];
              }
              accum = prod2;
              if (idx_tmp < 0) {
                idx = idx_tmp + buffstart;
                if (idx < 0) {
                  idx += 5763;
                }
                prod2 = cSFunObject.W1_BUFF[idx + buffIdx] *
                        cSFunObject.P1_FilterArgs[filtIdx + 1];
              } else {
                prod2 = varargin_1[idx_tmp + ioIdx] *
                        cSFunObject.P1_FilterArgs[filtIdx + 1];
              }
              accum += prod2;
              if (idx_tmp - 1 < 0) {
                idx = (idx_tmp + buffstart) - 1;
                if (idx < 0) {
                  idx += 5763;
                }
                prod2 = cSFunObject.W1_BUFF[idx + buffIdx] *
                        cSFunObject.P1_FilterArgs[filtIdx + 2];
              } else {
                prod2 = varargin_1[(idx_tmp + ioIdx) - 1] *
                        cSFunObject.P1_FilterArgs[filtIdx + 2];
              }
              accum += prod2;
              if (idx_tmp - 2 < 0) {
                idx = (idx_tmp + buffstart) - 2;
                if (idx < 0) {
                  idx += 5763;
                }
                prod2 = cSFunObject.W1_BUFF[idx + buffIdx] *
                        cSFunObject.P1_FilterArgs[filtIdx + 3];
              } else {
                prod2 = varargin_1[(idx_tmp + ioIdx) - 2] *
                        cSFunObject.P1_FilterArgs[filtIdx + 3];
              }
              accum += prod2;
              filtIdx += 4;
              cSFunObject.W2_PolyCoeffs[j] = accum;
            }
          }
          varargout_1[(i + ioIdx) + outOffset] =
              ((cSFunObject.W2_PolyCoeffs[0] * frac +
                cSFunObject.W2_PolyCoeffs[1]) *
                   frac +
               cSFunObject.W2_PolyCoeffs[2]) *
                  frac +
              cSFunObject.W2_PolyCoeffs[3];
        }
        if (incrementDelayPerSample && (nDelayElements == nSamps)) {
          dlyIdx -= nDelayElements;
        }
      }
    }
  }
}

} // namespace dspcodegen
} // namespace coder
void derivedAudioPlugin::updateHighFreqFilter()
{
  double A;
  double alpha;
  double d;
  double w0;
  if (highFreqQSmoother.isInitialized != 1) {
    highFreqQSmoother.isInitialized = 1;
  }
  //  Perform the smoothing
  highFreqQSmoother.currentValue =
      highFreqQSmoother.smoothingFactor * highFreqQSmoother.currentValue +
      (1.0 - highFreqQSmoother.smoothingFactor) * highFreqQSmoother.targetValue;
  if (highFreqSmoother.isInitialized != 1) {
    highFreqSmoother.isInitialized = 1;
  }
  //  Perform the smoothing
  highFreqSmoother.currentValue =
      highFreqSmoother.smoothingFactor * highFreqSmoother.currentValue +
      (1.0 - highFreqSmoother.smoothingFactor) * highFreqSmoother.targetValue;
  if (highFreqGainSmoother.isInitialized != 1) {
    highFreqGainSmoother.isInitialized = 1;
  }
  //  Perform the smoothing
  highFreqGainSmoother.currentValue =
      highFreqGainSmoother.smoothingFactor * highFreqGainSmoother.currentValue +
      (1.0 - highFreqGainSmoother.smoothingFactor) *
          highFreqGainSmoother.targetValue;
  w0 = 6.2831853071795862 * highFreqSmoother.currentValue / FS;
  alpha = std::sin(w0) / (2.0 * highFreqQSmoother.currentValue);
  A = std::sqrt(rt_powd_snf(10.0, highFreqGainSmoother.currentValue / 20.0));
  d = alpha * A;
  highFreqFilter.a0 = d + 1.0;
  w0 = -2.0 * std::cos(w0);
  highFreqFilter.a1 = w0;
  highFreqFilter.a2 = 1.0 - d;
  d = alpha / A;
  highFreqFilter.b0 = d + 1.0;
  highFreqFilter.b1 = w0;
  highFreqFilter.b2 = 1.0 - d;
}

void derivedAudioPlugin::updateLowFreqFilter()
{
  double A;
  double alpha;
  double d;
  double w0;
  if (lowFreqQSmoother.isInitialized != 1) {
    lowFreqQSmoother.isInitialized = 1;
  }
  //  Perform the smoothing
  lowFreqQSmoother.currentValue =
      lowFreqQSmoother.smoothingFactor * lowFreqQSmoother.currentValue +
      (1.0 - lowFreqQSmoother.smoothingFactor) * lowFreqQSmoother.targetValue;
  if (lowFreqSmoother.isInitialized != 1) {
    lowFreqSmoother.isInitialized = 1;
  }
  //  Perform the smoothing
  lowFreqSmoother.currentValue =
      lowFreqSmoother.smoothingFactor * lowFreqSmoother.currentValue +
      (1.0 - lowFreqSmoother.smoothingFactor) * lowFreqSmoother.targetValue;
  if (lowFreqGainSmoother.isInitialized != 1) {
    lowFreqGainSmoother.isInitialized = 1;
  }
  //  Perform the smoothing
  lowFreqGainSmoother.currentValue =
      lowFreqGainSmoother.smoothingFactor * lowFreqGainSmoother.currentValue +
      (1.0 - lowFreqGainSmoother.smoothingFactor) *
          lowFreqGainSmoother.targetValue;
  w0 = 6.2831853071795862 * lowFreqSmoother.currentValue / FS;
  alpha = std::sin(w0) / (2.0 * lowFreqQSmoother.currentValue);
  A = std::sqrt(rt_powd_snf(10.0, lowFreqGainSmoother.currentValue / 20.0));
  d = alpha * A;
  lowFreqFilter.a0 = d + 1.0;
  w0 = -2.0 * std::cos(w0);
  lowFreqFilter.a1 = w0;
  lowFreqFilter.a2 = 1.0 - d;
  d = alpha / A;
  lowFreqFilter.b0 = d + 1.0;
  lowFreqFilter.b1 = w0;
  lowFreqFilter.b2 = 1.0 - d;
}

void derivedAudioPlugin::updateMidFreqFilter()
{
  double A;
  double alpha;
  double d;
  double w0;
  if (midFreqQSmoother.isInitialized != 1) {
    midFreqQSmoother.isInitialized = 1;
  }
  //  Perform the smoothing
  midFreqQSmoother.currentValue =
      midFreqQSmoother.smoothingFactor * midFreqQSmoother.currentValue +
      (1.0 - midFreqQSmoother.smoothingFactor) * midFreqQSmoother.targetValue;
  if (midFreqSmoother.isInitialized != 1) {
    midFreqSmoother.isInitialized = 1;
  }
  //  Perform the smoothing
  midFreqSmoother.currentValue =
      midFreqSmoother.smoothingFactor * midFreqSmoother.currentValue +
      (1.0 - midFreqSmoother.smoothingFactor) * midFreqSmoother.targetValue;
  if (midFreqGainSmoother.isInitialized != 1) {
    midFreqGainSmoother.isInitialized = 1;
  }
  //  Perform the smoothing
  midFreqGainSmoother.currentValue =
      midFreqGainSmoother.smoothingFactor * midFreqGainSmoother.currentValue +
      (1.0 - midFreqGainSmoother.smoothingFactor) *
          midFreqGainSmoother.targetValue;
  w0 = 6.2831853071795862 * midFreqSmoother.currentValue / FS;
  alpha = std::sin(w0) / (2.0 * midFreqQSmoother.currentValue);
  A = std::sqrt(rt_powd_snf(10.0, midFreqGainSmoother.currentValue / 20.0));
  d = alpha * A;
  midFreqFilter.a0 = d + 1.0;
  w0 = -2.0 * std::cos(w0);
  midFreqFilter.a1 = w0;
  midFreqFilter.a2 = 1.0 - d;
  d = alpha / A;
  midFreqFilter.b0 = d + 1.0;
  midFreqFilter.b1 = w0;
  midFreqFilter.b2 = 1.0 - d;
}

static void binary_expand_op(coder::array<double, 2U> &in1, double in2,
                             const coder::array<double, 2U> &in3)
{
  coder::array<double, 2U> r;
  int aux_1_1;
  int loop_ub;
  int stride_0_0;
  int stride_1_0;
  int stride_1_1;
  if (in3.size(0) == 1) {
    loop_ub = in1.size(0);
  } else {
    loop_ub = in3.size(0);
  }
  r.set_size(loop_ub, 2);
  stride_0_0 = (in1.size(0) != 1);
  stride_1_0 = (in3.size(0) != 1);
  stride_1_1 = (in3.size(1) != 1);
  aux_1_1 = 0;
  for (int i{0}; i < 2; i++) {
    for (int i1{0}; i1 < loop_ub; i1++) {
      r[i1 + r.size(0) * i] =
          (1.0 - in2) * in1[i1 * stride_0_0 + in1.size(0) * i] +
          in2 * in3[i1 * stride_1_0 + in3.size(0) * aux_1_1];
    }
    aux_1_1 += stride_1_1;
  }
  in1.set_size(loop_ub, 2);
  for (int i{0}; i < 2; i++) {
    for (int i1{0}; i1 < loop_ub; i1++) {
      in1[i1 + in1.size(0) * i] = r[i1 + r.size(0) * i];
    }
  }
}

static void binary_expand_op_1(coder::array<double, 3U> &in1, int in2,
                               const double in3_data[], const int &in3_size)
{
  double in1_data[4096];
  int in1_idx_0_tmp;
  int stride_0_0;
  int stride_1_0;
  in1_idx_0_tmp = in1.size(0);
  stride_0_0 = (in1.size(0) != 1);
  stride_1_0 = (in3_size != 1);
  for (int i{0}; i < in1_idx_0_tmp; i++) {
    in1_data[i] =
        in1[(i * stride_0_0 + in1.size(0) * in2) + in1.size(0) * in1.size(1)] *
        in3_data[i * stride_1_0];
  }
  for (int i{0}; i < in1_idx_0_tmp; i++) {
    in1[(i + in1.size(0) * in2) + in1.size(0) * in1.size(1)] = in1_data[i];
  }
}

static void binary_expand_op_2(coder::array<double, 3U> &in1, int in2,
                               const double in3_data[], const int &in3_size)
{
  double in1_data[4096];
  int in1_idx_0_tmp;
  int stride_0_0;
  int stride_1_0;
  in1_idx_0_tmp = in1.size(0);
  stride_0_0 = (in1.size(0) != 1);
  stride_1_0 = (in3_size != 1);
  for (int i{0}; i < in1_idx_0_tmp; i++) {
    in1_data[i] =
        in1[i * stride_0_0 + in1.size(0) * in2] * in3_data[i * stride_1_0];
  }
  for (int i{0}; i < in1_idx_0_tmp; i++) {
    in1[i + in1.size(0) * in2] = in1_data[i];
  }
}

namespace coder {
static int filter(double b[3], double a[3], const double x_data[], int x_size,
                  const double zi[2], double y_data[], double zf[2])
{
  int lb_tmp;
  int naxpy;
  int niccp;
  int y_size;
  int y_tmp;
  if ((!std::isinf(a[0])) && (!std::isnan(a[0])) && (!(a[0] == 0.0)) &&
      (a[0] != 1.0)) {
    b[0] /= a[0];
    b[1] /= a[0];
    b[2] /= a[0];
    a[1] /= a[0];
    a[2] /= a[0];
    a[0] = 1.0;
  }
  y_size = x_size;
  zf[0] = 0.0;
  zf[1] = 0.0;
  if (x_size < 2) {
    niccp = x_size - 1;
  } else {
    niccp = 1;
  }
  if (niccp >= 0) {
    std::copy(&zi[0], &zi[niccp + 1], &y_data[0]);
  }
  y_tmp = niccp + 2;
  if (y_tmp <= x_size) {
    std::memset(&y_data[y_tmp + -1], 0,
                static_cast<unsigned int>((x_size - y_tmp) + 1) *
                    sizeof(double));
  }
  for (int k{0}; k < x_size; k++) {
    double as;
    niccp = x_size - k;
    if (niccp <= 3) {
      naxpy = niccp;
    } else {
      naxpy = 3;
    }
    for (int j{0}; j < naxpy; j++) {
      y_tmp = k + j;
      y_data[y_tmp] += x_data[k] * b[j];
    }
    if (niccp - 1 < 2) {
      naxpy = niccp - 2;
    } else {
      naxpy = 1;
    }
    as = -y_data[k];
    for (int j{0}; j <= naxpy; j++) {
      y_tmp = (k + j) + 1;
      y_data[y_tmp] += as * a[j + 1];
    }
  }
  if (x_size < 2) {
    y_tmp = 1 - x_size;
    for (int k{0}; k <= y_tmp; k++) {
      zf[k] = zi[k + x_size];
    }
  }
  if (x_size >= 3) {
    lb_tmp = x_size - 2;
  } else {
    lb_tmp = 0;
  }
  y_tmp = x_size - 1;
  for (int k{lb_tmp}; k <= y_tmp; k++) {
    niccp = x_size - k;
    naxpy = 2 - niccp;
    for (int j{0}; j <= naxpy; j++) {
      zf[j] += x_data[k] * b[niccp + j];
    }
  }
  for (int k{lb_tmp}; k <= y_tmp; k++) {
    niccp = x_size - k;
    naxpy = 2 - niccp;
    for (int j{0}; j <= naxpy; j++) {
      zf[j] += -y_data[k] * a[niccp + j];
    }
  }
  return y_size;
}

} // namespace coder
static derivedAudioPlugin *getPluginInstance(MrShiftyStackData *SD)
{
  if (!SD->pd->plugin_not_empty) {
    double initialValue;
    //  Pass constructor args to plugin.
    SD->pd->plugin.LF_FREQ = 100.0;
    SD->pd->plugin.MF_FREQ = 1000.0;
    SD->pd->plugin.HF_FREQ = 8000.0;
    SD->pd->plugin.LF_GAIN = 0.0;
    SD->pd->plugin.MF_GAIN = 0.0;
    SD->pd->plugin.HF_GAIN = 0.0;
    SD->pd->plugin.LF_Q = 0.707;
    SD->pd->plugin.MF_Q = 0.707;
    SD->pd->plugin.HF_Q = 0.707;
    SD->pd->plugin.PITCHSHIFT = 0.0;
    SD->pd->plugin.OVERLAY = 0.45;
    SD->pd->plugin.PITCHSHIFT_MIX = 50.0;
    SD->pd->plugin.DRIVE = 0.0;
    SD->pd->plugin.FS = 44100.0;
    SD->pd->plugin.Vt = 0.0253;
    SD->pd->plugin.eta = 1.68;
    SD->pd->plugin.Is = 0.105;
    SD->pd->plugin.lowFreqFilter.a0 = 1.0;
    SD->pd->plugin.lowFreqFilter.a1 = 0.0;
    SD->pd->plugin.lowFreqFilter.a2 = 0.0;
    SD->pd->plugin.lowFreqFilter.b0 = 1.0;
    SD->pd->plugin.lowFreqFilter.b1 = 0.0;
    SD->pd->plugin.lowFreqFilter.b2 = 0.0;
    SD->pd->plugin.lowFreqFilter.x1 = 0.0;
    SD->pd->plugin.lowFreqFilter.x2 = 0.0;
    SD->pd->plugin.lowFreqFilter.y1 = 0.0;
    SD->pd->plugin.lowFreqFilter.y2 = 0.0;
    SD->pd->plugin.midFreqFilter.a0 = 1.0;
    SD->pd->plugin.midFreqFilter.a1 = 0.0;
    SD->pd->plugin.midFreqFilter.a2 = 0.0;
    SD->pd->plugin.midFreqFilter.b0 = 1.0;
    SD->pd->plugin.midFreqFilter.b1 = 0.0;
    SD->pd->plugin.midFreqFilter.b2 = 0.0;
    SD->pd->plugin.midFreqFilter.x1 = 0.0;
    SD->pd->plugin.midFreqFilter.x2 = 0.0;
    SD->pd->plugin.midFreqFilter.y1 = 0.0;
    SD->pd->plugin.midFreqFilter.y2 = 0.0;
    SD->pd->plugin.lowFreqFilter.w[0] = 0.0;
    SD->pd->plugin.midFreqFilter.w[0] = 0.0;
    SD->pd->plugin.highFreqFilter.w[0] = 0.0;
    SD->pd->plugin.lowFreqFilter.w[1] = 0.0;
    SD->pd->plugin.midFreqFilter.w[1] = 0.0;
    SD->pd->plugin.highFreqFilter.w[1] = 0.0;
    SD->pd->plugin.lowFreqFilter.w[2] = 0.0;
    SD->pd->plugin.midFreqFilter.w[2] = 0.0;
    SD->pd->plugin.highFreqFilter.w[2] = 0.0;
    SD->pd->plugin.lowFreqFilter.w[3] = 0.0;
    SD->pd->plugin.midFreqFilter.w[3] = 0.0;
    SD->pd->plugin.highFreqFilter.w[3] = 0.0;
    SD->pd->plugin.highFreqFilter.a0 = 1.0;
    SD->pd->plugin.highFreqFilter.a1 = 0.0;
    SD->pd->plugin.highFreqFilter.a2 = 0.0;
    SD->pd->plugin.highFreqFilter.b0 = 1.0;
    SD->pd->plugin.highFreqFilter.b1 = 0.0;
    SD->pd->plugin.highFreqFilter.b2 = 0.0;
    SD->pd->plugin.highFreqFilter.x1 = 0.0;
    SD->pd->plugin.highFreqFilter.x2 = 0.0;
    SD->pd->plugin.highFreqFilter.y1 = 0.0;
    SD->pd->plugin.highFreqFilter.y2 = 0.0;
    SD->pd->plugin.PrivateLatency = 0;
    //  Initialize Pitch Shifter
    SD->pd->plugin.b_PitchShifter.PitchShift = 0.0;
    SD->pd->plugin.b_PitchShifter.Overlap = 0.01;
    SD->pd->plugin.b_PitchShifter.PrivateSampleRate = 44100.0;
    SD->pd->plugin.b_PitchShifter.isInitialized = 0;
    initialValue = SD->pd->plugin.PITCHSHIFT_MIX;
    SD->pd->plugin.pitchMixSmoother.isInitialized = 0;
    SD->pd->plugin.pitchMixSmoother.currentValue = initialValue;
    SD->pd->plugin.pitchMixSmoother.targetValue = initialValue;
    SD->pd->plugin.pitchMixSmoother.smoothingFactor = 0.5;
    initialValue = SD->pd->plugin.PITCHSHIFT;
    SD->pd->plugin.pitchSmoother.isInitialized = 0;
    SD->pd->plugin.pitchSmoother.currentValue = initialValue;
    SD->pd->plugin.pitchSmoother.targetValue = initialValue;
    SD->pd->plugin.pitchSmoother.smoothingFactor = 0.9;
    initialValue = SD->pd->plugin.OVERLAY;
    SD->pd->plugin.overlaySmoother.isInitialized = 0;
    SD->pd->plugin.overlaySmoother.currentValue = initialValue;
    SD->pd->plugin.overlaySmoother.targetValue = initialValue;
    SD->pd->plugin.overlaySmoother.smoothingFactor = 0.9;
    // initialize LF param smoother
    initialValue = SD->pd->plugin.LF_FREQ;
    SD->pd->plugin.lowFreqSmoother.isInitialized = 0;
    SD->pd->plugin.lowFreqSmoother.currentValue = initialValue;
    SD->pd->plugin.lowFreqSmoother.targetValue = initialValue;
    SD->pd->plugin.lowFreqSmoother.smoothingFactor = 0.9;
    initialValue = SD->pd->plugin.LF_GAIN;
    SD->pd->plugin.lowFreqGainSmoother.isInitialized = 0;
    SD->pd->plugin.lowFreqGainSmoother.currentValue = initialValue;
    SD->pd->plugin.lowFreqGainSmoother.targetValue = initialValue;
    SD->pd->plugin.lowFreqGainSmoother.smoothingFactor = 0.9;
    initialValue = SD->pd->plugin.LF_Q;
    SD->pd->plugin.lowFreqQSmoother.isInitialized = 0;
    SD->pd->plugin.lowFreqQSmoother.currentValue = initialValue;
    SD->pd->plugin.lowFreqQSmoother.targetValue = initialValue;
    SD->pd->plugin.lowFreqQSmoother.smoothingFactor = 0.9;
    // initialize mf param smoother
    initialValue = SD->pd->plugin.MF_FREQ;
    SD->pd->plugin.midFreqSmoother.isInitialized = 0;
    SD->pd->plugin.midFreqSmoother.currentValue = initialValue;
    SD->pd->plugin.midFreqSmoother.targetValue = initialValue;
    SD->pd->plugin.midFreqSmoother.smoothingFactor = 0.9;
    initialValue = SD->pd->plugin.MF_GAIN;
    SD->pd->plugin.midFreqGainSmoother.isInitialized = 0;
    SD->pd->plugin.midFreqGainSmoother.currentValue = initialValue;
    SD->pd->plugin.midFreqGainSmoother.targetValue = initialValue;
    SD->pd->plugin.midFreqGainSmoother.smoothingFactor = 0.9;
    initialValue = SD->pd->plugin.MF_Q;
    SD->pd->plugin.midFreqQSmoother.isInitialized = 0;
    SD->pd->plugin.midFreqQSmoother.currentValue = initialValue;
    SD->pd->plugin.midFreqQSmoother.targetValue = initialValue;
    SD->pd->plugin.midFreqQSmoother.smoothingFactor = 0.9;
    // initialize hf param smoother
    initialValue = SD->pd->plugin.HF_FREQ;
    SD->pd->plugin.highFreqSmoother.isInitialized = 0;
    SD->pd->plugin.highFreqSmoother.currentValue = initialValue;
    SD->pd->plugin.highFreqSmoother.targetValue = initialValue;
    SD->pd->plugin.highFreqSmoother.smoothingFactor = 0.9;
    initialValue = SD->pd->plugin.HF_GAIN;
    SD->pd->plugin.highFreqGainSmoother.isInitialized = 0;
    SD->pd->plugin.highFreqGainSmoother.currentValue = initialValue;
    SD->pd->plugin.highFreqGainSmoother.targetValue = initialValue;
    SD->pd->plugin.highFreqGainSmoother.smoothingFactor = 0.9;
    initialValue = SD->pd->plugin.HF_Q;
    SD->pd->plugin.highFreqQSmoother.isInitialized = 0;
    SD->pd->plugin.highFreqQSmoother.currentValue = initialValue;
    SD->pd->plugin.highFreqQSmoother.targetValue = initialValue;
    SD->pd->plugin.highFreqQSmoother.smoothingFactor = 0.9;
    // Initialize drive
    initialValue = SD->pd->plugin.DRIVE;
    SD->pd->plugin.driveSmoother.isInitialized = 0;
    SD->pd->plugin.driveSmoother.currentValue = initialValue;
    SD->pd->plugin.driveSmoother.targetValue = initialValue;
    SD->pd->plugin.driveSmoother.smoothingFactor = 0.2;
    SD->pd->plugin.matlabCodegenIsDeleted = false;
    SD->pd->plugin_not_empty = true;
  }
  return &SD->pd->plugin;
}

static void getPluginInstance_delete(MrShiftyStackData *SD)
{
  if (!SD->pd->plugin.matlabCodegenIsDeleted) {
    SD->pd->plugin.matlabCodegenIsDeleted = true;
  }
}

static void getPluginInstance_init(MrShiftyStackData *SD)
{
  SD->pd->plugin_not_empty = false;
}

static void getPluginInstance_new(MrShiftyStackData *SD)
{
  SD->pd->plugin.matlabCodegenIsDeleted = true;
}

static double rt_powd_snf(double u0, double u1)
{
  double y;
  if (std::isnan(u0) || std::isnan(u1)) {
    y = rtNaN;
  } else {
    double d;
    double d1;
    d = std::abs(u0);
    d1 = std::abs(u1);
    if (std::isinf(u1)) {
      if (d == 1.0) {
        y = 1.0;
      } else if (d > 1.0) {
        if (u1 > 0.0) {
          y = rtInf;
        } else {
          y = 0.0;
        }
      } else if (u1 > 0.0) {
        y = 0.0;
      } else {
        y = rtInf;
      }
    } else if (d1 == 0.0) {
      y = 1.0;
    } else if (d1 == 1.0) {
      if (u1 > 0.0) {
        y = u0;
      } else {
        y = 1.0 / u0;
      }
    } else if (u1 == 2.0) {
      y = u0 * u0;
    } else if ((u1 == 0.5) && (u0 >= 0.0)) {
      y = std::sqrt(u0);
    } else if ((u0 < 0.0) && (u1 > std::floor(u1))) {
      y = rtNaN;
    } else {
      y = std::pow(u0, u1);
    }
  }
  return y;
}

ParameterSmoother::ParameterSmoother() = default;

ParameterSmoother::~ParameterSmoother() = default;

namespace coder {
namespace audiopluginexample {
PitchShifter::PitchShifter() = default;

PitchShifter::~PitchShifter() = default;

} // namespace audiopluginexample
namespace dspcodegen {
VariableFractionalDelay::VariableFractionalDelay() = default;

VariableFractionalDelay::~VariableFractionalDelay() = default;

} // namespace dspcodegen
} // namespace coder
derivedAudioPlugin::derivedAudioPlugin() = default;

derivedAudioPlugin::~derivedAudioPlugin() = default;

void MrShifty_initialize(MrShiftyStackData *SD)
{
  getPluginInstance_new(SD);
  getPluginInstance_init(SD);
}

void MrShifty_terminate(MrShiftyStackData *SD)
{
  getPluginInstance_delete(SD);
}

void createPluginInstance(MrShiftyStackData *SD, unsigned long)
{
  getPluginInstance(SD);
}

int getLatencyInSamplesCImpl(MrShiftyStackData *SD)
{
  derivedAudioPlugin *plugin;
  plugin = getPluginInstance(SD);
  return plugin->PrivateLatency;
}

void onParamChangeCImpl(MrShiftyStackData *SD, int paramIdx, double b_value)
{
  derivedAudioPlugin *plugin;
  plugin = getPluginInstance(SD);
  switch (paramIdx) {
  case 0: {
    boolean_T flag;
    plugin->PITCHSHIFT = b_value;
    plugin->pitchSmoother.targetValue = b_value;
    if (plugin->pitchSmoother.isInitialized != 1) {
      plugin->pitchSmoother.isInitialized = 1;
    }
    //  Perform the smoothing
    plugin->pitchSmoother.currentValue =
        plugin->pitchSmoother.smoothingFactor *
            plugin->pitchSmoother.currentValue +
        (1.0 - plugin->pitchSmoother.smoothingFactor) *
            plugin->pitchSmoother.targetValue;
    flag = (plugin->b_PitchShifter.isInitialized == 1);
    if (flag) {
      plugin->b_PitchShifter.TunablePropsChanged = true;
    }
    plugin->b_PitchShifter.PitchShift = plugin->PITCHSHIFT;
    flag = (plugin->b_PitchShifter.isInitialized == 1);
    if (flag) {
      plugin->b_PitchShifter.TunablePropsChanged = true;
    }
    plugin->b_PitchShifter.Overlap = plugin->OVERLAY;
  } break;
  case 1: {
    boolean_T flag;
    plugin->OVERLAY = b_value;
    plugin->overlaySmoother.targetValue = b_value;
    if (plugin->overlaySmoother.isInitialized != 1) {
      plugin->overlaySmoother.isInitialized = 1;
    }
    //  Perform the smoothing
    plugin->overlaySmoother.currentValue =
        plugin->overlaySmoother.smoothingFactor *
            plugin->overlaySmoother.currentValue +
        (1.0 - plugin->overlaySmoother.smoothingFactor) *
            plugin->overlaySmoother.targetValue;
    flag = (plugin->b_PitchShifter.isInitialized == 1);
    if (flag) {
      plugin->b_PitchShifter.TunablePropsChanged = true;
    }
    plugin->b_PitchShifter.PitchShift = plugin->PITCHSHIFT;
    flag = (plugin->b_PitchShifter.isInitialized == 1);
    if (flag) {
      plugin->b_PitchShifter.TunablePropsChanged = true;
    }
    plugin->b_PitchShifter.Overlap = plugin->OVERLAY;
  } break;
  case 2:
    //  Low Frequency EQ updates
    plugin->LF_FREQ = b_value;
    plugin->lowFreqSmoother.targetValue = b_value;
    plugin->updateLowFreqFilter();
    break;
  case 3:
    plugin->LF_GAIN = b_value;
    plugin->lowFreqGainSmoother.targetValue = b_value;
    plugin->updateLowFreqFilter();
    break;
  case 4:
    plugin->LF_Q = b_value;
    plugin->lowFreqQSmoother.targetValue = b_value;
    plugin->updateLowFreqFilter();
    break;
  case 5:
    //  Mid Frequency EQ updates
    plugin->MF_FREQ = b_value;
    plugin->midFreqSmoother.targetValue = b_value;
    plugin->updateMidFreqFilter();
    break;
  case 6:
    plugin->MF_GAIN = b_value;
    plugin->midFreqGainSmoother.targetValue = b_value;
    plugin->updateMidFreqFilter();
    break;
  case 7:
    plugin->MF_Q = b_value;
    plugin->midFreqQSmoother.targetValue = b_value;
    plugin->updateMidFreqFilter();
    break;
  case 8:
    //  High Frequency EQ updates
    plugin->HF_FREQ = b_value;
    plugin->highFreqSmoother.targetValue = b_value;
    plugin->updateHighFreqFilter();
    break;
  case 9:
    plugin->HF_GAIN = b_value;
    plugin->highFreqGainSmoother.targetValue = b_value;
    plugin->updateHighFreqFilter();
    break;
  case 10:
    plugin->HF_Q = b_value;
    plugin->highFreqQSmoother.targetValue = b_value;
    plugin->updateHighFreqFilter();
    break;
  case 11:
    //  Drive params
    plugin->DRIVE = b_value;
    plugin->driveSmoother.targetValue = b_value;
    if (plugin->driveSmoother.isInitialized != 1) {
      plugin->driveSmoother.isInitialized = 1;
    }
    //  Perform the smoothing
    plugin->driveSmoother.currentValue =
        plugin->driveSmoother.smoothingFactor *
            plugin->driveSmoother.currentValue +
        (1.0 - plugin->driveSmoother.smoothingFactor) *
            plugin->driveSmoother.targetValue;
    break;
  case 12:
    plugin->PITCHSHIFT_MIX = b_value;
    plugin->pitchMixSmoother.targetValue = b_value;
    if (plugin->pitchMixSmoother.isInitialized != 1) {
      plugin->pitchMixSmoother.isInitialized = 1;
    }
    //  Perform the smoothing
    plugin->pitchMixSmoother.currentValue =
        plugin->pitchMixSmoother.smoothingFactor *
            plugin->pitchMixSmoother.currentValue +
        (1.0 - plugin->pitchMixSmoother.smoothingFactor) *
            plugin->pitchMixSmoother.targetValue;
    break;
  }
}

void processEntryPoint(MrShiftyStackData *SD, double samplesPerFrame,
                       const double i1_data[], const int i1_size[1],
                       const double i2_data[], const int i2_size[1],
                       double o1_data[], int o1_size[1], double o2_data[],
                       int o2_size[1])
{
  static const double dv[16]{-0.16666666666666663,
                             0.5,
                             -0.5,
                             0.16666666666666666,
                             0.50000000000000011,
                             -1.0,
                             0.5,
                             0.0,
                             -0.33333333333333315,
                             -0.5,
                             1.0,
                             -0.16666666666666674,
                             0.0,
                             1.0,
                             0.0,
                             0.0};
  derivedAudioPlugin *plugin;
  coder::array<double, 2U> clipped;
  coder::array<double, 2U> shiftedSignal;
  coder::array<double, 2U> x;
  double y_data[4096];
  double b_plugin;
  double mixFactor;
  double y;
  int b_i;
  int b_loop_ub;
  int i;
  int loop_ub;
  int n;
  plugin = getPluginInstance(SD);
  loop_ub = i1_size[0];
  x.set_size(i1_size[0], 2);
  for (i = 0; i < loop_ub; i++) {
    x[i] = i1_data[i];
  }
  loop_ub = i2_size[0];
  for (i = 0; i < loop_ub; i++) {
    x[i + x.size(0)] = i2_data[i];
  }
  //  DSP section
  //  Mix factor for pitch shifting
  mixFactor = plugin->PITCHSHIFT_MIX / 100.0;
  //  Pitch shifting and mix
  if (plugin->b_PitchShifter.isInitialized != 1) {
    plugin->b_PitchShifter.isInitialized = 1;
    plugin->b_PitchShifter.pShifter.isInitialized = 0;
    // System object Constructor function: dsp.VariableFractionalDelay
    plugin->b_PitchShifter.pShifter.cSFunObject.P0_IC = 0.0;
    for (b_i = 0; b_i < 16; b_i++) {
      plugin->b_PitchShifter.pShifter.cSFunObject.P1_FilterArgs[b_i] = dv[b_i];
    }
    plugin->b_PitchShifter.Phase1State = 0.0;
    y = 1.0 - plugin->b_PitchShifter.Overlap;
    plugin->b_PitchShifter.Phase2State = y;
    y = plugin->b_PitchShifter.PitchShift;
    y = std::round(y);
    plugin->b_PitchShifter.pRate = (1.0 - rt_powd_snf(2.0, y / 12.0)) / 0.03;
    y = plugin->b_PitchShifter.PrivateSampleRate;
    plugin->b_PitchShifter.pPhaseStep = plugin->b_PitchShifter.pRate / y;
    plugin->b_PitchShifter.pFaderGain = 1.0 / plugin->b_PitchShifter.Overlap;
    plugin->b_PitchShifter.TunablePropsChanged = false;
    plugin->b_PitchShifter.Phase1State = 0.0;
    y = 1.0 - plugin->b_PitchShifter.Overlap;
    plugin->b_PitchShifter.Phase2State = y;
    if (plugin->b_PitchShifter.pShifter.isInitialized == 1) {
      // System object Initialization function: dsp.VariableFractionalDelay
      plugin->b_PitchShifter.pShifter.cSFunObject.W0_BUFF_OFFSET = 5760;
      plugin->b_PitchShifter.pShifter.cSFunObject.W0_BUFF_OFFSET += 2;
      for (b_i = 0; b_i < 11526; b_i++) {
        plugin->b_PitchShifter.pShifter.cSFunObject.W1_BUFF[b_i] =
            plugin->b_PitchShifter.pShifter.cSFunObject.P0_IC;
      }
      plugin->b_PitchShifter.pShifter.cSFunObject.W4_PrevNumChan = -1;
      plugin->b_PitchShifter.pShifter.cSFunObject.W3_PrevNumTaps = -1;
    }
    y = plugin->b_PitchShifter.PrivateSampleRate;
    plugin->b_PitchShifter.pSampsDelay = std::round(0.03 * y);
    y = plugin->b_PitchShifter.PitchShift;
    y = std::round(y);
    plugin->b_PitchShifter.pRate = (1.0 - rt_powd_snf(2.0, y / 12.0)) / 0.03;
    y = plugin->b_PitchShifter.PrivateSampleRate;
    plugin->b_PitchShifter.pPhaseStep = plugin->b_PitchShifter.pRate / y;
    plugin->b_PitchShifter.pFaderGain = 1.0 / plugin->b_PitchShifter.Overlap;
  }
  if (plugin->b_PitchShifter.TunablePropsChanged) {
    plugin->b_PitchShifter.TunablePropsChanged = false;
    y = plugin->b_PitchShifter.PitchShift;
    y = std::round(y);
    plugin->b_PitchShifter.pRate = (1.0 - rt_powd_snf(2.0, y / 12.0)) / 0.03;
    y = plugin->b_PitchShifter.PrivateSampleRate;
    plugin->b_PitchShifter.pPhaseStep = plugin->b_PitchShifter.pRate / y;
    plugin->b_PitchShifter.pFaderGain = 1.0 / plugin->b_PitchShifter.Overlap;
  }
  plugin->b_PitchShifter.stepImpl(x, shiftedSignal);
  if ((x.size(0) == shiftedSignal.size(0)) && (shiftedSignal.size(1) == 2)) {
    loop_ub = x.size(0) << 1;
    x.set_size(x.size(0), 2);
    for (i = 0; i < loop_ub; i++) {
      x[i] = (1.0 - mixFactor) * x[i] + mixFactor * shiftedSignal[i];
    }
  } else {
    binary_expand_op(x, mixFactor, shiftedSignal);
  }
  //  Diode clipping
  loop_ub = x.size(0);
  clipped.set_size(x.size(0), 2);
  b_loop_ub = x.size(0) << 1;
  for (i = 0; i < b_loop_ub; i++) {
    clipped[i] = 0.0;
  }
  //  Preallocate the clipped signal
  b_i = x.size(0);
  if (b_i < 2) {
    b_i = 2;
  }
  if (x.size(0) == 0) {
    b_i = 0;
  }
  for (n = 0; n < b_i; n++) {
    y = plugin->DRIVE / 100.0 + 0.1;
    mixFactor = plugin->Is;
    b_plugin = plugin->eta * plugin->Vt;
    clipped[n] = mixFactor * (std::exp(y * x[n] / b_plugin) - 1.0);
    clipped[n + clipped.size(0)] =
        mixFactor * (std::exp(y * x[n + x.size(0)] / b_plugin) - 1.0);
  }
  b_loop_ub = clipped.size(0);
  x.set_size(clipped.size(0), 2);
  b_i = clipped.size(0) << 1;
  for (i = 0; i < b_i; i++) {
    x[i] = 0.0;
  }
  //  Preallocate correctly sized array for all channels
  //  EQ processing for each band (low, mid, high) per channel
  for (int ch{0}; ch < 2; ch++) {
    double filt_w[4];
    double b_filt_b0[3];
    double filt_a0[3];
    double filt_b0;
    double filt_b1;
    double filt_b2;
    //  Iterate over each channel
    //  Extract data for current channel
    filt_w[0] = plugin->lowFreqFilter.w[0];
    filt_w[1] = plugin->lowFreqFilter.w[1];
    filt_w[2] = plugin->lowFreqFilter.w[2];
    filt_w[3] = plugin->lowFreqFilter.w[3];
    y = plugin->lowFreqFilter.a0;
    b_plugin = plugin->lowFreqFilter.a1;
    mixFactor = plugin->lowFreqFilter.a2;
    filt_b0 = plugin->lowFreqFilter.b0;
    filt_b1 = plugin->lowFreqFilter.b1;
    filt_b2 = plugin->lowFreqFilter.b2;
    filt_a0[0] = y;
    filt_a0[1] = b_plugin;
    filt_a0[2] = mixFactor;
    b_filt_b0[0] = filt_b0;
    b_filt_b0[1] = filt_b1;
    b_filt_b0[2] = filt_b2;
    for (i = 0; i < loop_ub; i++) {
      SD->f0.clipped_data[i] = clipped[i + clipped.size(0) * ch];
    }
    double b_x[2];
    i = ch << 1;
    b_i = coder::filter(filt_a0, b_filt_b0, SD->f0.clipped_data, loop_ub,
                        &filt_w[i], y_data, b_x);
    plugin->lowFreqFilter.w[i] = b_x[0];
    plugin->lowFreqFilter.w[i + 1] = b_x[1];
    filt_w[0] = plugin->midFreqFilter.w[0];
    filt_w[1] = plugin->midFreqFilter.w[1];
    filt_w[2] = plugin->midFreqFilter.w[2];
    filt_w[3] = plugin->midFreqFilter.w[3];
    y = plugin->midFreqFilter.a0;
    b_plugin = plugin->midFreqFilter.a1;
    mixFactor = plugin->midFreqFilter.a2;
    filt_b0 = plugin->midFreqFilter.b0;
    filt_b1 = plugin->midFreqFilter.b1;
    filt_b2 = plugin->midFreqFilter.b2;
    filt_a0[0] = y;
    filt_a0[1] = b_plugin;
    filt_a0[2] = mixFactor;
    b_filt_b0[0] = filt_b0;
    b_filt_b0[1] = filt_b1;
    b_filt_b0[2] = filt_b2;
    n = coder::filter(filt_a0, b_filt_b0, y_data, b_i, &filt_w[i],
                      SD->f0.clipped_data, b_x);
    plugin->midFreqFilter.w[i] = b_x[0];
    plugin->midFreqFilter.w[i + 1] = b_x[1];
    filt_w[0] = plugin->highFreqFilter.w[0];
    filt_w[1] = plugin->highFreqFilter.w[1];
    filt_w[2] = plugin->highFreqFilter.w[2];
    filt_w[3] = plugin->highFreqFilter.w[3];
    y = plugin->highFreqFilter.a0;
    b_plugin = plugin->highFreqFilter.a1;
    mixFactor = plugin->highFreqFilter.a2;
    filt_b0 = plugin->highFreqFilter.b0;
    filt_b1 = plugin->highFreqFilter.b1;
    filt_b2 = plugin->highFreqFilter.b2;
    filt_a0[0] = y;
    filt_a0[1] = b_plugin;
    filt_a0[2] = mixFactor;
    b_filt_b0[0] = filt_b0;
    b_filt_b0[1] = filt_b1;
    b_filt_b0[2] = filt_b2;
    coder::filter(filt_a0, b_filt_b0, SD->f0.clipped_data, n, &filt_w[i],
                  y_data, b_x);
    plugin->highFreqFilter.w[i] = b_x[0];
    plugin->highFreqFilter.w[i + 1] = b_x[1];
    for (i = 0; i < b_loop_ub; i++) {
      x[i + x.size(0) * ch] = y_data[i];
    }
    //  Assign processed data back to x
  }
  //  Sum all filtered outputs (low, mid, high)
  //  Assign the complete processed signal to out
  if (samplesPerFrame < 1.0) {
    b_i = 0;
  } else {
    b_i = static_cast<int>(samplesPerFrame);
  }
  o1_size[0] = b_i;
  for (i = 0; i < b_i; i++) {
    o1_data[i] = x[i];
  }
  o2_size[0] = b_i;
  for (i = 0; i < b_i; i++) {
    o2_data[i] = x[i + x.size(0)];
  }
}

void resetCImpl(MrShiftyStackData *SD, double rate)
{
  derivedAudioPlugin *plugin;
  double b_rate;
  plugin = getPluginInstance(SD);
  plugin->PrivateSampleRate = rate;
  //  Called when sample rate changes or plugin is reloaded
  b_rate = plugin->PrivateSampleRate;
  plugin->FS = b_rate;
  plugin->lowFreqFilter.w[0] = 0.0;
  plugin->midFreqFilter.w[0] = 0.0;
  plugin->highFreqFilter.w[0] = 0.0;
  plugin->lowFreqFilter.w[1] = 0.0;
  plugin->midFreqFilter.w[1] = 0.0;
  plugin->highFreqFilter.w[1] = 0.0;
  plugin->lowFreqFilter.w[2] = 0.0;
  plugin->midFreqFilter.w[2] = 0.0;
  plugin->highFreqFilter.w[2] = 0.0;
  plugin->lowFreqFilter.w[3] = 0.0;
  plugin->midFreqFilter.w[3] = 0.0;
  plugin->highFreqFilter.w[3] = 0.0;
}

// End of code generation (MrShifty.cpp)
