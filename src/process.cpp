#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"
using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) {
	this->pid = pid;
}

int Process::Pid() { 
  return pid; 
}


float Process::CpuUtilization() const { 
  
  return LinuxParser::CpuUtilization(pid); 
}


string Process::Command() { return 
  LinuxParser::Command(pid);
}


string Process::Ram() { 
  return LinuxParser::Ram(pid);
}


string Process::User() { 
  return LinuxParser::User(pid); 
}


long int Process::UpTime() { 
  return LinuxParser::UpTime(pid); 
}
// ">" is used to compare two processes' cpu utilization,
// which is used for pids displaying orderly
bool Process::operator<(Process const& a) const {
  
  return CpuUtilization() < a.CpuUtilization(); 
}