/*! @file preemphasis.cc
 *  @brief Tests for SoundFeatureExtraction::Transforms::Preemphasis.
 *  @author Markovtsev Vadim <v.markovtsev@samsung.com>
 *  @version 1.0
 *
 *  @section Notes
 *  This code partially conforms to <a href="http://google-styleguide.googlecode.com/svn/trunk/cppguide.xml">Google C++ Style Guide</a>.
 *
 *  @section Copyright
 *  Copyright 2013 Samsung R&D Institute Russia
 */

#include <math.h>
#include "src/transforms/preemphasis.h"
#include "tests/transforms/transform_test.h"

using SoundFeatureExtraction::Formats::RawFormat16;
using SoundFeatureExtraction::BuffersBase;
using SoundFeatureExtraction::Transforms::Preemphasis;

class PreemphasisTest : public TransformTest<Preemphasis> {
 public:
  int Size;

  virtual void SetUp() {
    Size = 1024;
    SetUpTransform(1, Size, 18000);
    for (int i = 0; i < Size; i++) {
      (*Input)[0][i] = i + 1;
    }
  }
};

TEST_F(PreemphasisTest, Do) {
  Do((*Input)[0], (*Output)[0]);
  ASSERT_EQ(1, (*Output)[0][0]);
  for (int i = 1; i < Size; i++) {
    ASSERT_NEAR(int16_t((1 - kDefaultK) * (i + 1) + kDefaultK),
                (*Output)[0][i], 1.f);
  }
}

#undef CLASS_NAME
#define CLASS_NAME PreemphasisTest
#undef INPUT_TYPE
#define INPUT_TYPE int16_t
#define PARAM_REORDER
#define EXTRA_PARAM 0.5f
#include "tests/transforms/benchmark.inc"