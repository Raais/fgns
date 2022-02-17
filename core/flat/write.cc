#include "flat_utils.h"

bool FGNS::write(FGNS::FlatBlock &block, std::string dst, std::string content, int mode)
{
    if ((mode == 0) && (dst.back() == '*'))
        dst = FGNS::root_get_target_fuzzy(block.root, dst);
    
    if (FGNS::exists(block, dst, mode))
    {
        FGNS::File &file = *FGNS::get_file_ptr(block.root, dst, mode);

        file.content = content;
        
        return true;
    }
    else
    {
        fprintf(stderr, "File does not exist\n");
        return false;
    }
}