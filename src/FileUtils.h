#ifndef FileUtils_h
#define FileUtils_h

#include <Arduino.h>
#include <SD.h>
typedef void (*FileFunc)(File &file);

namespace FileUtils
{

#if defined(ESP32)
  bool isFile(File &f)
  {
    return f && !f.isDirectory();
  }
#else
  bool isFile(File &f)
  {
    return f.isFile();
  }
#endif

  void iterateRecursive(File &dir, FileFunc fileFunc)
  {
    File entry;
    while (entry = dir.openNextFile())
    {
      if (entry.isDirectory())
      {
        iterateRecursive(entry, fileFunc);
      }
      else
      {
        fileFunc(entry);
      }
      entry.close();
    }
  }

  void iterateRecursive(String dirName, FileFunc fileFunc)
  {
    File dir = SD.open(dirName);
    iterateRecursive(dir, fileFunc);
  }

  bool hasFiles(File &dir)
  {
    bool isDir = dir.isDirectory();
    File entry;
    while (isDir && (entry = dir.openNextFile()))
    {
      if (isFile(entry) || hasFiles(entry))
      {
        entry.close();
        return true;
      }
      entry.close();
    }
    return false;
  }

  bool hasFiles(const String &dirName)
  {
    File dir = SD.open(dirName);
    return hasFiles(dir);
  }
}

#endif
