#include "core_utils.h"

bool FGNS::cp(FGNS::Block &block, std::string src, std::string dst, int mode)
{
    if ((mode == 0) && (src.back() == '*'))
    {
        src = FGNS::root_get_target_fuzzy(block, src);
    }
    if (!FGNS::exists(block, src, mode))
    {
        std::cerr << "File does not exist" << std::endl;
        return false;
    }
    if (FGNS::exists(block, dst, 0))
    {
        std::cerr << "File already exists" << std::endl;
        return false;
    }
    else
    {
        FGNS::File &source = *FGNS::getf_ptr(block, src, mode);

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