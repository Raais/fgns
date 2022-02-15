#include "core_utils.h"

void FGNS::ls(FGNS::Block &block)
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
        printf("[%u] -- %u -- $=%d -- %s (%zu) -> \"%s\"\n", file.ID, file.TIMESTAMP, file.ENCRYPTED, file.name.c_str(), file.content.size(), preview.c_str());
    }
}