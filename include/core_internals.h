#pragma once

#include <string>
#include <fstream>

#include <rapidfuzz/fuzz.hpp>
using rapidfuzz::fuzz::ratio;

#include "core_types.h"

namespace FGNS
{

File* getf_ptr(Block &block, std::string target, int mode = 0);
std::string root_get_target_fuzzy(Block &block, std::string target);
bool exists_ext(std::string target_ext);

}