# Library-Management-System-DSA-

# Library Management System in C using DSA

This is a mini-project for my Data Structures course. It's a simple command-line based library management system written entirely in C. The goal was to use different data structures to build a functional program that models a real-world scenario.

## Features

The program provides a main menu for the librarian with the following options:

  * **Add a New Book**: Adds a book with an ID, title, and author to the library.
  * **Search for a Book**: Finds a book using its unique ID and displays its status.
  * **Check Out a Book**: Marks a book as "unavailable".
  * **Return a Book**: Marks a book as "available" again.
  * **Waiting List**: If a book is checked out, other students can join a waiting list for it.
  * **List All Books**: Displays a list of all books in the library, sorted by their ID.
  * **Return Cart**: Manages a cart of returned books that need to be re-shelved.


## Data Structures Used

This project uses three main data structures to manage the library's operations:

### 1\. Binary Search Tree (BST)

  * **Purpose**: To store the main collection of all the books in the library.
  * **Why**: A BST is great for this because it keeps all the books sorted by their ID. This makes searching for a specific book very fast (average time complexity of $O(\log n)$), which is much better than checking every single book one by one.

### 2\. Stack

  * **Purpose**: To manage the librarian's return cart.
  * **Why**: When books are returned, they are placed in a pile. The last book returned is the first one the librarian grabs to put back on the shelf. This **Last-In, First-Out (LIFO)** behavior is exactly what a stack is designed for.

### 3\. Queue

  * **Purpose**: To handle the waiting list for popular books.
  * **Why**: When multiple people want a book that's already checked out, they should get it in the order they asked. A queue provides this **First-In, First-Out (FIFO)** logic, ensuring the system is fair.

-----

## How to Compile and Run

1.  You will need a C compiler like **GCC**.
2.  Save the code in a file named `LibraryManagement.c`.
3.  Open a terminal or command prompt and navigate to the folder where you saved the file.
4.  Compile the program using the following command:
    ```bash
    gcc LibraryManagement.c -o LibraryManagement
    ```
5.  Run the program with this command:
    ```bash
    ./LibraryManagement
    ```
