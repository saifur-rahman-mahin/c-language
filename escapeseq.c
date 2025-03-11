#include <stdio.h>

int main()
{
    FILE *file = fopen("output.txt", "w"); // Open file for writing
    if (file == NULL)
    {
        printf("Failed to open file!\n");
        return 1;
    }

    // ANSI Escape sequences
    fprintf(file, "\033[31mThis text will be red.\033[0m\n");
    fprintf(file, "\033[32mThis text will be green.\033[0m\n");
    fprintf(file, "\033[1mThis text will be bold.\033[0m\n");
    fprintf(file, "\033[4mThis text will be underlined.\033[0m\n");
    fprintf(file, "\033[10;20HThis text will move to a specific position.\033[0m\n");

    // Clear screen in the terminal
    fprintf(file, "\033[2J");
    fprintf(file, "\033[33mThis text will be yellow.\033[0m\n");

    fclose(file); // Close the file
    printf("The file has been successfully created as 'output.txt'.\n");

    return 0;
}
