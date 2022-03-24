#include "flat_utils.h"

bool FGNS::Flat::mkdir(FGNS::Flat::Block &block, std::string dst)
{
    dst = FGNS::input_sanitizer_special_chars(dst);
    
    if (!FGNS::Flat::exists(block, dst, 0))
    {
        FGNS::Flat::File new_dir(block.IDSEED, block.WORKDIR);
        new_dir.NAME = dst;
        new_dir.DIRECTORY = true;
        block.root.push_back(new_dir);

        if (block.WORKDIR != -1)
        {
            FGNS::Flat::File &wd = *FGNS::Flat::get_file_ptr(block.root, std::to_string(block.WORKDIR), 1);
            wd.directories.push_back(new_dir.ID);
        }
        block.IDSEED++;
        block.SAVED = false;
        
        return true;
    }
    else
    {
        fprintf(stderr, "mkdir: file or directory '%s' already exists\n", dst.c_str());
        return false;
    }
}