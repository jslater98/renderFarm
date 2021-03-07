#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//render() creates a command string that, when run on a machine, will render a .blend file based on user inputs
const char* render(char file[50], char path[128], char format[8], char engine[24], int start, int end, int jump) {

    char command[500] = "blender -b "; //command to be run
    char s[8];  //start frame val
    char e[8];  //end frame val
    char j[8];  //jump frame val
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

    //if start val not 0, define start frame (-s <frame>)
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

    if (jump != 0) {
        sprintf(j, "%d", jump);
        strcat(command, " -j ");
        strcat(command, j);
    }

    strcat(command, " -a");  // -a renders specified frames
    char *runCmd = command;
    return runCmd;
}

//fragment() takes the start and end frames of a submitted job and divides them into equal chunks to be used as start/end frame vals for individual render nodes
int* fragment(int startFrame, int endFrame, int processors, int partitionArray[]) //processors = number of render nodes available
{
    int arraySize = 2*processors;  //2 vals (start and end frame) per machine
    int partition = (endFrame - startFrame)/processors;  //number of frames per machine
    int remainder = (endFrame - startFrame)%processors;  //leftover frames
    partitionArray[0] = startFrame;  //set first value to initial start frame
    for(int i = 1; i < (arraySize); i += 2)
    {
        partitionArray[i] = partitionArray[i - 1] + partition;  //set end val to previous val + partition
        if(i != 1)
        {
            partitionArray[i] = partitionArray[i - 1] + partition - 1;  //Tyler magic
        }
        partitionArray[i+1] = partitionArray[i] + 1; //set next start val to previous (end) val + 1
    }

    partitionArray[arraySize - 1] += remainder; //adds the remainder frames to the remainder
    return partitionArray;
}

int main() 
{
    char commands[4][500];  //2D array of commands to be sent to render nodes
    int partitionArray[8]; //C doesn't allow non constants as array sizes
    char file[] = "simple.blend";
    char path[] = "/home/justin.slater/tests/frame_";
    char format[] = "PNG";
    char engine[] = "CYCLES";
    int jump = 10;
    
    fragment(1, 254, 4, partitionArray);  //divide total frames evenly and store to partition array
    
    for(int i = 0, j = 0; i < 4; ++i, j+=2)
    {
        const char* cmd = render(file, path, format, engine, partitionArray[j], partitionArray[j+1], jump);  //store render command into cmd
        strcpy(commands[i], cmd);  //store cmd into 2D commands array  (extra steps because c.)
        printf("%s\n", commands[i]); //for testing purposes
        //todo: send values of commands[][] to render nodes.  note: can use system(commands[i])
    }
}
