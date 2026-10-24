#include <iostream>
#include <fstream>
#include <string>

template<typename T>
void CopyArray(T *srcarray, T *destarray, int arraysize){

    for(int i = 0; i < arraysize; i++){

        destarray[i] = srcarray[i];

    }


}




int main(){

    std::string* excludedpaths = new std::string[0];
    unsigned int excludedpathnums = 0;
    std::string tempexcludedpath{};

    std::ifstream excludenit("exclude.nit");

    if(excludenit.is_open() == true){
        while(std::getline(excludenit, tempexcludedpath)){

            std::string* tempexcludedpaths = new std::string[excludedpathnums];

            CopyArray(excludedpaths, tempexcludedpaths, excludedpathnums);

            delete[] excludedpaths;


            excludedpathnums ++;
            excludedpaths = new std::string[excludedpathnums];

            CopyArray(tempexcludedpaths, excludedpaths, excludedpathnums-1);

            delete[] tempexcludedpaths;

            excludedpaths[excludedpathnums-1] = tempexcludedpath;
            


        }

        for(int i = 0; i < excludedpathnums; i++){

            std::cout << excludedpaths[i] << "\n";

        }

        
        delete[] excludedpaths;

    }
    



    return 0;
}