#include "flat_utils.h"

bool FGNS::Flat::cp(FGNS::Flat::Block &block, std::string src, std::string dst, int mode)
{
    dst = FGNS::input_sanitizer_special_chars(dst);
    if ((mode == 0) && (src.back() == '*'))
        src = FGNS::Flat::get_target_fuzzy(block.root, src);
        
    if (!FGNS::Flat::exists(block, src, mode))
    {
        fprintf(stderr, "cp: '%s' does not exist\n", src.c_str());
        return false;
    }
    if (FGNS::Flat::exists(block, dst, 0))
    {
        fprintf(stderr, "cp: '%s' already exists\n", dst.c_str());
        return false;
    }
    else
    {
        FGNS::Flat::File &source = FGNS::Flat::get_file_wrapper(block, src, mode);

        if (!source.DIRECTORY)
        {
            FGNS::Flat::File new_file(block.IDSEED, block.WORKDIR);
            new_file.NAME = dst;
            new_file.content = source.content;
            new_file.ENCRYPTED = source.ENCRYPTED;
            new_file.HASH = source.HASH;
            new_file.SALT = source.SALT;
            block.root.push_back(new_file);
            block.IDSEED++;

            if (block.WORKDIR != -1)
            {
                FGNS::Flat::File &wd = *FGNS::Flat::get_file_ptr(block.root, std::to_string(block.WORKDIR), 1);
                wd.files.push_back(new_file.ID);
            }

            return true;
        }
        else
        {
            FGNS::Flat::File new_dir(block.IDSEED, block.WORKDIR);
            new_dir.NAME = dst;
            new_dir.DIRECTORY = true;
            block.root.push_back(new_dir);
            block.IDSEED++;

            if (block.WORKDIR != -1)
            {
                FGNS::Flat::File &wd = *FGNS::Flat::get_file_ptr(block.root, std::to_string(block.WORKDIR), 1);
                wd.directories.push_back(new_dir.ID);
            }

            auto oldwd = block.WORKDIR;
            auto srcroot = FGNS::Flat::gen_dir_root(block.root, std::to_string(source.ID), 1);
            block.WORKDIR = new_dir.ID;
            for (auto &file : srcroot)
            {
                FGNS::Flat::cp(block, std::to_string(file->ID), file->NAME, 1);
            }
            block.WORKDIR = oldwd;

            return true;
        }
    }
}