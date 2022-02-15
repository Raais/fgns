#include "core_utils.h"

bool FGNS::cat(FGNS::Block &block, std::string dst, int mode)
{
    if ((mode == 0) && (dst.back() == '*'))
    {
        dst = FGNS::root_get_target_fuzzy(block, dst);
    }
    if (FGNS::exists(block, dst, mode))
    {
        FGNS::File &file = *FGNS::get_file_ptr(block, dst, mode);

        printf("%s", file.content.c_str());
        
        return true;
    }
    else
    {
        printf("%s", "File does not exist");
        return false;
    }
}