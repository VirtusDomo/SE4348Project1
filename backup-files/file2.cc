
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

#define BUFFER 25
#define READ 0
#define WRITE 1

using namespace std;

string hold[] = {"d0", "d1", "d2"};
//Pipe Array
int p1[2];
int p2[2];
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
    fstream outfile;
    int num = 0;
    string content[5];
    DIR *d[threadnum];
    string str;
    struct dirent *dir1;
    int c = 1;
    cout << "GETS HERE 3" << getppid() << endl;
    if (threadnum == 1 && pipe(p1) < 0)
    {
        exit(1);
    }
    if (threadnum == 2 && pipe(p2) < 0)
    {
        exit(1);
    }

    d[threadnum] = opendir(hold[threadnum].c_str());
    if (d[threadnum])
    {
        string out[3] = {"fill", "output1.txt", "output2.txt"};
        outfile.open(out[threadnum]);
        //outfile << "This  Process is Child " << threadnum << " of the Fork= " << child_pid << endl;
        cout << "GETS HERE0" << getppid() << endl;
        while ((dir1 = readdir(d[threadnum])) != NULL)
        {
            //cout << dir1->d_name << endl << std::flush;
            //Sends output to a output file
            outfile << dir1->d_name << endl;
            
            string tmp = dir1->d_name;
            file1.name = dir1->d_name;
            if (strcmp(dir1->d_name, ".") != 0 && strcmp(dir1->d_name, "..") != 0)
            {
                //Anything not "." or ".." will be opened and it's string stored into an array
                //this should hopefully make it a full path - hard coded and will look for alternative later
                string paths[3] = {"fill", "/home/virtus/Desktop/Class/SE-4348/Project1/d1/", "/home/virtus/Desktop/Class/SE-4348/Project1/d2/"};
                fstream take(paths[threadnum] + tmp);
                //Checks if file can be opened
                if (!take)
                {
                    cerr << "File: " << dir1->d_name << " failed to be opened." << endl;
                    return EXIT_FAILURE;
                }
                

                //Reads individual files contents into arrays
                string str;
                string temp = "";
                //Size of directory
                while (getline(take, str))
                {
                    content[c - 1] = str;
                    //cout << content[c-1] << endl;
                    //Transfering file content to struct
                    file1.content[c - 1] = content[c - 1];
                    //cout << file1.content[c-1] << endl;
                    temp += str;
                }
                
                //cout << "GETS HERE " << threadnum << " " << getppid() << endl;

                //Sending
                if (threadnum == 1)
                {
                    char temp_char[temp.length() + 1];
                    strcpy(temp_char, temp.c_str());
                    //close(p1[READ]);
                    //write(p1[WRITE], temp_char, temp.length() + 1);
                    //close(p1[WRITE]);
                    //Reading from P2 starts
                    //read(p2[READ], temp_char, temp.length() + 1);
                    //close(p2[READ]);
                }
                else if (threadnum == 2)
                {
                    char temp_char[temp.length() + 1];
                    strcpy(temp_char, temp.c_str());
                    //close(p2[READ]);
                    //write(p2[WRITE], temp_char, temp.length() + 1);
                    //close(p2[WRITE]);
                    //Reading from P2 starts
                    //read(p1[READ], temp_char, temp.length() + 1);
                    //close(p1[READ]);
                }

                
                file1.numOfFile = num;
                take.close();
                outfile << "ERROR TEST HELLO HELLO" << endl;
            }

            //Iteration for array
            c++;
        }
        outfile.close();
    }
    outfile << "EXITS" << getppid << endl;
}


int main()
{
    //Creating fork process that run same program after the initial instruction
    pid_t pid1, pid2;
    int status = 0;
    (pid1 = fork()) && (pid2 = fork()); //Creates two children

    if (pid1 == 0)
    {
        /* Child A code */
        cout << "A" << endl;
        child_proc(1);
        cout << "A unbLOCKS" << endl;
        _exit(0);
    }
    else if (pid2 == 0)
    {
        /* Child B code */
        cout << "B" << endl;
        child_proc(2);
        cout << "B unbLOCKS" << endl;

        _exit(0);
    }
    else
    {
        /* Parent Code */
        cout << "PARENT " << getppid() << " " << pid1 << " " << pid2 << endl;
        waitpid(pid1, &status, 0);
        sleep(2);
        waitpid(pid2, &status, 0);
        
        cout << "C unbLOCKS" << endl;
        // not waiting for A or B to exit(), waitppid is not working
    }

    cout << "ONCE " << getppid() << endl;
}