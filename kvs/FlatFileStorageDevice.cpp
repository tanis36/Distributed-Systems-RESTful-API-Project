#include <cstdio>
#include <string>
#include <fstream>
#include <algorithm>
#include <stdexcept>
#include <sys/types.h>
#include <sys/stat.h>

#include "FlatFileStorageDevice.h"

using std::string;
using std::ifstream;
using std::ofstream;

bool FlatFileStorageDevice::exists(const string& loc) {
  // Helper function.
  ifstream ifs { loc };
  return ifs.is_open();
}

bool FlatFileStorageDevice::dir_exists(const string& dir) {

  struct stat info;
  
  // Helper function.
  if (stat(dir.c_str(), &info) != 0)
    return false;
  
  if (info.st_mode & S_IFDIR) 
    return true;

  return false;
  
}

FlatFileStorageDevice::FlatFileStorageDevice(const string& base_location) {

  if (!dir_exists(base_location))
    throw std::runtime_error("Directory " + base_location + " does not exist.");
  
  if (base_location[base_location.size() - 1] != '/')
    this->base_location = base_location + "/";
  else
    this->base_location = base_location;
}

void FlatFileStorageDevice::create_location(const string& location) {

  string filepath { this->base_location + location + ".txt" };

  if (exists(filepath))
    throw std::runtime_error("KVS " + location + " already exists, or the filepath " + filepath + " is wrong.");
  
  ofstream new_location { filepath };
  new_location.close();
  
}

string FlatFileStorageDevice::read(const string& location, const string& key) {

  // We assume that we're being handed the name of a KVS. If it exists, it will
  // be in the base_location filepath as <location>.txt.
  string filepath { this->base_location + location + ".txt" };
  
  ifstream ifs { filepath };
  if (!ifs.is_open())
    throw std::runtime_error("Could not open KVS location " + location + " at filepath " + filepath + ".");

  // No reason to construct a hash map here; that's a future optimization that
  // falls under caching.
  string temp;
  while (std::getline(ifs, temp)) {
  
    // Each key-value pair is a single line in the file, separated with a colon.
    // Everything before the colon is the key; everything after it, minus the
    // newline character, is the value. This has the consequence of preventing
    // usage of a colon in the key.
    // EXAMPLE:
    // this is all a key:the rest of this is a value
    // name:Green Goblin
    // his location:your house

    // Remove trailing newline.
    temp.erase(std::remove(temp.begin(), temp.end(), '\n'), temp.end());    
    
    // Split the key-value pair on the colon.
    auto delimiter_pos = temp.find(":");
    if (delimiter_pos == string::npos)
      throw std::runtime_error("Detected malformed key-value pair \"" + temp +"\" in KVS location " + location + " at filepath " + filepath + ".");
    
    string key_i = temp.substr(0, delimiter_pos);
    if (key_i == key)
      return temp.substr(delimiter_pos + 1);

  }

  // If this point is reached, the requested key was not found.
  throw std::runtime_error("Failed to find key \"" + key + "\" in KVS location " + location + " at filepath " + filepath + ".");
  
}

void FlatFileStorageDevice::write(const string& location, const string& key, const string& value) {

  string filepath { this->base_location + location + ".txt" };
  
  if (!exists(filepath))
    throw std::runtime_error("KVS " + location + " does not exist, or the filepath " + filepath + " is wrong.");

  // If the entry for this key already exists, we have to delete the old entry.
  bool key_found = false;
  bool remove_successful = false;
  try {

    string throwaway = read(location, key);
    key_found = true;

    remove(location, key);
    remove_successful = true;
    // If the key was found during the read() and the remove() was successful,
    // then no further work is required.
      
  }
  catch (std::exception e) {
    
    // If the key was found and the remove failed, in which case key_found
    // is true and remove_successful is false, we have a real error condition
    // to raise. The error message from the failure to remove the key should
    if (key_found && !remove_successful)
      throw std::runtime_error("While trying to overwrite key " + key + ", encountered an error: " + e.what());
    
    // If the entry did not exist, in which case key_found will be false,
    // no further work is required.
    
  }
  
  ofstream ofs;
  ofs.open(filepath, ofstream::app);
  
  // Build the key-value pair.
  string kvp = key + ":" + value + '\n';
  ofs << kvp;
  ofs.close();

}

void FlatFileStorageDevice::remove(const string& location, const string& key) {
  
  string filepath { this->base_location + location + ".txt" };
  
  if (!exists(filepath))
    throw std::runtime_error("KVS " + location + " does not exist, or the filepath " + filepath + " is wrong.");

  string temp_path = this->base_location + location + ".temp.txt";
  
  ifstream ifs { filepath };
  ofstream ofs { temp_path };
  if (!ofs)
    throw std::runtime_error("Temporary file " + temp_path + " could not be opened. Ensure the file does not already exist.");

  string temp;
  while (getline(ifs, temp)) {

    // Break apart each key-value pair for the key.
    auto delimiter_pos = temp.find(":");

    if (delimiter_pos == string::npos) {
      
      ofs.close();
      std::remove(temp_path.c_str());
      throw std::runtime_error("Detected malformed key-value pair \"" + temp +"\" in KVS location " + location + " at filepath " + filepath + ".");
      
    }

    string key_i = temp.substr(0, delimiter_pos);

    // If it's not the key we're going to delete, write it to the new copy.
    // If it is, ignore the line-- this will effectively delete it.
    if (key_i != key)
      ofs << temp << '\n';

  
  }

  ifs.close();
  ofs.close();


  
  // Delete the old file and rename the new file. These are C functions that
  // require C-style strings.
  std::remove(filepath.c_str());
  std::rename(temp_path.c_str(), filepath.c_str());
  
}