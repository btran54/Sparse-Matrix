/***
* Brian Tran
* btran54
* 2024 Winter CSE101 PA4
* Matrix.c
* Matrix C file containing the operations of Matrix functions.
***/ 

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "Matrix.h"
#include "List.h"

typedef struct EntryObj *Entry;

typedef struct EntryObj{
    double data;
    int cols;
} EntryObj;

typedef struct MatrixObj{
    List *array; 
    int size; 
    int NNZ; 
} MatrixObj;

// Constructors-Destructors =====================================================

Matrix newMatrix(int n) {
    Matrix M = malloc(sizeof(MatrixObj));
    M->array = calloc(n + 1, sizeof(List));
    M->size = n;
    M->NNZ = 0;

    for (int i = 1; i<= n; i++) {
        M->array[i] = newList();
    }
    return M;
}

Entry newEntry(double data, int cols) {
    Entry Obj = malloc(sizeof(EntryObj));
    Obj->cols = cols;
    Obj->data = data;
    return Obj;
}

void freeEntry(Entry* pE) {
    if ((*pE) != NULL && pE != NULL) {
        free((*pE));
        (*pE) = NULL;
    }
}

void freeMatrix(Matrix* pM) {
    if ((*pM) != NULL && pM != NULL) {
        makeZero(*pM);
        for (int i = 1; i <= size(*pM); i++) {
            freeList(&(*pM)->array[i]);
        }
        free((*pM)->array);
        free((*pM));
    }
}

// Access functions =============================================================

int size(Matrix M) {
    if (M == NULL) {
        printf("Matrix Error: Calling size() on NULL matrix reference.\n");
        exit(1);
    }

    return M->size;
}

int NNZ(Matrix M) {
    if (M == NULL) {
        printf("Matrix Error: Calling NNZ() on NULL matrix reference.\n");
        exit(1);
    }

    return M->NNZ;
}

int equals(Matrix A, Matrix B) {
    if (size(A) != size(B)) {
        return 0;
    }

    if (A->NNZ != B->NNZ) {
        return 0;
    }

    if (A == B) { 
        return 1;
    }

    for (int i = 1; i <= size(A); i++){

        if (length(A->array[i]) != length(B->array[i])) {
            return 0;
        }

        moveFront(A->array[i]);
        moveFront(B->array[i]);

        while (index(A->array[i]) >= 0 && index(B->array[i]) >= 0) {
            Entry C = (Entry) get(A->array[i]);
            Entry D = (Entry) get(B->array[i]);

            if (C->cols != D->cols || C->data != D->data) {
                return 0;
            }

            moveNext(A->array[i]);
            moveNext(B->array[i]);
        }
    }

    return 1;
}

// Manipulation procedures ======================================================

void makeZero(Matrix M) {
    for (int i = 1; i <= size(M); i++) {
        moveFront(M->array[i]);

        while (index(M->array[i]) >= 0) {

            Entry N = (Entry) get(M->array[i]);
            freeEntry(&N);
            moveNext(M->array[i]);
        }

        clear(M->array[i]);
    }

    M->NNZ = 0;
}

void changeEntry(Matrix M, int i, int j, double x) {
    if (i > size(M) || 1 > i || j > size(M) || 1 > i) {
        printf("Matrix Error: Calling changeEntry() on an entry outside the given range\n.");
        exit(1);
    }

    if (length(M->array[i]) <= 0 && x == 0) {
        return;
    }

    moveFront(M->array[i]);

    while (index(M->array[i]) >= 0) {
        Entry X = (Entry) get(M->array[i]); 
        if (X->cols == j && x == 0) {
            freeEntry(&X);
            delete(M->array[i]);
            M->NNZ --;
            return;
        }

        if (X->cols > j) {
            Entry E = newEntry(x, j);
            insertBefore(M->array[i], E);
            M->NNZ ++;
            return;
        }

        if (x != 0 && X->cols == j) {
            X->data = x;
            return;
        }

        moveNext(M->array[i]);
    }

    if (x != 0) {
    Entry E = newEntry(x, j);
    append(M->array[i], E);
    M->NNZ ++;
    return;
    }
}

Matrix copy(Matrix A) {
    if (A == NULL) {
        printf("Matrix Error: Calling copy() on NULL matrix reference.\n");
        exit(1);
    }

    Matrix B = newMatrix(size(A));

    for (int i = 1; i <= size(A); i++) {
        moveFront(A->array[i]);

        while (index(A->array[i]) >= 0) {
            Entry N = (Entry) get(A->array[i]);
            changeEntry(B, i, N->cols, N->data);
            moveNext(A->array[i]);
        }
    }

    return B;
}

Matrix transpose(Matrix A) {
    if (A == NULL) {
        printf("Matrix Error: Calling transpose() on NULL matrix reference.\n");
        exit(1);
    }
    
    Matrix B = newMatrix(size(A));

    for (int i = 1; i <= size(A); i++) {
        moveFront(A->array[i]);

        while (index(A->array[i]) >= 0) {
            Entry N = (Entry) get(A->array[i]);
            changeEntry(B, N->cols, i, N->data);
            moveNext(A->array[i]);
        }
    }

    return B;
    free(&B);
}

Matrix scalarMult(double x, Matrix A) {
    Matrix B = newMatrix(size(A));

    if (x == 0) {
        return B;
    }

    for (int i = 1; i <= size(A); i++) {
        moveFront(A->array[i]);

        while (index(A->array[i]) >= 0) {
            Entry N = (Entry) get(A->array[i]);
            changeEntry(B, i, N->cols, N->data * x);
            moveNext(A->array[i]);
        }
    }

    return B;
    free(&B);
}

Matrix sum(Matrix A, Matrix B) {
    if (A == B) {
        Matrix M = scalarMult(2, A);
        return M;
    }

    Matrix N = newMatrix(size(A));
    for (int i = 1; i <= size(A); i++) {
        moveFront(A->array[i]);
        moveFront(B->array[i]);

        while (index(A->array[i]) >= 0 && index(B->array[i]) >= 0) {
            Entry AE = (Entry) get(A->array[i]);
            Entry BE = (Entry) get(B->array[i]);

            if (AE->cols > BE->cols) {
                changeEntry(N, i, BE->cols, BE->data);
                moveNext(B->array[i]);
            }

            if (AE->cols < BE->cols) {
                changeEntry(N, i, AE->cols, AE->data);
                moveNext(A->array[i]);
            }

            if (AE->cols == BE->cols) {
                double temp = AE->data + BE->data;
                changeEntry(N, i, AE->cols, temp);
                moveNext(A->array[i]);
                moveNext(B->array[i]);
            }
        }

        while (index(A->array[i]) >= 0 && index(B->array[i]) < 0) {
            Entry AE = (Entry) get(A->array[i]);
            changeEntry(N, i, AE->cols, AE->data);
            moveNext(A->array[i]);
        }

        while (index(A->array[i]) < 0 && index(B->array[i]) >= 0) {
            Entry BE = (Entry) get(B->array[i]);
            changeEntry(N, i, BE->cols, BE->data);
            moveNext(B->array[i]);
        }
    }

    return N;
}

Matrix diff(Matrix A, Matrix B) {
    if (size(A) != size(B)) {
        fprintf(stderr, "Matrix Error: Calling diff() when size is not the same.\n");
        exit(1);
    }

    Matrix N = newMatrix(size(A));

    for (int i = 1; i <= size(A); i++) {
        moveFront(A->array[i]);
        moveFront(B->array[i]);

        while (index(A->array[i]) >= 0 && index(B->array[i]) >= 0) {
            Entry AE = (Entry) get(A->array[i]);
            Entry BE = (Entry) get(B->array[i]);

            if (AE->cols > BE->cols) {
                changeEntry(N, i, BE->cols, -BE->data);
                moveNext(B->array[i]);
            }

            else if (AE->cols < BE->cols) {
                changeEntry(N, i, AE->cols, AE->data);
                moveNext(A->array[i]);
            } 

            else {
                double temp = AE->data - BE->data;

                if (temp != 0) {
                    changeEntry(N, i, AE->cols, temp);
                }

                moveNext(A->array[i]);
                moveNext(B->array[i]);
            }
        }

        while (index(A->array[i]) >= 0 && index(B->array[i]) < 0) {
            Entry AE = (Entry) get(A->array[i]);
            changeEntry(N, i, AE->cols, AE->data);
            moveNext(A->array[i]);
        }

        while (index(A->array[i]) < 0 && index(B->array[i]) >= 0) {
            Entry BE = (Entry) get(B->array[i]);
            changeEntry(N, i, BE->cols, -BE->data);
            moveNext(B->array[i]);
        }
    }

    return N;
    free(&N);
}

double dotProduct(List A, List B) {
    moveFront(A);
    moveFront(B);
    double value = 0;

    while (index(A) >= 0 && index(B) >= 0) {
        Entry AE = (Entry) get(A);
        Entry BE = (Entry) get(B);

        if (AE->cols > BE->cols) {
            moveNext(B);
            continue;
        }

        if (AE->cols < BE->cols) {
            moveNext(A);
            continue;
        }

        if (AE->cols == BE->cols) {
            value += AE->data * BE->data;
            moveNext(A);
            moveNext(B);
        }
    }

    return value;
}

Matrix product(Matrix A, Matrix B) {
    if (size(A) != size(B)) {
        printf("Matrix Error: calling product() when size is not the same\n.");
        exit(1);
    }

    double temp = 0;
    Matrix N = newMatrix(size(A));
    Matrix BT = transpose(B);

    for (int i = 1; i <= size(A); i++) {

        if (length(A->array[i]) > 0) {

            for (int j = 1; j <= size(BT); j++) {

                if (length(BT->array[j]) > 0) {
                    temp = dotProduct(A->array[i], BT->array[j]);

                    if (temp != 0) {
                        changeEntry(N, i, j, temp);
                    }
                }
            }
        }
    }

    return N;
    free(&N);
}

void printMatrix(FILE* out, Matrix M) {
    for (int i = 1; i <= size(M); i++) {
        moveFront(M->array[i]);

        if(length(M->array[i]) <= 0) {
            continue;
        }

        fprintf(out, "%d: ", i);

        while (index(M->array[i]) >= 0) {
            Entry N = get(M->array[i]);
            fprintf(out, "(%d, %.1f) ", N->cols, N->data);
            moveNext(M->array[i]);
        }

        fprintf(out, "\n");
    }
}