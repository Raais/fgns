#include "core_internals.h"

FGNS::File* FGNS::get_file_ptr(FGNS::Block &block, std::string dst, int mode)
{
    std::string q;
    for (auto &file : block.root)
    {
        (mode == 0) ? q = file.name : q = std::to_string(file.ID);
        
        if (q == dst)
        {
            return &file;
        }
    }
    return nullptr;
}

std::string FGNS::root_get_target_fuzzy(FGNS::Block &block, std::string dst)
{
    if (dst.back() == '*')
    {
        dst.pop_back();
    }

    bool match_found = false;
    double best_score = 0.0;
    std::string best_match;

    auto scorer = rapidfuzz::fuzz::CachedRatio<std::string>(dst);

    for (auto &file : block.root)
    {
        double score = scorer.ratio(file.name, best_score);
        if (score >= best_score)
        {
            best_score = score;
            best_match = file.name;
            match_found = true;
        }
    }

    if (!match_found)
    {
        return "";
    }

    return best_match;
}

std::string FGNS::fs_get_target_fuzzy(std::string dst_ext)
{
    if (dst_ext.back() == '*')
    {
        dst_ext.pop_back();
    }

    bool match_found = false;
    double best_score = 0.0;
    std::string best_match;

    auto scorer = rapidfuzz::fuzz::CachedRatio<std::string>(dst_ext);

    for (auto &file : fs::directory_iterator("."))
    {
        if (file.is_directory())
        {
            continue;
        }
        double score = scorer.ratio(file.path().string(), best_score);
        if (score >= best_score)
        {
            best_score = score;
            best_match = file.path().string();
            match_found = true;
        }
    }

    if (!match_found)
    {
        return "";
    }

    return best_match;
}

std::string FGNS::input_sanitizer_special_chars(std::string input)
{
    std::string output = input;
    if (output.find("/") != std::string::npos)
    {
        output = output.substr(output.find_last_of("/") + 1);
    }
    std::string special_chars = "\\/:*?^%&`'{}[]=+~!()-_$@#;,\"<>|";
    for (auto &c : special_chars)
    {
        output.erase(std::remove(output.begin(), output.end(), c), output.end());
    }
    return output;
}

bool FGNS::exists_ext(std::string dst_ext)
{
    std::ifstream f(dst_ext.c_str());
    return f.good();
}

void FGNS::save_bin(FGNS::Block &block, std::string dst_ext)
{
    std::ofstream file(dst_ext, std::ios::binary);
    cereal::BinaryOutputArchive archive(file);
    archive(CEREAL_NVP(block));
    file.close();
}


FGNS::Block FGNS::load_bin(std::string dst_ext)
{
    FGNS::Block block;
    std::ifstream file(dst_ext, std::ios::binary);
    cereal::BinaryInputArchive archive(file);
    archive(CEREAL_NVP(block));
    file.close();
    return block;
}

void FGNS::save_json(FGNS::Block &block, std::string dst_ext)
{
    std::ofstream file(dst_ext);
    cereal::JSONOutputArchive archive(file);
    archive(CEREAL_NVP(block));
    file.close();
}

FGNS::Block FGNS::load_json(std::string dst_ext)
{
    FGNS::Block block;
    std::ifstream file(dst_ext);
    cereal::JSONInputArchive archive(file);
    archive(CEREAL_NVP(block));
    file.close();
    return block;
}

void FGNS::put_fstream_string_bin(std::string path, std::string str)
{
    std::ofstream ofs(path, std::ios::out | std::ios::binary | std::ios::trunc);
    ofs.write(str.c_str(), str.size());
    ofs.close();
}

std::string FGNS::get_fstream_string(std::string path)
{
    std::ifstream f(path.c_str());
    std::string content((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
    f.close();
    return content;
}

bool FGNS::compress_ext(std::string dst_ext)
{
    if(FGNS::exists_ext(dst_ext))
        {
            if(dst_ext.find(".xz") == std::string::npos)
            {
                std::string out = dst_ext + ".xz";

                std::stringstream uncompressed;
                std::stringstream compressed;

                uncompressed << FGNS::get_fstream_string(dst_ext);

                xz::compress(uncompressed, compressed);

                FGNS::put_fstream_string_bin(out, compressed.str());
                fs::remove(dst_ext);

                return true;
            }
            else
            {
                fprintf(stderr, "File might already be compressed.\n");
                return false;
            }
        }
        else
        {
            fprintf(stderr, "File does not exist.\n");
            return false;
        }
}

bool FGNS::decompress_ext(std::string dst_ext)
{
    if(FGNS::exists_ext(dst_ext))
        {
            if(dst_ext.find(".xz") != std::string::npos)
            {
                std::string out = dst_ext.substr(0, dst_ext.size() - 3);

                std::stringstream uncompressed;
                std::stringstream compressed;

                compressed << FGNS::get_fstream_string(dst_ext);

                xz::decompress(compressed, uncompressed);

                FGNS::put_fstream_string_bin(out, uncompressed.str());
                fs::remove(dst_ext);

                return true;
            }
            else
            {
                fprintf(stderr, "File might already be decompressed.\n");
                return false;
            }
        }
        else
        {
            fprintf(stderr, "File does not exist.\n");
            return false;
        }
}