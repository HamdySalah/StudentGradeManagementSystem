#pragma once
struct Subject {
    char Name[20];
    float Score;
}; typedef struct Subject subject;

struct Student {
    int ID;
    char Name[20];
    subject* subjects;
    int subjectCount;
    char Grade;
}; typedef struct Student student;
