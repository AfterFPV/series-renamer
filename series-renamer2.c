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


#define SEASON_ATOI_BUF_SIZE 3
#define EAT_FORWARD 3
int determine_season(char * filename) {

    const char* const season_str = "Season";
    char season[SEASON_ATOI_BUF_SIZE] = "";
    int i = 0;
    char *pch = NULL;
    int found_digit = 0;
    int season_num = 0;


    pch = strstr(filename, season_str);
    if (pch) {
        //printf("    * pch = %s   ", pch);
        pch += (strlen(season_str));

        while (i < EAT_FORWARD) {
            if (isdigit(pch[0])) {
                found_digit = 1;
                break;
            }
            pch += 1;
            i++;
        }

        if (found_digit) {
            i = 0;
            while (i < SEASON_ATOI_BUF_SIZE) {
                if (isdigit(pch[0])) {
                    season[i] = pch[0];
                    pch += 1;
                    i++;
                } else {
                    break;
                }
            }
            season_num = atoi(season);
        }
    }

    return season_num;
}

void scan_series_dirs(char * path, char * series_name){

    DIR *d;
    struct dirent *dir;
    int skip = 2;
    char buf[MAX_FILENAME];
    int season = 0;

    d = opendir(path);
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if (skip-- > 0)
                continue;

            sprintf(buf, "%s\\%s", path, dir->d_name);
            if (is_regular_file(buf)) {
                if (is_movie_file(dir->d_name)) {
                    //printf(" * Movie: %s\n", dir->d_name);
                } else {
                   // printf(" - RANDOM: %s\n", dir->d_name);
                   //printf("rm \"%s\"\n", buf); 
                }
            } else {
                season = 0;
                season = determine_season(dir->d_name);
                
                printf("mv \"%s\\%s\" \"%s\\%s - Season %d\"\n", series_name, dir->d_name, series_name, series_name, season);
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
            scan_series_dirs(buf, dir->d_name);
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