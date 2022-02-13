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
        std::cout << "[" << file.ID << "]" << " -- " << file.TIMESTAMP << " -- $=" << file.ENCRYPTED << " -- " << file.name << " (" << file.content.size() << ") -> \"" << preview << "\"" << std::endl;
    }
}