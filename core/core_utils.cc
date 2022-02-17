#include "core_utils.h"

FGNS::File* FGNS::get_file_ptr(std::vector<FGNS::File> &root, std::string dst, int mode)
{
    std::string q;
    for (auto &file : root)
    {
        (mode == 0) ? q = file.name : q = std::to_string(file.ID);
        
        if (q == dst)
            return &file;
    }
    return nullptr;
}

std::string FGNS::root_get_target_fuzzy(std::vector<FGNS::File> &root, std::string dst)
{
    if (dst.back() == '*')
        dst.pop_back();

    bool match_found = false;
    double best_score = 0.0;
    std::string best_match;

    auto scorer = rapidfuzz::fuzz::CachedRatio<std::string>(dst);

    for (auto &file : root)
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
        return "";

    return best_match;
}

std::string FGNS::fs_get_target_fuzzy(std::string dst_ext)
{
    if (dst_ext.back() == '*')
        dst_ext.pop_back();

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
        return "";

    return best_match;
}

std::string FGNS::input_sanitizer_special_chars(std::string input)
{
    std::string output = input;

    if (output.find("/") != std::string::npos)
        output = output.substr(output.find_last_of("/") + 1);

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

void FGNS::write_ext_bin(std::string path, std::string str)
{
    std::ofstream ofs(path, std::ios::out | std::ios::binary | std::ios::trunc);
    ofs.write(str.c_str(), str.size());
    ofs.close();
}

std::string FGNS::read_ext(std::string path)
{
    std::ifstream f(path.c_str());
    std::string content((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
    f.close();
    return content;
}

bool FGNS::compress_ext(std::string dst_ext)
{
    if (FGNS::exists_ext(dst_ext))
        {
            if (FGNS::get_file_magic(dst_ext) != "xz")
            {
                std::string out = dst_ext + ".xz";

                std::stringstream uncompressed;
                std::stringstream compressed;

                uncompressed << FGNS::read_ext(dst_ext);

                xz::compress(uncompressed, compressed);

                FGNS::write_ext_bin(out, compressed.str());
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
    if (FGNS::exists_ext(dst_ext))
        {
            if (FGNS::get_file_magic(dst_ext) == "xz")
            {
                std::string out = dst_ext;
                if (FGNS::has_suffix(out, ".xz"))
                    out.erase(out.size() - 3, 3);

                std::stringstream uncompressed;
                std::stringstream compressed;

                compressed << FGNS::read_ext(dst_ext);

                xz::decompress(compressed, uncompressed);

                FGNS::write_ext_bin(out, uncompressed.str());
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

bool FGNS::has_suffix(std::string str, std::string suffix)
{
    return str.size() >= suffix.size() && str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}