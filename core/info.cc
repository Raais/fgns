#include "core_utils.h"

bool FGNS::info(FGNS::Block &block, std::string target, int mode)
{
    if (FGNS::exists(block, target, mode))
    {
        FGNS::File &file = *FGNS::getf_ptr(block, target, mode);

        std::cout << "Name: " << file.name << std::endl;
        std::cout << "ID: " << file.ID << std::endl;
        std::cout << "Size: " << file.content.size() << std::endl;
        std::cout << "Encrypted: " << file.ENCRYPTED << std::endl;
        
        return true;
    }
    else
    {
        std::cerr << "File does not exist" << std::endl;
        return false;
    }
}