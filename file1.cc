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
#include <string>
#include <iomanip>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <dirent.h>

using namespace std;

//Function Declarations 
struct dirent *readdir(DIR *dirp);
int readdir_r(DIR *dirp, struct dirent *entry, struct dirent **result);

int main(){
  
  //Creating fork process that run same program after the initial instruction
  pid_t pid1, pid2;

  (pid1 = fork())&&(pid2 = fork()); //Creates two children

  if(pid1 < 0 || pid2 < 0){
    cerr << "A Fork call failed, please check child processes!"  << endl;
    cout << "Child 1: " << pid1 << endl;
    cout << "Child 2: " << pid2 << endl;
  }else if(pid1 == 0){
    //Child 1 Processes

  }else if(pid2 == 0){
    //Child 2 Processes 

  }else{
    //Parent Processes
    
  }





  if(pid1 < 0 || pid2 < 0){
    cerr << "A Fork call failed, please check child processes!"  << endl;
    cout << "Child 1: " << pid1 << endl;
    cout << "Child 2: " << pid2 << endl;
  }else if((pid1 && pid2) == 0){
    DIR *d1;
    DIR *d2;
    struct dirent *dir1;
    struct dirent *dir2;
    d1 = opendir("d1");
    d2 = opendir("d2");

    if(d1){
      //Reads through Directory until NULL is returned meaning end of directory
      printf("This  Process is Child 1 of the Fork=%d\n", pid1);
      //Returns the list of all entries within the directory 
      while((dir1 = readdir(d1))!= NULL){
        printf("%s\n", dir1->d_name);
      }
      closedir(d1);
    }
    if(d2){
      //Mirror of the prior command but for the d2 directory.
      printf("This  Process is Child 2 of the Fork=%d\n", pid2);
      //Returns the list of all entries within the directory
      while((dir2 = readdir(d2))!= NULL){
        printf("%s\n", dir2->d_name);
      }
      closedir(d2);
    }
    exit(0);
  }else{
    int test, test2;
    cout << "This Process is the Parent from Fork: " << pid1 << endl;
    waitpid(pid1, &test, 0);
    waitpid(pid2, &test2,0);

    if(test == 0){
      cout << "This process terminated successfully!" << endl; 
    }else if( test == 1){
      cerr << "This process terminated with an error." << endl;
    }

    if(test2 == 0){
      cout << "This process terminated successfully!" << endl; 
    }else if( test2 == 1){
      cerr << "This process terminated with an error." << endl;
    }


  }
  
  

  return 0;
}