#include "core_utils.h"

bool FGNS::info(FGNS::Block &block, std::string dst, int mode)
{
    if ((mode == 0) && (dst.back() == '*'))
    {
        dst = FGNS::root_get_target_fuzzy(block, dst);
    }
    if (FGNS::exists(block, dst, mode))
    {
        FGNS::File &file = *FGNS::get_file_ptr(block, dst, mode);

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