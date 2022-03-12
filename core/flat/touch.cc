#include "flat_utils.h"

bool FGNS::Flat::touch(FGNS::Flat::Block &block, std::string dst)
{
    dst = FGNS::input_sanitizer_special_chars(dst);
    
    if (!FGNS::Flat::exists(block, dst, 0))
    {
        FGNS::Flat::File new_file(block.IDSEED, block.WORKDIR);
        new_file.NAME = dst;
        block.root.push_back(new_file);

        if (block.WORKDIR != -1)
        {
            FGNS::Flat::File &wd = *FGNS::Flat::get_file_ptr(block.root, std::to_string(block.WORKDIR), 1);
            wd.files.push_back(new_file.ID);
        }
        block.IDSEED++;
        block.SAVED = false;

        return true;
    }
    else
    {
        fprintf(stderr, "touch: file or directory '%s' already exists\n", dst.c_str());
        return false;
    }
}