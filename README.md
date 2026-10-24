# **nit**
nit is an easy to use command line version controller made using C++ and the zlib library.

## Features
- Can be used easily in the command prompt
- Features an "exclude.nit" file, in which you can type the names of the files/directories that you want to be excluded (1 name per file). This works only if you type the name and not the path.
- Has a log file that saves the date, as well as the save description for each save.

## Commands
- **nit init**
     - Initializes a repository by creating a .nit directory
       
- **nit save**
     - Saves a compressed copy of your directory. It also includes all child directories and requires you to type a save description.
       
- **nit replace x (x = save number)**
     - Replaces the current directory with a specified save.
     - It does not remove files/directories that are specified in the exclude.nit file. However, it can and will change the file(s) if the file(s) was/were not excluded in the save that you are replacing with.
     - Recommended to save before replacing.
       
- **nit preview x (x = save number)**
     - Creates a preview directory inside the .nit directory where the specified save will be uncompressed and made available for preview.
     - Usually recommended before replacing.

- **nit log**
     - Displays the contents of the log file in the command prompt.
     - Shows each save's date, time and save description.

- **nit help**
     - Displays all available commands in the command prompt.

## Installation Guide
So far, the application is Windows only, mostly because it is my main operating system and because i barely have any experience with other operating systems i.e. Linux.
### Installing and Set up
1. Clone the repository or download the zip file
2. Navigate to the subdirectory with the nit executable - "nit.exe"
3. Add the subdirectory as a path to your system's environment variables
   - Now, you should be able to use nit in any directory. You can type "nit help" in your command prompt to test.
