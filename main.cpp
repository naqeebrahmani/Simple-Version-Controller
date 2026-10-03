#include <cstdlib>
#include <iostream>
#include <filesystem>
#include <fstream>

std::string commands[] = {"init", "add", "help"};


void init(){

    if(system("mkdir nit") == 0){
        system("touch info");

    }

}



















int main(int totalArgs, char* args[]){

    std::string *stringArgs = NULL;
    stringArgs = new std::string[totalArgs];

    for(int i = 0; i < totalArgs; i++){
        stringArgs[i] = args[i];
    }


    

    



    return 0;
}

