#include "format.h"

#include <string>

using std::string;
using std::to_string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
  int hrs = seconds / 3600;
  int min = (seconds / 60) % 60;
  int sec = seconds % 60;
  string hh = hrs < 10 ? '0' + to_string(hrs) : to_string(hrs);
  string mm = min < 10 ? '0' + to_string(min) : to_string(min);
  string ss = sec < 10 ? '0' + to_string(sec) : to_string(sec);
  return string(hh + ':' + mm + ':' + ss);
}