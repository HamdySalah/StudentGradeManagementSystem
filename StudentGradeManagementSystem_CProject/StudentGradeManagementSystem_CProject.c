#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include "Struct.h"
#include "Functions.h"

int main() {
    char menu[6][30] = {
        "Add New Student",
        "Update Student's Score",
        "Calculate Student's Grades",
        "Display Results",
        "Save Data",
        "Exit"
    };
    int choice = 0;
    int key;

    LoadData();


    while (1) {
        DisplayMenu(menu, 6, choice);

        key = _getch(); 
        if (key == 0 || key == 224) {
            key = _getch();
            if (key == 80) { // Down arrow
                choice++;
                if (choice >= 6) {
                    choice = 0;
                }
            }
            else if (key == 72) { // Up arrow
                choice--;
                if (choice < 0) {
                    choice = 5;
                }
            }
        }
        else if (key == 13) { // Enter key
            system("cls");
            switch (choice) {
            case 0:
                SetColor(15); // Green
                AddStudent();
                break;
            case 1:
                SetColor(15); // Yellow
                UpdateStudent();
                break;
            case 2:
                SetColor(15); // Cyan
                CalculateGrades();
                break;
            case 3:
                SetColor(15); // Magenta
                DisplayResults();
                break;
            case 4:
                SetColor(9); // Blue
                SaveData();
                break;
            case 5:
                SetColor(4); // Red
                printf("Do you want to save your data before exiting? (y/n): ");
                char saveChoice;
                scanf(" %c", &saveChoice);
                if (saveChoice == 'y' || saveChoice == 'Y') {
                    SaveData();
                }
                printf("Exiting the program...\n");
                FreeMemory(); // Free dynamically allocated memory
                exit(0);
            }
        }
    }

    return 0;
}
