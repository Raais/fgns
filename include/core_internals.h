#pragma once

#include <string>
#include <fstream>

#include "core_types.h"

namespace FGNS
{

File* getf_ptr(Block &block, std::string target, int mode = 0);
bool exists_ext(std::string target_ext);

}