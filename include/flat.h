#pragma once

#include <string>
#include <ctime>

#include <cereal/cereal.hpp>

#include "core_types.h"

#define FLAT_BLOCK_EXTENSION ".cfbo" // "Cereal"ized flat block object //

namespace FGNS
{

struct FlatBlock
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