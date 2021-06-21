
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

#define BUFFER 25
#define READ 0

using namespace std;

string hold[] = {"d0", "d1", "d2"};

//Struct to contain each file contents
typedef struct file_info
{
    string name;
    string content[BUFFER];
    int numOfFile;
} file;

int child_proc(int threadnum)
{
    file_info file1;
    int num = 0;
    string content[5];
    DIR *d[threadnum];
    string str;
    struct dirent *dir1;
    int c = 1;

    d[threadnum] = opendir(hold[threadnum].c_str());
    if (d[threadnum])
    {
        // cout << "This  Process is Child " << threadnum << " of the Fork= " << child_pid << endl;

        while ((dir1 = readdir(d[threadnum])) != NULL)
        {
            cout << dir1->d_name << endl
                 << std::flush;
            string tmp = dir1->d_name;
            file1.name = dir1->d_name;
            if(strcmp(dir1->d_name, ".")!=0 && strcmp(dir1->d_name, "..")!=0){
                        //Anything not "." or ".." will be opened and it's string stored into an array
                        //this should hopefully make it a full path - hard coded and will look for alternative later
                        string paths[3] = {"fill","/home/virtus/Desktop/Class/SE-4348/Project1/d1/", "/home/virtus/Desktop/Class/SE-4348/Project1/d2/"};
                        fstream take(paths[threadnum] + tmp);
                        //Checks if file can be opened
                        if(!take){
                            cerr << "File: " << dir1->d_name << " failed to be opened." << endl;
                            exit(1);//return EXIT_FAILURE;
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
        }
    }
}
                
int main() {
    // create fiorst child
    pid_t process_1 = fork();
    pid_t process_2 = NULL;
    // only parent calls this
    if (process_1 > 0)
    {
        // now second child
        process_2 = fork();
    }
    // start child specifications
    // cout << "process1 is " << process_1 << " process 2 is " << process_2 << endl;
    if (process_1 == 0 && process_2 == 0) {
        // in process 1
        cout << "process 1 " << getpid() << endl;
        child_proc(1);
    } else if (process_1 > 0 && process_2 == 0) {
        // process 2
        cout << "process 2 " << getpid() << endl;
        child_proc(2);
    } else {
        // in parent
        cout << "PARENT process " << getpid() << endl;
    }
    return 0;
}