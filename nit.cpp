#include "zconf.h"
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>

extern "C"{
    #include <zlib.h>
}


std::string commands[] = {"init", "save", "replace x (x is the save number)", "preview x (x is the save number)", "log", "help"};



void Init(){

    std::filesystem::create_directory(".nit");
    std::filesystem::create_directory(".nit/saves");


    if(!std::filesystem::exists(".nit/log")){
        std::ofstream infoFile(".nit/log");

        //time_t timeNow = time(nullptr); //time in seconds since 1st jan 1970
        //i didnt use ctime because it was showing up as deprecated
        // After some research,I found out that chrono is the newer (c++ 20+ if i remember correct) thing for time related stuff


        std::chrono::zoned_time timeZone{std::chrono::current_zone(), std::chrono::system_clock::now()};
        std::string timeString = std::format("{:%F %X %Z}", timeZone);

        infoFile << "0\nRepository Initialized on " + timeString + "\n";

        infoFile.close();

        std::cout << "Initialized a repository.\n";

    }
    else{
        std::cout << "A repository has already been initialized.\n";
    }


}


template<typename T>
void CopyArray(T *srcarray, T *destarray, int arraysize){

    for(int i = 0; i < arraysize; i++){

        destarray[i] = srcarray[i];

    }


}


void CompressAndCopyDirectory(std::string srcPath, std::string destPath){

    //std::cout << "starting iterating\n"; //for debugging

    for(std::filesystem::directory_entry const &dirEntry : std::filesystem::directory_iterator(srcPath)){



        if(dirEntry.is_directory() && dirEntry.path().filename().string() != ".nit"){ 
            std::filesystem::create_directory(destPath+"/"+dirEntry.path().filename().string());
            CompressAndCopyDirectory(srcPath + "/" + dirEntry.path().filename().string(), destPath + "/" + dirEntry.path().filename().string());
        }
        else if(dirEntry.path().filename().string() != ".nit"){

            std::ifstream srcFile(dirEntry.path(), std::ifstream::binary);

            int srcFileBytes = std::filesystem::file_size(dirEntry.path());
            unsigned char* srcFileBuffer = new unsigned char[srcFileBytes];

            srcFile.read(reinterpret_cast<char*>(srcFileBuffer), srcFileBytes);
            srcFile.close();


            uLong destFileBytes = compressBound(srcFileBytes);
            unsigned char* destFileBuffer = new unsigned char[destFileBytes];
            
            if(compress(destFileBuffer, &destFileBytes, srcFileBuffer, srcFileBytes) < 0){
                std::cout << "Error when trying to compress your files.\n";
                exit(0);
            }

            //compress(destFileBuffer, &destFileBytes, srcFileBuffer, srcFileBytes);

            std::ofstream destFile(destPath + "/" + dirEntry.path().filename().string(), std::ofstream::binary);
            destFile.write(reinterpret_cast<char*>(destFileBuffer), destFileBytes);


            std::cout << "Compressed and saved: " << dirEntry.path().filename().string() << "\n";

            delete[] srcFileBuffer;
            delete[] destFileBuffer;


        }


    }

    //std::cout << "iterated!\n"; //for debugging

}


void UnCompressAndCopyDirectory(std::string srcPath, std::string destPath){

    for(std::filesystem::directory_entry const &dirEntry : std::filesystem::directory_iterator(srcPath)){


        if(dirEntry.is_directory() && dirEntry.path().filename().string() != ".nit"){ 
            std::filesystem::create_directory(destPath + "/" + dirEntry.path().filename().string());
            UnCompressAndCopyDirectory(srcPath + "/" + dirEntry.path().filename().string(), destPath + "/" + dirEntry.path().filename().string());
        }
        else{

            std::ifstream srcFile(dirEntry.path(), std::ifstream::binary);

            int srcFileBytes = std::filesystem::file_size(dirEntry.path());
            unsigned char* srcFileBuffer = new unsigned char[srcFileBytes];

            srcFile.read(reinterpret_cast<char*>(srcFileBuffer), srcFileBytes);
            srcFile.close();


            uLong destFileBytes = srcFileBytes * 1.5;
            unsigned char* destFileBuffer = new unsigned char[destFileBytes];
            
            if(uncompress(destFileBuffer, &destFileBytes, srcFileBuffer, srcFileBytes) < 0){

                if(uncompress(destFileBuffer, &destFileBytes, srcFileBuffer, srcFileBytes) == Z_MEM_ERROR){
                    std::cout << "Not enough memory for uncompressing " << dirEntry.path().filename().string() << "\n";
                    continue;
                }
                else if(uncompress(destFileBuffer, &destFileBytes, srcFileBuffer, srcFileBytes) == Z_DATA_ERROR){
                    std::cout << "Could not uncompress " << dirEntry.path().filename().string() << "\n"; 
                    continue;
                }
                else if(uncompress(destFileBuffer, &destFileBytes, srcFileBuffer, srcFileBytes) == Z_BUF_ERROR){

                    while(uncompress(destFileBuffer, &destFileBytes, srcFileBuffer, srcFileBytes) == Z_BUF_ERROR){
                    
                        delete[] destFileBuffer;

                        destFileBytes *= 2;
                        destFileBuffer = new unsigned char[destFileBytes];

                    }

                }


            }

            

            std::ofstream destFile(destPath + "/" + dirEntry.path().filename().string(), std::ofstream::binary);
            destFile.write(reinterpret_cast<char*>(destFileBuffer), destFileBytes);


            std::cout << "Uncompressed: " << dirEntry.path().filename().string() << "\n";


            delete[] srcFileBuffer;
            delete[] destFileBuffer;


        }


    }

    //std::cout << "iterated!\n"; //for debugging

}




void Save(){
    if(std::filesystem::exists(".nit/log")){
        std::cout << "description (for whole directory): ";
        std::string commitMessage;

        std::getline(std::cin >> std:: ws, commitMessage);
        //std::cin >> commitMessage;

        std::fstream infoFile(".nit/log");

        std::string totalCommitsString;
        std::getline(infoFile, totalCommitsString);

        //converting the string with totalcommitnumbers to an integer
        int totalCommits = -1;

        try{
            totalCommits = std::stoi(totalCommitsString);
        }
        catch(std::exception exception){
            std::cout << "Something seems to be wrong with the log file. Please try deleting the \".nit\" directory and Initializing again.";
            exit(0);
        }
        
        //creating a new directory for the latest save
        std::string saveDirectoryPath = ".nit/saves/" + std::to_string(totalCommits + 1);
        std::filesystem::create_directory(saveDirectoryPath);
        ///////////////////////////////////////////////////////////////////////////////////

        //copying the directory

        CompressAndCopyDirectory(".", ".nit/saves/"+std::to_string(totalCommits+1));

        /////////////////////////


        std::chrono::zoned_time timeZone{std::chrono::current_zone(), std::chrono::system_clock::now()};
        std::string commitTime =std::format("{:%F %X %Z}", timeZone) + "\n";

        std::string commitMessageWithTimeAndCommitNumber = "(" + std::to_string(totalCommits + 1) + ")" + commitTime + commitMessage + "\n";

        //adding the newest add/commit to the log file

        std::ofstream CreatedTempInfoFile(".nit/logTemp");
        CreatedTempInfoFile.close();

        std::fstream infoFileTemp(".nit/logTemp");
        infoFileTemp << totalCommits + 1 << "\n";
        infoFileTemp << infoFile.rdbuf() << commitMessageWithTimeAndCommitNumber;

        infoFile.close();
        std::remove(".nit/log");
        infoFileTemp.close();
        std::rename(".nit/logTemp", ".nit/log");


        ////////////////////////////////////////////////////////////////////

        std::cout << "Save status: Successful" <<std::endl;

    }
    else{

        std::cout << "You need to Initialize before adding and commiting.";

    }
    
}

void Replace(std::string saveNumber){

    std::string srcPath = ".nit/saves/"+saveNumber;

    if(std::filesystem::exists(srcPath)){

        std::cout << "Replacing the current directory implies deleting everything and replacing afterwards. Please make sure that you have saved first.\n";
        std::cout << "Are you sure Y/n? ";
        std::string userAns;
        std::cin >> userAns;

        bool userSure = (userAns=="Y");

        if(userSure){

            //deleting everything from current directory//
            for(std::filesystem::directory_entry const &dirEntry : std::filesystem::directory_iterator(".")){

                if(dirEntry.path().filename().string() != ".nit"){
                    std::filesystem::remove_all(dirEntry.path());
                }

            }

            ///////////////////////////////////////////////

            UnCompressAndCopyDirectory(srcPath, ".");



        }
        else{
            exit(0);
        }


    }
    else{

        std::cout << "Could not find a save with this number.";

    }


}



void Preview(std::string saveNumber){

    if(std::filesystem::exists(".nit")){


        std::string srcPath = ".nit/saves/"+saveNumber;
        if(std::filesystem::exists(srcPath)){

            if(std::filesystem::exists(".nit/Preview")){
                std::filesystem::remove_all(".nit/Preview");
            }

            std::filesystem::create_directory(".nit/Preview");
            std::string PreviewDirPath = ".nit/Preview/"+saveNumber;
            std::filesystem::create_directory(PreviewDirPath);


            UnCompressAndCopyDirectory(srcPath, PreviewDirPath);

            std::cout << "The preview directory is ready.\n";
            std::cout << "Note: The preview directory can be deleted by you anytime, and using the preview command automatically clears the preview directory.\n";

        }
        else{

            std::cout << "Unable to find a save with that number.";

        }

    }
    else{
        std::cout << "You need to initialize and save at least once before previewing.";
    }

}




void ShowLog(){
    if(std::filesystem::exists(".nit/log")){

        std::fstream logFile(".nit/log");

        std::cout << std::endl;

        std::string row = "";
        do{
            if(row.size() == 0){
                std::cout << "Total Saves: ";
            }
            std::getline(logFile, row);
            std::cout << row;
            std::cout << std::endl;
        }while(row.size() > 0);


    }
    else{

        std::cout << "The log file doesn't seem to be where it's supposed to. Please try initializing first.";

    }
    
}



void Help(){
    std::cout << "Commands:\n";
    for(int i = 0; i < (sizeof(commands)/sizeof(std::string)); i++){
        std::cout << commands[i] + "\n";
    }
}





int main(int totalArgs, char* args[]){


    std::string *stringArgs =new std::string[totalArgs];

    for(int i = 0; i < totalArgs; i++){
        stringArgs[i] = args[i];
    }


    if(totalArgs > 1){

        if(stringArgs[1] == "init"){

            Init();

        }

        else if(stringArgs[1] == "save"){

            Save();

        }

        else if(stringArgs[1] == "replace"){
            
            if(totalArgs < 3){
                std::cout << "few arguments. You also need to specify which save you want to Preview by typing \"replace x\" (with x being the save number).";
                return 0;
            }
            
            Replace(stringArgs[2]);
            
            
        }

        else if(stringArgs[1] == "preview"){

            if(totalArgs < 3){
                std::cout << "few arguments. You also need to specify which save you want to Preview by typing \"preview x\" (with x being the save number).";
                return 0;
            }
            
            Preview(stringArgs[2]);

        }

        else if(stringArgs[1] == "log"){

            ShowLog();

        }

        else if(stringArgs[1] == "help"){

            Help();

        }

        else{
            std::cout << "Invalid Command. Type \"nit help\" to see all available commands.";
        }


    }
    else{
        std::cout << "Not enough argumnets. Type \"nit help\" to see all available commands.";
    }

    

    delete[] stringArgs;

    return 0;
}

