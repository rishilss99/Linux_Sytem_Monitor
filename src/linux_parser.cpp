#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <sstream>
#include <string>
#include <vector>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

float LinuxParser::MemoryUtilization() {
  float mem_total, mem_free;
  string line;
  string label;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> label;
      if (label == "MemTotal:")
        linestream >> mem_total;
      else if (label == "MemFree:") {
        linestream >> mem_free;
        break;
      }
    }
  }
  return (mem_total - mem_free) / mem_total;
}

long LinuxParser::UpTime() {
  long sys_uptime;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> sys_uptime;
  }
  return sys_uptime;
}

long LinuxParser::Jiffies() { return UpTime() * sysconf(_SC_CLK_TCK); }

long LinuxParser::ActiveJiffies(int pid) {
  long utime, stime, cutime, cstime;
  string line;
  string value;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    for (int i = 1; i <= 17; i++) {
      linestream >> value;
      if (i == 14)
        utime = stol(value);
      else if (i == 15)
        stime = stol(value);
      else if (i == 16)
        cutime = stol(value);
      else if (i == 17)
        cstime = stol(value);
    }
  }
  return utime + stime + cutime + cstime;
}

long LinuxParser::ActiveJiffies(const vector<long>& cpu_stats) {
  return cpu_stats[kUser_] + cpu_stats[kNice_] + cpu_stats[kSystem_] +
         cpu_stats[kIRQ_] + cpu_stats[kSoftIRQ_] + cpu_stats[kSteal_];
}

long LinuxParser::IdleJiffies(const vector<long>& cpu_stats) {
  return cpu_stats[kIdle_] + cpu_stats[kIOwait_];
}

vector<long> LinuxParser::CpuUtilization() {
  string line;
  string label;
  long value;
  vector<long> cpu_stats;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> label;
    while (linestream >> value) cpu_stats.push_back(value);
  }
  return cpu_stats;
}

int LinuxParser::SystemProcesses(std::string process_label) {
  int processes;
  string line;
  string label;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> label;
      if (label == process_label) {
        linestream >> processes;
        break;
      }
    }
  }
  return processes;
}

int LinuxParser::TotalProcesses() {
  return LinuxParser::SystemProcesses("processes");
}

int LinuxParser::RunningProcesses() {
  return LinuxParser::SystemProcesses("procs_running");
}

string LinuxParser::Command(int pid) {
  string pid_command;
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> pid_command;
  }
  return pid_command;
}

string LinuxParser::Ram(int pid) {
  string label;
  long ram;
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> label;

      // Used "VmData" instead of "VmSize" as it gives the exact physical memory being used as a part of Physical RAM
      // Resource : https://man7.org/linux/man-pages/man5/proc.5.html
      if (label == "VmData:") {  
        linestream >> ram;
        break;
      }
    }
  }
  ram *= 0.001;
  return to_string(ram);
}

string LinuxParser::Uid(int pid) {
  string uid, label;
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> label;
      if (label == "Uid:") {
        linestream >> uid;
        break;
      }
    }
  }
  return uid;
}

string LinuxParser::User(int uid) {
  string line;
  string user, password, match_uid;
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream{line};
      std::getline(linestream, user, ':');
      std::getline(linestream, password, ':');
      std::getline(linestream, match_uid, ':');
      if (stoi(match_uid) == uid) break;
    }
  }
  return user;
}

long LinuxParser::UpTime(int pid) {
  long starttime;
  string line;
  string value;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    for (int i = 1; i <= 22; i++) {
      linestream >> value;
      if (i == 22) starttime = stol(value);
    }
  }
  return starttime;
}
