#include "flat_utils.h"

bool FGNS::Flat::exportdir(FGNS::Flat::Block &block, std::string dst, int mode)
{
    if ((mode == 0) && (dst.back() == '*'))
        dst = FGNS::Flat::get_target_fuzzy(block, dst);

    if (FGNS::Flat::exists(block, dst, mode))
    {
        FGNS::Flat::File &file = FGNS::Flat::get_file_wrapper(block, dst, mode);

        if (!file.DIRECTORY)
        {
            fprintf(stderr, "exportdir: '%s' is not a directory\n", dst.c_str());
            return false;
        }
        if (fs::exists(dst))
        {
            fprintf(stderr, "exportdir: '%s' already exists\n", dst.c_str());
            return false;
        }

        std::string oldwd = fs::absolute(fs::current_path());
        fs::create_directory(file.NAME);
        fs::current_path(file.NAME);
        auto root = FGNS::Flat::gen_dir_root(block.root, std::to_string(file.ID), 1);       
        for (auto &file : root)
        {
            if (!file->DIRECTORY)
            {
                std::ofstream outfile(file->NAME, std::ios::out | std::ios::binary | std::ios::trunc);
                outfile.write(file->content.c_str(), file->content.size());
                outfile.close();
            }
            else
            {
                FGNS::Flat::exportdir(block, std::to_string(file->ID), 1);
            }
        }
        fs::current_path(oldwd);

        return true;
    }
    else
    {
        fprintf(stderr, "exportdir: directory does not exist\n");
        return false;
    }

}