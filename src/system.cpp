#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <iostream>
#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;


Processor& System::Cpu() { return cpu_; }

// just a helper to sort the processes by their cpu utilization
// it is NOT a member function
static bool CompareProcessCPUUtil(Process p1, Process p2) { 
	return p1 < p2;
}

vector<Process>& System::Processes() { 
  vector<int> pids = LinuxParser::Pids();
  processes_.clear(); 
  for(auto i : pids) {
  	processes_.push_back(Process(i));
  }
  sort(processes_.begin(), processes_.end(), CompareProcessCPUUtil);
  return processes_; 
}


std::string System::Kernel() { 
  return LinuxParser::Kernel();
}


float System::MemoryUtilization() {

  return LinuxParser::MemoryUtilization(); 
}


std::string System::OperatingSystem() { 
  return LinuxParser::OperatingSystem();
}


int System::RunningProcesses() { 
  return LinuxParser::RunningProcesses();
}


int System::TotalProcesses() { 
  return LinuxParser::TotalProcesses(); 
}


long int System::UpTime() { 
  return LinuxParser::UpTime(); 
}