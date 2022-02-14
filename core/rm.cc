#include "core_utils.h"

bool FGNS::rm(FGNS::Block &block, std::string dst, int mode)
{
    if(FGNS::exists(block, dst, mode))
    {
        if (mode == 0)
        {
            block.root.erase(std::remove_if(block.root.begin(), block.root.end(), [&dst](FGNS::File &file) { return file.name == dst; }), block.root.end());
            return true;
        }
        else
        {
            block.root.erase(std::remove_if(block.root.begin(), block.root.end(), [&dst](FGNS::File &file) { return file.ID == std::stoi(dst); }), block.root.end());
            return true;
        }
    }
    else
    {
        std::cerr << "File does not exist" << std::endl;
        return false;
    }
}