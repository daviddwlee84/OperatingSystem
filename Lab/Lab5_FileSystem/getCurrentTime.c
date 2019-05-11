#include <stdio.h> /* printf */
#include <time.h> /* time_t, struct tm, time, localtime, asctime */

const char* getCurrentTimeChar1() {
    time_t rawtime;
    time(&rawtime);

    return asctime(gmtime(&rawtime));
}

void getCurrentTimeChar2() {
    time_t rawtime;
    struct tm * timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);

    printf("The current date/time is: %s\n", asctime(timeinfo));
}

struct tm* getCurrentTimeChar3() {
    time_t rawtime;
    time(&rawtime);

    return gmtime(&rawtime);
}

struct tm* getCurrentTimeChar4() {
    time_t rawtime;
    time(&rawtime);

    return localtime(&rawtime);
}

int main() {
    printf("%s\n", getCurrentTimeChar1());
    getCurrentTimeChar2();
    printf("%s\n", asctime(getCurrentTimeChar3()));
    printf("%s\n", asctime(getCurrentTimeChar4()));
    return 0;
}
