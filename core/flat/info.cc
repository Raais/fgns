#include "flat_utils.h"

bool FGNS::info(FGNS::FlatBlock &block, std::string dst, int mode)
{
    if ((mode == 0) && (dst.back() == '*'))
        dst = FGNS::root_get_target_fuzzy(block.root, dst);
    
    if (FGNS::exists(block, dst, mode))
    {
        FGNS::File &file = *FGNS::get_file_ptr(block.root, dst, mode);

        printf("Name: %s\n", file.name.c_str());
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