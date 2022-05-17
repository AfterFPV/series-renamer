#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <ctype.h>

#include "series-renamer2.h"


//rename downloaded series in a consistent manner
//    scan base 'Series' directories 
//    generate mv scripts


int find_special_char(char * dir_name) {

    int i = 0;
    while (dir_name[i]) {
        if ( dir_name[i] == '[' ||
             isdigit(dir_name[i])  ||
             dir_name[i] == '(' || 
             dir_name[i] == '.'         ) {
                 return 1;
             }
        i++;
    }
    return 0;
}



void scan_base_dir(char * path) {

    DIR *d;
    struct dirent *dir;

    printf("path: %s\n", path);
    d = opendir(path);
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            
            if (find_special_char(dir->d_name)) {
                printf("%s\n", dir->d_name);
            }
        }

        closedir(d);
    } else {
        printf("err\n");
    }
}

int main( int argc, char *argv[] )
{
    char s_rouse_dir[] = "e:\\User - Consolidated\\Videos\\Series";
    char big_olive_dir[] = "g:\\User_2\\User - Consolidated\\Videos\\Series";
    char tmp_dir[MAX_PATH];

    
    scan_base_dir(s_rouse_dir);
    //scan_base_dir(big_olive_dir);
    
    
    

	return 0;
}