#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

#define limit 255

int getWords(char fileName[], char data[limit][limit]);
void convertMatrix(char matrix[limit][limit], int numberOfRows);
void initUsedWords(bool usedWords[], int numberOfWords);
void initUsedMatrixSlots(bool usedMatrixSlots[limit][limit], int numberOfRows, char matrix[limit][limit]);
bool solveCrossword(char data[limit][limit], char matrix[limit][limit], int numberOfWords, int numberOfRows, bool usedWords[], bool usedMatrixSlots[limit][limit], char tempMatrix[limit][limit]);
void fillHorizontal(char data[limit][limit], char matrix[limit][limit], int numberOfWords, int numberOfRows, bool usedWords[], bool usedMatrixSlots[limit][limit]);
void fillVertical(char data[limit][limit], char matrix[limit][limit], int numberOfWords, int numberOfRows, bool usedWords[], bool usedMatrixSlots[limit][limit]);
void initArray(int array[], int length);
int findPosition(int array[], int length, int wordLength);
int findPositionOfVertical(int j, int array[], int length, int wordLength, char *word, char matrix[limit][limit]);
void fillHorizontalMatrix(int i, int j, int position, char *word, char matrix[limit][limit], bool usedWords[], bool usedMatrixSlots[limit][limit], int wordLength);
void fillVerticalMatrix(int i, int j, int position, char *word, char matrix[limit][limit], bool usedWords[], bool usedMatrixSlots[limit][limit], int wordLength);
void flipMatrix(char matrix[limit][limit], int numberOfRows, bool usedMatrixSlots[limit][limit]);
bool checkIfSolved(char matrix[limit][limit], int numberOfRows);
void shiftWords(char data[limit][limit], int numberOfWords);
void initMatrix(char matrix[limit][limit], char tempMatrix[limit][limit], int numberOfRows);
void printMatrix(char matrix[limit][limit], int numberOfRows);

int solvingTries = 0;

int main()
{
    char data[limit][limit];
    char matrix[limit][limit];
    int numberOfWords = getWords("data.txt", data);
    int numberOfRows = getWords("matrix.txt", matrix);

    if (numberOfWords == -1 || numberOfRows == -1) {
        return 0;
    }

    bool usedWords[limit];
    bool usedMatrixSlots[limit][limit];

    convertMatrix(matrix, numberOfRows);

    char tempMatrix[limit][limit];
    for (int i = 0; i < numberOfRows; ++i) {
        for (int j = 0; j < strlen(matrix[i]); ++j) {
            tempMatrix[i][j] = matrix[i][j];
        }
    }

    initUsedWords(usedWords, numberOfWords);
    initUsedMatrixSlots(usedMatrixSlots, numberOfRows, matrix);

    bool possibleToSolve = solveCrossword(data, matrix, numberOfWords, numberOfRows, usedWords, usedMatrixSlots, tempMatrix);

    if (possibleToSolve == false) {
        printf("Not possible to solve\n");
    }
    else {
        printMatrix(matrix, numberOfRows);
    }
}

int getWords(char fileName[], char data[limit][limit])
{
    FILE *file;
    file = fopen(fileName, "r");

    if (file == NULL) {
        printf("File was not found\n");
        return -1;
    }

    int wordLength = 0;
    int numberOfWords = 0;
    char *word = (char *)malloc(wordLength);

    while (!feof(file)) {
        char temp;
        wordLength++;
        temp = fgetc(file);
        word = realloc(word, sizeof(char) * wordLength);
        if (temp != '\n' && !feof(file)) {
            *(word + wordLength - 1) = temp;
        }
        else {
            *(word + wordLength - 1) = '\0';
            wordLength = 0;
            numberOfWords++;
            strcpy(data[numberOfWords - 1], word);
        }
    }

    return numberOfWords;
}

void convertMatrix(char matrix[limit][limit], int numberOfRows)
{
    for (int i = 0; i < numberOfRows; ++i) {
        for (int j = 0; j < strlen(matrix[i]); ++j) {
            if (matrix[i][j] == '0') {
                matrix[i][j] = 'O';
            }
            else {
                matrix[i][j] = 'X';
            }
        }
    }
}

void initUsedWords(bool usedWords[], int numberOfWords)
{
    for (int i = 0; i < numberOfWords; ++i) {
        usedWords[i] = 0;
    }
}

void initUsedMatrixSlots(bool usedMatrixSlots[limit][limit], int numberOfRows, char matrix[limit][limit])
{
    for (int i = 0; i < numberOfRows; ++i) {
        for (int j = 0; j < strlen(matrix[i]); ++j) {
            if (matrix[i][j] == 'X') {
                usedMatrixSlots[i][j] = 1;
            }
            else {
                usedMatrixSlots[i][j] = 0;
            }
        }
    }
}

bool solveCrossword(char data[limit][limit], char matrix[limit][limit], int numberOfWords, int numberOfRows, bool usedWords[], bool usedMatrixSlots[limit][limit], char tempMatrix[limit][limit])
{
    fillHorizontal(data, matrix, numberOfWords, numberOfRows, usedWords, usedMatrixSlots);
    flipMatrix(matrix, numberOfRows, usedMatrixSlots);
    fillVertical(data, matrix, numberOfWords, numberOfRows, usedWords, usedMatrixSlots);

    if (checkIfSolved(matrix, numberOfRows) == true) {
        return true;
    }
    else {
        if (solvingTries == numberOfWords) {
            return false;
        }
        else {
            solvingTries++;
            shiftWords(data, numberOfWords);
            initUsedWords(usedWords, numberOfWords);
            initMatrix(matrix, tempMatrix, numberOfRows);
            return solveCrossword(data, matrix, numberOfWords, numberOfRows, usedWords, usedMatrixSlots, tempMatrix);
        }
    }
}

void fillHorizontal(char data[limit][limit], char matrix[limit][limit], int numberOfWords, int numberOfRows, bool usedWords[], bool usedMatrixSlots[limit][limit])
{
    for (int i = 0; i < numberOfWords; ++i) {
        int wordLength = strlen(data[i]);
        for (int j = 0; j < numberOfRows; ++j) {
            int length[limit];
            initArray(length, strlen(matrix[j]));

            for (int h = 0; h < strlen(matrix[j]); ++h) {
                if (h == 0 && usedMatrixSlots[j][h] == 0) {
                    length[0] = 1;
                }
                else if (usedMatrixSlots[j][h] == 0) {
                    length[h] = length[h - 1] + 1;
                }
                else {
                    length[h];
                }
            }
            int position = findPosition(length, strlen(matrix[j]), wordLength);
            if (position != -1 && usedWords[i] == 0) {
                fillHorizontalMatrix(i, j, position, data[i], matrix, usedWords, usedMatrixSlots, wordLength);
            }
        }
    }
}

void fillVertical(char data[limit][limit], char matrix[limit][limit], int numberOfWords, int numberOfRows, bool usedWords[], bool usedMatrixSlots[limit][limit])
{
    for (int i = 0; i < numberOfWords; ++i) {
        int wordLength = strlen(data[i]);
        for (int j = 0; j < numberOfRows; ++j) {
            int length[limit];
            initArray(length, strlen(matrix[j]));

            for (int h = 0; h < strlen(matrix[j]); ++h) {
                if (h == 0 && usedMatrixSlots[j][h] == 0) {
                    length[0] = 1;
                }
                else if (usedMatrixSlots[j][h] == 0) {
                    length[h] = length[h - 1] + 1;
                }
                else {
                    length[h];
                }
            }
            int position = findPositionOfVertical(j, length, strlen(matrix[j]), wordLength, data[i], matrix);
            if (position != -1 && usedWords[i] == 0) {
                fillHorizontalMatrix(i, j, position, data[i], matrix, usedWords, usedMatrixSlots, wordLength);
            }
        }
    }
}

void initArray(int array[], int length)
{
    for (int i = 0; i < length; ++i) {
        array[i] = 0;
    }
}

int findPosition(int array[], int length, int wordLength)
{
    for (int i = 0; i < length; ++i) {
        if (array[i] == wordLength && i == length - 1) {
            return i;
        }
        else if (array[i] == wordLength && i < length - 1 && array[i + 1] == 0) {
            return i;
        }
    }
    return -1;
}

int findPositionOfVertical(int j, int array[], int length, int wordLength, char *word, char matrix[limit][limit])
{
    int position = findPosition(array, length, wordLength);
    if (position == -1) {
        return -1;
    }
    for (int h = 0; h != wordLength; ++h) {
        if (matrix[j][position - h] != 'X' && matrix[j][position - h] != 'O') {
            if (matrix[j][position - h] != word[wordLength - h - 1]) {
                return -1;
            }
        }
    }
    return position;
}

void fillHorizontalMatrix(int i, int j, int position, char *word, char matrix[limit][limit], bool usedWords[], bool usedMatrixSlots[limit][limit], int wordLength)
{
    usedWords[i] = 1;

    for (int h = 0; h != wordLength; ++h) {
        matrix[j][position - h] = word[wordLength - h - 1];
        usedMatrixSlots[j][position - h] = 1;
    }
}

void flipMatrix(char matrix[limit][limit], int numberOfRows, bool usedMatrixSlots[limit][limit])
{
    char tempMatrix[limit][limit];
    bool tempUsedMatrixSlots[limit][limit];

    for (int i = 0; i < numberOfRows; ++i) {
        for (int j = 0; j < strlen(matrix[i]); ++j) {
            tempMatrix[j][i] = matrix[i][j];
        }
    }

    for (int i = 0; i < numberOfRows; ++i) {
        for (int j = 0; j < strlen(matrix[i]); ++j) {
            matrix[i][j] = tempMatrix[i][j];
        }
    }

    initUsedMatrixSlots(usedMatrixSlots, numberOfRows, matrix);
}

bool checkIfSolved(char matrix[limit][limit], int numberOfRows)
{
    for (int i = 0; i < numberOfRows; ++i) {
        for (int j = 0; j < strlen(matrix[i]); ++j) {
            if (matrix[i][j] == 'O') {
                return false;
            }
        }
    }
    return true;
}

void shiftWords(char data[limit][limit], int numberOfWords)
{
    char tempData[limit][limit];

    for (int i = 0; i < numberOfWords; ++i) {
        strcpy(tempData[i], data[i]);
    }

    for (int i = 1; i < numberOfWords; ++i) {
        strcpy(data[i], tempData[i - 1]);
    }

    strcpy(data[0], tempData[numberOfWords - 1]);
}

void initMatrix(char matrix[limit][limit], char tempMatrix[limit][limit], int numberOfRows)
{
    for (int i = 0; i < numberOfRows; ++i) {
        for (int j = 0; j < strlen(matrix[i]); ++j) {
            matrix[i][j] = tempMatrix[i][j];
        }
    }
}

void printMatrix(char matrix[limit][limit], int numberOfRows)
{
    for (int i = 0; i < numberOfRows; ++i) {
        for (int j = 0; j < strlen(matrix[i]); ++j) {
            if (matrix[i][j] != 'X') {
                matrix[i][j] -= 32;
            }
            printf("%c", matrix[i][j]);
        }
        printf("\n");
    }
}