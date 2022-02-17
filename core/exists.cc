#include "core_utils.h"

bool FGNS::exists(FGNS::Block &block, std::string dst, int mode)
{
    std::string q;
    for (auto &file : block.root)
    {
        (mode == 0) ? q = file.name : q = std::to_string(file.ID);
        
        if (q == dst)
            return true;
    }
    return false;
}