#ifndef KEY_STORAGE_H
#define KEY_STORAGE_H

#include <string>

#include "StorageDevice.h"

class KeyStorage {
public:

  KeyStorage() = default;
  ~KeyStorage() = default;

  /**
  Initialize the KVS.
  @param directory_name The name of the filesystem folder where
    the persistent data for the KVS's will be located. If the folder does not
    exist, it will be created.
  @throws std::runtime_error Thrown if the folder does not exist
    and creating the folder fails.
  */
  void initialize(const std::string& directory_name);

  /**
  Create a new KVS with the specified name.
  @param name The name of the KVS to create. Names should be unique.
  @throws std::runtime_error Thrown if the KVS cannot be created (if it already
    exists or the name is otherwise invalid).
  */
  void create_kvs(const std::string& name);

  /**
  Add a key-value pair to the KVS with the given name.
  @param name The name of the KVS in which to store the key-value pair.
  @param key The key to insert.
  @param value The value to insert.
  @throws std::runtime_error Thrown if the KVS does not exist or if the key
    is already present.
  */
  void add(const std::string& name, const std::string& key, const std::string& value);

  /**
  Deletes the key-value pair with the given key from the named KVS. Does nothing
  if the key-value pair doesn't exist.
  @param name The name of the KVS from which to delete the key-value pair.
  @param key The key to delete.
  @throws std::runtime_error Thrown if the named KVS does not exist.
  */
  void remove(const std::string& name, const std::string& key);

  /**
  Replaces the value associated with a given key in the named KVS.
  @param name The name of the KVS in which the key-value pair to update resides.
  @param key The key which maps to the value to update.
  @param value The value to update the key-value pair with.
  @throws std::runtime_error Thrown if the KVS does not exist or if the key
    does not exist. 
  */
  void update(const std::string& name, const std::string& key, const std::string& value);

  /**
  Returns a copy of the value associated with the given key in the named KVS.
  @param name The name of the KVS in which the key-value pair is located.
  @param key The key corresponding to the desired value.
  @return The value corresponding to the given key.
  @throws std::runtime_error Thrown if the KVS does not exist or if the key
    does not exist.
  */
  std::string find(const std::string& name, const std::string& key);

private:

  StorageDevice* storage_device;
  
};

#endif