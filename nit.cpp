#include <cstdlib>
#include <iostream>
#include <filesystem>
#include <fstream>

std::string commands[] = {"init", "add", "help"};


void init(){

    if(system("mkdir nit") == 0){
        system("touch nit/info");

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

