#include "system.h"

#include <unistd.h>

#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

System::System() {
  operating_system_ = LinuxParser::OperatingSystem();
  kernel_ = LinuxParser::Kernel();
  sys_uptime_ = LinuxParser::UpTime();
}

Processor& System::Cpu() { return cpu_; }

void System::UpdateProcesses() {
  processes_.clear();
  for (int i : LinuxParser::Pids()) processes_.push_back(Process{i});
  sort(processes_.begin(), processes_.end(), std::greater<Process>());
}

vector<Process>& System::Processes() {
  UpdateProcesses();
  return processes_;
}

string System::Kernel() { return kernel_; }

float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

string System::OperatingSystem() { return operating_system_; }

int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

long int System::UpTime() { return sys_uptime_++; }