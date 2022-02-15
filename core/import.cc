#include "core_utils.h"

bool FGNS::import(FGNS::Block &block, std::string dst_ext)
{
    if (dst_ext.back() == '*')
    {
        dst_ext = FGNS::fs_get_target_fuzzy(dst_ext);
    }
    if(FGNS::exists_ext(dst_ext))
    {
        std::ifstream file_ext(dst_ext);
        std::string content((std::istreambuf_iterator<char>(file_ext)), std::istreambuf_iterator<char>());
        file_ext.close();
        if(!FGNS::exists(block, dst_ext))
        {
            dst_ext = FGNS::input_sanitizer_special_chars(dst_ext);
            FGNS::touch(block, dst_ext);
            FGNS::write(block, dst_ext, content);

            return true;
        }
        else
        {
            fprintf(stderr, "File already exists\n");
            return false;
        }
    }
    else
    {
        fprintf(stderr, "File does not exist\n");
        return false;
    }
}