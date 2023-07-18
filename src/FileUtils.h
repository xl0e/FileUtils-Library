#ifndef FileUtils_h
#define FileUtils_h

#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <Logger.h>

#if defined (ESP32)
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

#ifndef SDCARD_SS
#define SDCARD_SS 15
#endif
#ifdef ESP8266
#define SDCARD_CLK 14
#define SDCARD_MOSI 13
#define SDCARD_MISO 12
#endif

typedef void (*FileFunc)(File &file);

class FileUtilsClass
{
public:
  File readFile(const String &fileName)
  {
    init();
    return SD.open(fileName, FILE_READ);
  }

  File writeFile(const String &fileName)
  {
    init();
    return SD.open(fileName, FILE_WRITE);
  }

  bool exists(const String &fileName)
  {
    init();
    return SD.exists(fileName);
  }

  void removeFile(const String &fileName)
  {
    init();
    SD.remove(fileName);
  }

  void move(const String &fileFrom, const String &fileTo)
  {
    init();
    SD.rename(fileFrom, fileTo);
  }

  void iterateFiles(String dirName, FileFunc fileFunc)
  {
    init();
    File dir = SD.open(dirName);
    iterateRecursive(dir, fileFunc);
  }

  bool hasFiles(const String &dirName)
  {
    init();
    File dir = SD.open(dirName);
    return hasFiles(dir);
  }

private:
  bool inited = false;
  bool init()
  {
    if (!inited)
    {
      logger.println("Checking SD card... ");

      if (!SD.begin(SDCARD_SS))
      {
        logger.println("Cannot read SD card");
        return false;
      }
      logger.println("SD card is mounted");
      inited = true;
    }
    return true;
  }

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
};

FileUtilsClass FileUtils;

#endif
