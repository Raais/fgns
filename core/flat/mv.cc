#include "flat_utils.h"

bool FGNS::Flat::mv(FGNS::Flat::Block &block, std::string src, std::string dst, int mode)
{
    dst = FGNS::input_sanitizer_special_chars(dst);
    if ((mode == 0) && (src.back() == '*'))
        src = FGNS::Flat::get_target_fuzzy(block, src);
    
    if (!FGNS::Flat::exists(block, src, mode))
    {
        fprintf(stderr, "mv: '%s' does not exist\n", src.c_str());
        return false;
    }
    if (FGNS::Flat::exists(block, dst, 0))
    {
        fprintf(stderr, "mv: '%s' already exists\n", dst.c_str());
        return false;
    }
    else
    {
        FGNS::Flat::File &source = FGNS::Flat::get_file_wrapper(block, src, mode);

        source.TIMESTAMP = (unsigned int)std::time(NULL);
        source.NAME = dst;
        
        return true;
    }
}