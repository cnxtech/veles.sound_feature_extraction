/*! @file rectify.cc
 *  @brief Tests for SoundFeatureExtraction::Transforms::RECTIFY.
 *  @author Shapichev Alexey <a.shapichev@samsung.com>
 *  @version 1.0
 *
 *  @section Notes
 *  This code partially conforms to <a href="http://google-styleguide.googlecode.com/svn/trunk/cppguide.xml">Google C++ Style Guide</a>.
 *
 *  @section Copyright
 *  Copyright 2013 Samsung R&D Institute Russia
 */

#include <gtest/gtest.h>
#include <math.h>
#include "src/transforms/rectify.h"

using SoundFeatureExtraction::Formats::WindowF;
using SoundFeatureExtraction::Formats::WindowFormatF;
using SoundFeatureExtraction::BuffersBase;
using SoundFeatureExtraction::Transforms::RectifyWindow;

class RectifyTest : public RectifyWindow, public testing::Test {
 public:
  BuffersBase<WindowF> Input;
  BuffersBase<WindowF> Output;
  int Size;

  RectifyTest()
      : Input(inputFormat_),
        Output(outputFormat_) {
  }

  virtual void SetUp() {
    Size = 486;
    Input.Initialize(1, Size);
    for (int i = 0; i < Size; ++i) {
      Input[0][i] = i;
      if (i % 2 == 0) {
        Input[0][i] *= -1;
      }
    }
    auto format = std::make_shared<WindowFormatF>(Size * 1000 / 18000, 18000);
    format->SetAllocatedSize(2 * Size);
    SetInputFormat(format);
    InitializeBuffers(Input, &Output);
  }
};

TEST_F(RectifyTest, Do) {
  Do(Input[0], &Output[0]);
  for (int i = 0; i < Size; i++) {
    ASSERT_EQ(i, Output[0][i]);
  }
}

#define CLASS_NAME RectifyTest
#define ITER_COUNT 500000
#define BENCH_FUNC RectifyBase::Do
#include "tests/transforms/benchmark.inc"

#include "tests/google/src/gtest_main.cc"
