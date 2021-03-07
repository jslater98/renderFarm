#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int render(char file[50], char path[128], char engine[24], int start, int end) {

    char command[500]; //command to be run

    //create command string to be run on lab machine
    sprintf(command, "blender -b %s -o %s -E %s -s %d -e %d", file, path, engine, start, end);

    system(command); //run command on lab machine
    
    return 0;
}

int main() {

    /*file = file to be rendered (*.blend)
    path = path of where to save rendered image(s)
    engine = render engine to be used
    start = start frame
    end = end frame */

    char file[50], path[128], engine[24];
    int start = 1, end;

    //todo: store values from "submit job" form in respective vars
    strcpy(file, "simple.blend");
    strcpy(path, "/home/justin.slater/frame_#####");
    end = 240;
    strcpy(engine, "CYCLES");
    printf("%s", path);



    render(file, path, engine, start, end); //call render function



}

