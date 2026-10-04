#include <chrono>
#include <cstdlib>
#include <time.h>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>

extern "C"{
    #include "external-libraries/zlib-1.3.2/zlib.h"
}


std::string commands[] = {"init", "add", "help"};


void init(){

    std::filesystem::create_directory("nit");

    if(!std::filesystem::exists("nit/info")){
        std::ofstream infoFile("nit/info");

        //time_t timeNow = time(nullptr); //time in seconds since 1st jan 1970
        //i didnt use ctime because autocorrect was showing it as deprecated
        // After some research,I found out that chrono is the newer (c++ 20+ if i remember correct) thing for time related stuff

        auto clock = std::chrono::system_clock::now();

        infoFile << "\nRepository Initialized on " << std::chrono::system_clock::now();

        infoFile.close();
    }

    

}





void help(){
    std::cout << "Commands:\n";
    for(int i = 0; i < (sizeof(commands)/sizeof(std::string)); i++){
        std::cout << commands[i] + "\n";
    }
}





int main(int totalArgs, char* args[]){


    std::string *stringArgs = NULL;
    stringArgs = new std::string[totalArgs];

    for(int i = 0; i < totalArgs; i++){
        stringArgs[i] = args[i];
    }


    if(totalArgs > 1){

        if(stringArgs[1] == "init"){

            init();

        }







        else if(stringArgs[1] == "help"){

            help();

        }


    }

    



    return 0;
}

