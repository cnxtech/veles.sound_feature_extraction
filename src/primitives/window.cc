/*! @file window.c
 *  @brief New file description.
 *  @author Markovtsev Vadim <v.markovtsev@samsung.com>
 *  @version 1.0
 *
 *  @section Notes
 *  This code partially conforms to <a href="http://google-styleguide.googlecode.com/svn/trunk/cppguide.xml">Google C++ Style Guide</a>.
 *
 *  @section Copyright
 *  Copyright 2013 Samsung R&D Institute Russia
 */


#include "src/primitives/window.h"
#include <math.h>

const std::unordered_map<std::string, WindowType> WindowTypeMap {
  { "rectangular", WINDOW_TYPE_RECTANGULAR },
  { "hamming", WINDOW_TYPE_HAMMING }
};

/// @brief Calculates the element of Hamming window of length "length"
/// at index "index".
/// @note This function conforms to Matlab hamming() function.
static float HammingWindow(int index, int length) {
  return 0.54f - 0.46f * cosf(2 * M_PI * index / (length - 1));
}

float WindowElement(WindowType type, int length, int index) {
  switch (type) {
    case WINDOW_TYPE_RECTANGULAR:
      return 1.0f;
    case WINDOW_TYPE_HAMMING:
      return HammingWindow(index, length);
  }
  return 0.0f;
}
