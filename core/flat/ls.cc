#include "flat_utils.h"

void FGNS::Flat::ls(FGNS::Flat::Block &block)
{
    auto wdroot = FGNS::Flat::gen_dir_root(block.root, std::to_string(block.WORKDIR), 1);

    for (auto fileptr : wdroot)
    {
        FGNS::Flat::File &file = *fileptr;

        std::string preview = "";

        if (file.DIRECTORY)
        {
            if (file.files.empty() && file.directories.empty())
                preview = "[]";
            std::string blue = "\033[34m";
            std::string yellow = "\033[33m";
            std::string clear = "\033[0m";
            bool exceeded = false;
            for (auto &f : file.files)
            {
                if (preview.size() <= 60)
                    preview += "[" + yellow + std::to_string(f) + clear + "]";
                else
                    exceeded = true;
            }
            for (auto &d : file.directories)
            {   
                if (preview.size() <= 60)
                    preview += "[" + blue + std::to_string(d) + clear + "]";
                else
                    exceeded = true;
            }
            if (exceeded)
            {
                preview += clear + "[...]";
            }

            printf("[%u] - -------------- - --- - \033[1;34m%s\033[0m (%zud,%zuf) -> %s\n",
                file.ID, file.NAME.c_str(), file.directories.size(), file.files.size(), preview.c_str());
        }
        else
        {
            std::string blue = "\033[34m";
            std::string magenta = "\033[35m";
            std::string clear = "\033[0m";
            std::string cyan = "\033[36m";

            std::string checksum = StrCrypto::SHA256Digest(file.content);
            checksum = cyan + checksum.substr(0, 6) + clear + magenta + checksum.substr(checksum.size() - 6, 6) + clear;
            
            if (file.content.size() > 20)
            {
                preview = file.content.substr(0, 20) + "(...)";
            }
            else
            {
                preview = file.content;
            }

            printf("[%u] - #=%s - $=%d - \033[33m%s\033[0m (%zuB) -> \"%s\"\n",
                file.ID, checksum.c_str(), (file.content.substr(0, 9) != "$argon2id") ? 0 : 1, file.NAME.c_str(), file.content.size(), preview.c_str());
        }

    }
}