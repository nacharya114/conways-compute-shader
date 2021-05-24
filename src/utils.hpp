#ifndef UTILS_HPP
#define UTILS_HPP

//  utility functions for IO and that model compute shader functions
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>

std::string pathToStr(const char* filePath){
    std::string codeOutput;
    std::ifstream inputStream;

    inputStream.exceptions (std::ifstream::failbit | std::ifstream::badbit);

    try 
    {
        // open files
        inputStream.open(filePath);
        std::stringstream ss;
        // read file's buffer contents into streams
        ss << inputStream.rdbuf();		
        // close file handlers
        inputStream.close();
        // convert stream into string
        codeOutput   = ss.str();	
    }
    catch(std::ifstream::failure e)
    {
        std::cout << "ERROR::IO::FILE_NOT_SUCCESFULLY_READ:" << filePath  << std::endl;
    }
    return codeOutput;
}


#endif