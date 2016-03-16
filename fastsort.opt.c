#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


int lineSize = 128;
int maxLines = 20000;
int numLines = 0;
int column = 1;
typedef struct strAndTok { 
    char *line;
    char *token;
} strAndTok;
int elemSize = sizeof(strAndTok);
int pointerSize = sizeof(strAndTok *);

char *tokenizeString(char *line){
    // make a copy of the string
    char *temp = strtok(line, " ");
    char *token = "";
    // Token wasn't found on the line so return empty string
    if(!temp){
        return token;
    }
    token = temp;
    int i = 1;
    // Get to the column we need to compare on
    while(i < column && temp != NULL){
        temp = strtok(NULL, " ");
        if(temp != NULL){
            token = temp;
        }
        i++;
    }
    return token;
}

int sort(void const *arg1 , void const *arg2){
    // qsort passes in a ref to the array so we need to get the strings from the array
        // compare based on the token
    strAndTok *temp1 = *(strAndTok **)arg1;
    strAndTok *temp2 = *(strAndTok **)arg2;
    printf("Token1: %s\nToken2: %s\n--------------------\n", temp1->token, temp2->token);
    return strcmp(temp1->token, temp2->token);
    
}

strAndTok **readFileToArray(strAndTok **strPtr, FILE *fp){
    strAndTok *elem = *(strPtr+numLines);
    char *str = malloc(lineSize);
    if(str == NULL){
        fprintf(stderr, "malloc failed\n"); 
        exit(1); 
    }
    // Read the lines in one at at time up to max size lineSize
    while(fgets(str, lineSize, fp) != NULL){
        elem = *(strPtr+numLines);
        elem = malloc(elemSize);
        if(elem == NULL){
            fprintf(stderr, "malloc failed\n"); 
            exit(1); 
        }
        int len = strlen(str);
        if(*(str+len-1) != '\n'){
            fprintf(stderr, "Line too long\n"); 
            exit(1);
        }
        elem->line = str;
        char *dup1 = strdup(str);
        // Implicit malloc in strdup
        if(dup1 == NULL){
            fprintf(stderr, "malloc failed\n");
            exit(1);
        }
        elem->token = tokenizeString(dup1);

        //free(dup1);
        numLines++;
        printf("Line: %s\nToken: %s\n",elem->line,elem->token );
        str = malloc(lineSize);
        if(str == NULL){
            fprintf(stderr, "malloc failed\n"); 
            exit(1);
        }
        //printf("%s\n", *(linePtr+lineNum-1));
        if(numLines >= maxLines){
            // If more lines than can fit in array, expand array
            printf("In expand\n");
            maxLines = 2*maxLines;
            strPtr = realloc(strPtr, maxLines*pointerSize);
            if(strPtr == NULL){
                fprintf(stderr, "malloc failed\n"); 
                exit(1);        
            }
        }
        //*(strPtr + numLines) = malloc(elemSize);
        // if(*(strPtr + numLines) == NULL){
        //     fprintf(stderr, "malloc failed\n"); 
        //     exit(1);
        // }
    }


    return strPtr;
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
            fprintf(stderr, "Error: Bad command line parameters\n");
            exit(1);
        }
        fileName = argv[2];
    } else {
        fprintf(stderr, "Error: Bad command line parameters\n");
        exit(1);
    }
    FILE *fp = fopen(fileName, "r");
    if(!fp){
        fprintf(stderr, "Error: Cannot open file %s\n", fileName);
        exit(1);
    }
    //int pointerSize = sizeof(strAndTok *);
    strAndTok **strPtr = malloc(pointerSize*maxLines);
    if(!strPtr){
        fprintf(stderr, "malloc failed\n");   
        exit(1);     
    }
    strPtr = readFileToArray(strPtr, fp);
    fclose(fp);
    int i = 0;
    for(i = 0; i < numLines; i++){
        printf("%s",(*(strPtr+i))->line);
    }
    printf("Before sort\n");
    qsort(strPtr, numLines, pointerSize, sort);
    // int i = 0;
    printf("before print\n");
    for(i = 0; i < numLines; i++){
        printf("%s",(*(strPtr+i))->line);
    }
    free(strPtr);
    return 0;
}
