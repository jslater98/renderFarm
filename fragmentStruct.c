#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Job
{
    char file[50];
    char path[128];
    char format[8];
    char engine[24];
    int startFrame;
    int endFrame;
};

const char* render(char file[50], char path[128], char format[8], char engine[24], int start, int end) {

    char command[500] = "blender -b "; //command to be run
    char s[8];  //start frame val
    char e[8];  //end frame val
    strcat(command, file);  //add file to command
    strcat(command, " -o ");  //add -o <path> to command
    strcat(command, path);

    //if format value not empty, add -F <format> -x 1  (-x 1 adds file extension to end of file)
    if (strcmp(format, "") != 0) {
        strcat(command, " -F ");
        strcat(command, format);
        strcat(command, " -x 1");
    }

    //if engine value not empty, add -E <engine> to command (if left empty, will default to engine specified in .blend file)
    if (strcmp(engine, "") != 0) {
        strcat(command, " -E ");
        strcat(command, engine);
    }

    //
    if (start != 0) {
        sprintf(s, "%d", start);  //convert start val to string
        strcat(command, " -s ");
        strcat(command, s);
    }
    else if (end != 0) {  //user error handling: if start = 0 and end != 0, assume user wants to start from first frame, define start frame as 1 (-s 1)
        strcat(command, " -s 1");
    }
    if (end != 0) {  //if end val not 0, define end frame (-e <frame>)
        sprintf(e, "%d", end);  //converts end val to string
        strcat(command, " -e ");
        strcat(command, e);
    }
    strcat(command, " -a");  // -a renders specified frames

    char *runCmd = command;

    //printf("%s\n", command);  //for testing purposes

    //system(command); //run command on lab machine
    
    return runCmd;
}

int* fragment(int startFrame, int endFrame, int processors, int arraysize, int partitionArray[]) //C can't return an array by it's self and needed a pointer and processors are the ammount of computers we have
{
    int i;
    int partition;
    int remainder;

    
    partition = (endFrame - startFrame)/processors;  //number of frames per machine
    remainder = (endFrame - startFrame)%processors;
    partitionArray[0] = startFrame;  //set first value to initial start frame
    for(i = 1; i < arraysize; i += 2)
    {
         partitionArray[i] = partitionArray[i - 1] + partition;  //set end val to previous val + partition
         if(i != 1)
         {
             partitionArray[i] = partitionArray[i - 1] + partition - 1;  //
         }
         partitionArray[i+1] = partitionArray[i] + 1; //set next start val to previous (end) val + 1
    }
    
    partitionArray[arraysize - 1] += remainder; //adds the remainder frames to the remainder
    return partitionArray;
}

int main() 
{
    char commands[4][500];  //2D array of commands to be sent to render nodes
    int partitionArray[8]; //C doesn't allow non constants as array sizes
    struct Job jobArray[4]; 
    int i;
    int j;
    char file[] = "simple.blend";
    char path[] = "/home/justin.slater/tests/frame_";
    char format[] = "PNG";
    char engine[] = "CYCLES";

    fragment(1, 121, 4, 8, partitionArray);  //divide total frames evenly and store to partition array
    for(i = 0, j = 0; i < 4; ++i, j+=2)
    {
        //store values in job structs 
        strcpy(jobArray[i].path, path);
        strcpy(jobArray[i].format, format);
        strcpy(jobArray[i].engine, engine);
        jobArray[i].startFrame = partitionArray[j];
        jobArray[i].endFrame = partitionArray[j + 1];
    }

    
    for(i = 0; i < 4; ++i)
    {
        const char* cmd = render(file, path, format, engine, jobArray[i].startFrame, jobArray[i].endFrame);
        strcpy(commands[i], cmd);
        printf("%s\n", commands[i]);
    }

    for(i = 0; i < 8; ++i)
    {
        printf("%d\n",partitionArray[i]);
    }
}
