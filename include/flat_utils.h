#pragma once

#include <cstdio>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include <iomanip>

#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>

#include "str_crypto.hpp"

#include "flat_types.h"
#include "core_utils.h"

namespace FGNS
{

namespace Flat
{
    //Core
    void   ls        (Block &block);
    bool   cd        (Block &block, std::string dst,                       int type = 0);
    bool   touch     (Block &block, std::string dst);
    bool   mkdir     (Block &block, std::string dst);
    bool   rm        (Block &block, std::string dst,                       int mode = 0);
    bool   cat       (Block &block, std::string dst,                       int mode = 0);
    bool   info      (Block &block, std::string dst,                       int mode = 0);
    bool   exists    (Block &block, std::string dst,                       int mode = 0);
    bool   cp        (Block &block, std::string src, std::string dst,      int mode = 0);
    bool   mv        (Block &block, std::string src, std::string dst,      int mode = 0);
    bool   write     (Block &block, std::string dst, std::string content,  int mode = 0);
    bool   encrypt   (Block &block, std::string dst, std::string password, int mode = 0);
    bool   decrypt   (Block &block, std::string dst, std::string password, int mode = 0);
    bool   fexport   (Block &block, std::string dst,                       int mode = 0);
    bool   exportdir (Block &block, std::string dst,                       int mode = 0);
    bool   import    (Block &block, std::string dst_ext);
    bool   importdir (Block &block, std::string dstdir_ext);

    //Misc
    File*              get_file_ptr     (std::vector<Flat::File> &root, std::string dst, int mode = 0);
    File*              get_file_ptr     (std::vector<Flat::File*> &root, std::string dst, int mode = 0);
    File&              get_file_wrapper (Block &block, std::string dst, int mode = 0);
    std::string        get_path         (std::vector<Flat::File> &root, std::string dst, int mode = 0);
    std::vector<File*> gen_dir_root     (std::vector<Flat::File> &root, std::string dst, int mode = 0);
    std::string        get_target_fuzzy (Block &block, std::string dst);
    std::string        checksum         (Block &block, std::string dst, int mode = 0);
    
    //IO
    void               save_bin         (Block &block, std::string dst_ext);
    Flat::Block        load_bin         (std::string dst_ext);
    void               save_json        (Block &block, std::string dst_ext); //FIXME
    Flat::Block        load_json        (std::string dst_ext);        //FIXME

}

}