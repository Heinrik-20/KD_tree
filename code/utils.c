/*
	Utilities implementation referenced from Assignment1 solutions
	by Grady Fitzpatrick
*/

#include <stdio.h>
#include <math.h>
#include <string.h>

ssize_t getline_clean(char **line_ptr, size_t *n, FILE *stream){
	ssize_t ret_val = getline(line_ptr, n, stream);
    if(ret_val != (-1) && strlen(*line_ptr) > 0 && 
       (*line_ptr)[strlen(*line_ptr) - 1] == '\n'){
        (*line_ptr)[strlen(*line_ptr) - 1] = '\0';
    }
    if(ret_val != (-1) && strlen(*line_ptr) > 0 && 
       (*line_ptr)[strlen(*line_ptr) - 1] == '\r'){
        (*line_ptr)[strlen(*line_ptr) - 1] = '\0';
    }
    return ret_val;
}
