#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <ctype.h>
#include <regex.h>

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
int determine_season(char * filename, char * series_name) {
    regex_t regex;
    regmatch_t rm[3];
    int reti;
    char msgbuf[MAX_FILENAME];
    const char* const season_str = "Season";
    char season[SEASON_ATOI_BUF_SIZE] = "";
    char episode[SEASON_ATOI_BUF_SIZE] = "";
    int season_int, episode_int;
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

    if (!found_digit){
        /* Compile regular expression */
        if (regcomp(&regex, "S([0-9][0-9])E([0-9][0-9])", REG_EXTENDED) != 0){
            printf("FAIL - regex compilation failure\n");
            return 0;
        }
        /* Execute regular expression */
        reti = regexec(&regex, filename, 3, rm, 0);
        
        if (!reti) {
            //printf("Match!   %s\n", filename); 
            
            sprintf(season, "%.*s", (int)(rm[1].rm_eo - rm[1].rm_so), filename + rm[1].rm_so);
            sprintf(episode, "%.*s", (int)(rm[2].rm_eo - rm[2].rm_so), filename + rm[2].rm_so);
           
            season_int = atoi(season);
            episode_int = atoi(episode);

            printf("mv \"./%s/%s\" \"./%s/%s - Season %d/\"\n", series_name, filename, series_name, series_name, season_int);

            //printf("<%d,%d>\n\n", season_int, episode_int);
            
            /* Free memory allocated to the pattern buffer by regcomp() */
            regfree(&regex);
            season_num = season_int;
        } else if (reti == REG_NOMATCH) {
            regfree(&regex);
            return 0;;
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
    unsigned int season_bitfield[1];
    season_bitfield[0] = 0;


    d = opendir(path);
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if (skip-- > 0)
                continue;

            sprintf(buf, "%s\\%s", path, dir->d_name);
            if (is_regular_file(buf)) {
                if (is_movie_file(dir->d_name)) {
                    //rintf(" * Movie: %s\n", dir->d_name);
                    season = 0;
                    season = determine_season(dir->d_name, series_name);

                    if (season != 0 && season < 32) {
                        if (!TestBit(season_bitfield, season)) {
                            printf("mkdir \"./%s/%s - Season %d\"\n", series_name, series_name, season);
                            SetBit(season_bitfield, season);
                        }
                    }
                } else {
                    //printf(" - RANDOM: %s\n", dir->d_name);
                    //printf("rm \"%s\"\n", buf); 
                }
            } else {
                //season = 0;
                //season = determine_season(dir->d_name);
                
                //printf("mv \"%s\\%s\" \"%s\\%s - Season %d\"\n", series_name, dir->d_name, series_name, series_name, season);
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
    
    
    scan_base_dir(s_rouse_dir);
    //scan_base_dir(big_olive_dir);
        

	return 0;
}