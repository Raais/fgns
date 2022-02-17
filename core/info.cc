#include "core_utils.h"

bool FGNS::info(FGNS::Block &block, std::string dst, int mode)
{
    if ((mode == 0) && (dst.back() == '*'))
        dst = FGNS::root_get_target_fuzzy(block, dst);
    
    if (FGNS::exists(block, dst, mode))
    {
        FGNS::File &file = *FGNS::get_file_ptr(block, dst, mode);

        printf("Name: %s\n", file.name.c_str());
        printf("ID: %u\n", file.ID);
        printf("Size: %zu\n", file.content.size());
        printf("Encrypted: %d\n", file.ENCRYPTED);
        
        return true;
    }
    else
    {
        fprintf(stderr, "File does not exist\n");
        return false;
    }
}