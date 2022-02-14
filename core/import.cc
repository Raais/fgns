#include "core_utils.h"

bool FGNS::import(FGNS::Block &block, std::string dst_ext)
{
    if (dst_ext.back() == '*')
    {
        dst_ext = FGNS::fs_get_target_fuzzy(dst_ext);
    }
    if(FGNS::exists_ext(dst_ext))
    {
        std::ifstream file_ext(dst_ext);
        std::string content((std::istreambuf_iterator<char>(file_ext)), std::istreambuf_iterator<char>());
        file_ext.close();
        if(!FGNS::exists(block, dst_ext))
        {
            FGNS::touch(block, dst_ext);
            FGNS::write(block, dst_ext, content);

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