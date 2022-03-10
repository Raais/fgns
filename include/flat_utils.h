#pragma once

#include <cstdio>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>

#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>

#include "flat_types.h"
#include "core_utils.h"
#include "str_crypto.h"

namespace FGNS
{
    void   ls        (Flat::Block &block);
    bool   touch     (Flat::Block &block, std::string dst);
    bool   rm        (Flat::Block &block, std::string dst,                       int mode = 0);
    bool   cat       (Flat::Block &block, std::string dst,                       int mode = 0);
    bool   info      (Flat::Block &block, std::string dst,                       int mode = 0);
    bool   exists    (Flat::Block &block, std::string dst,                       int mode = 0);
    bool   cp        (Flat::Block &block, std::string src, std::string dst,      int mode = 0);
    bool   mv        (Flat::Block &block, std::string src, std::string dst,      int mode = 0);
    bool   write     (Flat::Block &block, std::string dst, std::string content,  int mode = 0);
    bool   encrypt   (Flat::Block &block, std::string dst, std::string password, int mode = 0);
    bool   decrypt   (Flat::Block &block, std::string dst, std::string password, int mode = 0);
    bool   fexport   (Flat::Block &block, std::string src, std::string dst_ext,  int mode = 0);
    bool   import    (Flat::Block &block, std::string dst_ext);
    bool   importdir (Flat::Block &block, std::string dstdir_ext);

    File*  get_file_ptr  (std::vector<Flat::File> &root, std::string dst, int mode = 0);
    std::string root_get_target_fuzzy (std::vector<Flat::File> &root, std::string dst);
    
    void   save_bin  (Flat::Block &block, std::string dst_ext);
    FlatBlock   load_bin  (std::string dst_ext);
    void   save_json (Flat::Block &block, std::string dst_ext); //FIXME
    FlatBlock   load_json (std::string dst_ext);               //FIXME
}