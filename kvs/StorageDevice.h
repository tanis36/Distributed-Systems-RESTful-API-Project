#ifndef STORAGE_DEVICE_H
#define STORAGE_DEVICE_H

#include <string>

class StorageDevice {
public:

  /**
  Attempts to create a new location.
  @param location The location to create. Locations should be unique.
  @throws std::runtime_error if the location was not successfully created (for
    example, if the location already exists).
  */
  virtual void create_location(const std::string& location) = 0;
  
  /**
  Accesses the given location in the database and attempts to find the given
  key, then returns its corresponding value.
  @param location The location in which to find the given key.
  @param key The key corresponding to the desired value.
  @throws std::runtime_error if the value is not found.
  */
  virtual std::string read(const std::string& location, const std::string& key) = 0;

  /**
  Accesses the given location and attempts to store the given key-value pair.
  @param location The location in which to place the given key-value pair.
  @param key The key to save.
  @param value The value to save.
  @throws std::runtime_error if the write operation was not successful.
  */
  virtual void write(const std::string& location, const std::string& key, const std::string& value) = 0;

  /**
  Accesses the given location and deletes the given key-value pair, if it exists.
  If it doesn't exist, or if the operation otherwise fails, returns false.
  @param location The location in which to find the key-value pair to delete.
  @param key The key belonging to the key-value pair to delete.
  @throws std::runtime_error if the delete operation was not successful.
  */
  virtual void remove(const std::string& location, const std::string& key) = 0;
  
};

#endif