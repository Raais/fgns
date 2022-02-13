#include "core_internals.h"

FGNS::File* FGNS::getf_ptr(FGNS::Block &block, std::string target, int mode)
{
    std::string q;
    for (auto &file : block.root)
    {
        (mode == 0) ? q = file.name : q = std::to_string(file.ID);
        
        if (q == target)
        {
            return &file;
        }
    }
    return nullptr;
}

std::string FGNS::root_get_target_fuzzy(FGNS::Block &block, std::string target)
{
    if (target.back() == '*')
    {
        target.pop_back();
    }

    bool match_found = false;
    double best_score = 0.0;
    std::string best_match;

    auto scorer = rapidfuzz::fuzz::CachedRatio<std::string>(target);

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

bool FGNS::exists_ext(std::string target_ext)
{
    std::ifstream f(target_ext.c_str());
    return f.good();
}