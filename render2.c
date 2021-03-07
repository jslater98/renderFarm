#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int render(char file[50], char path[128], char format[8], char engine[24], int start, int end) {

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

    //printf("%s", command);  //for testing purposes

    system(command); //run command on lab machine
    
    return 0;
}

int main() {  //currently just a test, values will need to be assigned from user inputs

    /*file = file to be rendered (*.blend)
    path = path of where to save rendered image(s)
    format = file format of rendered image(s) (PNG, JPEG, etc.)
    engine = render engine to be used
    start = start frame
    end = end frame;  start and end frames cannot be NULL, use 0 instead */

    char file[50], path[128], format[8], engine[24];
    int start, end;

    //todo: store values from "submit job" form in respective vars
    strcpy(file, "simple.blend");
    strcpy(path, "/home/justin.slater/tests/frame_");
    strcpy(engine, "CYCLES");
    strcpy(format, "PNG");
    start = 2;
    end = 2;
    
    
    render(file, path, format, engine, start, end); //call render function



}

