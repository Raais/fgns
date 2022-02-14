#include <iostream>
#include <string>
#include <vector>

#include "core_types.h"
#include "core_utils.h"

#include "cxxopts.hpp"

#define FGNS_VERSION         "0.9.0"
#define FGNS_BLOCK_EXTENSION ".fgns"

int main(int argc, char *argv[])
{
    int EXIT_CODE = 0;
    
    std::vector<std::string> args;
    for (int i = 1; i < argc; i++)
    {
        args.push_back(argv[i]);
    }

    cxxopts::Options options("fgns", "fgns - virtual file system");

    options.add_options()
        ("h,help", "print usage")
        ("v,version", "print version")
        ("f,file", "input block <FILE>", cxxopts::value<std::string>())
        ("t,target", "positional argument (-t <src> -...)", cxxopts::value<std::string>())
        ("l,ls", "print root")
        ("T,touch", "create <dst>", cxxopts::value<std::string>())
        ("r,rm", "delete <dst>", cxxopts::value<std::string>())
        ("c,cp", "(-t <src>) copy to <dst>", cxxopts::value<std::string>())
        ("m,mv", "(-t <src>) rename to <dst>", cxxopts::value<std::string>())
        ("C,cat", "cat <dst>", cxxopts::value<std::string>())
        ("w,write", "(-t <src>) write <content>", cxxopts::value<std::string>())
        ("Q,info", "print info about <dst>", cxxopts::value<std::string>())
        ("A,encrypt", "(-t <src>) AES encrypt with <password>", cxxopts::value<std::string>())
        ("D,decrypt", "(-t <src>) decrypt with <password>", cxxopts::value<std::string>())
        ("i,import", "import external <dst_ext>", cxxopts::value<std::string>())
        ("e,export", "(-t <src>) export to <dst_ext>", cxxopts::value<std::string>())
        ("q,exists", "check if <dst> exists", cxxopts::value<std::string>())
        ("X,compress", "compress block", cxxopts::value<std::string>())
        ("x,decompress", "decompress block.xz", cxxopts::value<std::string>())
        ("J,jsondump", "dump block as json to <dst_ext>", cxxopts::value<std::string>())
        ("Z,autosave", "toggle autosave", cxxopts::value<bool>()->default_value("false"))
        ("s,save", "save loaded block (if autosave off)")
        ("S,Save", "save buffer to file", cxxopts::value<std::string>());
    options.allow_unrecognised_options();

    cxxopts::ParseResult result;

    try
    {
        result = options.parse(argc, argv);
    }
    catch (std::exception &e)
    {
        std::cerr << "Invalid argument(s)" << std::endl;
        exit(1);
    }

    if (result.count("help"))
    {
        std::cout << options.help() << std::endl;
        exit(0);
    }

    if (result.count("version"))
    {
        std::cout << FGNS_VERSION << std::endl;
        exit(0);
    }

    if (result.count("compress"))
    {
        std::string arg = result["compress"].as<std::string>();
        exit(FGNS::compress_ext(arg));
    }

    if (result.count("decompress"))
    {
        std::string arg = result["decompress"].as<std::string>();
        exit(FGNS::decompress_ext(arg));
    }

    FGNS::Block block;

    std::string src;
    bool compressed = false;
    bool loaded = false;
    std::string loaded_file;

    if (result.count("file"))
    {
        std::string arg = result["file"].as<std::string>();
        if(arg.find(".xz") != std::string::npos)
        {
            FGNS::decompress_ext(arg);
            arg.erase(arg.size() - 3, 3);
            compressed = true;
        }
        if(FGNS::exists_ext(arg))
        {
            block = FGNS::load_bin(arg);
            loaded = true;
            loaded_file = arg;
        }
        else
        {
            std::cerr << "File not found: " << arg << std::endl;
            exit(1);
        }
    }

    if (loaded)
    {
        if (result.count("autosave"))
        {
            block.AUTOSAVE = !block.AUTOSAVE;
        }
    }

    if (result.count("jsondump"))
    {
        std::string arg = result["jsondump"].as<std::string>();
        FGNS::save_json(block, arg);
        exit(0);
    }

    for (auto &arg : result.arguments())
    {
        if (arg.key() == "target")
        {
            src = arg.value();
        }
        else if (arg.key() == "ls")
        {
            FGNS::ls(block);
        }
        else if (arg.key() == "touch")
        {
            EXIT_CODE = !FGNS::touch(block, arg.value());
        }
        else if (arg.key() == "rm")
        {
            std::string dst = arg.value();
            if (dst.front() == '@')
            {
                dst.erase(0, 1);
                EXIT_CODE = !FGNS::rm(block, dst, 1);
            }
            else
            {
                EXIT_CODE = !FGNS::rm(block, dst);
            }
        }
        else if (arg.key() == "cp")
        {
            std::string dst = arg.value();
            if (src.front() == '@')
            {
                src.erase(0, 1);
                EXIT_CODE = !FGNS::cp(block, src, dst, 1);
            }
            else
            {
                EXIT_CODE = !FGNS::cp(block, src, dst);
            }
        }
        else if (arg.key() == "mv")
        {
            std::string dst = arg.value();
            if (src.front() == '@')
            {
                src.erase(0, 1);
                EXIT_CODE = !FGNS::mv(block, src, dst, 1);
            }
            else
            {
                EXIT_CODE = !FGNS::mv(block, src, dst);
            }
        }
        else if (arg.key() == "cat")
        {
            std::string dst = arg.value();
            if (dst.front() == '@')
            {
                dst.erase(0, 1);
                EXIT_CODE = !FGNS::cat(block, dst, 1);
            }
            else
            {
                EXIT_CODE = !FGNS::cat(block, dst);
            }
        }
        else if (arg.key() == "write")
        {
            std::string dst = src;
            std::string content = arg.value();
            if (dst.front() == '@')
            {
                dst.erase(0, 1);
                EXIT_CODE = !FGNS::write(block, dst, content, 1);
            }
            else
            {
                EXIT_CODE = !FGNS::write(block, dst, content);
            }
        }
        else if (arg.key() == "info")
        {
            std::string dst = arg.value();
            if (dst.front() == '@')
            {
                dst.erase(0, 1);
                EXIT_CODE = !FGNS::info(block, dst, 1);
            }
            else
            {
                EXIT_CODE = !FGNS::info(block, dst);
            }
        }
        else if (arg.key() == "encrypt")
        {
            std::string dst = src;
            std::string password = arg.value();
            if (dst.front() == '@')
            {
                dst.erase(0, 1);
                EXIT_CODE = !FGNS::encrypt(block, dst, password, 1);
            }
            else
            {
                EXIT_CODE = !FGNS::encrypt(block, dst, password);
            }
        }
        else if (arg.key() == "decrypt")
        {
            std::string dst = src;
            std::string password = arg.value();
            if (dst.front() == '@')
            {
                dst.erase(0, 1);
                EXIT_CODE = !FGNS::decrypt(block, dst, password, 1);
            }
            else
            {
                EXIT_CODE = !FGNS::decrypt(block, dst, password);
            }
        }
        else if (arg.key() == "import")
        {
            EXIT_CODE = !FGNS::import(block, arg.value());
        }
        else if (arg.key() == "export")
        {
            std::string dst = arg.value();
            if (src.front() == '@')
            {
                src.erase(0, 1);
                EXIT_CODE = !FGNS::fexport(block, src, dst, 1);
            }
            else
            {
                EXIT_CODE = !FGNS::fexport(block, src, dst);
            }
        }
        else if (arg.key() == "exists")
        {
            std::string dst = arg.value();
            if (dst.front() == '@')
            {
                dst.erase(0, 1);
                EXIT_CODE = !FGNS::exists(block, dst, 1);
            }
            else
            {
                EXIT_CODE = !FGNS::exists(block, dst);
            }
        }
        else if (arg.key() == "save")
        {
            if (loaded)
            {
                if (!block.AUTOSAVE)
                {
                    FGNS::save_bin(block, loaded_file);
                }
            }
        }
        else if (arg.key() == "Save")
        {
            std::string filename = arg.value();
            if (filename.find(FGNS_BLOCK_EXTENSION) == std::string::npos)
            {
                filename += FGNS_BLOCK_EXTENSION;
            }
            FGNS::save_bin(block, filename);
        }
    }

    if (loaded)
    {
        if (block.AUTOSAVE)
        {
            FGNS::save_bin(block, loaded_file);
        }
        if (compressed)
        {
            FGNS::compress_ext(loaded_file);
        }
    }

    return EXIT_CODE;
}