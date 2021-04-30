#include <string>

#include "format.h"

using std::string;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
#define SECONDS_PER_HOUR 3600;
#define SECONDS_PER_MIN 60

string Format::ElapsedTime(long seconds) { 
  string rst = "";
  int h = seconds / SECONDS_PER_HOUR;
  //for output "HH" it adds "0" if h is a single number 
  rst += (h < 10 ? "0" : "") + std::to_string(h) + ":"; 
  int h_part = h * SECONDS_PER_HOUR;
  int m = (seconds - h_part) / SECONDS_PER_MIN;
  //for output "MM" it adds "0" if m is a single number 
  rst += (m < 10 ? "0" : "") + std::to_string(m) + ":";
  int m_part = m * SECONDS_PER_MIN;
  int s = seconds - h_part - m_part;
  //for output "SS" it adds "0" if s is a single number 
  rst += (s < 10 ? "0" : "") + std::to_string(s);
  return rst; 
}