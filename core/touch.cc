#include "core_utils.h"

bool FGNS::touch(FGNS::Block &block, std::string target)
{
    if (!FGNS::exists(block, target, 0))
    {
        FGNS::File new_file(block.F_IDGEN);
        new_file.name = target;
        block.root.push_back(new_file);
        block.F_IDGEN++;

        return true;
    }
    else
    {
        std::cerr << "File already exists" << std::endl;
        return false;
    }
}