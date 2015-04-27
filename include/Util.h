/*
 * Copyright (c) 2015 qqibrow@gmail.com
 */

//
// Created by Lu Niu on 4/26/15.
//

#ifndef INCLUDE_UTIL_H_
#define INCLUDE_UTIL_H_

#include <iomanip>
#include <locale>
#include <sstream>
#include <iomanip>

class comma_numpunct : public std::numpunct<char> {
 protected:
  virtual char do_thousands_sep() const { return ','; }

  virtual std::string do_grouping() const { return "\03"; }
};

template <class T>
std::string FormatWithCommas(T value) {
  std::stringstream ss;
  std::locale comma_locale(std::locale(), new comma_numpunct());
  ss.imbue(comma_locale);
  ss << std::setprecision(2) << std::fixed << value;
  return ss.str();
}

#endif  // INCLUDE_UTIL_H_
