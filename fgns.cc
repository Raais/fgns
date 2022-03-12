#include <cstdio>
#include <string>
#include <vector>

#include "flat.h"
//#include "tree.h"

#include "cxxopts.hpp"

#define FGNS_VERSION "0.9.2"
#define LICENSE_TEXT "Copyright (c) 2022 Raais N.\n" \
"\n" \
"THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR\n" \
"IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,\n" \
"FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE\n" \
"AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER\n" \
"LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,\n" \
"OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE\n" \
"SOFTWARE.\n"


int main(int argc, char *argv[])
{
    int EXIT_CODE = 0;

    cxxopts::Options options("fgns", "fgns - virtual file system");

    options.add_options()
        ("h,help", "print usage")
        ("v,version", "print version")
        ("f,mount", "mount block <FILE>", cxxopts::value<std::string>())
        ("L,load", "load in interactive mode", cxxopts::value<bool>()->default_value("false"))
        ("t,target", "positional argument (-t <src> -...)", cxxopts::value<std::string>())
        ("l,ls", "print root")
        ("c,cd", "cd to <dst>", cxxopts::value<std::string>())
        ("n,touch", "create file <dst>", cxxopts::value<std::string>())
        ("m,mkdir", "create directory <dst>", cxxopts::value<std::string>())
        ("r,rm", "delete <dst>", cxxopts::value<std::string>())
        ("p,cp", "(-t <src>) copy to <dst>", cxxopts::value<std::string>())
        ("M,mv", "(-t <src>) rename to <dst>", cxxopts::value<std::string>())
        ("C,cat", "cat <dst>", cxxopts::value<std::string>())
        ("w,write", "(-t <src>) write <content>", cxxopts::value<std::string>())
        ("Q,info", "print info about <dst>", cxxopts::value<std::string>())
        ("A,encrypt", "(-t <src>) AES encrypt with <password>", cxxopts::value<std::string>())
        ("D,decrypt", "(-t <src>) decrypt with <password>", cxxopts::value<std::string>())
        ("i,import", "import external <dst_ext>", cxxopts::value<std::string>())
        ("I,importdir", "import external <dstdir_ext>\nSee also:\n"\
                        "find <expr> -type f -exec fgns -f <FILE> -i {} \\;",\
         cxxopts::value<std::string>())
        ("e,export", "export <dst>", cxxopts::value<std::string>())
        ("E,exportdir", "export directory <dst>", cxxopts::value<std::string>())
        ("q,exists", "check if <dst> exists and return 0/1", cxxopts::value<std::string>())
        ("H,checksum", "print SHA512 sum of <dst>", cxxopts::value<std::string>())
        ("X,compress", "compress <FILE>", cxxopts::value<std::string>())
        ("x,decompress", "decompress <FILE.xz>", cxxopts::value<std::string>())
        ("J,jsondump", "dump block as json to <dst_ext>", cxxopts::value<std::string>())
        ("Z,autosave", "toggle autosave (-s)\n"\
                       "blocks are autosaved by default",\
         cxxopts::value<bool>()->default_value("false"))
        ("s,save", "save mounted block (if autosave off)")
        ("S,Save", "save buffer to <FILE> / create new empty <FILE>", cxxopts::value<std::string>());
    options.allow_unrecognised_options();

    cxxopts::ParseResult result;

    try { result = options.parse(argc, argv); }
    catch (std::exception &e)
    {
        fprintf(stderr, "Invalid argument(s)\n");
        exit(1);
    }

    if (result.count("help"))
    {
        printf("%s\n\n"\
        "Commands can be repeated and will be executed in order.\n"\
        "Append asterisk (*) to a query to get fuzzy/closest match.\n"\
            "\tEg. \"--cat paswrod.txt*\" -> password.txt\n"\
        "\"@id\" can also be used in a query instead of the filename.\n"\
            "\tEg. \"--rm @0\" -> deleted [0] - cat.png\n",\
        options.help().c_str());
        exit(0);
    }

    if (result.count("version"))
    {
        printf("fgns %s\n%s\n", FGNS_VERSION, LICENSE_TEXT);
        exit(0);
    }

    if (result.count("compress"))
    {
        std::string arg = result["compress"].as<std::string>();
        exit(!FGNS::compress_ext(arg));
    }

    if (result.count("decompress"))
    {
        std::string arg = result["decompress"].as<std::string>();
        if (!FGNS::exists_ext(arg))
            if (FGNS::exists_ext(arg + ".xz"))
            {
                fprintf(stderr, "Did you mean %s.xz ?\n", arg.c_str());
                exit(1);
            }
        exit(!FGNS::decompress_ext(arg));
    }

    FGNS::Flat::Block block;

    std::string src_g;
    bool compressed = false;
    bool mounted = false;
    std::string mounted_file;

    if (result.count("mount"))
    {
        std::string arg = result["mount"].as<std::string>();
        if (FGNS::exists_ext(arg))
        {
            if (FGNS::get_file_magic(arg) == "xz")
            {
                FGNS::decompress_ext(arg);
                arg.erase(arg.size() - 3, 3);
                compressed = true;
            }
            else if (FGNS::get_file_magic(arg) != "fgnsflat")
            {
                fprintf(stderr, "Invalid or corrupted file\n");
                exit(1);
            }
            block = FGNS::Flat::load_bin(arg);
            mounted = true;
            mounted_file = arg;
        }
        else
        {
            fprintf(stderr, "File not found: %s\n", arg.c_str());
            exit(1);
        }
    }

    if (mounted)
    {
        if (result.count("autosave"))
            block.AUTOSAVE = !block.AUTOSAVE;
    }

    for (auto &arg : result.arguments())
    {
        if (arg.key() == "target")
            src_g = arg.value();

        else if (arg.key() == "ls")
            FGNS::Flat::ls(block);

        else if (arg.key() == "cd")
        {
            std::string dst = arg.value();
            if (dst.front() == '@')
            {
                dst.erase(0, 1);
                EXIT_CODE = !FGNS::Flat::cd(block, dst, 1);
            }
            else
                EXIT_CODE = !FGNS::Flat::cd(block, dst);
        }

        else if (arg.key() == "touch")
            EXIT_CODE = !FGNS::Flat::touch(block, arg.value());

        else if (arg.key() == "mkdir")
            EXIT_CODE = !FGNS::Flat::mkdir(block, arg.value());

        else if (arg.key() == "rm")
        {
            std::string dst = arg.value();
            if (dst.front() == '@')
            {
                dst.erase(0, 1);
                EXIT_CODE = !FGNS::Flat::rm(block, dst, 1);
            }
            else
                EXIT_CODE = !FGNS::Flat::rm(block, dst);
        }

        else if (arg.key() == "cp")
        {
            std::string dst = arg.value();
            if (src_g.front() == '@')
            {
                src_g.erase(0, 1);
                EXIT_CODE = !FGNS::Flat::cp(block, src_g, dst, 1);
            }
            else
                EXIT_CODE = !FGNS::Flat::cp(block, src_g, dst);
        }

        else if (arg.key() == "mv")
        {
            std::string dst = arg.value();
            if (src_g.front() == '@')
            {
                src_g.erase(0, 1);
                EXIT_CODE = !FGNS::Flat::mv(block, src_g, dst, 1);
            }
            else
                EXIT_CODE = !FGNS::Flat::mv(block, src_g, dst);
        }

        else if (arg.key() == "cat")
        {
            std::string dst = arg.value();
            if (dst.front() == '@')
            {
                dst.erase(0, 1);
                EXIT_CODE = !FGNS::Flat::cat(block, dst, 1);
            }
            else
                EXIT_CODE = !FGNS::Flat::cat(block, dst);
        }

        else if (arg.key() == "write")
        {
            std::string dst = src_g;
            std::string content = arg.value();
            if (dst.front() == '@')
            {
                dst.erase(0, 1);
                EXIT_CODE = !FGNS::Flat::write(block, dst, content, 1);
            }
            else
                EXIT_CODE = !FGNS::Flat::write(block, dst, content);
        }

        else if (arg.key() == "info")
        {
            std::string dst = arg.value();
            if (dst.front() == '@')
            {
                dst.erase(0, 1);
                EXIT_CODE = !FGNS::Flat::info(block, dst, 1);
            }
            else
                EXIT_CODE = !FGNS::Flat::info(block, dst);
        }

        else if (arg.key() == "encrypt")
        {
            std::string dst = src_g;
            std::string password = arg.value();
            if (dst.front() == '@')
            {
                dst.erase(0, 1);
                EXIT_CODE = !FGNS::Flat::encrypt(block, dst, password, 1);
            }
            else
                EXIT_CODE = !FGNS::Flat::encrypt(block, dst, password);
        }

        else if (arg.key() == "decrypt")
        {
            std::string dst = src_g;
            std::string password = arg.value();
            if (dst.front() == '@')
            {
                dst.erase(0, 1);
                EXIT_CODE = !FGNS::Flat::decrypt(block, dst, password, 1);
            }
            else
                EXIT_CODE = !FGNS::Flat::decrypt(block, dst, password);
        }

        else if (arg.key() == "import")
            EXIT_CODE = !FGNS::Flat::import(block, arg.value());

        else if (arg.key() == "importdir")
            EXIT_CODE = !FGNS::Flat::importdir(block, arg.value());

        else if (arg.key() == "export")
        {
            std::string dst = arg.value();
            if (dst.front() == '@')
            {
                dst.erase(0, 1);
                EXIT_CODE = !FGNS::Flat::fexport(block, dst, 1);
            }
            else
                EXIT_CODE = !FGNS::Flat::fexport(block, dst);
        }

        else if (arg.key() == "exportdir")
        {
            std::string dst = arg.value();
            if (dst.front() == '@')
            {
                dst.erase(0, 1);
                EXIT_CODE = !FGNS::Flat::exportdir(block, dst, 1);
            }
            else
                EXIT_CODE = !FGNS::Flat::exportdir(block, dst);
        }

        else if (arg.key() == "exists")
        {
            std::string dst = arg.value();
            if (dst.front() == '@')
            {
                dst.erase(0, 1);
                EXIT_CODE = !FGNS::Flat::exists(block, dst, 1);
            }
            else
                EXIT_CODE = !FGNS::Flat::exists(block, dst);
        }

        else if (arg.key() == "checksum")
        {
            std::string dst = arg.value();
            if (dst.front() == '@')
            {
                dst.erase(0, 1);
                printf("%s\n", FGNS::Flat::checksum(block, dst, 1).c_str());
            }
            else
                printf("%s\n", FGNS::Flat::checksum(block, dst).c_str());
        }

        else if (arg.key() == "save")
        {
            if (mounted)
                if (!block.AUTOSAVE)
                    FGNS::Flat::save_bin(block, mounted_file);
        }

        else if (arg.key() == "Save")
        {
            std::string filename = arg.value();
            if (!FGNS::has_suffix(filename, FLAT_BLOCK_EXTENSION))
                filename += FLAT_BLOCK_EXTENSION;

            FGNS::Flat::save_bin(block, filename);
            mounted = true;
            mounted_file = filename;
        }
    }

    if (result.count("jsondump"))
    {
        std::string arg = result["jsondump"].as<std::string>();
        FGNS::Flat::save_json(block, arg);
        exit(0);
    }

    if (mounted)
    {
        if (block.AUTOSAVE)
            FGNS::Flat::save_bin(block, mounted_file);
        if (compressed)
            FGNS::compress_ext(mounted_file);
    }

    if (!result.count("load"))
    {
        return EXIT_CODE;
    }

    // interactive mode
    printf("FGNS %s © Raais N. 2022\n", FGNS_VERSION);
    std::string command;
    while (std::getline(std::cin, command))
    {
        std::vector<std::string> cmd = FGNS::usplit(command, " ");
        if (cmd[0] == "help")
        {

        }

        else if (cmd[0] == "exit")
        {
            if (cmd.size() == 2)
            {
                if (cmd[1] == "force")
                {
                    return 0;
                }
            }
            
            if (mounted)
            {
                if (!block.SAVED)
                {
                    printf("WARNING: mounted block not saved.\n");
                    printf("Save block or exit force\n");
                }
                else
                {
                    return 0;
                }
            }
            else
            {
                // a one-liner can easily be recovered by pressing up
                // this is not the case for interactive mode
                if (!block.SAVED)
                {
                    printf("WARNING: block not saved.\n");
                    printf("Save block or exit force\n");
                }
                else
                {
                    return 0;
                }
            }
        }

        else if (cmd[0] == "ls")
        {
            FGNS::Flat::ls(block);
        }

        else if (cmd[0] == "cd")
        {
            if (cmd.size() >= 2)
            {
                std::string dst = cmd[1];
                if (dst.front() == '@')
                {
                    dst.erase(0, 1);
                    FGNS::Flat::cd(block, dst, 1);
                }
                else
                    FGNS::Flat::cd(block, dst);
            }
        }

        else if (cmd[0] == "touch")
        {
            if (cmd.size() >= 2)
            {
                FGNS::Flat::touch(block, cmd[1]);
            }
        }

        else if (cmd[0] == "mkdir")
        {
            if (cmd.size() >= 2)
            {
                FGNS::Flat::mkdir(block, cmd[1]);
            }
        }

        else if (cmd[0] == "touch")
        {
            if (cmd.size() == 1)
                FGNS::Flat::touch(block, "");
            else
                FGNS::Flat::touch(block, cmd[1]);
        }

        else if (cmd[0] == "rm")
        {
            if (cmd.size() >= 2)
            {
                std::string dst = cmd[1];
                if (dst.front() == '@')
                {
                    dst.erase(0, 1);
                    FGNS::Flat::rm(block, dst, 1);
                }
                else
                    FGNS::Flat::rm(block, dst);
            }
        }

        else if (cmd[0] == "cp")
        {
            if (cmd.size() >= 3)
            {
                std::string src = cmd[1];
                if (src.front() == '@')
                {
                    src.erase(0, 1);
                    FGNS::Flat::cp(block, src, cmd[2], 1);
                }
                else
                    FGNS::Flat::cp(block, src, cmd[2]);
            }
        }

        else if (cmd[0] == "mv")
        {
            if (cmd.size() >= 3)
            {
                std::string src = cmd[1];
                if (src.front() == '@')
                {
                    src.erase(0, 1);
                    FGNS::Flat::mv(block, src, cmd[2], 1);
                }
                else
                    FGNS::Flat::mv(block, src, cmd[2]);
            }
        }

        else if (cmd[0] == "cat")
        {
            if (cmd.size() >= 2)
            {
                std::string dst = cmd[1];
                if (dst.front() == '@')
                {
                    dst.erase(0, 1);
                    FGNS::Flat::cat(block, dst, 1);
                }
                else
                    FGNS::Flat::cat(block, dst);
            }
        }

        else if (cmd[0] == "write")
        {
            if (cmd.size() >= 3)
            {
                std::string src = cmd[1];
                if (src.front() == '@')
                {
                    src.erase(0, 1);
                    FGNS::Flat::write(block, src, cmd[2], 1);
                }
                else
                    FGNS::Flat::write(block, src, cmd[2]);
            }
        }

        else if (cmd[0] == "info")
        {
            if (cmd.size() >= 2)
            {
                std::string dst = cmd[1];
                if (dst.front() == '@')
                {
                    dst.erase(0, 1);
                    FGNS::Flat::info(block, dst, 1);
                }
                else
                    FGNS::Flat::info(block, dst);
            }
        }

        else if (cmd[0] == "encrypt")
        {
            if (cmd.size() >= 3)
            {
                std::string dst = cmd[1];
                if (dst.front() == '@')
                {
                    dst.erase(0, 1);
                    FGNS::Flat::encrypt(block, dst, cmd[2], 1);
                }
                else
                    FGNS::Flat::encrypt(block, dst, cmd[2]);
            }
        }

        else if (cmd[0] == "decrypt")
        {
            if (cmd.size() >= 3)
            {
                std::string dst = cmd[1];
                if (dst.front() == '@')
                {
                    dst.erase(0, 1);
                    FGNS::Flat::decrypt(block, dst, cmd[2], 1);
                }
                else
                    FGNS::Flat::decrypt(block, dst, cmd[2]);
            }
        }

        else if (cmd[0] == "import")
        {
            FGNS::Flat::import(block, cmd[1]);
        }

        else if (cmd[0] == "importdir")
        {
            FGNS::Flat::importdir(block, cmd[1]);
        }

        else if (cmd[0] == "export")
        {
            if (cmd.size() >= 2)
            {
                std::string dst = cmd[1];
                if (dst.front() == '@')
                {
                    dst.erase(0, 1);
                    FGNS::Flat::fexport(block, dst, 1);
                }
                else
                    FGNS::Flat::fexport(block, dst);
            }
        }

        else if (cmd[0] == "exportdir")
        {
            if (cmd.size() >= 2)
            {
                std::string dst = cmd[1];
                if (dst.front() == '@')
                {
                    dst.erase(0, 1);
                    FGNS::Flat::exportdir(block, dst, 1);
                }
                else
                    FGNS::Flat::exportdir(block, dst);
            }
        }

        else if (cmd[0] == "exists")
        {
            if (cmd.size() >= 2)
            {
                std::string dst = cmd[1];
                if (dst.front() == '@')
                {
                    dst.erase(0, 1);
                    FGNS::Flat::exists(block, dst, 1);
                }
                else
                    FGNS::Flat::exists(block, dst);
            }
        }

        else if (cmd[0] == "checksum")
        {
            if (cmd.size() >= 2)
            {
                std::string dst = cmd[1];
                if (dst.front() == '@')
                {
                    dst.erase(0, 1);
                    printf("%s\n", FGNS::Flat::checksum(block, dst, 1).c_str());
                }
                else
                    printf("%s\n", FGNS::Flat::checksum(block, dst).c_str());
            }
        }

        else if (cmd[0] == "mount")
        {
            if (cmd.size() >= 2)
            {
                std::string arg = cmd[1];
                if (FGNS::exists_ext(arg))
                {
                    if (FGNS::get_file_magic(arg) == "xz")
                    {
                        FGNS::decompress_ext(arg);
                        arg.erase(arg.size() - 3, 3);
                        compressed = true;
                    }
                    else if (FGNS::get_file_magic(arg) != "fgnsflat")
                    {
                        fprintf(stderr, "Invalid or corrupted file\n");
                    }
                    block = FGNS::Flat::load_bin(arg);
                    mounted = true;
                    mounted_file = arg;
                }
                else
                {
                    fprintf(stderr, "File not found: %s\n", arg.c_str());
                }
            }
        }

        else if (cmd[0] == "save")
        {
            if (cmd.size() == 1)
            {
                if (mounted)
                    {
                        FGNS::Flat::save_bin(block, mounted_file);
                        if (compressed)
                            FGNS::compress_ext(mounted_file);
                    }
                else
                    fprintf(stderr, "save: missing operand 'filename'\n");
            }
            else if (cmd.size() >= 2)
            {
                std::string filename = cmd[1];
                if (!FGNS::has_suffix(filename, FLAT_BLOCK_EXTENSION))
                    filename += FLAT_BLOCK_EXTENSION;

                FGNS::Flat::save_bin(block, filename);
                mounted = true;
                mounted_file = filename;
            }
        }

        else if (cmd[0] == "jsondump")
        {
            if (cmd.size() >= 2)
            {
                FGNS::Flat::save_json(block, cmd[1]);
            }
        }
    }

    return 0;
}
