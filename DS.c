#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define structures
struct history {
    char student_name[30];
    int borrow_count;
    struct history *next;
};

struct book {
    char name[30];
    char author[30];
    int id;
    int borrow_count;
    int is_issued; // 0 if available, 1 if issued
    struct history *history; // Linked list for book history
    struct book *next;
};

struct student {
    char name[30];
    char email[20];
    char book[30];
    char author[30];
    int id;
    struct student *next;
};

// Global variables
struct book *start_lib = NULL;
struct student *start = NULL;

// Function prototypes
struct book *initialize_lib(struct book *);
struct student *book_issue(struct student *);
struct student *book_return(struct student *);
void display_students(struct student *);
void display_books();
void display_book_history(int id);
void greetings();
void main_menu();

int main() {
    start_lib = initialize_lib(start_lib);
    greetings();
    main_menu();
    return 0;
}

void greetings() {
    printf("\n\n");
    printf("\t\t\t     ****************************************\n");
    printf("\t\t\t     *      WELCOME TO STUDENT LIBRARY      *\n");
    printf("\t\t\t     ****************************************\n");
    printf("\n\t\t\t             Press any key to continue: ");
    getchar();
}

void main_menu() {
    int choice, book_id;
    do {
        printf("\n\n");
        printf("\n\t\t\t*\n");
        printf("\n\t\t\t\t      MAIN MENU: ");
        printf("\n\t\t\t\t     1. ISSUE A BOOK ");
        printf("\n\t\t\t\t     2. RETURN A BOOK ");
        printf("\n\t\t\t\t     3. DISPLAY STUDENT DETAILS ");
        printf("\n\t\t\t\t     4. DISPLAY AVAILABLE BOOKS ");
        printf("\n\t\t\t\t     5. DISPLAY BOOK HISTORY ");
        printf("\n\t\t\t\t     6. EXIT\n ");
        printf("\n\t\t\t*\n");
        printf("\n\t\t\t\t      Enter your choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                start = book_issue(start);
                break;
            case 2:
                start = book_return(start);
                break;
            case 3:
                display_students(start);
                break;
            case 4:
                display_books();
                break;
            case 5:
                printf("\n\t Enter the Book ID: ");
                scanf("%d", &book_id);
                display_book_history(book_id);
                printf("\n\n\t Press any key to go to the main menu: ");
                getchar();
                getchar();
                break;
            case 6:
                exit(1);
            default:
                printf("\n\t\t\t\t      ...Invalid Option!...\n");
        }
    } while (choice != 6);
}

struct book *initialize_lib(struct book *start) {
    struct book *ptr, *new_book;

    char books[][30] = {"The Kite Runner", "To Kill A Mockingbird", "The Alchemist", "Pride And Prejudice", "A Tale Of Two Cities"};
    char authors[][30] = {"Khaled Hosseini", "Harper Lee", "Paulo Coelho", "Jane Austen", "Charles Dickens"};
    int ids[] = {101, 102, 103, 104, 105};

    for (int i = 0; i < 5; i++) {
        new_book = (struct book *)malloc(sizeof(struct book));
        strcpy(new_book->name, books[i]);
        strcpy(new_book->author, authors[i]);
        new_book->id = ids[i];
        new_book->borrow_count = 0;
        new_book->is_issued = 0; // Initially all books are available
        new_book->history = NULL;
        new_book->next = NULL;

        if (start == NULL) {
            start = new_book;
            ptr = new_book;
        } else {
            ptr->next = new_book;
            ptr = new_book;
        }
    }

    return start;
}

struct student *book_issue(struct student *start) {
    struct book *ptr = start_lib;
    struct student *new_student;
    struct history *new_history, *hist_ptr;
    int id, flag = 0;

    printf("\n\t*************** Books in Library: ****************\n");
    while (ptr != NULL) {
        printf("\n\t Book ID: %d, Title: %s, Author: %s, Status: %s",
               ptr->id, ptr->name, ptr->author, ptr->is_issued ? "Issued" : "Available");
        ptr = ptr->next;
    }

    printf("\n\n\t Enter the Book ID to issue: ");
    scanf("%d", &id);

    ptr = start_lib;
    while (ptr != NULL) {
        if (ptr->id == id) {
            flag = 1;
            if (ptr->is_issued) {
                printf("\n\t This book is already issued! Please return it before issuing again.\n");
                return start;
            }
            break;
        }
        ptr = ptr->next;
    }

    if (flag) {
        new_student = (struct student *)malloc(sizeof(struct student));
        printf("\n\t Enter Student Name: ");
        scanf("%s", new_student->name);
        printf("\n\t Enter Student Email: ");
        scanf("%s", new_student->email);
        strcpy(new_student->book, ptr->name);
        strcpy(new_student->author, ptr->author);
        new_student->id = ptr->id;
        new_student->next = start;
        start = new_student;

        ptr->is_issued = 1; // Mark the book as issued
        ptr->borrow_count++;

        // Update book history
        new_history = (struct history *)malloc(sizeof(struct history));
        strcpy(new_history->student_name, new_student->name);
        new_history->borrow_count = 1;
        new_history->next = NULL;

        if (ptr->history == NULL) {
            ptr->history = new_history;
        } else {
            hist_ptr = ptr->history;
            while (hist_ptr != NULL) {
                if (strcmp(hist_ptr->student_name, new_student->name) == 0) {
                    hist_ptr->borrow_count++;
                    free(new_history);
                    break;
                }
                if (hist_ptr->next == NULL) {
                    hist_ptr->next = new_history;
                    break;
                }
                hist_ptr = hist_ptr->next;
            }
        }

        printf("\n\t Book issued successfully!\n");
    } else {
        printf("\n\t Invalid Book ID!\n");
    }

    return start;
}

struct student *book_return(struct student *start) {
    struct student *ptr = start, *prev = NULL;
    int id, found = 0;

    printf("\n\t Enter the Book ID to return: ");
    scanf("%d", &id);

    while (ptr != NULL) {
        if (ptr->id == id) {
            found = 1;
            break;
        }
        prev = ptr;
        ptr = ptr->next;
    }

    if (found) {
        struct book *book_ptr = start_lib;
        while (book_ptr != NULL) {
            if (book_ptr->id == id) {
                book_ptr->is_issued = 0; // Mark the book as available
                break;
            }
            book_ptr = book_ptr->next;
        }

        if (prev == NULL) {
            start = start->next;
        } else {
            prev->next = ptr->next;
        }
        free(ptr);

        printf("\n\t Book returned successfully!\n");
    } else {
        printf("\n\t No matching book found for return!\n");
    }

    return start;
}

void display_students(struct student *start) {
    struct student *ptr = start;

    if (ptr == NULL) {
        printf("\n\t No students found!\n");
        return;
    }

    while (ptr != NULL) {
        printf("\n\t Student Name: %s, Email: %s, Book: %s", ptr->name, ptr->email, ptr->book);
        ptr = ptr->next;
    }

    printf("\n");
}

void display_books() {
    struct book *ptr = start_lib;

    printf("\n\t*************** Library Books: ****************\n");
    while (ptr != NULL) {
        printf("\n\t Book ID: %d, Title: %s, Author: %s, Status: %s",
               ptr->id, ptr->name, ptr->author, ptr->is_issued ? "Issued" : "Available");
        ptr = ptr->next;
    }
    printf("\n");
}

void display_book_history(int id) {
    struct book *ptr = start_lib;

    while (ptr != NULL) {
        if (ptr->id == id) {
            printf("\n\t Book Title: %s, Total Borrows: %d", ptr->name, ptr->borrow_count);
        }
    }
}