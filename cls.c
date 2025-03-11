#include <stdio.h>

void clear_screen()
{
    printf("\033[H\033[J"); // ANSI escape code (Linux, Mac, Windows with ANSI support)
}

int main()
{
    printf("Hello, World!\n");
    getchar(); // স্ক্রিন ক্লিয়ার হওয়ার আগে Enter চাপতে হবে
    clear_screen();
    printf("Screen Cleared!\n");

    return 0;
}
