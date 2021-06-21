/*
  Filename:   file2.cc
  Date:       06/18/2021
  Author:     James Anyabine
  Email:      joa170000@utdallas.edu
  Version:    1.0
  Copyright:  2021, All Rights Reserved

  Description:
    This is the main source file of the code. 

*/

#include <iostream> 
#include <string.h>
#include <iomanip>
#include <stdio.h>
#include <cstdio>
#include <string>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <dirent.h>
#include <fstream>
#include <stdlib.h>

//Some Macros for the Piping processes 
#define BUFFER 25
#define READ 0
#define WRITE 1

using namespace std;

//Struct to contain each file contents
typedef struct file_info{
    string name;
    string content[BUFFER];
    int numOfFile;
}file;


int main(){

    pid_t child_pid, wpid; 
    int status = 0;
    int fd[2];
    file_info file1;
    //Read and write char arrays for pipe STUCK HERE
    char write_msg[BUFFER];
    char read_msg[BUFFER]; 

    
    //Creating the Pipe
    if(pipe(fd) == -1){
        cerr << "Pipe failed! Sorry." << endl;
        return 1;
    }
    

    for(int i = 1; i <= 2; i++){
        //Forks child process and only runs is Forked Successfully
        if((child_pid = fork()) == 0){
            // child_proc starts here
            int c = 1;
            //Amount of Files in Directory
            int num = 0;
            //char arrray that will read directory entries content
            string content[5];
            DIR *d[i];
            struct dirent *dir1;
            string hold[] = {"d0","d1", "d2"};
            d[i] = opendir(hold[i].c_str());
            if(d[i]){
                //Reads through Directory until NULL is returned meaning end of directoryf
                cout << "This  Process is Child " << i << " of the Fork= " << child_pid << endl;
                //Returns the list of all entries within the directory 
                while((dir1 = readdir(d[i]))!= NULL){
                    cout << dir1->d_name << endl << std::flush;
                    string tmp = dir1->d_name;
                    file1.name = dir1->d_name;
                    if(strcmp(dir1->d_name, ".")!=0 && strcmp(dir1->d_name, "..")!=0){
                        //Anything not "." or ".." will be opened and it's string stored into an array
                        //this should hopefully make it a full path - hard coded and will look for alternative later
                        string paths[3] = {"fill","/home/virtus/Desktop/Class/SE-4348/Project1/d1/", "/home/virtus/Desktop/Class/SE-4348/Project1/d2/"};
                        fstream take(paths[i] + tmp);
                        //Checks if file can be opened
                        if(!take){
                            cerr << "File: " << dir1->d_name << " failed to be opened." << endl;
                            return EXIT_FAILURE;
                        }
                        //Reads individual files contents into arrays
                        string str;
                        //Size of directory
                        while(getline(take, str)){
                            content[c-1] = str;
                            cout << content[c-1] << endl;
                            //Transfering file content to struct
                            file1.content[c-1] = content[c-1]; 
                        }
                        file1.numOfFile = num;
                        take.close();
                    }
                    
                    //Iteration for array
                    c++;
                    num++;
                } 
                cout << "test: " << file1.numOfFile << endl;
                closedir(d[i]);
            }
            exit(0);
        }
    }
    while((wpid = wait(&status)) > 0);
    cout << "test" << endl;
    exit(0); 
}