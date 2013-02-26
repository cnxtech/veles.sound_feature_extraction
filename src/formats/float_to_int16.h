/*! @file intF_to_int16.h
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

#ifndef FLOAT_TO_INT16_H_
#define FLOAT_TO_INT16_H_

#include "src/formats/raw_format.h"
#include "src/formats/raw_format_converter_base.h"
#include "src/formats/window_format.h"
#include "src/formats/window_format_converter_base.h"

namespace SpeechFeatureExtraction {
namespace Formats {

class FloatToInt16Raw
    : public RawFormatConverterBase<RawFormatF, RawFormat16> {
 protected:
  virtual void TypeSafeDo(const BuffersBase<RawF>& in,
                          BuffersBase<Raw16> *out) const noexcept;
};

class FloatToInt16Window
    : public WindowFormatConverterBase<WindowFormatF, WindowFormat16> {
 protected:
  virtual void TypeSafeDo(const BuffersBase<WindowF>& in,
                          BuffersBase<Window16> *out) const noexcept;
};

}  // namespace Formats
}  // namespace SpeechFeatureExtraction
#endif  // INCLUDE_FLOAT_TO_INT16_H_