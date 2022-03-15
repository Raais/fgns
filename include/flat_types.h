/*
Flat block:
  "virtual" depth.
  All files are at root / in a contiguous vector.
  Files can be regular files or directories.
  Directories only store IDs of constituents.

Tree block:
  "physical" depth.
  Files == regular files; Directories == vector of files;
  Files are contained in vectors and nested vectors simulating
    real directories and subdirectories.
*/

#pragma once

#include <string>
#include <ctime>

#include <cereal/cereal.hpp>

#define FLAT_BLOCK_EXTENSION ".cfbo" // "Cereal"ized flat block object //

namespace FGNS
{

namespace Flat
{

struct File
{
    unsigned int TIMESTAMP;
    std::string  NAME;
    unsigned int ID;
    signed int   PARENT;

    //Regular File
    bool         ENCRYPTED;
    std::string  HASH;
    std::string  SALT;
    std::string  IV;

    std::string  content;

    //Directory
    bool         DIRECTORY;

    std::vector<unsigned int> directories;
    std::vector<unsigned int> files;

    File(){}
    File(unsigned int &IDSEED, signed int &PARENT_ID) : TIMESTAMP((unsigned int)std::time(NULL)), NAME(""), ID(IDSEED),
      PARENT(PARENT_ID), ENCRYPTED(false), HASH(""), SALT(""), IV(""), content(""), DIRECTORY(false){}

  private:
    friend class cereal::access;
    template <class Archive> void serialize(Archive &ar)
    {
        ar(CEREAL_NVP(TIMESTAMP), CEREAL_NVP(NAME), CEREAL_NVP(ID), CEREAL_NVP(PARENT), CEREAL_NVP(ENCRYPTED), CEREAL_NVP(HASH),
          CEREAL_NVP(SALT), CEREAL_NVP(IV), CEREAL_NVP(content), CEREAL_NVP(DIRECTORY), CEREAL_NVP(directories), CEREAL_NVP(files));
    }
};

struct Block
{
    unsigned int TIMESTAMP;
    unsigned int IDSEED;
    bool         SAVED;
    bool         AUTOSAVE;
    signed int   WORKDIR;

    std::vector<File> root;

    Block() : TIMESTAMP((unsigned int)std::time(NULL)), IDSEED(0), SAVED(true), AUTOSAVE(true), WORKDIR(-1){}

  private:
    friend class cereal::access;
    template <class Archive> void serialize(Archive &ar)
    {
        ar(CEREAL_NVP(TIMESTAMP), CEREAL_NVP(IDSEED), CEREAL_NVP(SAVED), CEREAL_NVP(AUTOSAVE), CEREAL_NVP(WORKDIR), CEREAL_NVP(root));
    }
};

}

}