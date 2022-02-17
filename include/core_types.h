#pragma once

#include <string>
#include <ctime>

#include <cereal/cereal.hpp>

namespace FGNS
{

struct File
{
    unsigned int TIMESTAMP;
    unsigned int ID;
    bool         ENCRYPTED;
    std::string  HASH;
    std::string  SALT;

    std::string  name;
    std::string  content;

    File(){}
    File(unsigned int &F_IDGEN) : TIMESTAMP((unsigned int)std::time(NULL)), ID(F_IDGEN), ENCRYPTED(false), HASH(""), SALT(""), name(""), content(""){}

  private:
    friend class cereal::access;
    template <class Archive> void serialize(Archive &ar)
    {
        ar(CEREAL_NVP(TIMESTAMP), CEREAL_NVP(ID), CEREAL_NVP(ENCRYPTED), CEREAL_NVP(HASH), CEREAL_NVP(SALT), CEREAL_NVP(name), CEREAL_NVP(content));
    }
};

}