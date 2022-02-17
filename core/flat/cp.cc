#include "flat_utils.h"

bool FGNS::cp(FGNS::FlatBlock &block, std::string src, std::string dst, int mode)
{
    dst = FGNS::input_sanitizer_special_chars(dst);
    if ((mode == 0) && (src.back() == '*'))
        src = FGNS::root_get_target_fuzzy(block.root, src);
        
    if (!FGNS::exists(block, src, mode))
    {
        fprintf(stderr, "File does not exist\n");
        return false;
    }
    if (FGNS::exists(block, dst, 0))
    {
        fprintf(stderr, "File already exists\n");
        return false;
    }
    else
    {
        FGNS::File &source = *FGNS::get_file_ptr(block.root, src, mode);

        FGNS::File new_file(block.F_IDGEN);
        new_file.name = dst;
        new_file.content = source.content;
        new_file.ENCRYPTED = source.ENCRYPTED;
        new_file.HASH = source.HASH;
        new_file.SALT = source.SALT;
        block.root.push_back(new_file);
        block.F_IDGEN++;
        
        return true;
    }
}