#include "core_utils.h"

std::string FGNS::get_file_magic(std::string dst_ext)
{
    std::string content = "";
    std::string magic = "";

    if (!FGNS::exists_ext(dst_ext))
        return magic;
    
    content = FGNS::read_ext(dst_ext);
    if (content.size() < 6)
        return magic;

    unsigned char *buf = (unsigned char *)content.c_str();

    if (content.size() >= 6 && buf[0] == 0xfd && buf[1] == 0x37 && buf[2] == 0x7a && buf[3] == 0x58 && buf[4] == 0x5a && buf[5] == 0x00)
        return magic = "xz";
        
    if (content.size() >= 8 && buf[0] == 0xde && buf[1] == 0x90 && buf[2] == 0xde && buf[3] == 0x83 && buf[4] == 0x66 && buf[5] == 0x67 && buf[6] == 0x6E && buf[7] == 0x73)
        return magic = "fgnsflat";

    return magic;
}