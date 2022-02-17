#include "flat_utils.h"

bool FGNS::mv(FGNS::FlatBlock &block, std::string src, std::string dst, int mode)
{
    dst = FGNS::input_sanitizer_special_chars(dst);
    if ((mode == 0) && (src.back() == '*'))
        src = FGNS::root_get_target_fuzzy(block.root, src);
    
    if (!FGNS::exists(block, src, mode))
    {
        fprintf(stderr, "File does not exist\n");
        return false;
    }
    if (FGNS::exists(block, dst, 0))
    {
        fprintf(stderr, "File already exists\n");
        return false;
    }
    else
    {
        FGNS::File &source = *FGNS::get_file_ptr(block.root, src, mode);

        source.TIMESTAMP = (unsigned int)std::time(NULL);
        source.name = dst;
        
        return true;
    }
}