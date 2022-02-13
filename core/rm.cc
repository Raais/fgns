#include "core_utils.h"

bool FGNS::rm(FGNS::Block &block, std::string target, int mode)
{
    if(FGNS::exists(block, target, mode))
    {
        if (mode == 0)
        {
            block.root.erase(std::remove_if(block.root.begin(), block.root.end(), [&target](FGNS::File &file) { return file.name == target; }), block.root.end());
            return true;
        }
        else
        {
            block.root.erase(std::remove_if(block.root.begin(), block.root.end(), [&target](FGNS::File &file) { return file.ID == std::stoi(target); }), block.root.end());
            return true;
        }
    }
    else
    {
        std::cerr << "File does not exist" << std::endl;
        return false;
    }
}