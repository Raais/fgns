#include "flat_utils.h"

void FGNS::save_bin(FGNS::FlatBlock &block, std::string dst_ext)
{
    std::ofstream file(dst_ext, std::ios::binary);
    unsigned char magic[] = {0xDE, 0x90, 0xDE, 0x83, 0x66, 0x67, 0x6E, 0x73};
    file.write((char*)magic, 8);
    cereal::BinaryOutputArchive archive(file);
    archive(CEREAL_NVP(block));
    file.close();
}

FGNS::FlatBlock FGNS::load_bin(std::string dst_ext)
{
    FGNS::FlatBlock block;
    std::ifstream file(dst_ext, std::ios::binary);
    file.seekg(8, std::ios::beg);
    cereal::BinaryInputArchive archive(file);
    archive(CEREAL_NVP(block));
    file.close();
    return block;
}

void FGNS::save_json(FGNS::FlatBlock &block, std::string dst_ext)
{
    std::ofstream file(dst_ext);
    cereal::JSONOutputArchive archive(file);
    archive(CEREAL_NVP(block));
    file.close();
}

FGNS::FlatBlock FGNS::load_json(std::string dst_ext)
{
    FGNS::FlatBlock block;
    std::ifstream file(dst_ext);
    cereal::JSONInputArchive archive(file);
    archive(CEREAL_NVP(block));
    file.close();
    return block;
}