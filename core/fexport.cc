#include "core_utils.h"

bool FGNS::fexport(FGNS::Block &block, std::string target, std::string target_ext, int mode)
{
    if (target_ext == "")
    {
        target_ext = target;
    }

    if(FGNS::exists(block, target, mode))
    {
        FGNS::File &file = *FGNS::getf_ptr(block, target, mode);

        std::ofstream outfile(target_ext, std::ios::out | std::ios::binary | std::ios::trunc);
        outfile.write(file.content.c_str(), file.content.size());
        outfile.close();

        return true;
    }
    else
    {
        std::cerr << "File does not exist" << std::endl;
        return false;
    }
}