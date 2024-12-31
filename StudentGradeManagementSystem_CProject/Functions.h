#pragma once
COORD coord = { 0, 0 };

student* students = NULL;
int studentCount = 0;

void gotoxy(int x, int y) {
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void SetColor(int ForgC) {
    WORD wColor;
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    if (GetConsoleScreenBufferInfo(hStdOut, &csbi)) {
        wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
        SetConsoleTextAttribute(hStdOut, wColor);
    }
}

int isInteger(char* str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] < '0' || str[i] > '9') {
            return 0;
        }
    }
    return 1;
}
int isValidName(char* str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (!((str[i] >= 'A' && str[i] <= 'Z') || (str[i] >= 'a' && str[i] <= 'z'))) {
            return 0; // Not a valid name
        }
    }
    return 1; // Valid name
}

void AddStudent() {
    student s;
    int ns;
    char idStr[20], nameStr[100];

    printf("Enter Student Details\n");
    printf("Student's ID: ");
    fgets(idStr, sizeof(idStr), stdin);
    idStr[strcspn(idStr, "\n")] = 0;

    if (!isInteger(idStr)) {
        printf("Invalid ID! Please enter a valid number for ID.\n");
        return;
    }

    s.ID = atoi(idStr);
    for (int i = 0; i < studentCount; i++) {
        if (students[i].ID == s.ID) {
            printf("Error: Student with ID %d already exists.\n", s.ID);
            return;
        }
    }
    printf("Student's Name: ");
    fgets(nameStr, sizeof(nameStr), stdin);
    nameStr[strcspn(nameStr, "\n")] = 0;

    if (!isValidName(nameStr)) {
        printf("Invalid Name! Please enter a valid name consisting of alphabets only.\n");
        return;
    }

    strcpy(s.Name, nameStr);
    do {
        printf("How many subjects does the student have? (Max 10): ");
        if (scanf("%d", &ns) != 1 || ns < 1) {
            printf("Invalid number of subjects! Please enter a value greater than 0.\n");
            while (getchar() != '\n'); // Clear invalid input in buffer
        }
    } while (ns < 1);

    s.subjects = (subject*)malloc(ns * sizeof(subject));
    if (s.subjects == NULL) {
        printf("Error: Unable to allocate memory for subjects.\n");
        return;
    }
    s.subjectCount = ns;

    for (int i = 0; i < ns; i++) {
        while (getchar() != '\n');
        printf("Enter Subject Number-%d\n", i + 1);
        printf("  Name: ");
        fgets(s.subjects[i].Name, sizeof(s.subjects[i].Name), stdin);
        s.subjects[i].Name[strcspn(s.subjects[i].Name, "\n")] = 0;

        if (!isValidName(s.subjects[i].Name)) {
            printf("Invalid subject name! Only alphabetic characters are allowed.\n");
            i--;
            continue;
        }
        printf("  Score: ");
        if (scanf("%f", &s.subjects[i].Score) != 1 || s.subjects[i].Score < 0 || s.subjects[i].Score > 100) {
            printf("Invalid score! Please enter a value between 0 and 100.\n");
            while (getchar() != '\n');
            i--;
        }
    }
    students = (student*)realloc(students, (studentCount + 1) * sizeof(student));
    if (students == NULL) {
        printf("Error: Unable to allocate memory for students.\n");
        free(s.subjects);
        return;
    }
    students[studentCount] = s;
    studentCount++;
    printf("\nStudent added successfully!\n");
}


void FreeMemory() {
    if (students == NULL) {
        return;
    }

    for (int i = 0; i < studentCount; i++) {
        if (students[i].subjects != NULL) {
            free(students[i].subjects);
            students[i].subjects = NULL;
        }
    }

    free(students);
    students = NULL; //free
    studentCount = 0;
}

void UpdateStudent() {
    if (studentCount == 0) {
        printf("No students to edit.\n");
        printf("\nPress Enter to return to the main menu...");
        _getch();
        system("cls");
        return;
    }

    int id, found = 0;
    printf("\nEnter the ID of the Student you want to edit: ");
    scanf("%d", &id);

    for (int i = 0; i < studentCount; i++) {
        if (students[i].ID == id) {
            found = 1;
            printf("\nStudent Details\nID: %d\nName: %s\n", students[i].ID, students[i].Name);

            for (int j = 0; j < students[i].subjectCount; j++) {
                printf("Subject %d: %s (Score: %.2f)\n", j + 1, students[i].subjects[j].Name, students[i].subjects[j].Score);
            }

            printf("\nWhat would you like to edit?\n");
            printf("1. Update subject scores\n");
            printf("2. Update student name\n");
            printf("3. Cancel\n");
            printf("Enter your choice: ");
            int choice;
            scanf("%d", &choice);

            switch (choice) {
            case 1: {
                for (int j = 0; j < students[i].subjectCount; j++) {
                    printf("\nEnter new score for %s: ", students[i].subjects[j].Name);
                    scanf("%f", &students[i].subjects[j].Score);

                    while (students[i].subjects[j].Score < 0 || students[i].subjects[j].Score > 100) {
                        printf("Invalid score. Enter a value between 0 and 100: ");
                        scanf("%f", &students[i].subjects[j].Score);
                    }
                }
                printf("Scores updated successfully.\n");
                break;
            }
            case 2: {
                printf("\nEnter new name for the student: ");
                scanf("%s", students[i].Name);
                printf("Name updated successfully.\n");
                break;
            }
            case 3:
                printf("Update canceled.\n");
                break;
            default:
                printf("Invalid choice.\n");
                break;
            }
            break;
        }
    }

    if (!found) {
        printf("Student with ID %d not found.\n", id);
    }

    printf("\nPress Enter to return to the main menu...");
    _getch();
    system("cls");
}

void CalculateGrades() {
    if (studentCount == 0) {
        printf("No students to calculate grades for.\n");
        printf("\nPress Enter to return to the main menu...");
        _getch();
        system("cls");
        return;
    }

    int choice;
    printf("Do you want to calculate grades for:\n");
    printf("1. All students\n");
    printf("2. A specific student\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    if (choice == 1) {
        printf("\nCalculating Grades for All Students...\n");

        for (int i = 0; i < studentCount; i++) {
            float total = 0;
            for (int j = 0; j < students[i].subjectCount; j++) {
                total += students[i].subjects[j].Score;
            }

            float average = total / students[i].subjectCount;

            if (average >= 90) {
                students[i].Grade = 'A';
            }
            else if (average >= 80) {
                students[i].Grade = 'B';
            }
            else if (average >= 70) {
                students[i].Grade = 'C';
            }
            else if (average >= 60) {
                students[i].Grade = 'D';
            }
            else {
                students[i].Grade = 'F';
            }

            printf("ID: %d, Name: %s, Average Score: %.2f, Grade: %c\n",
                students[i].ID, students[i].Name, average, students[i].Grade);
        }
        printf("\nGrades calculated successfully!\n");
    }
    else if (choice == 2) {
        int id, found = 0;
        printf("\nEnter the ID of the student: ");
        scanf("%d", &id);

        for (int i = 0; i < studentCount; i++) {
            if (students[i].ID == id) {
                found = 1;
                float total = 0;
                for (int j = 0; j < students[i].subjectCount; j++) {
                    total += students[i].subjects[j].Score;
                }

                float average = total / students[i].subjectCount;
                if (average >= 90) {
                    students[i].Grade = 'A';
                }
                else if (average >= 80) {
                    students[i].Grade = 'B';
                }
                else if (average >= 70) {
                    students[i].Grade = 'C';
                }
                else if (average >= 60) {
                    students[i].Grade = 'D';
                }
                else {
                    students[i].Grade = 'F';
                }

                printf("ID: %d, Name: %s, Average Score: %.2f, Grade: %c\n",
                    students[i].ID, students[i].Name, average, students[i].Grade);

                printf("\nGrade calculated successfully!\n");
                break;
            }
        }

        if (!found) {
            printf("\nStudent with ID %d not found.\n", id);
        }
    }
    else {
        printf("\nInvalid choice. Returning to main menu.\n");
    }

    printf("\nPress Enter to return to the main menu...");
    _getch();
    system("cls");
}

void DisplayResults() {
    if (studentCount == 0) {
        printf("No student records to display.\n");
        printf("\nPress Enter to return to the main menu...");
        _getch();
        system("cls");
        return;
    }

    int choice;
    printf("\nWhat would you like to display?\n");
    printf("1. All student records\n");
    printf("2. Student with the highest average score\n");
    printf("3. Average score of all students\n");
    printf("4. List of students who passed\n");
    printf("5. List of students who failed\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    // Clear the screen for clean output
    system("cls");

    switch (choice) {
    case 1: { // Display all student records
        printf("All Student Records:\n");
        printf("----------------------------------------------------------\n");
        printf("| ID    | Name                | Avg Score   | Grade     |\n");
        printf("----------------------------------------------------------\n");
        for (int i = 0; i < studentCount; i++) {
            float average = CalculateAverage(students[i]);
            printf("| %-6d| %-20s| %-11.2f| %-10c|\n",
                students[i].ID, students[i].Name, average, students[i].Grade);
        }
        printf("----------------------------------------------------------\n");
        break;
    }
    case 2: { // Display the top performer
        int topIndex = -1;
        float highestAverage = -1;

        for (int i = 0; i < studentCount; i++) {
            float average = CalculateAverage(students[i]);
            if (average > highestAverage) {
                highestAverage = average;
                topIndex = i;
            }
        }

        if (topIndex != -1) {
            printf("Top Performer:\n");
            printf("----------------------------------------------------------\n");
            printf("| ID    | Name                | Avg Score   | Grade     |\n");
            printf("----------------------------------------------------------\n");
            printf("| %-6d| %-20s| %-11.2f| %-10c|\n",
                students[topIndex].ID, students[topIndex].Name, highestAverage, students[topIndex].Grade);
            printf("----------------------------------------------------------\n");
        }
        else {
            printf("No data available to determine the top performer.\n");
        }
        break;
    }
    case 3: {
        float totalAverage = 0;

        for (int i = 0; i < studentCount; i++) {
            totalAverage += CalculateAverage(students[i]);
        }

        printf("Average Score of All Students: %.2f\n", totalAverage / studentCount);
        break;
    }
    case 4: {
        printf("Students Who Passed (Grade >= D):\n");
        printf("----------------------------------------------------------\n");
        printf("| ID    | Name                | Avg Score   | Grade     |\n");
        printf("----------------------------------------------------------\n");
        for (int i = 0; i < studentCount; i++) {
            if (students[i].Grade != 'F') {
                float average = CalculateAverage(students[i]);
                printf("| %-6d| %-20s| %-11.2f| %-10c|\n",
                    students[i].ID, students[i].Name, average, students[i].Grade);
            }
        }
        printf("----------------------------------------------------------\n");
        break;
    }
    case 5: { // Display students who failed
        printf("Students Who Failed (Grade = F):\n");
        printf("----------------------------------------------------------\n");
        printf("| ID    | Name                | Avg Score   | Grade     |\n");
        printf("----------------------------------------------------------\n");
        for (int i = 0; i < studentCount; i++) {
            if (students[i].Grade == 'F') {
                float average = CalculateAverage(students[i]);
                printf("| %-6d| %-20s| %-11.2f| %-10c|\n",
                    students[i].ID, students[i].Name, average, students[i].Grade);
            }
        }
        printf("----------------------------------------------------------\n");
        break;
    }
    default:
        printf("Invalid choice. Returning to the main menu.\n");
        break;
    }

    printf("\nPress Enter to return to the main menu...");
    _getch();
    system("cls");
}
float CalculateAverage(student s) {
    if (s.subjectCount == 0) return 0;

    float total = 0;
    for (int i = 0; i < s.subjectCount; i++) {
        total += s.subjects[i].Score;
    }
    return total / s.subjectCount;
}

void SaveData() {
    FILE* file = fopen("students.txt", "w");
    if (file == NULL) {
        printf("Error saving data.\n");
        return;
    }

    fwrite(&studentCount, sizeof(int), 1, file);

    for (int i = 0; i < studentCount; i++) {
        fwrite(&students[i].ID, sizeof(int), 1, file);
        fwrite(students[i].Name, sizeof(students[i].Name), 1, file);
        fwrite(&students[i].subjectCount, sizeof(int), 1, file);
        fwrite(&students[i].Grade, sizeof(char), 1, file);
        for (int j = 0; j < students[i].subjectCount; j++) {
            fwrite(students[i].subjects[j].Name, sizeof(students[i].subjects[j].Name), 1, file);
            fwrite(&students[i].subjects[j].Score, sizeof(float), 1, file);
        }
    }
    fclose(file);
    printf("Data saved to students.txt successfully!\n");
}
void LoadData() {
    FILE* file = fopen("students.txt", "r");
    if (file == NULL) {
        printf("No saved data found.\n");
        return;
    }
    fread(&studentCount, sizeof(int), 1, file);
    students = (student*)malloc(studentCount * sizeof(student));
    if (students == NULL) {
        printf("Error allocating memory for students.\n");
        fclose(file);
        return;
    }

    for (int i = 0; i < studentCount; i++) {
        fread(&students[i].ID, sizeof(int), 1, file);
        fread(students[i].Name, sizeof(students[i].Name), 1, file);
        fread(&students[i].subjectCount, sizeof(int), 1, file);
        fread(&students[i].Grade, sizeof(char), 1, file);

        students[i].subjects = (subject*)malloc(students[i].subjectCount * sizeof(subject));
        if (students[i].subjects == NULL) {
            printf("Error allocating memory for subjects.\n");
            fclose(file);
            return;
        }
        for (int j = 0; j < students[i].subjectCount; j++) {
            fread(students[i].subjects[j].Name, sizeof(students[i].subjects[j].Name), 1, file);
            fread(&students[i].subjects[j].Score, sizeof(float), 1, file);
        }
    }

    fclose(file);
    printf("Data loaded from students.txt successfully!\n");
}
void DisplayMenu(char menu[][30], int menuSize, int choice) {
    system("cls"); // Clear screen

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_BLUE | BACKGROUND_INTENSITY);

    for (int i = 0; i < menuSize; i++) {
        gotoxy(50, 10 + i); // Position the menu items
        if (i == choice) {
            SetColor(14); // Highlighted text in yellow
        }
        else {
            SetColor(7); // Default white text
        }
        printf("%s\n", menu[i]);
    }

    SetColor(7); // Reset to default white text
}