#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
#define SECONDS_PER_HOUR 3600;
#define SECONDS_PER_MIN 60

string Format::ElapsedTime(long seconds) { 
  string rst = "";
  int h = seconds / SECONDS_PER_HOUR;
  rst += std::to_string(h) + ":";
  int h_part = h * SECONDS_PER_HOUR;
  int m = (seconds - h_part) / SECONDS_PER_MIN;
  rst += std::to_string(m) + ":";
  int m_part = m * SECONDS_PER_MIN;
  int s = seconds - h_part - m_part;
  rst += std::to_string(s);
  return rst; 
}