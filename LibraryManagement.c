#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define PASSWORD "admin123"

struct Queue {
    char name[50];
    struct Queue *next;
};

struct Book {
    int id;
    char title[100];
    char author[100];
    int available;
    struct Queue *waitingList;
    struct Book *next;
};

struct Book *head = NULL;

void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void login() {
    char pass[20];

    printf("Enter Admin Password: ");
    scanf("%s", pass);

    if(strcmp(pass,PASSWORD)!=0) {
        setColor(12);
        printf("Access Denied!\n");
        exit(0);
    }

    setColor(10);
    printf("Login Successful!\n");
}

struct Book* createBook(int id,char title[],char author[]) {

    struct Book *newBook = (struct Book*)malloc(sizeof(struct Book));

    newBook->id=id;
    strcpy(newBook->title,title);
    strcpy(newBook->author,author);
    newBook->available=1;
    newBook->waitingList=NULL;
    newBook->next=NULL;

    return newBook;
}

void addBook() {

    int id;
    char title[100];
    char author[100];

    printf("Enter Book ID: ");
    scanf("%d",&id);

    printf("Enter Title: ");
    scanf(" %[^\n]",title);

    printf("Enter Author: ");
    scanf(" %[^\n]",author);

    struct Book *newBook=createBook(id,title,author);

    if(head==NULL)
        head=newBook;
    else {

        struct Book *temp=head;

        while(temp->next!=NULL)
            temp=temp->next;

        temp->next=newBook;
    }

    setColor(10);
    printf("Book added successfully!\n");
    setColor(7);
}

void displayBooks() {

    struct Book *temp=head;

    if(temp==NULL) {
        printf("No books available\n");
        return;
    }

    setColor(11);

    printf("\n%-5s %-30s %-25s %-10s\n",
           "ID","TITLE","AUTHOR","STATUS");

    printf("------------------------------------------------------------------\n");

    setColor(7);

    while(temp!=NULL) {

        printf("%-5d %-30s %-25s ",
               temp->id,temp->title,temp->author);

        if(temp->available) {
            setColor(10);
            printf("%-10s\n","Available");
        }
        else {
            setColor(12);
            printf("%-10s\n","Issued");
        }

        setColor(7);

        temp=temp->next;
    }
}

void searchBook() {

    int id;

    printf("Enter Book ID: ");
    scanf("%d",&id);

    struct Book *temp=head;

    while(temp!=NULL) {

        if(temp->id==id) {

            setColor(10);
            printf("Book Found!\n");

            printf("Title: %s\n",temp->title);
            printf("Author: %s\n",temp->author);

            if(temp->available)
                printf("Status: Available\n");
            else
                printf("Status: Issued\n");

            setColor(7);

            return;
        }

        temp=temp->next;
    }

    setColor(12);
    printf("Book not found!\n");
    setColor(7);
}

void issueBook() {

    int id;
    char student[50];

    printf("Enter Book ID: ");
    scanf("%d",&id);

    struct Book *temp=head;

    while(temp!=NULL) {

        if(temp->id==id) {

            if(temp->available) {

                temp->available=0;
                printf("Book issued successfully\n");
            }

            else {

                printf("Book already issued\n");

                printf("Enter your name for waiting list: ");
                scanf(" %[^\n]",student);

                struct Queue *newNode=(struct Queue*)malloc(sizeof(struct Queue));
                strcpy(newNode->name,student);
                newNode->next=temp->waitingList;

                temp->waitingList=newNode;

                printf("Added to waiting list\n");
            }

            return;
        }

        temp=temp->next;
    }

    printf("Book not found\n");
}

void returnBook() {

    int id;

    printf("Enter Book ID: ");
    scanf("%d",&id);

    struct Book *temp=head;

    while(temp!=NULL) {

        if(temp->id==id) {

            if(temp->waitingList!=NULL) {

                struct Queue *q=temp->waitingList;

                printf("Book issued to next student: %s\n",q->name);

                temp->waitingList=q->next;

                free(q);
            }

            else {

                temp->available=1;

                printf("Book returned successfully\n");
            }

            return;
        }

        temp=temp->next;
    }

    printf("Book not found\n");
}

/* MERGE SORT FOR LINKED LIST */

struct Book* merge(struct Book* a, struct Book* b) {

    if(a==NULL) return b;
    if(b==NULL) return a;

    struct Book* result=NULL;

    if(a->id <= b->id) {
        result=a;
        result->next=merge(a->next,b);
    }
    else {
        result=b;
        result->next=merge(a,b->next);
    }

    return result;
}

void split(struct Book* source, struct Book** front, struct Book** back) {

    struct Book* fast;
    struct Book* slow;

    slow=source;
    fast=source->next;

    while(fast!=NULL) {

        fast=fast->next;

        if(fast!=NULL) {
            slow=slow->next;
            fast=fast->next;
        }
    }

    *front=source;
    *back=slow->next;

    slow->next=NULL;
}

void mergeSort(struct Book** headRef) {

    struct Book* head=*headRef;
    struct Book* a;
    struct Book* b;

    if(head==NULL || head->next==NULL)
        return;

    split(head,&a,&b);

    mergeSort(&a);
    mergeSort(&b);

    *headRef=merge(a,b);
}

void loadBooksFromFile() {

    FILE *file;

    int id;
    char title[100];
    char author[100];

    file=fopen("books.txt","r");

    if(file==NULL) {
        printf("Dataset file not found\n");
        return;
    }

    while(fscanf(file,"%d,%[^,],%[^\n]\n",&id,title,author)!=EOF) {

        struct Book *newBook=createBook(id,title,author);

        if(head==NULL)
            head=newBook;
        else {

            struct Book *temp=head;

            while(temp->next!=NULL)
                temp=temp->next;

            temp->next=newBook;
        }
    }

    fclose(file);
}

int main() {

    login();

    loadBooksFromFile();

    int choice;

    while(1) {

        setColor(11);

        printf("\n=====================================\n");
        printf(" LIBRARY MANAGEMENT SYSTEM\n");
        printf(" Linked List + Queue + Merge Sort\n");
        printf("=====================================\n");

        setColor(10);

        printf("1 Add Book\n");
        printf("2 Display Books\n");
        printf("3 Search Book\n");
        printf("4 Issue Book\n");
        printf("5 Return Book\n");
        printf("6 Sort Books (Merge Sort)\n");
        printf("7 Exit\n");

        setColor(7);

        printf("Enter choice: ");
        scanf("%d",&choice);

        switch(choice) {

            case 1: addBook(); break;
            case 2: displayBooks(); break;
            case 3: searchBook(); break;
            case 4: issueBook(); break;
            case 5: returnBook(); break;
            case 6: mergeSort(&head); printf("Books sorted!\n"); break;
            case 7: exit(0);

            default: printf("Invalid choice\n");
        }
    }

    return 0;
}