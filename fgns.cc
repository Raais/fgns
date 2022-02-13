#include <iostream>
#include <string>
#include <vector>

#include "core_types.h"
#include "core_utils.h"

#include "cxxopts.hpp"

int main(int argc, char *argv[])
{
    FGNS::Block block;
    FGNS::touch(block, "test.txt");
    FGNS::touch(block, "test2.tsxt");
    FGNS::write(block, "test2.tsxt", "hfdsjhf fdsjh djsfhsdj fdskjfhdfkjds hfk jhsdkfh dskf");
    FGNS::touch(block, "test3.txt");
    FGNS::ls(block);

    return 0;
}