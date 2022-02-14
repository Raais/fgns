#include "core_utils.h"

bool FGNS::fexport(FGNS::Block &block, std::string src, std::string dst_ext, int mode)
{
    if ((mode == 0) && (src.back() == '*'))
    {
        src = FGNS::root_get_target_fuzzy(block, src);
    }
    if (dst_ext == "")
    {
        dst_ext = src;
    }

    if(FGNS::exists(block, src, mode))
    {
        FGNS::File &file = *FGNS::get_file_ptr(block, src, mode);

        std::ofstream outfile(dst_ext, std::ios::out | std::ios::binary | std::ios::trunc);
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