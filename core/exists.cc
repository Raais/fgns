#include "core_utils.h"

bool FGNS::exists(FGNS::Block &block, std::string target, int mode)
{
    std::string q;
    for (auto &file : block.root)
    {
        (mode == 0) ? q = file.name : q = std::to_string(file.ID);
        
        if (q == target)
        {
            return true;
        }
    }
    return false;
}