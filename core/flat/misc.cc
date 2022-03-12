#include "flat_utils.h"

FGNS::Flat::File* FGNS::Flat::get_file_ptr(std::vector<FGNS::Flat::File> &root, std::string dst, int mode)
{
    std::string q;
    for (auto &file : root)
    {
        (mode == 0) ? q = file.NAME : q = std::to_string(file.ID);
        
        if (q == dst)
            return &file;
    }
    return nullptr;
}

FGNS::Flat::File* FGNS::Flat::get_file_ptr(std::vector<FGNS::Flat::File*> &root, std::string dst, int mode)
{
    std::string q;
    for (auto &file : root)
    {
        (mode == 0) ? q = file->NAME : q = std::to_string(file->ID);
        
        if (q == dst)
            return file;
    }
    return nullptr;
}

/* ID is searched globally in root; Name search is confined to working dir; */
FGNS::Flat::File& FGNS::Flat::get_file_wrapper(FGNS::Flat::Block &block, std::string dst, int mode)
{
    FGNS::Flat::File* f = nullptr;
    if (mode == 1)
        f = FGNS::Flat::get_file_ptr(block.root, dst, 1);
    else
    {
        auto wd = FGNS::Flat::gen_dir_root(block.root, std::to_string(block.WORKDIR), 1);
        f = FGNS::Flat::get_file_ptr(wd, dst, 0);
    }
    return *f;
}

std::string FGNS::Flat::get_path(std::vector<FGNS::Flat::File> &root, std::string dst, int mode)
{
    std::string q;
    for (auto &file : root)
    {
        (mode == 0) ? q = file.NAME : q = std::to_string(file.ID);
        
        if (q == dst)
        {
            if (file.PARENT == -1)
                return "/" + file.NAME + "/";
            else
            {
                std::string path = file.NAME + "/";
                FGNS::Flat::File* parent = get_file_ptr(root, std::to_string(file.PARENT), 1);
                return get_path(root, std::to_string(parent->ID), 1) + path;
            }
        }
    }
    return "";
}

std::vector<FGNS::Flat::File*> FGNS::Flat::gen_dir_root(std::vector<FGNS::Flat::File> &root, std::string dst, int mode)
{
    std::vector<FGNS::Flat::File*> virtualroot;
    std::string q;
    if ((mode == 1) && (dst == "-1"))
    {
        for (auto &file : root)
        {
            if (file.PARENT == -1)
                virtualroot.push_back(&file);
        }
        return virtualroot;
    }
    else
    {
        for (auto &rootfile : root)
        {
            (mode == 0) ? q = rootfile.NAME : q = std::to_string(rootfile.ID);

            if (q == dst)
            {
                for (auto &fileid : rootfile.files)
                    virtualroot.push_back(get_file_ptr(root, std::to_string(fileid), 1));
                for (auto &dirid : rootfile.directories)
                    virtualroot.push_back(get_file_ptr(root, std::to_string(dirid), 1));
            }
        }
        return virtualroot;
    }
}

std::string FGNS::Flat::get_target_fuzzy(FGNS::Flat::Block &block, std::string dst)
{
    auto wd = FGNS::Flat::gen_dir_root(block.root, std::to_string(block.WORKDIR), 1);

    if (dst.back() == '*')
        dst.pop_back();

    bool match_found = false;
    double best_score = 0.0;
    std::string best_match;

    auto scorer = rapidfuzz::fuzz::CachedRatio<std::string>(dst);

    for (auto file : wd)
    {
        double score = scorer.ratio(file->NAME, best_score);
        if (score >= best_score)
        {
            best_score = score;
            best_match = file->NAME;
            match_found = true;
        }
    }

    if (!match_found)
        return "";

    return best_match;
}

std::string FGNS::Flat::checksum(FGNS::Flat::Block &block, std::string dst, int mode)
{
    if (FGNS::Flat::exists(block, dst, mode))
    {
        FGNS::Flat::File &file = FGNS::Flat::get_file_wrapper(block, dst, mode);
        return FGNS::Crypto::SHA512Digest(file.content);
    }
    else
        return "";
}