#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <ctype.h>

#include "series-renamer2.h"


//rename downloaded series in a consistent manner
//    scan base 'Series' directories 
//    generate mv scripts


int find_special_char(char * dir_name) {

    int i = 0;
    while (dir_name[i]) {
        if ( dir_name[i] == '['  ||
             dir_name[i] == '('  || 
             dir_name[i] == '.'      ) {
                 return 1;
             }
        i++;
    }
    return 0;
}

void scan_series_dirs(char * path){

    DIR *d;
    struct dirent *dir;
    int skip = 2;
    char buf[MAX_FILENAME];

    d = opendir(path);
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if (skip-- > 0)
                continue;

            // if not a folder
            sprintf(buf, "%s\\%s", path, dir->d_name);
            if (is_regular_file(buf)) {
                if (is_movie_file(dir->d_name)) {
                    //printf(" * Movie: %s\n", dir->d_name);
                } else {
                   // printf(" - RANDOM: %s\n", dir->d_name);
                   printf("rm \"%s\"\n", buf); 
                }
            }

        }

        closedir(d);
    } else {
        printf("err\n");
    }
}

void scan_base_dir(char * path) {

    DIR *d;
    struct dirent *dir;
    int skip = 2;
    char buf[MAX_FILENAME];

    printf("path: %s\n", path);
    d = opendir(path);
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if (skip-- > 0)
                continue;
            
            sprintf(buf, "%s\\%s", path, dir->d_name);
            //printf("%s\n", buf);
            scan_series_dirs(buf);
            //printf("\n");

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

    
    //scan_base_dir(s_rouse_dir);
    scan_base_dir(big_olive_dir);
    
    
    

	return 0;
}