#include <iostream>
#include <dirent.h>
using namespace std;

int main()
{
    cout << "Hello World!" << endl;
    DIR *d;
    struct dirent *dir;
    d = opendir(".");
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            printf("%s\n", dir->d_name);
        }
        closedir(d);
    }
    if (remove("Makefile") == 0)
       printf("Deleted successfully");
    else
       printf("Unable to delete the file");
    return 0;
}

