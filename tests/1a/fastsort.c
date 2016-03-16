#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


int lineSize = 128;
int maxLines = 200;
int numLines = 0;
int column = 1;

char * tokenizeString(char *line){
    char *delim = " ";
    char *temp = strtok(line, delim);
    char *token = "";
    // Token wasn't found on the line so return empty string
    if(!temp){
        return token;
    }
    token = temp;
    int i = 1;
    // Get to the column we need to compare on
    while(i < column && temp != NULL){
        temp = strtok(NULL, delim);
        if(temp != NULL){
            token = temp;
        }
        i++;
    }
    return token;
}

int sort(void const *arg1 , void const *arg2){
    // qsort passes in a ref to the array so we need to get the strings from the array
    char **a = (char **)arg1;
    char **b = (char **)arg2;

    // need to duplicate the strings because strtok is destructive
    //printf("String1:%s\nStirng2:\n",*a);
    char *dup1 = strdup(*a);
    char *dup2 = strdup(*b);
    // Implicit malloc in strdup
    if(dup1 == NULL || dup2 == NULL){
        fprintf(stderr, "malloc failed\n");
        exit(1);
    }
    char *tok1 = tokenizeString(dup1);
    char *tok2 = tokenizeString(dup2);
    // printf("String1: %sString2: %s",a,b);
    // printf("Token1: %s\nToken2: %s\n----------------------\n", tok1, tok2);
    // Free duplicates so they don't leak
    int cmpVal = strcmp(tok1,tok2);
    free(dup1);
    free(dup2);
    return cmpVal;
    
}

char **readFileToArray(char **linePtr, FILE *fp){
    // TODO Add checking for too long of line
    int lineNum = 0;
    linePtr[lineNum] = malloc(lineSize);
    if(linePtr[lineNum] == NULL){
        fprintf(stderr, "malloc failed\n"); 
        exit(1); 
    }
    // Read the lines in one at at time up to max size lineSize
    while(fgets(linePtr[lineNum], lineSize, fp) != NULL){
        char *str = *(linePtr+lineNum);
        int len = strlen(str);
        if(*(str+len-1) != '\n'){
            fprintf(stderr, "Line too long"); 
            exit(1);
        }
        lineNum++;
        //printf("%s\n", *(linePtr+lineNum-1));
        if(lineNum >= maxLines){
            // If more lines than can fit in array, expand array
            maxLines = 2*maxLines;
            linePtr = realloc(linePtr, maxLines*lineSize);
            if(linePtr == NULL){
                fprintf(stderr, "malloc failed\n"); 
                exit(1);        
            }
        }
        linePtr[lineNum] = malloc(lineSize);
        if(linePtr[lineNum] == NULL){
            fprintf(stderr, "malloc failed\n"); 
            exit(1);
        }
    }
    numLines = lineNum;
    return linePtr;
}

int main(int argc, char *argv[]) {
    char *fileName;
    if(argc == 2){
        fileName = argv[1];
    } else if(argc == 3){
        char *colStr = argv[1];
        column = atoi(colStr);
        if(column < 0){
            // Correct form of arg
            column = column * -1;
        }
        else {
            // Incorrect arg
            fprintf(stderr, "Error: Bad command line parameters");
            exit(1);
        }
        fileName = argv[2];
    } else {
        fprintf(stderr, "Error: Bad command line parameters");
        exit(1);
    }
    FILE *fp = fopen(fileName, "r");
    if(!fp){
        fprintf(stderr, "Error: Cannot open file %s\n", fileName);
        exit(1);
    }
    
    char **linePtr = malloc(lineSize*maxLines);
    if(!linePtr){
        fprintf(stderr, "malloc failed\n");   
        exit(1);     
    }
    linePtr = readFileToArray(linePtr, fp);
    fclose(fp);
    qsort(linePtr, numLines, sizeof(char *), sort);
    int i = 0;
    for(i = 0; i < numLines; i++){
        printf("%s",*(linePtr+i));
    }
    free(linePtr);
    return 0;
}
