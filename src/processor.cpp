#include "processor.h"

#include "linux_parser.h"

Processor::Processor() : prev_cpu_stats_(10, 0) {}

float Processor::Utilization() {
  cpu_stats_ = LinuxParser::CpuUtilization();
  long prev_idle = LinuxParser::IdleJiffies(prev_cpu_stats_);
  long idle = LinuxParser::IdleJiffies(cpu_stats_);
  long prev_total = LinuxParser::ActiveJiffies(prev_cpu_stats_) +
                    LinuxParser::IdleJiffies(prev_cpu_stats_);
  long total = LinuxParser::ActiveJiffies(cpu_stats_) +
               LinuxParser::IdleJiffies(cpu_stats_);

  long totald = total - prev_total;
  long idled = idle - prev_idle;
  prev_cpu_stats_ = cpu_stats_;
  return (totald - idled) * 1.0 / totald;
}