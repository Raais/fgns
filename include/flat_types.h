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

    //Regular File
    bool         ENCRYPTED;
    std::string  HASH;
    std::string  SALT;
    unsigned int PARENT;

    std::string  content;

    //Directory
    bool         DIRECTORY;

    std::vector<unsigned int> directories;
    std::vector<unsigned int> files;

    File(){}
    File(unsigned int &F_IDGEN) : TIMESTAMP((unsigned int)std::time(NULL)), NAME(""), ID(F_IDGEN), ENCRYPTED(false), HASH(""), SALT(""),
      PARENT(0), content(""), DIRECTORY(false){}

  private:
    friend class cereal::access;
    template <class Archive> void serialize(Archive &ar)
    {
        ar(CEREAL_NVP(TIMESTAMP), CEREAL_NVP(NAME), CEREAL_NVP(ID), CEREAL_NVP(ENCRYPTED), CEREAL_NVP(HASH), CEREAL_NVP(SALT),
          CEREAL_NVP(PARENT), CEREAL_NVP(content), CEREAL_NVP(DIRECTORY), CEREAL_NVP(directories), CEREAL_NVP(files));
    }
};

struct Block
{
    unsigned int TIMESTAMP;
    unsigned int F_IDGEN;
    bool         AUTOSAVE;

    std::vector<File> root;

    FlatBlock() : TIMESTAMP((unsigned int)std::time(NULL)), F_IDGEN(0), AUTOSAVE(true){}

  private:
    friend class cereal::access;
    template <class Archive> void serialize(Archive &ar)
    {
        ar(CEREAL_NVP(TIMESTAMP), CEREAL_NVP(F_IDGEN), CEREAL_NVP(AUTOSAVE), CEREAL_NVP(root));
    }
};

}

}