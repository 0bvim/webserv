//
// Created by vinicius on 11/2/24.
//

#include "check_file.h"

#include <cstdlib>
#include <fstream>
#include <iostream>

void check_file(std::string fileName)
{
    if (!fopen(fileName.c_str(), "r"))
    {
        std::cerr << "Configuration file does not exist." << std::endl;
        exit(0);
    }

    std::ifstream file(fileName.c_str());
    file.seekg(0, std::ios::end);
    if (file.tellg() == 0)
    {
      std::cerr << "Configuration file is empty." << std::endl;
      exit(0);
    }
}