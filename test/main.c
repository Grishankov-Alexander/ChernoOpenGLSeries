#pragma warning(disable : 4996)


#include <stdio.h>


#define FILENAME        "../src/res/shaders/basic.shader"






/*
* Skip the input line from the stream f.
* 
* Return 1 on sucess.
* Return EOF on failure.
*/
static int skipLine(FILE *f);






int main(void)
{
    return 0;
}






/*
* Skip the input line from the stream f.
* 
* Return 1 on sucess.
* Return EOF on failure.
*/
static int skipLine(FILE *f)
{
    switch (fscanf(f, "%*[^\n]")) { /* Reads everything but \n */
        case 0:     fgetc(f); return 1; /* Reads \n */
        case EOF:   return EOF; /* Enf of File or File Error */
    }
}