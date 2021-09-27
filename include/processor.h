#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <vector>

class Processor {
 public:
  Processor();
  float Utilization();

 private:
  std::vector<long> cpu_stats_;
  std::vector<long> prev_cpu_stats_;
};

#endif