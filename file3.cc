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
//#include <stdlib.h>

//Some Macros for the Piping processes
#define BUFFER_NAME 25
#define BUFFER 100
#define READ_END 0
#define WRITE_END 1

using namespace std;

//Struct to contain each file contents
typedef struct file_info
{
    string name;
    string content;
    int numOfBytes;
} file;

int child(int i, int fd[2][2])
{
    file_info file1;
    fstream outfile;
    // child_proc starts here
    int c = 1;
    //char arrray that will read directory entries content
    string content;
    DIR *d[i];
    struct dirent *dir1;
    string hold[] = {"d0", "d1", "d2"};
    d[i] = opendir(hold[i].c_str());
    if (d[i])
    {
        //Reads through Directory until NULL is returned meaning end of directoryf
        //cout << "This  Process is Child " << i << " of the Fork= " << child_pid << endl;
        string out[3] = {"fill", "output1.txt", "output2.txt"};
        outfile.open(out[i]);

        //Returns the list of all entries within the directory
        while ((dir1 = readdir(d[i])) != NULL)
        {
            //cout << dir1->d_name << endl << std::flush;
            string tmp = dir1->d_name;

            if (strcmp(dir1->d_name, ".") != 0 && strcmp(dir1->d_name, "..") != 0)
            {
                file1.name = dir1->d_name;
                outfile << file1.name << endl;
                //Anything not "." or ".." will be opened and it's string stored into an array
                //this should hopefully make it a full path - hard coded and will look for alternative later
                string paths[3] = {"fill", "/home/virtus/Desktop/Class/SE-4348/Project1/d1/", "/home/virtus/Desktop/Class/SE-4348/Project1/d2/"};
                fstream take(paths[i] + tmp);
                //Checks if file can be opened
                if (!take)
                {
                    cerr << "File: " << dir1->d_name << " failed to be opened." << endl;
                    return EXIT_FAILURE;
                }
                //Reads individual files contents into arrays
                string str;
                int byte = 0;
                //Size of directory
                while (getline(take, str))
                {
                    content += str;
                    content += "\n";
                    file1.content = content;
                    //Transfering file content to struct
                }
                outfile << file1.content;

                //Converts content length to bytes
                byte = (file1.content).length();
                file1.numOfBytes = byte;
                outfile << "Num of Bytes: " << file1.numOfBytes << endl;
                //write( fd[i-1][WRITE_END], );
                content = "";
                string stringbyte = to_string(file1.numOfBytes);

                //The writing processes
                write(fd[i - 1][WRITE_END], (file1.name).c_str(), BUFFER_NAME);
                write(fd[i - 1][WRITE_END], stringbyte.c_str(), BUFFER_NAME);
                
                //Checks if there is any contents in file before writing
                if (file1.numOfBytes > 0)
                {
                    write(fd[i - 1][WRITE_END], (file1.content).c_str(), BUFFER);
                }
                else
                {
                    cerr << "This file is empty!" << endl;
                }

                take.close();
            }

            //Iteration for array
            c++;
        } //end of while(readdir)

        //Reading Portion of the
        close(fd[i - 1][WRITE_END]);
        file_info file2;
        char array[BUFFER];
        while (read(fd[1 - (i - 1)][READ_END], array, BUFFER_NAME))
        {
            file2.name = array;
            string paths[3] = {"fill", "/home/virtus/Desktop/Class/SE-4348/Project1/d1/", "/home/virtus/Desktop/Class/SE-4348/Project1/d2/"};
            paths[i] = paths[i] + file2.name;
            ofstream newfile(paths[i]);
            //Reads the amount of Bytes in the file
            read(fd[1 - (i - 1)][READ_END], array, BUFFER_NAME);
            file2.numOfBytes = strtol(array, NULL, 10);

            //Checks if there are any contents in the file
            if (file2.numOfBytes > 0)
            {
                read(fd[1 - (i - 1)][READ_END], array, BUFFER);
                file2.content = array;
                newfile << file2.content;
            }
            else
            {
                cerr << "This file is empty!" << endl;
            }
        }
        //Close the read end of the pipe
        close(fd[1-(i-1)][READ_END]);
        outfile.close();
        closedir(d[i]);
    }
    return 0;
}

int main()
{
    pid_t child_pid, wpid;
    int status = 0;
    int fd[2][2];
    //Creating Pipes
    for (int p1 = 0; p1 < 2; p1++)
    {
        if ((status = pipe(fd[p1])) == -1)
        {
            cerr << "Pipe: " << fd[p1] << endl;
            exit(1);
        }
    }

    for (int i = 1; i <= 2; i++)
    {
        //Forks child process and only runs is Forked Successfully
        if ((child_pid = fork()) == 0)
        {
            close(fd[i - 1][READ_END]);
            close(fd[1 - (i - 1)][WRITE_END]);
            child(i, fd);
            _exit(0);
        }
    }

    //Closing Pipe Processes
    for (int p1 = 0; p1 < 2; p1++)
    {
        close(fd[p1][READ_END]);
        close(fd[p1][WRITE_END]);
    }

    while ((wpid = wait(&status)) > 0);
    exit(0);
}