#ifndef FileUtils_h
#define FileUtils_h

#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <Logger.h>

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
  File readFile(String fileName)
  {
    init();
    return SD.open(fileName, FILE_READ);
  }

  void removeFile(String fileName)
  {
    init();
    SD.remove(fileName);
  }

  void iterateFiles(String dirName, FileFunc fileFunc)
  {
    init();
    File dir = SD.open(dirName);
    iterateRecursive(dir, fileFunc);
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

  void iterateRecursive(File dir, FileFunc fileFunc)
  {
    while (true)
    {
      File entry = dir.openNextFile();
      if (!entry)
      {
        logger.println("no more files");
        break;
      }

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
};

FileUtilsClass FileUtils;

#endif
