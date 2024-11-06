#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <functional>

namespace Serializer
{
  void in(std::string filepath, std::function<void(std::ifstream &)> callback)
  {
    std::ifstream stream(filepath, std::ios::binary);
    if (stream)
    {
      callback(stream);
      stream.close();
      return;
    }
    printf("%s", "SerializerError: Failed to read file.");
  }

  void out(std::string filepath, std::function<void(std::ofstream &)> callback)
  {
    std::ofstream stream(filepath, std::ios::binary);
    if (stream)
    {
      callback(stream);
      stream.close();
      return;
    }
    printf("%s", "SerializerError: Failed to write to file.");
  }

  namespace Serialize
  {
    void string(std::ofstream &stream, std::string &string)
    {
      // Get the size of the string. This will be the number of characters in the string.
      size_t size = string.size();

      // Write the size of the string to the file first.
      // The size is written as raw binary data, so we cast the address of `size` to a pointer of type `const char*`.
      // `sizeof(size)` ensures we write the exact amount of bytes required to store the size (typically 8 bytes on most platforms).
      stream.write((const char *)(&size), sizeof(size));

      // Write the actual string data to the file.
      // `c_str()` returns a pointer to a null-terminated C-style string.
      // `size` is the number of characters in the string, so we write exactly that many bytes.
      stream.write(string.c_str(), size);
    }
  }

  namespace Deserialize
  {
    void string(std::ifstream &stream, std::string &string)
    {
      // Declare a variable to store the size of the string.
      // This will be used to know how many bytes to read for the string content.
      size_t size;

      // Read the size of the string from the file first.
      // The size is read as raw binary data, so we cast the address of `size` to `char*` to match the expected type for `stream.read`.
      // `sizeof(size)` ensures we read the exact number of bytes required to store the size (typically 8 bytes on most platforms).
      stream.read((char *)(&size), sizeof(size));

      // Resize the string to hold the data we are about to read.
      // The size read from the file tells us how many characters to allocate in the string.
      string.resize(size);

      // Read the actual string data from the file.
      // The string's internal buffer is resized, so we can safely use `&string[0]` to access the underlying memory where the string's data is stored.
      // We read exactly `size` characters from the file into the string.
      stream.read(&string[0], size);
    }
  }
}