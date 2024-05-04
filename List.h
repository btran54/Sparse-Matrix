/***
* Brian Tran
* btran54
* 2024 Winter CSE101 PA4
* List.h
* List file containing the forward declarations of functions.
***/ 

#ifndef LIST_H
#define LIST_H

#include <stdbool.h>
#include <stdio.h>

typedef struct ListObj* List;
typedef int ListElement;
typedef void* ptr;

// Constructors-Destructors ---------------------------------------------------

// Creates and returns a new empty List.
List newList(void); 

// Frees all heap memory associated with *pL, and sets
// *pL to NULL.
void freeList(List* pL); 

// Access functions -----------------------------------------------------------

// Returns the number of elements in L.
int length(List L); 

// Returns index of cursor element if defined, -1 otherwise.
int index(List L); 

// Returns front element of L. Pre: length()>0
ptr front(List L); 

// Returns back element of L. Pre: length()>0
ptr back(List L); 

// Returns cursor element of L. Pre: length()>0, index()>=0
ptr get(List L); 

// Manipulation procedures ----------------------------------------------------

// Resets L to its original empty state.
void clear(List L); 

// Overwrites the cursor element’s data with x.
// Pre: length()>0, index()>=0
void set(List L, ptr x); 

// If L is non-empty, sets cursor under the front element,
// otherwise does nothing.
void moveFront(List L); 

// If L is non-empty, sets cursor under the back element,
// otherwise does nothing.
void moveBack(List L); 

// If cursor is defined and not at front, move cursor one
// step toward the front of L; if cursor is defined and at
// front, cursor becomes undefined; if cursor is undefined
// do nothing
void movePrev(List L); 

// If cursor is defined and not at back, move cursor one
// step toward the back of L; if cursor is defined and at
// back, cursor becomes undefined; if cursor is undefined
// do nothing
void moveNext(List L); 

// Insert new element into L. If L is non-empty,
// insertion takes place before front element.
void prepend(List L, ptr x);

// Insert new element into L. If L is non-empty,
// insertion takes place after back element.
void append(List L, ptr x); 

// Insert new element before cursor.
// Pre: length()>0, index()>=0
void insertBefore(List L, ptr x); 

// Insert new element after cursor.
// Pre: length()>0, index()>=0
void insertAfter(List L, ptr x); 

// Delete the front element. Pre: length()>0
void deleteFront(List L); 

// Delete the back element. Pre: length()>0
void deleteBack(List L); 

// Delete cursor element, making cursor undefined.
void delete(List L); 

#endif