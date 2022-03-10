#include "flat_utils.h"

bool FGNS::touch(FGNS::FlatBlock &block, std::string dst)
{
    dst = FGNS::input_sanitizer_special_chars(dst);
    
    if (!FGNS::exists(block, dst, 0))
    {
        FGNS::File new_file(block.F_IDGEN);
        new_file.NAME = dst;
        block.root.push_back(new_file);
        block.F_IDGEN++;

        return true;
    }
    else
    {
        fprintf(stderr, "File already exists\n");
        return false;
    }
}