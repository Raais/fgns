#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>

#include "core_types.h"
#include "core_internals.h"
#include "str_crypto.h"

namespace FGNS
{

void   ls(Block &block);
bool   touch(Block &block, std::string target);
bool   rm(Block &block, std::string target, int mode = 0);
bool   cp(Block &block, std::string src, std::string dst, int mode = 0);
bool   mv(Block &block, std::string src, std::string dst, int mode = 0);
bool   cat(Block &block, std::string target, int mode = 0);
bool   write(Block &block, std::string target, std::string content, int mode = 0);
bool   info(Block &block, std::string target, int mode = 0);
bool   encrypt(Block &block, std::string target, std::string password, int mode = 0);
bool   decrypt(Block &block, std::string target, std::string password, int mode = 0);
bool   import(Block &block, std::string target_ext);
bool   fexport(Block &block, std::string target, std::string target_ext, int mode = 0);
bool   exists(Block &block, std::string target, int mode = 0);

}