#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>


#define MAX_FILENAME 1028
#define MAX_PATH 4098

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


int is_regular_file(const char *path)
{
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
}

#define EXT_BUF_SIZE 6
int is_movie_file(const char *filename)
{
    char extension[EXT_BUF_SIZE];
    char *p = NULL;
    int i = 0;

    p = strrchr(filename, '.');
    if (p) {
        strncpy(extension, p, EXT_BUF_SIZE);

        while (movie_extensions[i]) {
            //printf("COMPARE    * %s - %s", movie_extensions[i], extension + 1);
            if (!strcmp(movie_extensions[i], extension + 1)) {
                return 1;
            }
            i++;
        }
    }

    return 0;
}