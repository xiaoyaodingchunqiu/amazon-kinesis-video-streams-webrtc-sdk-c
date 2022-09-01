#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <string>

#define BUF_SIZE 128
#define WEBRTC_NAME "kvsWebrtcClient"
#define SN_FILENAME "sn.txt"

using namespace std;

// get the process id by use it name: "kvs_gstreamer_s", for kill
int getPidByName(char* task_name)
{
    DIR *dir = NULL;
    struct dirent *ptr;
    FILE *fp;
    char filepath[BUF_SIZE] = {0};
    char cur_task_name[BUF_SIZE] = {0};
    char buf[BUF_SIZE] = {0};
    int pid = 0;
    dir = opendir("/proc");
    if (NULL != dir) {
        while ((ptr = readdir(dir)) != NULL) {
            // jump . or ..
            if ((strcmp(ptr->d_name, ".") == 0) || (strcmp(ptr->d_name, "..") == 0))
                continue;
            if (DT_DIR != ptr->d_type) {
                continue;
            }

            sprintf(filepath, "/proc/%s/status", ptr->d_name);
            fp = fopen(filepath, "r");
            if (NULL != fp) {
                if( fgets(buf, BUF_SIZE-1, fp)== NULL ){
                    fclose(fp);
                    continue;
                }
                sscanf(buf, "%*s %s", cur_task_name);

                // get pid
                if (!strcmp(task_name, cur_task_name)) {
                    pid = atoi(ptr->d_name);
                }
                fclose(fp);
            }

        }
        closedir(dir);
    }
    printf("pid: %d\n", pid);
    return pid;
}

string getSN() {
        FILE* fd = fopen(SN_FILENAME, "r");
	if(fd == NULL) {
		printf("fopen sn filename err");
		exit(1);
	}
        char sn[256] = {0};
        fscanf(fd,"%[^\n]", sn);
        fclose(fd);
	string ret = sn;
        return ret;
}

int doWork(string cmd) {
        while(1) {
                if(getPidByName(WEBRTC_NAME) == 0) {
                        // do start
                        system(cmd.c_str());
                }
                sleep(5);
		cout << "sleep once" << endl;
        }
}

int main(int argc, char** argv)
{
        char sn[128] = {0};
	string SN = getSN();
        string cmd = "samples/kvsWebrtcClientMasterGstSample ";
	cmd += SN;
	cout << cmd << endl;
        doWork(cmd);
        return 0;
}
