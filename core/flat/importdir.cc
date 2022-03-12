#include "flat_utils.h"

bool FGNS::Flat::importdir(FGNS::Flat::Block &block, std::string dstdir_ext)
{
    if (fs::exists(dstdir_ext) && fs::is_directory(dstdir_ext))
     {
        auto oldwd = block.WORKDIR;
        std::string dn = dstdir_ext;
        if (dn.find("/") != std::string::npos)
        {
            dn = dn.substr(dn.find_last_of("/") + 1);
        }
        FGNS::Flat::mkdir(block, dn);
        FGNS::Flat::cd(block, dn);
        for(fs::directory_iterator it(dstdir_ext), end; it != end; ++it)
        {
            
            if (fs::is_regular_file(it->path()))
            {
                std::string filename = it->path().filename().string();
                filename = FGNS::input_sanitizer_special_chars(filename);
                FGNS::Flat::touch(block, filename);
                FGNS::Flat::write(block, filename, FGNS::read_ext(it->path().string()));
            }
            else if (fs::is_directory(it->path()))
            {
                std::string dirname = it->path().string();
                FGNS::Flat::importdir(block, dirname);
            }
        }
        block.WORKDIR = oldwd;
        
        return true;
     }
     else
     {
        fprintf(stderr, "importdir: '%s' does not exist\n", dstdir_ext.c_str());
        return false;
     }
}