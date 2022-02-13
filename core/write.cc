#include "core_utils.h"

bool FGNS::write(FGNS::Block &block, std::string target, std::string content, int mode)
{
    if ((mode == 0) && (target.back() == '*'))
    {
        target = FGNS::root_get_target_fuzzy(block, target);
    }
    if (FGNS::exists(block, target, mode))
    {
        FGNS::File &file = *FGNS::getf_ptr(block, target, mode);

        file.content = content;
        
        return true;
    }
    else
    {
        std::cerr << "File does not exist" << std::endl;
        return false;
    }
}