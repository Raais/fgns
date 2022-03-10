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
        if (file.content.size() > 20)
        {
            preview = file.content.substr(0, 20);
            preview += "(...)";
        }
        else
        {
            preview = file.content;
        }

        printf("[%u] -- %u -- $=%d -- \033[1;34m%s\033[0m (%zu) -> \"%s\"\n",
                file.ID, file.TIMESTAMP, file.ENCRYPTED, file.NAME.c_str(), file.content.size(), preview.c_str());
    }
}