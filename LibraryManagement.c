#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>



// A node for the waiting list Queue
typedef struct QueueNode {
    char studentName[100]; // name of the person waiting
    struct QueueNode* next;
} QueueNode;

// Waiting list made with Queue
typedef struct Queue {
    QueueNode* front; // front of the line
    QueueNode* rear;  // back of the line
} Queue;

// Single book structure
typedef struct Book {
    int id;
    char title[100];
    char author[100];
    bool isAvailable;
    Queue waitingList; // Each book has its own waiting list
} Book;

// This is a node for the main BST
typedef struct TreeNode {
    Book book; // the actual book data
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;

// This is a node for the return cart Stack
typedef struct StackNode {
    int bookId;
    struct StackNode* next;
} StackNode;




// This function adds a person to the end of the waiting list
void enqueue(Queue* q, const char* name) {
    QueueNode* newNode = (QueueNode*)malloc(sizeof(QueueNode));
    strcpy(newNode->studentName, name);
    newNode->next = NULL;

    // if the list is empty, the new person is the first and last
    if (q->rear == NULL) {
        q->front = q->rear = newNode;
        return;
    }

    // add the new person to the very end
    q->rear->next = newNode;
    q->rear = newNode;
}

// This function removes the person from the front of the waiting list
char* dequeue(Queue* q) {
    if (q->front == NULL) {
        return NULL; // nobody is in the line
    }

    QueueNode* temp = q->front;
    char* personName = strdup(temp->studentName); // copy the name

    // move the front of the line to the next person
    q->front = q->front->next;

    // if the line is now empty, update the rear too
    if (q->front == NULL) {
        q->rear = NULL;
    }

    free(temp); // free the old front node
    return personName;
}

// This helper function creates a new node for the tree
TreeNode* createNode(Book newBook) {
    // making space for the new node in memory
    TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode));
    newNode->book = newBook;
    
    // a new book needs an empty waiting list
    newNode->book.waitingList.front = NULL;
    newNode->book.waitingList.rear = NULL;

    // the new node doesnt have children yet, so they are NULL
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

// This function adds a new book to our BST
TreeNode* insertBook(TreeNode* root, Book newBook) {
    // if the tree is empty, this new node is the first one!
    if (root == NULL) {
        return createNode(newBook);
    }

    // if the new book's ID is smaller, it goes in the left part
    if (newBook.id < root->book.id) {
        root->left = insertBook(root->left, newBook);
    } 
    // if the new book's ID is bigger, it goes in the right part
    else if (newBook.id > root->book.id) {
        root->right = insertBook(root->right, newBook);
    }

    return root;
}

// This function finds a book in the tree using its ID
TreeNode* searchBook(TreeNode* root, int bookId) {
    // if tree is empty or we found the book
    if (root == NULL || root->book.id == bookId) {
        return root;
    }

    // if the id we want is smaller, search in the left part
    if (bookId < root->book.id) {
        return searchBook(root->left, bookId);
    } 
    // otherwise, search in the right part
    else {
        return searchBook(root->right, bookId);
    }
}

// This prints all the books sorted by ID
void printInorder(TreeNode* root) {
    if (root == NULL) {
        return; // stop if the node is empty
    }
    // 1. Go all the way to the left
    printInorder(root->left);
    // 2. Print the current node's info
    printf("ID: %-5d | Title: %-30s | Author: %-20s | Available: %s\n",
           root->book.id, root->book.title, root->book.author,
           root->book.isAvailable ? "Yes" : "No");
    // 3. Go all the way to the right
    printInorder(root->right);
}




// PUSH: puts a book ID on top of the return cart stack
void pushToCart(StackNode** top, int bookId) {
    StackNode* newNode = (StackNode*)malloc(sizeof(StackNode));
    newNode->bookId = bookId;
    newNode->next = *top; // new node points to the old top
    *top = newNode;      // the top is now the new node
    printf("OK. Book %d put in the return cart.\n", bookId);
}

// POP: takes the top book ID from the cart to be re-shelved
int popFromCart(StackNode** top) {
    if (*top == NULL) {
        printf("Return cart is empty!\n");
        return -1; // -1 means error
    }
    StackNode* temp = *top;
    int bookIdToReshelve = temp->bookId;
    *top = (*top)->next; // move top to the next item
    free(temp);
    return bookIdToReshelve;
}



void checkoutBook(TreeNode* root, int bookId) {
    TreeNode* bookNode = searchBook(root, bookId);

    if (bookNode == NULL) {
        printf("Error: Can't find any book with ID %d.\n", bookId);
        return;
    }

    // Check if the book is on the shelf
    if (bookNode->book.isAvailable) {
        bookNode->book.isAvailable = false; // mark it as checked out
        printf("Success! Book '%s' is now checked out.\n", bookNode->book.title);
    } else {
        // If it's already checked out, ask about waiting list
        printf("Sorry, '%s' is already checked out.\n", bookNode->book.title);
        char choice;
        printf("Want to get on the waiting list? (y/n): ");
        scanf(" %c", &choice);

        if (choice == 'y' || choice == 'Y') {
            char name[100];
            printf("OK. What's your name?: ");
            scanf(" %[^\n]s", name);
            enqueue(&bookNode->book.waitingList, name);
            printf("Great, %s. You are on the waiting list!\n", name);
        }
    }
}

void returnBook(TreeNode* root, StackNode** cartTop, int bookId) {
    TreeNode* bookNode = searchBook(root, bookId);

    if (bookNode == NULL) {
        printf("Error: This book (ID %d) doesn't belong to our library.\n", bookId);
        return;
    }

    // You can't return a book that is already here
    if (bookNode->book.isAvailable) {
        printf("Hold on, this book is already marked as available.\n", bookId);
        return;
    }

    // Checks if someone is waiting for this book
    if (bookNode->book.waitingList.front != NULL) {
        char* nextPerson = dequeue(&bookNode->book.waitingList);
        printf("\n!!! SOMEONE IS WAITING FOR THIS BOOK !!!\n");
        printf("Please hold book '%s' for: %s\n", bookNode->book.title, nextPerson);
        pushToCart(cartTop, bookId); // still add to cart for librarian to handle
        free(nextPerson);
    } else {
        // If nobody is waiting, just make it available for everyone
        bookNode->book.isAvailable = true;
        printf("Thank you! Book '%s' is now marked as AVAILABLE.\n", bookNode->book.title);
        pushToCart(cartTop, bookId);
    }
}

//Main Function
int main() {
    TreeNode* root = NULL; // Our main BST starts empty
    StackNode* cartTop = NULL; // Our return cart starts empty
    int choice;

    printf("--- Welcome to the Library Program! ---\n");

    while (1) {
        printf("\n--- Main Menu ---\n");
        printf("1. Add New Book\n");
        printf("2. Find a Book\n");
        printf("3. Checkout a Book\n");
        printf("4. Return a Book\n");
        printf("5. Show All Books\n");
        printf("6. Reshelve a Book from Cart\n");
        printf("7. Exit\n");
        printf("Your Choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: {
                Book newBook;
                printf("Enter ID: ");
                scanf("%d", &newBook.id);
                printf("Enter Title: ");
                scanf(" %[^\n]s", newBook.title);
                printf("Enter Author: ");
                scanf(" %[^\n]s", newBook.author);
                newBook.isAvailable = true;
                root = insertBook(root, newBook);
                printf("Added '%s' to the library!\n", newBook.title);
                break;
            }
            case 2: {
                int id;
                printf("Enter the Book ID you want to find: ");
                scanf("%d", &id);
                TreeNode* found = searchBook(root, id);
                if (found) {
                     printf("FOUND: ID %d, Title: %s, Author: %s, Available: %s\n",
                           found->book.id, found->book.title, found->book.author,
                           found->book.isAvailable ? "Yes" : "No");
                } else {
                    printf("Sorry, no book found with ID %d.\n", id);
                }
                break;
            }
            case 3: {
                int id;
                printf("Enter the ID of the book to checkout: ");
                scanf("%d", &id);
                checkoutBook(root, id);
                break;
            }
            case 4: {
                int id;
                printf("Enter the ID of the book being returned: ");
                scanf("%d", &id);
                returnBook(root, &cartTop, id);
                break;
            }
            case 5:
                if (root == NULL) {
                    printf("The library has no books yet.\n");
                } else {
                    printf("\n--- All Books in the Library ---\n");
                    printInorder(root);
                    printf("----------------------------------\n");
                }
                break;
            case 6: {
                int id = popFromCart(&cartTop);
                if (id != -1) {
                    printf("Librarian, please put the book with ID %d back on the shelf.\n", id);
                }
                break;
            }
            case 7:
                printf("Closing the program. Bye!\n");
                return 0; // this exits the program
            default:
                printf("That's not a valid choice. Please enter a number from 1 to 7.\n");
        }
    }
    return 0;
}