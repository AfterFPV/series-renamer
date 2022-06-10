#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>


#define MAX_FILENAME 1028
#define MAX_PATH 4098
#define MAX_EXTENSION 16

#define MAX_SERIES 36
#define MAX_EPISODES 50
#define MAX_EPISODE_NAME 512


extern char dictionary[MAX_SERIES][MAX_EPISODES][MAX_EPISODE_NAME];

void clear_dictionary() {
    int i, j;

    for (i = 0; i < MAX_SERIES; i++) {
        for (j = 0; j < MAX_EPISODES; j++) {
            dictionary[i][j][0] = '\0';
        }
    }
}

struct MySeries {
    char series_name[MAX_FILENAME];                     // F1 - Drive To Survive
    char series_abbreviation[MAX_FILENAME];             // F1 - Drive To Survive
    unsigned int season_bitfield[1];
} ;

struct MyFile {
    char full_season_folder_path[MAX_PATH];       // ./F1 - Drive To Survive/F1 - Drive To Survive - Season 1/
    char original_filename[MAX_FILENAME];         // Formula.1.Drive.to.Survive.S01E07.WEBRip.x264-ION10.mp4
    int season_num;                               // 1
    int episode_num;                              // 7
    char episode_name[MAX_EPISODE_NAME];          // Keeping Your Head
    char extension[MAX_EXTENSION];                // mp4
} ;

void clear_series(struct MySeries *cur_series)
{
    memset(cur_series->series_name, '\0', sizeof(cur_series->series_name));
    memset(cur_series->series_abbreviation, '\0', sizeof(cur_series->series_abbreviation));
    cur_series->season_bitfield[0] = 0;
}

void clear_file(struct MyFile *cur_file)
{
    memset(cur_file->full_season_folder_path, '\0', sizeof(cur_file->full_season_folder_path));
    memset(cur_file->original_filename, '\0', sizeof(cur_file->original_filename));
    cur_file->season_num = 0;
    cur_file->episode_num = 0;
    memset(cur_file->episode_name, '\0', sizeof(cur_file->episode_name));
    memset(cur_file->extension, '\0', sizeof(cur_file->extension));
}

// mv "./F1 - Drive To Survive/F1 - Drive To Survive - Season 1/Formula.1.Drive.to.Survive.S01E07.WEBRip.x264-ION10.mp4"
// "./F1 - Drive To Survive/F1 - Drive To Survive - Season 1/F1 - Drive To Survive - S01E07 - Keeping Your Head.mp4"

void print_mv(struct MySeries *cur_series, struct MyFile *cur_file)
{
    char left_file[MAX_PATH];
    char right_file[MAX_PATH];

    sprintf(left_file, "%s\\%s", cur_file->full_season_folder_path, cur_file->original_filename);
    //printf("left_file: %s\n", left_file);
    sprintf(right_file, "%s\\%s - %dx%02d - %s.%s", cur_file->full_season_folder_path, (cur_series->series_abbreviation[0]) ? cur_series->series_abbreviation: cur_series->series_name, cur_file->season_num, cur_file->episode_num, cur_file->episode_name, cur_file->extension);
    //printf("right_file: %s\n", right_file);

    printf("mv \"%s\" \"%s\"\n", left_file, right_file);

}


void print_file(struct MyFile *cur_file)
{
    printf("        MyFile:\n");
    printf("            * %s\n", cur_file->full_season_folder_path);
    printf("            * %s\n", cur_file->original_filename);
    printf("            * %d\n", cur_file->season_num);
    printf("            * %d\n", cur_file->episode_num);
    printf("            * %s\n", cur_file->episode_name);
    printf("            * %s\n", cur_file->extension);
}

#define SetBit(A,k)     ( A[(k/32)] |= (1 << (k%32)) )
#define ClearBit(A,k)   ( A[(k/32)] &= ~(1 << (k%32)) )
#define TestBit(A,k)    ( A[(k/32)] & (1 << (k%32)) )


const char* const movie_extensions[] = { 
                                    "mov",
                                    "mp4",
                                    "avi",
                                    "mkv",
                                    "mpg",
                                    "mpeg",
                                    "ts",
                                    "swf",
                                    "m4v",
                                    "wmv",
                                    0
                                 };


void get_extension(char *extension_ret, const char *filename) 
{
    char *p = NULL;

    p = strrchr(filename, '.');

    strcpy(extension_ret, p + 1);

}


int is_movie_file(const char *extension)
{
    int i = 0;

    while (movie_extensions[i]) {
        //printf("COMPARE    * %s - %s", movie_extensions[i], extension + 1);
        if (!strcmp(movie_extensions[i], extension)) {
            return 1;
        }
        i++;
    }

    return 0;
}


int is_regular_file(const char *path)
{
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
}


const char const special_chars[] = { 
                                    '[',
                                    '(',
                                    '.',
                                    0
                                 };

int find_special_char(char * dir_name) {

    int i, j;

    i = 0;
    while (dir_name[i]) {
        j = 0;
        while (special_chars[i]) {
            if (dir_name[i] == special_chars[j])
            {
                return 1;
            }
            j++;
        }
        i++;
    }
    return 0;
}

int load_abbreviations()
{



}


#define SEASON_ATOI_BUF_SIZE 3
#define EAT_FORWARD 3

int parse_season(char * foldername) 
{
    const char* const season_str = "Season";
    char season[SEASON_ATOI_BUF_SIZE] = "";
    int i = 0;
    char *pch = NULL;
    int season_num = 0;
    int found_digit = 0;

    pch = strstr(foldername, season_str);
    if (pch) {
        
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


const char* const regex_patterns[] = { 
                                        "S([0-9][0-9])E([0-9][0-9])",
                                        "S([0-9])E([0-9][0-9])",
                                        "([0-9]+)x([0-9][0-9])",
                                        "([0-9]+)e([0-9][0-9])",                                        
                                        "([0-9]+)-([0-9][0-9])",
                                        "S([0-9][0-9]) E([0-9][0-9])",
                                        "([0-9]+)([0-9][0-9])",
                                        "Season ([0-9]+) Episode ([0-9]+)",
                                        0
                                     };


#define SEASON_ATOI_BUF_SIZE 3
void parse_season_and_episode(int * season_ret, int * episode_ret, char * filename) 
{
    regex_t regex;
    regmatch_t rm[3];
    int reti;
    char season[SEASON_ATOI_BUF_SIZE] = "";
    char episode[SEASON_ATOI_BUF_SIZE] = "";
    int i = 0;
    int found_match = 0;

    while (regex_patterns[i] && !found_match) {

        /* Compile regular expression */
        if (regcomp(&regex, regex_patterns[i], REG_EXTENDED) != 0){
            printf("FAIL - regex compilation failure\n");
            return;
        }
        /* Execute regular expression */
        reti = regexec(&regex, filename, 3, rm, 0);
        
        if (!reti) {
            sprintf(season, "%.*s", (int)(rm[1].rm_eo - rm[1].rm_so), filename + rm[1].rm_so);
            sprintf(episode, "%.*s", (int)(rm[2].rm_eo - rm[2].rm_so), filename + rm[2].rm_so);
            
            *season_ret = atoi(season);
            *episode_ret = atoi(episode);
            
            regfree(&regex);
            found_match = 1;
            //printf("                * Match! (%d,%d)\n", season_int, episode_int);
            break;

        } else if (reti == REG_NOMATCH) {
            regfree(&regex);
        }

        i++;
    }

    //printf("                * Fail\n");

    return;
}


void parse_csv_line(int * season_ret, int * episode_ret, char * line) 
{
    const char s[2] = ",";
    char *token;
    int index_num = 0;
    
    /* get the first token */
    token = strtok(line, s);
    index_num = atoi(token);

    if (index_num != 0) {
        token = strtok(NULL, s);
        *season_ret = atoi(token);
        token = strtok(NULL, s);
        *episode_ret = atoi(token);
    }
}


void tokenize_csv_line(char * episode_name, int * season_ret, int * episode_ret, char * line) 
{
    const char s[2] = ",";
    const char t[2] = "\"";
    char *token;
    int index_num = 0;
    
    /* get the first token */
    token = strtok(line, s);
    index_num = atoi(token);

    if (index_num != 0) {
        token = strtok(NULL, s);
        *season_ret = atoi(token);
        token = strtok(NULL, s);
        *episode_ret = atoi(token);
        token = strtok(NULL, s);
        token = strtok(NULL, t);
        strcpy(episode_name, token);
        //printf("%d - %dx%02d - %s\n", index_num, *season_ret, *episode_ret, episode_name);
    }
}