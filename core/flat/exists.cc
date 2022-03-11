#include "flat_utils.h"

/* ID is searched globally in root; Name search is confined to working dir; */
bool FGNS::Flat::exists(FGNS::Flat::Block &block, std::string dst, int mode)
{
    if (mode == 1)
    {
        for (auto &file : block.root)
        {
            if (std::to_string(file.ID) == dst)
                return true;
        }
        return false;
    }
    else
    {
        auto wd = FGNS::Flat::gen_dir_root(block.root, std::to_string(block.WORKDIR), 1);

        for (auto fileptr : wd)
        {
            FGNS::Flat::File &file = *fileptr;

            if (file.NAME == dst)
                return true;
        }
        return false;
    }
}