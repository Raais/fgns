#include "core_utils.h"

bool FGNS::write(FGNS::Block &block, std::string dst, std::string content, int mode)
{
    if ((mode == 0) && (dst.back() == '*'))
        dst = FGNS::root_get_target_fuzzy(block, dst);
    
    if (FGNS::exists(block, dst, mode))
    {
        FGNS::File &file = *FGNS::get_file_ptr(block, dst, mode);

        file.content = content;
        
        return true;
    }
    else
    {
        fprintf(stderr, "File does not exist\n");
        return false;
    }
}