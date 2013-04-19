/*! @file spectral_energy.cc
 *  @brief Calculate the magnitude of each complex number.
 *  @author Markovtsev Vadim <v.markovtsev@samsung.com>
 *  @version 1.0
 *
 *  @section Notes
 *  This code partially conforms to <a href="http://google-styleguide.googlecode.com/svn/trunk/cppguide.xml">Google C++ Style Guide</a>.
 *
 *  @section Copyright
 *  Copyright 2013 Samsung R&D Institute Russia
 */

#include "src/transforms/spectral_energy.h"
#include <math.h>
#ifdef __AVX__
#include <immintrin.h>
#elif defined(__ARM_NEON__)
#include <arm_neon.h>
#endif

namespace SoundFeatureExtraction {
namespace Transforms {

void SpectralEnergy::OnFormatChanged() {
  if (inputFormat_->Size() % 2 == 1) {
    fprintf(stderr, "Input buffer size is odd (%zu), truncated\n",
            inputFormat_->Size());
  }
  outputFormat_->SetSize(inputFormat_->Size() / 2);
}

void SpectralEnergy::InitializeBuffers(
    const BuffersBase<Formats::WindowF>& in,
    BuffersBase<Formats::WindowF>* buffers) const noexcept {
  buffers->Initialize(in.Size(), inputFormat_->Size() / 2);
}

void SpectralEnergy::Do(
    const BuffersBase<Formats::WindowF>& in,
    BuffersBase<Formats::WindowF>* out) const noexcept {
  for (size_t i = 0; i < in.Size(); i++) {
    Do(true, in[i]->Data.get(), inputFormat_->Size(), (*out)[i]->Data.get());
  }
}

void SpectralEnergy::Do(bool simd, const float* input, int length,
                float* output) noexcept {
  if (simd) {
#ifdef __AVX__
    for (int j = 0; j < length - 15; j += 16) {
      __m256 vec1 = _mm256_load_ps(input + j);
      __m256 vec2 = _mm256_load_ps(input + j + 8);
      vec1 = _mm256_mul_ps(vec1, vec1);
      vec2 = _mm256_mul_ps(vec2, vec2);
      __m256 r1 = _mm256_permute2f128_ps(vec1, vec2, 0x20);
      __m256 r2 = _mm256_permute2f128_ps(vec1, vec2, 0x31);
      __m256 res = _mm256_hadd_ps(r1, r2);
      _mm256_store_ps(output + j / 2, res);
    }
    for (int j = ((length >> 4) << 4); j < length; j += 2) {
      float re = input[j];
      float im = input[j + 1];
      output[j / 2] = re * re + im * im;
    }
  } else {
#elif defined(__ARM_NEON__)
    for (int j = 0; j < length - 3; j += 4) {
      float32x4_t cvec = vld1q_f32(input + j);
      float32x4_t sqrvec = vmulq_f32(cvec, cvec);
      float32x2_t sums = vpadd_f32(vget_high_f32(sqrvec),
                                   vget_low_f32(sqrvec));
      vst1_f32(output + j / 2, sums);
    }
    for (int j = ((length >> 2) << 2); j < length; j += 2) {
      float re = input[j];
      float im = input[j + 1];
      output[j / 2] = re * re + im * im;
    }
  } else {
#else
  } {
#endif
    for (int j = 0; j < length; j += 2) {
      float re = input[j];
      float im = input[j + 1];
      output[j / 2] = re * re + im * im;
    }
  }
}

REGISTER_TRANSFORM(SpectralEnergy);

}  // namespace Transforms
}  // namespace SoundFeatureExtraction