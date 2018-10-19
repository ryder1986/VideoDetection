#include <iostream>

using namespace std;


#include <dirent.h>

int dir_count(char *dirname)
{
    DIR *d;
    struct dirent *dir;
    d = opendir(dirname);
    if (d) {
        int num=0;
        while ((dir = readdir(d)) != NULL) {
            num++;

        }
        closedir(d);
        return num;
    }
    return -1;
}
void delete_dir_files(char *dirname,int total,int left)
{
    DIR *d;
    struct dirent *dir;
    d = opendir(dirname);
    if (d) {
        int num=0;
        while ((dir = readdir(d)) != NULL&&  num++<(total-left)) {

               printf("Deleteing %s, %d  type\n",dir->d_name,dir->d_type);
            if (remove(dir->d_name) == 0)
                printf("Deleted %s successfully\n",dir->d_name);
            else
                printf("Unable to %s  delete the file\n",dir->d_name);
        }
        closedir(d);
    }
}

int main()
{
    int f_count=   dir_count(".");
    cout << f_count << endl;

    delete_dir_files(".",f_count,2);
    return 1;
    //    cout << "Hello World!" << endl;
    //    DIR *d;
    //    struct dirent *dir;
    //    d = opendir("/etc");
    //    if (d) {
    //        int num=0;
    //        while ((dir = readdir(d)) != NULL) {
    //            num++;
    //            printf("get name %s\n", dir->d_name);
    //        }
    //        printf(" file num-> %d\n", num);
    //        //             closedir(d);
    //        //                d = opendir(".");
    //        seekdir(d,0);
    //        num=0;
    //        while ((dir = readdir(d)) != NULL) {
    //            num++;
    //            if(num<3)
    //                printf("get name %s\n", dir->d_name);
    //        }
    //        closedir(d);
    //    }
    //    if (remove("Makefile") == 0)
    //       printf("Deleted successfully\n");
    //    else
    //       printf("Unable to delete the file");
    return 0;
}

