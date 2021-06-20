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
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <dirent.h>
#include <fstream>
#include <stdlib.h>


using namespace std;







int main(){

    pid_t child_pid, wpid; 
    int status = 0;

    for(int i = 1; i <= 2; i++){
        if((child_pid = fork()) == 0){
            int c = 1;
        //char arrray that will read directory entries content
        string temp1[5];
        DIR *d[i];
        struct dirent *dir1;
        string hold[] = {"d0","d1", "d2"};
        d[i] = opendir(hold[i].c_str());
        if(d[i]){
        //Reads through Directory until NULL is returned meaning end of directory
        cout << "This  Process is Child " << i << " of the Fork= " << child_pid << endl;
        //Returns the list of all entries within the directory 
        while((dir1 = readdir(d[i]))!= NULL){
            //printf("%s\n", dir1->d_name);
            cout << dir1->d_name << endl << std::flush;
            string tmp = dir1->d_name;
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
            while(getline(take, str)){
                temp1[c-1] = str;
                cout << temp1[c-1] << endl;
            }
            take.close();
            //ERROR OF NOTICE: 3RD FILE in D1 isnt't being properly read, and no particular order is ever maintained in execution. Look for ways to solve. 
            }
            //Iteration for array
            c++;
        }
        closedir(d[i]);
        }

        exit(0);
        }
    }

    while((wpid = wait(&status)) > 0);


}