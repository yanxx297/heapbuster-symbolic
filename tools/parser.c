#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_SIZE 256

void print_csv(FILE *fd){
    int iter, score, offset, depth;
    char *line= (char *)malloc(MAX_LINE_SIZE*sizeof(char));
    while(getline(&line, NULL, fd) != -1){
	printf("%s\n", line);
    }

/*
    while(!feof(fd)){
	fscanf(fd, "Iteration %d:\n", &iter);
	fscanf(fd, "--\n");
	fscanf(fd, "Achieved score %d with offset %d and depth %d\n", &score, &offset, &depth);
	getline(&input, NULL ,fd);
	printf("%d, %s, %d\n", iter, input, offset);
    }
    */
}

void filter_log(FILE *fd_in, char *line){
    int addr;
    char insn[MAX_LINE_SIZE];
    int res;
    if((res = sscanf(line, "%08x: %s\n", &addr, insn)) == 2){
	fwrite(line, strlen(line), 1,fd_in);
	return;
    }

    int off;
    char byte[MAX_LINE_SIZE];
    if((res = sscanf(line, "Store to target string offset %d: %s\n", &off, byte)) == 2){
//	printf("[%d] %s", res, line);
	fwrite(line, strlen(line), 1,fd_in);
    }
    return;
}

int split_by_iter(FILE *fd){
    int iter = -1;
    char *line = (char *)malloc(MAX_LINE_SIZE*sizeof(char));
    size_t len = 0;
    char filename[20];
    strcpy(filename, "startlog.log");

    FILE *fd_log = fopen(filename, "w");
    if(fd_log == NULL){
	perror("fopen");
	return -1;
    }	    

    while(getline(&line, &len, fd) != -1){
	if(sscanf(line, "Iteration %d:\n", &iter) > 0){
	    /* switch to the next file */
	    fclose(fd_log);

	    snprintf(filename, 20, "iter%d.log", iter);
	    printf("%s\n",filename);
	    fd_log = fopen(filename, "w");
	    if(fd_log == NULL){
		perror("fopen");
		return -1;
	    }else{
		fwrite(line, strlen(line), 1,fd_log);
	    }
	}else{
	    filter_log(fd_log, line);
	}
    }

    fclose(fd_log);
    return 0;
}

int main(int argc, char **argv){
    FILE *fd = fopen(argv[1], "r");
    if(fd == NULL){
	perror("fopen");
	return -1;
    }

    print_csv(fd);
    //split_by_iter(fd);

    fclose(fd);
    return 0;
}

