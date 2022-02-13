#include "core_utils.h"

bool FGNS::write(FGNS::Block &block, std::string target, std::string content, int mode)
{
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