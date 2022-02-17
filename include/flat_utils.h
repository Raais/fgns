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

#include "flat.h"
#include "core_utils.h"
#include "str_crypto.h"

namespace FGNS
{
    void   ls        (FlatBlock &block);
    bool   touch     (FlatBlock &block, std::string dst);
    bool   rm        (FlatBlock &block, std::string dst,                       int mode = 0);
    bool   cat       (FlatBlock &block, std::string dst,                       int mode = 0);
    bool   info      (FlatBlock &block, std::string dst,                       int mode = 0);
    bool   exists    (FlatBlock &block, std::string dst,                       int mode = 0);
    bool   cp        (FlatBlock &block, std::string src, std::string dst,      int mode = 0);
    bool   mv        (FlatBlock &block, std::string src, std::string dst,      int mode = 0);
    bool   write     (FlatBlock &block, std::string dst, std::string content,  int mode = 0);
    bool   encrypt   (FlatBlock &block, std::string dst, std::string password, int mode = 0);
    bool   decrypt   (FlatBlock &block, std::string dst, std::string password, int mode = 0);
    bool   fexport   (FlatBlock &block, std::string src, std::string dst_ext,  int mode = 0);
    bool   import    (FlatBlock &block, std::string dst_ext);
    bool   importdir (FlatBlock &block, std::string dstdir_ext);

    void   save_bin  (FlatBlock &block, std::string dst_ext);
    FlatBlock  load_bin  (std::string dst_ext);
    void   save_json (FlatBlock &block, std::string dst_ext); //FIXME
    FlatBlock  load_json (std::string dst_ext);               //FIXME
}