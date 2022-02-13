#include "core_utils.h"

bool FGNS::cat(FGNS::Block &block, std::string target, int mode)
{
    if (FGNS::exists(block, target, mode))
    {
        FGNS::File &file = *FGNS::getf_ptr(block, target, mode);

        std::cout << file.content << std::endl;
        
        return true;
    }
    else
    {
        std::cerr << "File does not exist" << std::endl;
        return false;
    }
}