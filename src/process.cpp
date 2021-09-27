#include "process.h"

#include <unistd.h>

#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid)
    : pid_(pid),
      elapsed_time_(ElapsedTime()),
      cpu_utilization_(CalculateCpu()) {}

int Process::Pid() { return pid_; }

long int Process::ElapsedTime() {
  return LinuxParser::Jiffies() - LinuxParser::UpTime(pid_);
}

float Process::CalculateCpu() {
  long total_time = LinuxParser::ActiveJiffies(pid_);
  return total_time * 1.0 / elapsed_time_;
}

float Process::CpuUtilization() const { return cpu_utilization_; }

string Process::Command() { return LinuxParser::Command(pid_); }

string Process::Ram() { return LinuxParser::Ram(pid_); }

string Process::User() {
  return LinuxParser::User(stoi(LinuxParser::Uid(pid_)));
}

long int Process::UpTime() { return elapsed_time_ / sysconf(_SC_CLK_TCK); }

bool Process::operator>(Process const& a) const {
  return CpuUtilization() > a.CpuUtilization();
}
