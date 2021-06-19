/*
  Filename:   file1.cc
  Date:       06/16/2021
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
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <dirent.h>
#include <fstream>


using namespace std;

//Function Declarations 
struct dirent *readdir(DIR *dirp);
int readdir_r(DIR *dirp, struct dirent *entry, struct dirent **result);

int main(){
  
  //Creating fork process that run same program after the initial instruction
  pid_t pid1, pid2;

  (pid1 = fork())&&(pid2 = fork()); //Creates two children
  /*
  if(pid1 == 0){
    cerr << "A Fork call failed, please check child processes!"  << endl;
    cout << "Child 1: " << pid1 << endl;
    cout << "Child 2: " << pid2 << endl;
  }else*/ 

  if(pid1 == 0){
    //Child 1 Processes
    ifstream take;
    int c = 0;
    //char arrray that will read directory entries content
    char temp1[5][50];
    DIR *d1;
    struct dirent *dir1;
    d1 = opendir("d1");
    if(d1){
      //Reads through Directory until NULL is returned meaning end of directory
      printf("This  Process is Child 1 of the Fork=%d\n", pid1);
      //Returns the list of all entries within the directory 
      while((dir1 = readdir(d1))!= NULL){
        //printf("%s\n", dir1->d_name);
        //cout << dir1->d_name << endl;
        if(strcmp(dir1->d_name, ".")!=0 && strcmp(dir1->d_name, "..")!=0){
          take.open(dir1->d_name);
          take >> temp1[c][50];
          cout << temp1[c][50];
        }
        c++;
      }
      closedir(d1);
    }
    exit(0);
  }else if(pid2 == 0){
    //Child 2 Processes

    ifstream take2;

    //char array that will read directory entries content
    char temp2[5][50];
    int d = 0;
    DIR *d2;
    struct dirent *dir2;
    d2 = opendir("d2");
    if(d2){
      //Mirror of the prior command but for the d2 directory.
      printf("This  Process is Child 2 of the Fork=%d\n", pid2);
      //Returns the list of all entries within the directory
      while((dir2 = readdir(d2))!= NULL){
        //printf("%s\n", dir2->d_name);
        //cout << dir2->d_name << endl;
        if(strcmp(dir2->d_name, ".")!=0 && strcmp(dir2->d_name, "..")!=0){
          take2.open(dir2->d_name);
          take2 >> temp2[d][50];
          cout << temp2[d][50];
        }
        d++;
      }
      closedir(d2);
    } 
    exit(0);
  }else{
    //Parent Processes
     int test, test2;
    cout << "This Process is the Parent from Fork: " << pid1 << endl;
    waitpid(pid1, &test, 0);
    if(test == 0){
      cout << "This process terminated successfully!" << endl; 
    }else if( test == 1){
      cerr << "This process terminated with an error." << endl;
    }

    waitpid(pid2, &test2,0);
    if(test2 == 0){
      cout << "This process terminated successfully!" << endl; 
    }else if( test2 == 1){
      cerr << "This process terminated with an error." << endl;
    }

  }
  return 0;
}