#include "flat_utils.h"

bool FGNS::Flat::importdir(FGNS::Flat::Block &block, std::string dstdir_ext)
{
    if (dstdir_ext.back() == '%')
    {
        dstdir_ext.pop_back();
        if (fs::exists(dstdir_ext) && fs::is_directory(dstdir_ext))
         {
            auto oldwd = block.WORKDIR;
            FGNS::Flat::mkdir(block, dstdir_ext);
            FGNS::Flat::cd(block, dstdir_ext);
            for(fs::recursive_directory_iterator it(dstdir_ext), end; it != end; ++it)
            {
                
                if (fs::is_regular_file(it->path()))
                {
                    std::string filename = it->path().string();
                    filename = FGNS::input_sanitizer_special_chars(filename);
                    FGNS::Flat::touch(block, filename);
                    FGNS::Flat::write(block, filename, FGNS::read_ext(it->path().string()));
                }
            }
            block.WORKDIR = oldwd;

            return true;
         }
         else
         {
            fprintf(stderr, "importdir: directory does not exist\n");
            return false;
         }
    }
    else
    {
        if (fs::exists(dstdir_ext) && fs::is_directory(dstdir_ext))
         {
            auto oldwd = block.WORKDIR;
            FGNS::Flat::mkdir(block, dstdir_ext);
            FGNS::Flat::cd(block, dstdir_ext);
            for(fs::directory_iterator it(dstdir_ext), end; it != end; ++it)
            {
                if (fs::is_regular_file(it->path()))
                {
                    std::string filename = it->path().string();
                    filename = FGNS::input_sanitizer_special_chars(filename);
                    FGNS::Flat::touch(block, filename);
                    FGNS::Flat::write(block, filename, FGNS::read_ext(it->path().string()));
                }
            }
            block.WORKDIR = oldwd;

            return true;
         }
         else
         {
            fprintf(stderr, "importdir: directory does not exist\n");
            return false;
         }
    }
}