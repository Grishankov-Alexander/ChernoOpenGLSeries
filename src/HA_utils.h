#ifndef HA_UTILS
#	define HA_UTILS


#ifdef __cplusplus
extern "C" { /* Don't magle function names */
#endif /* #ifdef __cplusplus */




#define __STDC_WANT_LIB_EXT1__ 1 /* Want bounds-checked functions */
#include <stddef.h>
#include <stdio.h>






/**
* Skips the line from the input stream f.
* 
* Return 1 on sucess.
* Return EOF on End of file or file error.
*/
static int HA_skipLine(FILE *f);






/**
* Skips the line from the input stream f.
* 
* Return 1 on sucess.
* Return EOF on End of file or file error.
*/
static int HA_skipLine(FILE *f)
{
	int assigned;

#if defined(__STDC_LIB_EXT1__) || defined(_MSC_VER) /* bounds-checked functions available */
	assigned = fscanf_s(f, "%*[^\n]"); /* Read everything but \n */
#else
	assigned = fscanf(f, "%*[^\n]"); /* Read everything but \n */
#endif __STDC_LIB_EXT1__ /* #ifdef __STDC_LIB_EXT1__ */

	switch (assigned) {
		case 0:			fgetc(f); return 1; /* Read \n */
		case EOF:
		default:		return EOF;
	}
}






#ifdef __cplusplus
}
#endif /* #ifdef __cplusplus */


#endif /* #ifndef HA_UTILS */