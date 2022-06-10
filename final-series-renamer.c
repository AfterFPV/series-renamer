#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <ctype.h>
#include <regex.h>

#include "final-series-renamer.h"

//rename downloaded series in a consistent manner
//    scan base 'Series' directories 
//    generate mv scripts


int load_ep_names(char * series_name) {

    char buf[MAX_PATH];
    char episode_name[MAX_EPISODE_NAME];
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    int season, episode;
    int total_season = 0, max_episodes = 0;


    sprintf(buf, ".\\csv2\\%s.txt", series_name);
    if (is_regular_file(buf)) {
        {
            printf("FOUND: %s\n", buf);
            fp = fopen(buf, "r");
            if (fp == NULL)
                return 0;
            
            while ((read = getline(&line, &len, fp)) != -1) {
                season = episode = 0;
                parse_csv_line(&season, &episode, line);
                if (season > total_season) {
                    total_season = season;
                }
                if (episode > max_episodes) {
                    max_episodes = episode;
                }
            }

            printf("(%d, %d)\n", total_season, max_episodes);

            fclose(fp);
        }

        {
            fp = fopen(buf, "r");
            if (fp == NULL)
                return 0;

            while ((read = getline(&line, &len, fp)) != -1) {
                season = episode = 0;
                episode_name[0] = '\0';
                
                tokenize_csv_line(episode_name, &season, &episode, line);
                //if (season) {
                    //printf("%dx%02d - %s\n", season, episode, episode_name);
                //}
            }

            if (line)
                free(line);
        }

        return 1;
    } else {
        //printf("NOT FOUND: %s\n", buf);
        return 0;
    }
}

void scan_episodes(char * path, int season_num, struct MySeries *cur_series) {

    DIR *d;
    struct dirent *dir;
    int skip = 2;
    char buf[MAX_PATH];
    char extension[MAX_EXTENSION];
    int season_num2 = 0;
    int episode_num = 0;
    struct MyFile *f;

    d = opendir(path);
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if (skip-- > 0)
                continue;
            
            sprintf(buf, "%s\\%s", path, dir->d_name);
            if (is_regular_file(buf)) {
                f = malloc(sizeof(struct MyFile));
                clear_file(f);
                strcpy(f->full_season_folder_path, path);
                strcpy(f->original_filename, dir->d_name);
                get_extension(extension, dir->d_name);
                strcpy(f->extension, extension);
                //printf("%s, %s\n", extension, dir->d_name);

                if (is_movie_file(extension)) {

                    // The.Good.Place.S01E01.Pilot.720p.WEB.x264-[MULVAcoded].mkv
                    // printf("            * %s\n", dir->d_name);
                    parse_season_and_episode(&season_num2, &episode_num, dir->d_name);
                    if (episode_num != 0 && season_num2 !=0 && season_num2 == season_num) {

                        f->episode_num = episode_num;
                        f->season_num = season_num2;

                        print_mv(cur_series, f);
                    } else {
                        printf("    err4: %s\\%s\n", path, dir->d_name);
                        print_file(f);
                        printf("\n");
                    }
                }
                free(f);
            }
        }

        closedir(d);
    } else {
        printf("err3: %s\n", path);
    }
}


void scan_series_dirs_for_seasons(char * path, struct MySeries *cur_series) {

    DIR *d;
    struct dirent *dir;
    int skip = 2;
    char buf[MAX_PATH];
    int season_num = 0;

    d = opendir(path);
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if (skip-- > 0)
                continue;
            
            
            season_num = parse_season(dir->d_name);

            sprintf(buf, "%s\\%s", path, dir->d_name);
            
            // The Good Place - Season 1
            // printf("        * %s\n", dir->d_name);
            if (!is_regular_file(buf)) {
                scan_episodes(buf, season_num, cur_series);
            }
        }

        closedir(d);
    } else {
        printf("err2\n");
    }
}


void scan_series_base_dir(char * path) {

    DIR *d;
    struct dirent *dir;
    int skip = 2;
    char buf[MAX_PATH];
    struct MySeries *s;


    //printf("%s\n", path);
    d = opendir(path);
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if (skip-- > 0)
                continue;
            
            if (load_ep_names(dir->d_name)) {
                continue;
                s = malloc(sizeof(struct MySeries));
                clear_series(s);
                strcpy(s->series_name, dir->d_name);
                sprintf(buf, "%s\\%s", path, dir->d_name);
                
                // The Good Place
                // printf("    * %s\n", dir->d_name);
                scan_series_dirs_for_seasons(buf, s);
                free(s);
            }
        }

        closedir(d);
    } else {
        printf("err1\n");
    }
}


int main( int argc, char *argv[] )
{
    char s_rouse_dir[] = "e:\\User - Consolidated\\Videos\\Series";
    char big_olive_dir[] = "g:\\User_2\\User - Consolidated\\Videos\\Series";
    
    printf("Starting Scan: \'%s\'\n", s_rouse_dir);
    scan_series_base_dir(s_rouse_dir);
    //scan_base_dir(big_olive_dir);
    
    printf("\nDone\n\n\n");

	return 0;
}