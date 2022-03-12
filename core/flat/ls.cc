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

        std::string preview = "";

        if (file.DIRECTORY)
        {
            std::string blue = "\033[34m";
            std::string yellow = "\033[33m";
            std::string clear = "\033[0m";
            for (auto &f : file.files)
            {
                preview += "[" + yellow + std::to_string(f) + clear + "]";
            }
            for (auto &d : file.directories)
            {   
                preview += "[" + blue + std::to_string(d) + clear + "]";
            }
            if (preview.size() > 60)
            {
                preview = preview.substr(0, 60) + "[...]";
            }

            printf("[%u] - --------------- - --- - \033[1;34m%s\033[0m (%zud,%zuf) -> %s\n",
                file.ID, file.NAME.c_str(), file.directories.size(), file.files.size(), preview.c_str());
        }
        else
        {
            std::string checksum = FGNS::Crypto::SHA512Digest(file.content);
            checksum = checksum.substr(0, 6) + "." + checksum.substr(checksum.size() - 6, 6);
            
            if (file.content.size() > 20)
            {
                preview = file.content.substr(0, 20) + "(...)";
            }
            else
            {
                preview = file.content;
            }

            printf("[%u] - #=%s - $=%d - \033[33m%s\033[0m (%zuB) -> \"%s\"\n",
                file.ID, checksum.c_str(), file.ENCRYPTED, file.NAME.c_str(), file.content.size(), preview.c_str());
        }

    }
}