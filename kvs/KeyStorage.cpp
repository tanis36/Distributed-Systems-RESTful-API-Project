#include <string>
#include <stdexcept>

#include "KeyStorage.h"
#include "FlatFileStorageDevice.h"

using std::string;

void KeyStorage::initialize(const string& directory_name) {

  // Attempt to create a StorageDevice at the specified location.
  // If anything goes wrong, the error will propagate on its own.
  // Around these parts, we fail often and we fail DESCRIPTIVELY,
  // daggummit.

  this->storage_device = new FlatFileStorageDevice(directory_name);
  
}

void KeyStorage::create_kvs(const string& name) {

  if (!this->storage_device)
    throw std::runtime_error("Cannot create KVS without first initialize()ing.");

  this->storage_device->create_location(name);
  
}

void KeyStorage::add(const std::string& name,const std::string& key,const std::string& value){
    bool key_found = false;
    try{
        auto str = this->storage_device->read(name, key);
        key_found = true;
        throw std::runtime_error("Error: Key already has assigned value. Use update().");
    }
    catch(std::exception e) {
        if (key_found)
            throw e;
        else {
            this->storage_device->write(name, key, value);
        }
    }
}

void KeyStorage::remove(const std::string& name,const std::string& key){
    this->storage_device->remove(name, key);
}

void KeyStorage::update(const std::string& name, const std::string& key, const std::string& value) {
    // Find the existing value for the key provided
    std::string existing_value = find(name, key);

    // update() will crash if find() does not return a value.
    // This means that either the key was not found or the
    // named KVS does not exist. We assume that find() works
    // correctly otherwise.

    // We can proceed to call write(), which will overwrite
    // the existing key with the new one (even though they are
    // the same, this acts as a way of updating the value while
    // keeping the "original" key).

    this->storage_device->write(name, key, value);
}

std::string KeyStorage::find(const std::string& name, const std::string& key) {
    // Search the KVS for the provided key
    std::string value = this->storage_device->read(name, key);

    // find() will crash if read() does not return a value,
    // meaning that the key was not found. We assume that
    // read() works correctly otherwise.

    // At this point, we can just return value.
    return value;
}