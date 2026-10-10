#include <fstream>
#include <iostream>
#include "zlib.h"



int main(){

    std::ofstream compressedFile("test.compressed");

    char buffer[64];
    uLong bufferSize = 64;

    unsigned char info[13] = "Hello World!";
    uLong outputBufferSize = 13;

    unsigned char bufferUS = *buffer;
    int compressedText = compress(&bufferUS, &bufferSize, info, outputBufferSize);
    
    std::ofstream testFile("test.txt");

    testFile << buffer;


    std::cout << buffer;


    return 0;
}