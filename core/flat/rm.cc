#include "flat_utils.h"

bool FGNS::Flat::rm(FGNS::Flat::Block &block, std::string dst, int mode)
{
    if(FGNS::Flat::exists(block, dst, mode))
    {
        FGNS::Flat::File &file = FGNS::Flat::get_file_wrapper(block, dst, mode);
        if (file.DIRECTORY)
        {
            auto dirroot = FGNS::Flat::gen_dir_root(block.root, std::to_string(file.ID), 1);
            for (auto fileptr : dirroot)
            {
                block.root.erase(std::remove_if(block.root.begin(), block.root.end(), [&](const FGNS::Flat::File &f)
                    { return f.ID == fileptr->ID; }), block.root.end());
            }
        }
        block.root.erase(std::remove_if(block.root.begin(), block.root.end(), [&](const FGNS::Flat::File &f)
            { return f.ID == file.ID; }), block.root.end());

        return true;
    }
    else
    {
        fprintf(stderr, "rm: file does not exist\n");
        return false;
    }
}