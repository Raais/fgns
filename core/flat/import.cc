#include "flat_utils.h"

bool FGNS::Flat::import(FGNS::Flat::Block &block, std::string dst_ext)
{
    if (dst_ext.back() == '*')
        dst_ext = FGNS::fs_get_target_fuzzy(dst_ext);
    
    if (FGNS::exists_ext(dst_ext))
    {
        std::ifstream file_ext(dst_ext);
        std::string content((std::istreambuf_iterator<char>(file_ext)), std::istreambuf_iterator<char>());
        file_ext.close();
        if (!FGNS::Flat::exists(block, dst_ext))
        {
            dst_ext = FGNS::input_sanitizer_special_chars(dst_ext);
            FGNS::Flat::touch(block, dst_ext);
            FGNS::Flat::write(block, dst_ext, content);

            return true;
        }
        else
        {
            fprintf(stderr, "import: '%s' already exists\n", dst_ext.c_str());
            return false;
        }
    }
    else
    {
        fprintf(stderr, "import: '%s' does not exist\n", dst_ext.c_str());
        return false;
    }
}