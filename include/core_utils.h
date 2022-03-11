#pragma once

#ifdef __APPLE__
#include <Availability.h>
#endif
#if ((defined(_MSVC_LANG) && _MSVC_LANG >= 201703L) || (defined(__cplusplus) && __cplusplus >= 201703L)) && defined(__has_include)
#if __has_include(<filesystem>) && (!defined(__MAC_OS_X_VERSION_MIN_REQUIRED) || __MAC_OS_X_VERSION_MIN_REQUIRED >= 101500)
#define GHC_USE_STD_FS
#include <filesystem>
namespace fs = std::filesystem;
#endif
#endif
#ifndef GHC_USE_STD_FS
#include <ghc/filesystem.hpp>
namespace fs = ghc::filesystem;
#endif

#include <string>
#include <fstream>
#include <sstream>

#include <rapidfuzz/fuzz.hpp>
using rapidfuzz::fuzz::ratio;

#include "encode.hpp"
#include "decode.hpp"

namespace FGNS
{
    std::string fs_get_target_fuzzy (std::string dst_ext);

    std::string input_sanitizer_special_chars (std::string input);

           bool exists_ext (std::string dst_ext);

           void write_ext_bin (std::string path, std::string str);
    std::string read_ext (std::string path);

           bool compress_ext (std::string dst_ext);
           bool decompress_ext (std::string dst_ext);

    std::string get_file_magic (std::string dst_ext);

           bool has_suffix (std::string str, std::string suffix);

    std::vector<std::string> usplit(std::string s, std::string delimiter);
}