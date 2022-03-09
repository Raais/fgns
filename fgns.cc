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
        ("t,target", "positional argument (-t <src> -...)", cxxopts::value<std::string>())
        ("l,ls", "print root")
        ("n,touch", "create <dst>", cxxopts::value<std::string>())
        ("r,rm", "delete <dst>", cxxopts::value<std::string>())
        ("c,cp", "(-t <src>) copy to <dst>", cxxopts::value<std::string>())
        ("m,mv", "(-t <src>) rename to <dst>", cxxopts::value<std::string>())
        ("C,cat", "cat <dst>", cxxopts::value<std::string>())
        ("w,write", "(-t <src>) write <content>", cxxopts::value<std::string>())
        ("Q,info", "print info about <dst>", cxxopts::value<std::string>())
        ("A,encrypt", "(-t <src>) AES encrypt with <password>", cxxopts::value<std::string>())
        ("D,decrypt", "(-t <src>) decrypt with <password>", cxxopts::value<std::string>())
        ("i,import", "import external <dst_ext>", cxxopts::value<std::string>())
        ("I,importdir", "import external <dstdir_ext>\nSee also:\n"\
                        "find <expr> -type f -exec fgns -f <FILE> -i {} \\;",\
         cxxopts::value<std::string>())
        ("e,export", "(-t <src>) export to <dst_ext>", cxxopts::value<std::string>())
        ("q,exists", "check if <dst> exists and return 0/1", cxxopts::value<std::string>())
        ("X,compress", "compress <FILE>", cxxopts::value<std::string>())
        ("x,decompress", "decompress <FILE.xz>", cxxopts::value<std::string>())
        ("J,jsondump", "dump block as json to <dst_ext>", cxxopts::value<std::string>())
        ("Z,autosave", "toggle autosave (-s)\n"\
                       "blocks will be autosaved by default",\ 
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

    FGNS::FlatBlock block;

    std::string src;
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
            block = FGNS::load_bin(arg);
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

    if (result.count("jsondump"))
    {
        std::string arg = result["jsondump"].as<std::string>();
        FGNS::save_json(block, arg);
        exit(0);
    }

    for (auto &arg : result.arguments())
    {
        if (arg.key() == "target")
            src = arg.value();

        else if (arg.key() == "ls")
            FGNS::ls(block);

        else if (arg.key() == "touch")
            EXIT_CODE = !FGNS::touch(block, arg.value());

        else if (arg.key() == "rm")
        {
            std::string dst = arg.value();
            if (dst.front() == '@')
            {
                dst.erase(0, 1);
                EXIT_CODE = !FGNS::rm(block, dst, 1);
            }
            else
                EXIT_CODE = !FGNS::rm(block, dst);
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
                EXIT_CODE = !FGNS::cp(block, src, dst);
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
                EXIT_CODE = !FGNS::mv(block, src, dst);
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
                EXIT_CODE = !FGNS::cat(block, dst);
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
                EXIT_CODE = !FGNS::write(block, dst, content);
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
                EXIT_CODE = !FGNS::info(block, dst);
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
                EXIT_CODE = !FGNS::encrypt(block, dst, password);
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
                EXIT_CODE = !FGNS::decrypt(block, dst, password);
        }

        else if (arg.key() == "import")
            EXIT_CODE = !FGNS::import(block, arg.value());

        else if (arg.key() == "importdir")
            EXIT_CODE = !FGNS::importdir(block, arg.value());

        else if (arg.key() == "export")
        {
            std::string dst = arg.value();
            if (src.front() == '@')
            {
                src.erase(0, 1);
                EXIT_CODE = !FGNS::fexport(block, src, dst, 1);
            }
            else
                EXIT_CODE = !FGNS::fexport(block, src, dst);
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
                EXIT_CODE = !FGNS::exists(block, dst);
        }

        else if (arg.key() == "save")
        {
            if (mounted)
                if (!block.AUTOSAVE)
                    FGNS::save_bin(block, mounted_file);
        }

        else if (arg.key() == "Save")
        {
            std::string filename = arg.value();
            if (!FGNS::has_suffix(filename, FLAT_BLOCK_EXTENSION))
                filename += FLAT_BLOCK_EXTENSION;

            FGNS::save_bin(block, filename);
        }
    }

    if (mounted)
    {
        if (block.AUTOSAVE)
            FGNS::save_bin(block, mounted_file);
        if (compressed)
            FGNS::compress_ext(mounted_file);
    }

    return EXIT_CODE;
}
