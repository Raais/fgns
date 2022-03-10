#include "flat_utils.h"

bool FGNS::Flat::info(FGNS::Flat::Block &block, std::string dst, int mode)
{
    if ((mode == 0) && (dst.back() == '*'))
        dst = FGNS::Flat::get_target_fuzzy(block.root, dst);
    
    if (FGNS::Flat::exists(block, dst, mode))
    {
        FGNS::Flat::File &file = FGNS::Flat::get_file_wrapper(block, dst, mode);

        printf("Name: %s\n", file.NAME.c_str());
        printf("ID: %u\n", file.ID);
        printf("Size: %zu\n", file.content.size());
        printf("Encrypted: %d\n", file.ENCRYPTED);
        printf("Timestamp: %u\n", file.TIMESTAMP);
        
        return true;
    }
    else
    {
        fprintf(stderr, "File does not exist\n");
        return false;
    }
}