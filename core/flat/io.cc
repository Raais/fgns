#include "flat_utils.h"

void FGNS::Flat::save_bin(FGNS::Flat::Block &block, std::string dst_ext)
{
    block.SAVED = true;
    block.WORKDIR = -1;
    std::ofstream file(dst_ext, std::ios::binary);
    unsigned char magic[] = {0xDE, 0x90, 0xDE, 0x83, 0x66, 0x67, 0x6E, 0x73};
    file.write((char*)magic, 8);
    cereal::BinaryOutputArchive archive(file);
    archive(CEREAL_NVP(block));
    file.close();
}

FGNS::Flat::Block FGNS::Flat::load_bin(std::string dst_ext)
{
    FGNS::Flat::Block block;
    std::ifstream file(dst_ext, std::ios::binary);
    file.seekg(8, std::ios::beg);
    cereal::BinaryInputArchive archive(file);
    archive(CEREAL_NVP(block));
    file.close();
    block.SAVED = true;
    block.WORKDIR = -1;
    return block;
}

void FGNS::Flat::save_json(FGNS::Flat::Block &block, std::string dst_ext)
{
    std::stringstream ss;
    cereal::JSONOutputArchive archive(ss);
    archive(CEREAL_NVP(block));

    if (dst_ext == "stdout")
    {
        std::cout << ss.str() << std::endl;
    }
    else
    {
        std::ofstream file(dst_ext);
        file << ss.str();
        file.close();
    }
}

FGNS::Flat::Block FGNS::Flat::load_json(std::string dst_ext)
{
    FGNS::Flat::Block block;
    std::ifstream file(dst_ext);
    cereal::JSONInputArchive archive(file);
    archive(CEREAL_NVP(block));
    file.close();
    return block;
}