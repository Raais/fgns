#include "flat_utils.h"

bool FGNS::Flat::fexport(FGNS::Flat::Block &block, std::string dst, int mode)
{
    if ((mode == 0) && (dst.back() == '*'))
        dst = FGNS::Flat::get_target_fuzzy(block, dst);

    if (FGNS::Flat::exists(block, dst, mode))
    {
        FGNS::Flat::File &file = FGNS::Flat::get_file_wrapper(block, dst, mode);

        if (fs::exists(dst))
        {
            fprintf(stderr, "export: '%s' already exists\n", dst.c_str());
            return false;
        }

        std::ofstream outfile(dst, std::ios::out | std::ios::binary | std::ios::trunc);
        outfile.write(file.content.c_str(), file.content.size());
        outfile.close();

        return true;
    }
    else
    {
        fprintf(stderr, "export: file does not exist\n");
        return false;
    }
}