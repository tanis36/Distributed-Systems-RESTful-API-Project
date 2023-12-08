/*
This is just a small implementation of the StorageDevice interface for testing
purposes. If it's sufficient for our uses, we can continue to use it; if not,
we can easily implement the same StorageDevice interface with another method.
*/

#ifndef FLAT_FILE_STORAGE_DEVICE_H
#define FLAT_FILE_STORAGE_DEVICE_H

#include "StorageDevice.h"

class FlatFileStorageDevice : public StorageDevice {
public:

  FlatFileStorageDevice(const std::string& base_location);

  void create_location(const std::string& location);

  std::string read(const std::string& location, const std::string& key);

  void write(const std::string& location, const std::string& key, const std::string& value);

  void remove(const std::string& location, const std::string& key);
  
private:

  std::string base_location;

  // Helper function to check whether a given text file exists. C++17 has
  // good std::filesystem tools to do this, but not everyone is running it.
  // That includes my creaky old laptops which I'm too lazy to update with
  // Cygwin.
  bool exists(const std::string& loc);

  bool dir_exists(const std::string& dir);
  
};

#endif