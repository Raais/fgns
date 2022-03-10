#include "flat_utils.h"

bool FGNS::Flat::cat(FGNS::Flat::Block &block, std::string dst, int mode)
{
    if ((mode == 0) && (dst.back() == '*'))
        dst = FGNS::Flat::get_target_fuzzy(block.root, dst);
        
    if (FGNS::Flat::exists(block, dst, mode))
    {
        FGNS::Flat::File &file = FGNS::Flat::get_file_wrapper(block, dst, mode);
        if (file.DIRECTORY)
        {
            fprintf(stderr, "Cannot cat a directory\n");
            return false;
        }

        printf("%s", file.content.c_str());
        
        return true;
    }
    else
    {
        printf("%s", "File does not exist");
        return false;
    }
}