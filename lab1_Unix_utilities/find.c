#include"kernel/types.h"
#include"kernel/stat.h"
#include"user/user.h"
#include"kernel/fs.h"

char*
fmtname(char *path) {
    static char buf[DIRSIZ+1];
    char *p;

    // Find first character after last slash.
    for(p=path+strlen(path); p >= path && *p != '/'; p--)
        ;
    p++;

    // Return blank-padded name.
    if(strlen(p) >= DIRSIZ)
        return p;
    memmove(buf, p, strlen(p));
    memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
    return buf;
}

void
find(char *path, char *filename) {
    int bufsize = 512;
    //char *buf = malloc(bufsize * sizeof(char)), *p;
    char buf[bufsize], *p;
    int fd;
    struct dirent de;
    struct stat st;
    if((fd = open(path, 0)) < 0) {
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    if(fstat(fd, &st) < 0) {
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    switch(st.type) {
        case T_FILE:
            char name1[DIRSIZ + 1], name2[DIRSIZ + 1];
            strcpy(name1, fmtname(path));
            strcpy(name2, fmtname(filename));
            int v1 = strcmp(name1, name2);
            if(!v1) {
                printf("%s\n", path);
            }
            break;
        case T_DIR:
            if(strlen(path) + 1 + DIRSIZ + 1 > bufsize) {
                printf("find: path too long\n");
                break;
            }
            strcpy(buf, path);
            p = buf + strlen(buf);
            *p++ = '/';
            while(read(fd, &de, sizeof(de)) == sizeof(de)) {
                if(de.inum == 0) continue;  // inum == 0 indicates the deleted file
                if(!strcmp(de.name, ".")) continue;
                if(!strcmp(de.name, "..")) continue;
                memmove(p, de.name, DIRSIZ);
                p[DIRSIZ] = '\0';
                if(stat(buf, &st) < 0) {
                    printf("find: cannot stat %s\n", buf);
                    continue;
                }
                find(buf, filename);
            }
            break;
         default : break;
    }
    close(fd);
    //free(buf);
}

int
main(int argc, char *argv[]) {
    find(argv[1], argv[2]);
    return 0;
}
