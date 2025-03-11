#include <stdio.h>
#include <windows.h> // For Sleep in Windows

int main()
{
    // বিপ সাউন্ড (ASCII Bell character)
    printf("\a");

    // কিছু সময়ের জন্য অপেক্ষা করতে
    Sleep(1000); // 1000 মিলিসেকেন্ড (1 সেকেন্ড) অপেক্ষা করবে

    // বিপ সাউন্ড আবার তৈরি করা
    printf("\a");

    return 0;
}
