#include "flat_utils.h"

bool FGNS::cat(FGNS::FlatBlock &block, std::string dst, int mode)
{
    if ((mode == 0) && (dst.back() == '*'))
        dst = FGNS::root_get_target_fuzzy(block.root, dst);
        
    if (FGNS::exists(block, dst, mode))
    {
        FGNS::File &file = *FGNS::get_file_ptr(block.root, dst, mode);

        printf("%s", file.content.c_str());
        
        return true;
    }
    else
    {
        printf("%s", "File does not exist");
        return false;
    }
}