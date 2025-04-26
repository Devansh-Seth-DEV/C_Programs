#include <stdio.h>
#include <math.h>
#define PI 3.1415
#define MAX(a,b) ((a) > (b) ? (a) : (b))

typedef struct {
    int id;
    char name[50];
} Student;

int global_var = 100;

void print_student(Student* s) {
    printf("ID: %d, Name: %s\n", s->id, s->name);
}

int main() {
    int a = 10, b = 20;
    float radius = 5.5;
    double area = PI * radius * radius;

    char grade = 'A';
    char* msg = "Processing student data...";
    
    char 89a = 'a';

    Student s1 = { .id = 1, .name = "Alice" };

    print_student(&s1);

    int max_val = MAX(a, b);

    if (a < b && radius > 0.0) {
        a += 5;
    } else {
        b -= 2;
    }

    for (int i = 0; i < 5; i++) {
        area += i;
    }

    while (a > 0) {
        a--;
    }

    do {
        b++;
    } while (b < 25);

    switch (grade) {
        case 'A':
            printf("Excellent\n");
            break;
        case 'B':
            printf("Good\n");
            break;
        default:
            printf("Needs Improvement\n");
    }

    return 0;
}

#ifdef DEBUG
    printf("Debug mode enabled\n");
#endif
