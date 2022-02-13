#include "core_internals.h"

FGNS::File* FGNS::getf_ptr(Block &block, std::string target, int mode)
{
    std::string q;
    for (auto &file : block.root)
    {
        (mode == 0) ? q = file.name : q = std::to_string(file.ID);
        
        if (q == target)
        {
            return &file;
        }
    }
    return nullptr;
}

bool FGNS::exists_ext(std::string target_ext)
{
    std::ifstream f(target_ext.c_str());
    return f.good();
}