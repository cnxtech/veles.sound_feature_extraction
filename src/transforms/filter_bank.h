/*! @file filter_bank.h
 *  @brief Psychoacoustic scale conversion.
 *  @author Markovtsev Vadim <v.markovtsev@samsung.com>
 *  @version 1.0
 *
 *  @section Notes
 *  This code partially conforms to <a href="http://google-styleguide.googlecode.com/svn/trunk/cppguide.xml">Google C++ Style Guide</a>.
 *
 *  @section Copyright
 *  Copyright 2013 Samsung R&D Institute Russia
 */

#ifndef FILTER_BANK_H_
#define FILTER_BANK_H_

#include "src/formats/window_format.h"
#include "src/uniform_format_transform.h"

namespace SpeechFeatureExtraction {
namespace Transforms {

#define DEFAULT_FB_LENGTH 40
#define DEFAULT_FB_MIN_FREQ 130
#define DEFAULT_FB_MAX_FREQ 6854

class FilterBank
    : public UniformFormatTransform<Formats::WindowFormatF> {
 public:
  FilterBank();

  TRANSFORM_INTRO("FilterBank",
                  "Converts the signal to the selected psychoacoustic scale "
                  "(default is mel).")

  TRANSFORM_PARAMETERS(
      _TP_("type", "The type of the scale. Supported values are \"mel\" "
                   "and \"bark\".", "mel")
      _TP_("number", "The number of triangular filters.",
           std::to_string(DEFAULT_FB_LENGTH))
      _TP_("frequency_min", "Minimal frequency of the filter bank.",
           std::to_string(DEFAULT_FB_MIN_FREQ))
      _TP_("frequency_max", "Maximal frequency of the filter bank.",
           std::to_string(DEFAULT_FB_MAX_FREQ))
  )

  virtual bool HasInverse() const noexcept;

  virtual void Initialize() const noexcept;

 protected:
  enum ScaleType {
    SCALE_TYPE_MEL,
    SCALE_TYPE_BARK
  };

  ScaleType type_;
  size_t length_;
  int minFreq_;
  int maxFreq_;
  mutable std::shared_ptr<float> filterBank_;

  virtual void SetParameter(const std::string& name,
                            const std::string& value);

  virtual void TypeSafeInitializeBuffers(
      const BuffersBase<Formats::WindowF>& in,
      BuffersBase<Formats::WindowF>* buffers)
  const noexcept;

  virtual void TypeSafeDo(const BuffersBase<Formats::WindowF>& in,
                          BuffersBase<Formats::WindowF> *out) const noexcept;


  static float LinearToScale(ScaleType type, float freq);
  static float ScaleToLinear(ScaleType type, float value);

 private:
  /// @brief Adds a triangular filter to the filter bank.
  /// @param center The value of the peak of the triangle,
  /// in psychoacoustic scale units.
  /// @param halfWidth The half width of the base of the triangle,
  /// in psychoacoustic scale units.
  void AddTriangularFilter(float center, float halfWidth) const;
};

}  // namespace Transforms
}  // namespace SpeechFeatureExtraction
#endif  // INCLUDE_FILTER_BANK_H_
