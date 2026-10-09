#include <chrono>
#include <cstdio>
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

    if(!std::filesystem::exists("nit/log")){
        std::ofstream infoFile("nit/log");

        //time_t timeNow = time(nullptr); //time in seconds since 1st jan 1970
        //i didnt use ctime because autocorrect was showing it as deprecated
        // After some research,I found out that chrono is the newer (c++ 20+ if i remember correct) thing for time related stuff


        std::chrono::zoned_time timeZone{"CET", std::chrono::system_clock::now()};
        std::string timeString = std::format("{:%Y-%m-%d %H:%M}", timeZone);

        infoFile << "Repository Initialized on " << timeString;

        infoFile.close();
    }


}


void addAndCommit(){
    if(std::filesystem::exists("nit/log")){
        std::cout << "description (for whole directory): ";
        std::string commitMessage;
        std::cin >> commitMessage;


        for (const auto &file: std::filesystem::directory_iterator(".")) {


            return;


        }




        std::chrono::zoned_time timeZone{"CET", std::chrono::system_clock::now()};
        std::string commitTime = std::format("{:%y-%m-%d %H:%M}", timeZone) + "\n";

        std::string commitMessageWithTime = commitTime + commitMessage;
        
        

    }
    else{

        std::cout << "You need to initialize before adding and commiting.";

    }
    
}


void log(){
    return;
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

        else if(stringArgs[1] == "add"){

            addAndCommit();

        }





        else if(stringArgs[1] == "help"){

            help();

        }

        else{
            std::cout << "Invalid Command. Type \"nit help\" to see all available commands.";
        }


    }
    else{
        std::cout << "Not enough argumnets. Type \"nit help\" to see all available commands.";
    }

    



    return 0;
}

