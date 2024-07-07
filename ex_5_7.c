/*
 * Exercise 5-7. Rewrite readlines to store lines in an array supplied by main, rather than
 * calling alloc to maintain storage. How much faster is the program?
 * */

#define _GNU_SOURCE

#include <stdio.h>
#include <string.h>

#define MAX_LEN 1000   // Max len of any input line
#define MAX_LINES 5000 // Max number of line to be sorted
#define BUFF_SIZE 5000 // Storage for read lines

int read_lines(char *lineptr[], int maxlines, char *buf, size_t buf_size);

int main(){
    char buf[BUFF_SIZE]; 
    char *lineptr[MAX_LEN];
    int lines_num = read_lines(lineptr, MAX_LINES, buf, BUFF_SIZE);
    printf("Read lines: %d\n", lines_num);
    for(int i = 0; i < lines_num; ++i){
        printf("%s\n", lineptr[i]);
    }
}

int read_lines(char *lineptr[], int maxlines, char *buf, size_t buf_size){
    ssize_t read = 0; 
    size_t n = 0;
    int lines_read = 0;
    char line[MAX_LEN];
    char *buf_begin = buf; // just save the begin pointer of the buffer

    char *l = &(line[0]); // this is for casting from char*[] to char** to pass to the getline
    while(maxlines-- > 0 &&
         (read = getline(&l, &n, stdin)) != -1 &&
         (buf + read < buf_begin + buf_size))
    {
        strcpy(buf, l);
        buf[read-1] = '\0';
        *lineptr++ = buf;
        buf += read;
        lines_read++;
    } 
    return lines_read;
}
