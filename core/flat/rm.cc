#include "flat_utils.h"

bool FGNS::Flat::rm(FGNS::Flat::Block &block, std::string dst, int mode)
{
    if(FGNS::Flat::exists(block, dst, mode))
    {
        FGNS::Flat::File &file = FGNS::Flat::get_file_wrapper(block, dst, mode);

        if (!file.DIRECTORY)
        {
            if (file.PARENT != -1)
            {
                FGNS::Flat::File &parent = *FGNS::Flat::get_file_ptr(block.root, std::to_string(file.PARENT), 1);
                auto it = std::find(parent.files.begin(), parent.files.end(), file.ID);
                parent.files.erase(it);
            }
            
            block.root.erase(std::remove_if(block.root.begin(), block.root.end(), [&file](FGNS::Flat::File &f)
                { return f.ID == file.ID; }), block.root.end());
            block.SAVED = false;

            return true;
        }
        else
        {
            std::vector<unsigned int> ids;
            for (auto &f : file.files)
                ids.push_back(f);
            for (auto &d : file.directories)
                ids.push_back(d);

            for (auto &id : ids)
                FGNS::Flat::rm(block, std::to_string(id), 1);
            
            if (file.PARENT != -1)
            {
                FGNS::Flat::File &parent = *FGNS::Flat::get_file_ptr(block.root, std::to_string(file.PARENT), 1);
                auto it = std::find(parent.directories.begin(), parent.directories.end(), file.ID);
                parent.directories.erase(it);
            }

            block.root.erase(std::remove_if(block.root.begin(), block.root.end(), [&file](FGNS::Flat::File &f)
                { return f.ID == file.ID; }), block.root.end());
            block.SAVED = false;
            
            return true;
        }
    }
    else
    {
        fprintf(stderr, "rm: '%s' does not exist\n", dst.c_str());
        return false;
    }
}