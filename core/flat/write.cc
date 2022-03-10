#include "flat_utils.h"

bool FGNS::Flat::write(FGNS::Flat::Block &block, std::string dst, std::string content, int mode)
{
    if ((mode == 0) && (dst.back() == '*'))
        dst = FGNS::Flat::get_target_fuzzy(block.root, dst);
    
    if (FGNS::Flat::exists(block, dst, mode))
    {
        FGNS::Flat::File &file = FGNS::Flat::get_file_wrapper(block, dst, mode);
        if (file.DIRECTORY)
        {
            fprintf(stderr, "Cannot write to a directory\n");
            return false;
        }

        file.content = content;
        
        return true;
    }
    else
    {
        fprintf(stderr, "File does not exist\n");
        return false;
    }
}