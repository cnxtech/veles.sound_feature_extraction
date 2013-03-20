/*! @file dft.cc
 *  @brief Discrete Fourier Transform using FFT.
 *  @author Markovtsev Vadim <v.markovtsev@samsung.com>
 *  @version 1.0
 *
 *  @section Notes
 *  This code partially conforms to <a href="http://google-styleguide.googlecode.com/svn/trunk/cppguide.xml">Google C++ Style Guide</a>.
 *
 *  @section Copyright
 *  Copyright 2013 Samsung R&D Institute Russia
 */

#include "src/transforms/rdft.h"
#include <fftf/api.h>
#include <vector>
#include "src/primitives/arithmetic-inl.h"

namespace SpeechFeatureExtraction {
namespace Transforms {

bool RDFT::HasInverse() const noexcept {
  return true;
}

void RDFT::OnFormatChanged() {
  if (!IsInverse()) {
    outputFormat_->SetSize(inputFormat_->Size() + 2);
  } else {
    outputFormat_->SetSize(inputFormat_->Size() - 2);
  }
}

void RDFT::InitializeBuffers(
    const BuffersBase<Formats::WindowF>& in,
    BuffersBase<Formats::WindowF>* buffers) const noexcept {
  buffers->Initialize(in.Size(), outputFormat_->Size());
}

void RDFT::Do(
    const BuffersBase<Formats::WindowF>& in,
    BuffersBase<Formats::WindowF>* out) const noexcept {
  int length = outputFormat_->Size();
  if (!IsInverse()) {
    length -= 2;
  }
  std::vector<float*> inputs(in.Size()), outputs(in.Size());
  for (size_t i = 0; i < in.Size(); i++) {
    inputs[i] = in[i]->Data.get();
    outputs[i] = (*out)[i]->Data.get();
  }
  auto fftPlan = fftf_init_batch(
      FFTF_TYPE_REAL,
      IsInverse()? FFTF_DIRECTION_BACKWARD : FFTF_DIRECTION_FORWARD,
      FFTF_DIMENSION_1D,
      &length,
      FFTF_NO_OPTIONS,
      in.Size(),
      &inputs[0], &outputs[0]);

  fftf_calc(fftPlan);
  if (IsInverse()) {
    for (size_t i = 0; i < in.Size(); i++) {
      real_multiply_scalar(outputs[i], length, 1.0f / length, outputs[i]);
    }
  }

  fftf_destroy(fftPlan);
}

REGISTER_TRANSFORM(RDFT);

}  // namespace Transforms
}  // namespace SpeechFeatureExtraction