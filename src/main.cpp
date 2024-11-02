//
// Created by vinicius on 11/2/24.
//


#include <cstdlib>
#include "common.hpp"
#include "parser_config_file/check_file.h"

int main(int argc, char* argv[])
{
    std::string configuration_file;

    if(argc < 2)
        configuration_file = DEFAULT_CONFIG_FILE;
    else
        configuration_file = argv[1];

    check_file(configuration_file);

    return EXIT_SUCCESS;
}
