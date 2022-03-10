#include "flat_utils.h"

bool FGNS::exists(FGNS::FlatBlock &block, std::string dst, int mode)
{
    std::string q;
    for (auto &file : block.root)
    {
        (mode == 0) ? q = file.NAME : q = std::to_string(file.ID);
        
        if (q == dst)
            return true;
    }
    return false;
}