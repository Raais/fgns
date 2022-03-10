#include "flat_utils.h"

void FGNS::Flat::ls(FGNS::Flat::Block &block)
{
    std::string wd = "/";
    if (block.WORKDIR != -1)
    {
        wd = FGNS::Flat::get_path(block.root, std::to_string(block.WORKDIR), 1);
    }
    printf("Working directory: [\033[32m%s\033[0m]\n\n", wd.c_str());

    auto wdroot = FGNS::Flat::gen_dir_root(block.root, std::to_string(block.WORKDIR), 1);

    for (auto fileptr : wdroot)
    {
        FGNS::Flat::File &file = *fileptr;

        std::string preview;

        if (file.DIRECTORY)
        {
            for (auto &f : file.files)
            {
                preview += "[" + std::to_string(f) + "]";
            }
            for (auto &d : file.directories)
            {
                preview += "[" + std::to_string(d) + "]";
            }
            if (preview.size() > 20)
            {
                preview = preview.substr(0, 20) + "(...)";
            }

            printf("[%u] - %u - --- - \033[1;34m%s\033[0m (%zud,%zuf) -> %s\n",
                file.ID, file.TIMESTAMP, file.NAME.c_str(), file.directories.size(), file.files.size(), preview.c_str());
        }
        else
        {
            
            if (file.content.size() > 20)
            {
                preview = file.content.substr(0, 20) + "(...)";
            }
            else
            {
                preview = file.content;
            }

            printf("[%u] - %u - $=%d - \033[33m%s\033[0m (%zuB) -> \"%s\"\n",
                file.ID, file.TIMESTAMP, file.ENCRYPTED, file.NAME.c_str(), file.content.size(), preview.c_str());
        }

    }
}