#include "flat_utils.h"

bool FGNS::Flat::cd(FGNS::Flat::Block &block, std::string dst, int mode)
{
    if (dst == "..")
    {
        if (block.WORKDIR == -1)
        {
            fprintf(stderr, "cd: already at root\n");
            return false;
        }
        else
        {
            block.WORKDIR = FGNS::Flat::get_file_ptr(block.root, std::to_string(block.WORKDIR), 1)->PARENT;
            return true;
        }
    }
    else if (dst == "/")
    {
        block.WORKDIR = -1;
        return true;
    }
    else
    {
        if ((mode == 0) && (dst.back() == '*'))
            dst = FGNS::Flat::get_target_fuzzy(block.root, dst);
            
        if (FGNS::Flat::exists(block, dst, mode))
        {
            FGNS::Flat::File &file = FGNS::Flat::get_file_wrapper(block, dst, mode);

            if (file.DIRECTORY)
            {
                block.WORKDIR = file.ID;
                return true;
            }
            else
            {
                fprintf(stderr, "cd: not a directory\n");
                return false;
            }
        }
        else
        {
            fprintf(stderr, "cd: target does not exist\n");
            return false;
        }
    }
}