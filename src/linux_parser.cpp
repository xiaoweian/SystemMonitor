#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include "linux_parser.h"
#include <unordered_map>

using std::stof;
using std::string;
using std::to_string;
using std::vector;
using std::unordered_map;
// DONE: An example of how to read data from the filesystem
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

// DONE: An example of how to read data from the filesystem
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

// BONUS: Update this to use std::filesystem
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
        //std::cout << "pid = " << pid << "\n";
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {  
  string line;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    int n = 5; // here we only need the first 5 lines
    unordered_map<string, int> umap;
    umap["MemTotal:"] = 0; // init the keys that we expect
    umap["MemFree:"] = 0;
    umap["MemAvailable:"] = 0;
    umap["Buffers:"] = 0;
    umap["Cached:"] = 0;
    while (std::getline(filestream, line) && n > 0) {
      string key, val, unit;
      std::istringstream linestream(line);
      linestream >> key >> val >> unit;
      
      umap[key] = std::stoi(val);
      //std::cout << umap[key] << "\n";
      n--;
    }
    
    //below algo is refered to Linux htop code
   float usedMem = umap["MemTotal:"] - umap["MemFree:"];
   usedMem -= umap["Buffers:"] + umap["Cached:"];
   //float rst = umap["MemTotal:"] / usedMem;
   //std::cout << "rst = " << rst << "\n";
    return umap["MemTotal:"] / usedMem;
  }
  return 0.0; 
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  string systemUpTime, ideleTime, line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    //std::cout << line << "\n";
    std::istringstream linestream(line);
    linestream >> systemUpTime >> ideleTime;
    //std::cout << "s: " << long(std::stof(systemUpTime)) << "\n";
    //std::cout << "i: " << ideleTime << "\n";
    return long(std::stof(systemUpTime));
  }
  return 0; 
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
 // it seems that we don't need this function...
  return 0; 
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) { 
  // it seems that we don't need this function...
  return 0; 
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  // it seems that we don't need this function...
  return 0; 
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  // it seems that we don't need this function...
  return 0; 
}

// TODO: Read and return CPU utilization
float LinuxParser::CpuUtilization() { 
  string item_name; // "CPU"
  string user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice; // cpu time info
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    //std::cout << line << "\n";
    std::istringstream linestream(line);
    linestream >> item_name >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;
    
    // Guest time is already accounted in usertime
	long usertime = std::stol(user) - std::stol(guest);   
	long nicetime = std::stol(nice) - std::stol(guest_nice);

	long idlealltime = std::stol(idle) + std::stol(iowait); 
	long systemalltime = std::stol(system) + std::stol(irq) + std::stol(softirq);
	long virtalltime = std::stol(guest) + std::stol(guest_nice);
	long totaltime = usertime + nicetime + systemalltime + idlealltime + std::stol(steal) + virtalltime;
    
    return (float(totaltime - idlealltime)) / totaltime;
  }
  
  return 0.0; 
}

float LinuxParser::CpuUtilization(int pid) { 
  string starttime, utime, stime, cutime, cstime; // the 22th arg 
  long n_starttime, n_utime, n_stime, n_cutime, n_cstime;
  string line;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    //std::cout << line << "\n";
    std::istringstream linestream(line);
    int idx = 1;
    string trashcan;
    while(idx < 14) {
    	linestream >> trashcan;
      	idx++;
    }
    linestream >> utime >> stime >> cutime >> cstime;
    idx = 18;
    while(idx < 22) {
    	linestream >> trashcan;
      	idx++;
    }
    linestream >> starttime;
    
    //std::cout << "@@ " << utime << " @@ " << stime << " @@ " << cutime << " @@ " << cstime << " @@ " << starttime << "\n";
    
    //below code is to calculated the Cpu utilization of a specified process
    n_starttime = std::stol(starttime); 
    n_utime = std::stol(utime);
    n_stime = std::stol(stime);
    n_cutime = std::stol(cutime);
    n_cstime = std::stol(cstime);
    
    long total_time = n_utime + n_stime + n_cutime + n_cstime; //include the time from children processes
    long uptime =  UpTime();
    long seconds = uptime - (n_starttime / sysconf(_SC_CLK_TCK));
    
    
    return 100 * ((float(total_time) / sysconf(_SC_CLK_TCK)) / seconds);
  }
  
  return 0.0; 
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string line, key, val;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while(std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> val;
      if(key == "processes") {
          return std::stoi(val);
      }
    }

  }
  return 0;
}

string GetProcessState(int pid, string attri) { // just a helper
  std::ifstream filestream(LinuxParser::kProcDirectory + std::to_string(pid) + LinuxParser::kStatusFilename);
  string line;
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      string key, val, descr;
      std::istringstream linestream(line);
      linestream >> key >> val >> descr;
      
      if(key == attri) {
        //std::cout << "val = " << val << "\n";
      	return val;
      }
    }
    
  }
  return ""; 
}
// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  string line, key, val;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while(std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> val;
      if(key == "procs_running") {
          return std::stoi(val);
      }
    }

  }
  return 0;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  string line, cmd;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    //std::cout << line << "\n";
    std::istringstream linestream(line);
    linestream >> cmd;
    //std::cout << "s: " << long(std::stof(systemUpTime)) << "\n";
    //std::cout << "i: " << ideleTime << "\n";
    return cmd;
  }
  return "open failed!!";
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  return GetProcessState(pid, "VmSize:");
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  return GetProcessState(pid, "Uid:");
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  string uid = Uid(pid);
  std::ifstream filestream(kPasswordPath);
  string line;
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      //std::cout << line << "\n";
      string usr, x, id, left;
      std::istringstream linestream(line);
      linestream >> usr >> x >> id >> left;
      
      if(id == uid) {
        //std::cout << "usr = " << usr << "\n";
      	return usr;
      }
    }
    
  }
  return "";  
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  //here we only need "starttime"
  string starttime; // the 22th arg 
  string line;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    //std::cout << line << "\n";
    std::istringstream linestream(line);
    int idx = 1;
    string trashcan;
    while(idx < 22) {
    	linestream >> trashcan;
      	idx++;
    }
    linestream >> starttime;
    
    
    return (std::stol(starttime)) / sysconf(_SC_CLK_TCK);
  }
  
  return 0; 
}
