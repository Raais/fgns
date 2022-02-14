#include "core_utils.h"

bool FGNS::touch(FGNS::Block &block, std::string dst)
{
    if (!FGNS::exists(block, dst, 0))
    {
        FGNS::File new_file(block.F_IDGEN);
        new_file.name = dst;
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