#include "core_utils.h"

bool FGNS::mv(FGNS::Block &block, std::string src, std::string dst, int mode)
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

        source.TIMESTAMP = (unsigned int)std::time(NULL);
        source.name = dst;
        
        return true;
    }
}