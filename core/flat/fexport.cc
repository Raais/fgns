#include "flat_utils.h"

bool FGNS::Flat::fexport(FGNS::Flat::Block &block, std::string src, std::string dst_ext, int mode)
{
    if ((mode == 0) && (src.back() == '*'))
        src = FGNS::Flat::get_target_fuzzy(block.root, src);

    if (FGNS::Flat::exists(block, src, mode))
    {
        FGNS::Flat::File &file = FGNS::Flat::get_file_wrapper(block, src, mode);
        if (dst_ext == "")
            dst_ext = file.NAME;

        std::ofstream outfile(dst_ext, std::ios::out | std::ios::binary | std::ios::trunc);
        outfile.write(file.content.c_str(), file.content.size());
        outfile.close();

        return true;
    }
    else
    {
        fprintf(stderr, "File does not exist\n");
        return false;
    }
}