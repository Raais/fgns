#include "flat_utils.h"

void FGNS::ls(FGNS::FlatBlock &block)
{
    for (auto &file : block.root)
    {
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
        std::string from = " ", to = "";

        printf("[%u] -- %u -- $=%d -- \033[1;34m%s\033[0m (%zu) -> \"%s\"\n",
                file.ID, file.TIMESTAMP, file.ENCRYPTED, file.NAME.c_str(), file.content.size(), preview.c_str());
    }
}