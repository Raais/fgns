#include "flat_utils.h"

bool FGNS::Flat::info(FGNS::Flat::Block &block, std::string dst, int mode)
{
    if ((mode == 0) && (dst.back() == '*'))
        dst = FGNS::Flat::get_target_fuzzy(block, dst);
    
    if (FGNS::Flat::exists(block, dst, mode))
    {
        FGNS::Flat::File &file = FGNS::Flat::get_file_wrapper(block, dst, mode);

        printf("Name: %s\n", file.NAME.c_str());
        printf("Type: %s\n", (!file.DIRECTORY) ? "File" : "Directory");
        printf("ID: %u\n", file.ID);
        (!file.DIRECTORY) ? printf("Size: %zuB\n", file.content.size())
         : printf("Size: %zu\n", file.files.size()+file.directories.size());
        if (!file.DIRECTORY) printf("Encrypted: %d\n", file.ENCRYPTED);
        printf("Timestamp: %u\n", file.TIMESTAMP);
        
        return true;
    }
    else
    {
        fprintf(stderr, "info: '%s' does not exist\n", dst.c_str());
        return false;
    }
}