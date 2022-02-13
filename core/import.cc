#include "core_utils.h"

bool FGNS::import(FGNS::Block &block, std::string target_ext)
{
    if(FGNS::exists_ext(target_ext))
    {
        std::ifstream file_ext(target_ext);
        std::string content((std::istreambuf_iterator<char>(file_ext)), std::istreambuf_iterator<char>());
        file_ext.close();
        if(!FGNS::exists(block, target_ext))
        {
            FGNS::touch(block, target_ext);
            FGNS::write(block, target_ext, content);

            return true;
        }
        else
        {
            std::cerr << "File already exists" << std::endl;
            return false;
        }
    }
    else
    {
        std::cerr << "External file not found" << std::endl;
        return false;
    }
}