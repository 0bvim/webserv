#ifndef TEST_HELPERS_HPP
#define TEST_HELPERS_HPP

#include <string>
#include <cstdio>

/**
 * Helper function to get the path to the mimes.json file.
 * This function checks multiple possible paths to find the file.
 */
inline std::string getMimesPath() {
    // Try different relative paths that might work
    const char* paths[] = {
        "resources/src/parser/mime/mimes.json",  // From build dir
        "../src/parser/mime/mimes.json",         // From project root
        "../../src/parser/mime/mimes.json"       // From nested build dir
    };
    
    for (const char* path : paths) {
        FILE* file = fopen(path, "r");
        if (file) {
            fclose(file);
            return path;
        }
    }
    
    // If all else fails, return the original path
    return "../src/parser/mime/mimes.json";
}

/**
 * Helper function to get the path to the default.conf file.
 * This function checks multiple possible paths to find the file.
 */
inline std::string getConfigPath() {
    // Try different relative paths that might work
    const char* paths[] = {
        "resources/configurations/default.conf",  // From build dir
        "../configurations/default.conf",         // From project root
        "../../configurations/default.conf"       // From nested build dir
    };
    
    for (const char* path : paths) {
        FILE* file = fopen(path, "r");
        if (file) {
            fclose(file);
            return path;
        }
    }
    
    // If all else fails, return the original path
    return "../configurations/default.conf";
}

#endif /* TEST_HELPERS_HPP */ 
