/*! @file api.cc
 *  @brief Tests for libSpeechFeatureExtraction API.
 *  @author Markovtsev Vadim <v.markovtsev@samsung.com>
 *  @version 1.0
 *
 *  @section Notes
 *  This code partially conforms to <a href="http://google-styleguide.googlecode.com/svn/trunk/cppguide.xml">Google C++ Style Guide</a>.
 *
 *  @section Copyright
 *  Copyright 2013 Samsung R&D Institute Russia
 */


#include <gtest/gtest.h>
#include <spfextr/api.h>
#include <boost/regex.hpp>
#include <fstream>
#include <streambuf>

TEST(API, query_transforms_list) {
  char** names = nullptr;
  int count = 0;
  query_transforms_list(&names, &count);
  ASSERT_GT(count, 0);
  ASSERT_NE(nullptr, names);
  for (int i = 0; i < count; i++) {
    ASSERT_NE(nullptr, names[i]);
  }
  destroy_transforms_list(names, count);
}

TEST(API, query_transform_details) {
  char* description = nullptr;
  char *inputFormat;
  char *outputFormat;
  char** pnames = nullptr;
  char** pdescs = nullptr;
  char** pdefs = nullptr;
  int count = 0;
  query_transform_details("Window", &description, &inputFormat, &outputFormat,
                          &pnames, &pdescs, &pdefs, &count);
  ASSERT_NE(nullptr, description);
  ASSERT_NE(nullptr, pnames);
  ASSERT_NE(nullptr, pdescs);
  ASSERT_NE(nullptr, pdefs);
  ASSERT_GT(count, 0);
  for (int i = 0; i < count; i++) {
    ASSERT_NE(nullptr, pnames[i]);
    ASSERT_NE(nullptr, pdescs[i]);
    ASSERT_NE(nullptr, pdefs[i]);
  }
  destroy_transform_details(description, inputFormat, outputFormat,
                            pnames, pdescs, pdefs, count);
}

TEST(API, extract_speech_features) {
  const char *feature = "MFCC [Window(length=32), RDFT, Energy, FilterBank, "
      "FilterBank, Log, Square, UnpackRDFT, DCT, Selector(length=24)]";
  auto config = setup_features_extraction(&feature, 1, 48000, 16000);
  ASSERT_NE(nullptr, config);
  auto buffer = new int16_t[48000];
  for (int i = 0; i < static_cast<int>(sizeof(buffer) / 2); i++) {
    buffer[i] = 1;
  }
  char **featureNames = nullptr;
  float **results = nullptr;
  int *lengths = nullptr;
  extract_speech_features(config, buffer, &featureNames,
                          reinterpret_cast<void ***>(&results), &lengths);
  ASSERT_NE(nullptr, featureNames);
  ASSERT_NE(nullptr, results);
  ASSERT_NE(nullptr, lengths);
  ASSERT_GT(lengths[0], 0);
  for (int i = 0; i < lengths[0] / 4; i++) {
    ASSERT_EQ(results[0][i], results[0][i]);
  }
  free_results(1, featureNames, reinterpret_cast<void **>(results), lengths);
  destroy_features_configuration(config);
  delete[] buffer;
}

FeaturesConfiguration* test_calculate_features() {
  const char *feature = "MFCC [Window(length=32), RDFT, Energy, FilterBank, "
      "FilterBank, Log, Square, UnpackRDFT, DCT, Selector(length=24)]";
  auto config = setup_features_extraction(&feature, 1, 48000, 16000);
  auto buffer = new int16_t[48000];
  for (int i = 0; i < static_cast<int>(sizeof(buffer) / 2); i++) {
    buffer[i] = 1;
  }
  char **featureNames = nullptr;
  float **results = nullptr;
  int *lengths = nullptr;
  extract_speech_features(config, buffer, &featureNames,
                          reinterpret_cast<void ***>(&results), &lengths);
  free_results(1, featureNames, reinterpret_cast<void **>(results), lengths);
  delete[] buffer;
  return config;
}

TEST(API, report_extraction_graph) {
  auto config = test_calculate_features();
  report_extraction_graph(config, "/tmp/test_report_extraction_graph.dot");
  struct stat res;
  ASSERT_EQ(0, stat("/tmp/test_report_extraction_graph.dot", &res));

  // The following checks for percent numbers saneness
  std::ifstream dotFile("/tmp/test_report_extraction_graph.dot");
  std::string dotStr((std::istreambuf_iterator<char>(dotFile)),
                     std::istreambuf_iterator<char>());
  static const boost::regex percentRegex("(\\d+)(?=%)");
  static const boost::sregex_token_iterator empty;
  boost::sregex_token_iterator percentIterator(
      dotStr.begin(), dotStr.end(), percentRegex, 1);
  ASSERT_NE(empty, percentIterator);
  do {
    std::string numberStr = *percentIterator++;
    int value = std::stoi(numberStr);
    ASSERT_GT(value, -1);
    ASSERT_LT(value, 101);
  }
  while (percentIterator != empty);

  // Check for dot file validity
  ASSERT_EQ(0, system("dot -Tpng /tmp/test_report_extraction_graph.dot "
      "-o/tmp/test_report_extraction_graph.png"));
  ASSERT_EQ(0, stat("/tmp/test_report_extraction_graph.png", &res));
  destroy_features_configuration(config);
}

TEST(API, report_extraction_time) {
  auto config = test_calculate_features();
  char** transformNames;
  float* values;
  int length;
  report_extraction_time(config, &transformNames, &values, &length);
  ASSERT_EQ(11 + 1, length);
  ASSERT_NE(nullptr, transformNames);
  ASSERT_NE(nullptr, values);
  for (int i = 0; i < length; i++) {
    ASSERT_NE(nullptr, transformNames[i]);
    ASSERT_GT(values[i], .0f);
    ASSERT_LT(values[i], 1.0f);
  }
  destroy_extraction_time_report(transformNames, values, length);
  destroy_features_configuration(config);
}

#include "tests/google/src/gtest_main.cc"
