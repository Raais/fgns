#include "flat_utils.h"

bool FGNS::Flat::cp(FGNS::Flat::Block &block, std::string src, std::string dst, int mode)
{
    dst = FGNS::input_sanitizer_special_chars(dst);
    if ((mode == 0) && (src.back() == '*'))
        src = FGNS::Flat::get_target_fuzzy(block.root, src);
        
    if (!FGNS::Flat::exists(block, src, mode))
    {
        fprintf(stderr, "cp: file does not exist\n");
        return false;
    }
    if (FGNS::Flat::exists(block, dst, 0))
    {
        fprintf(stderr, "cp: file already exists\n");
        return false;
    }
    else
    {
        FGNS::Flat::File &source = FGNS::Flat::get_file_wrapper(block, dst, mode);

        FGNS::Flat::File new_file(block.IDSEED, block.WORKDIR);
        new_file.NAME = dst;
        new_file.content = source.content;
        new_file.ENCRYPTED = source.ENCRYPTED;
        new_file.HASH = source.HASH;
        new_file.SALT = source.SALT;
        block.root.push_back(new_file);
        block.IDSEED++;
        
        return true;
    }
}