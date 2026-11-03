#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_WORDS 8
#define MAX_LEN 100

char words[MAX_WORDS][MAX_LEN];
char letters[10];

int wordCount = 0;
int letterCount = 0;

int values[26];
int used[10];
int leading[26];

int frequency[26];
int maxColumnCount;

long long getNumber(char* word) {
    long long result = 0;

    for (int i = 0; word[i] != '\0'; i++) {
        int digit = values[word[i] - 'A'];
        result = result * 10 + digit;
    }

    return result;
}

void printSolution() {
    for (int i = 0; i < wordCount; i++) {
        if (i > 0) {
            if (i == wordCount - 1)
                printf(" = ");
            else
                printf(" + ");
        }

        printf("%lld", getNumber(words[i]));
    }

    printf("\n");
}

int processColumn(int col, long long carry) {

    long long sum = carry;

    for (int i = 0; i < wordCount - 1; i++) {

        int len = strlen(words[i]);

        if (col < len) {
            int id = words[i][len - 1 - col];
            sum += values[id - 'A'];
        }
    }

    int digit = sum % 10;
    long long nextCarry = sum / 10;

    int len = strlen(words[wordCount - 1]);

    if (col >= len) {
        if (digit != 0)
            return 0;

        return solveColumn(col + 1, nextCarry);
    }

    int id = words[wordCount - 1][len - 1 - col] - 'A';

    if (values[id] != -1) {
        if (values[id] != digit)
            return 0;

        return solveColumn(col + 1, nextCarry);
    }

    if (used[digit] || (digit == 0 && leading[id]))
        return 0;

    values[id] = digit;
    used[digit] = 1;

    if (solveColumn(col + 1, nextCarry))
        return 1;

    values[id] = -1;
    used[digit] = 0;

    return 0;
}

int assignColumnLetters(int index, int col,
    int columnLetters[], int count, long long carry) {

    if (index == count)
        return processColumn(col, carry);

    int id = columnLetters[index];

    if (values[id] != -1)
        return assignColumnLetters(index + 1, col,
            columnLetters, count, carry);

    for (int d = 0; d <= 9; d++) {

        if (used[d])
            continue;

        if (d == 0 && leading[id])
            continue;

        values[id] = d;
        used[d] = 1;

        if (assignColumnLetters(index + 1, col,
            columnLetters, count, carry))
            return 1;

        values[id] = -1;
        used[d] = 0;
    }

    return 0;
}

int solveColumn(int col, long long carry) {

    if (col == maxColumnCount)
        return carry == 0;

    int columnLetters[MAX_WORDS];
    int count = 0;

    for (int i = 0; i < wordCount - 1; i++) {

        int len = strlen(words[i]);

        if (col >= len)
            continue;

        int id = words[i][len - 1 - col] - 'A';

        int found = 0;

        for (int j = 0; j < count; j++) {
            if (columnLetters[j] == id) {
                found = 1;
                break;
            }
        }

        if (!found)
            columnLetters[count++] = id;
    }

    for (int i = 0; i < count - 1; i++) {

        for (int j = i + 1; j < count; j++) {

            if (frequency[columnLetters[j]] >
                frequency[columnLetters[i]]) {

                int temp = columnLetters[i];
                columnLetters[i] = columnLetters[j];
                columnLetters[j] = temp;
            }
        }
    }

    return assignColumnLetters(0, col,
        columnLetters, count, carry);
}

int solveColumns() {

    maxColumnCount = 0;

    for (int i = 0; i < wordCount; i++) {

        int len = strlen(words[i]);

        if (len > maxColumnCount)
            maxColumnCount = len;
    }

    return solveColumn(0, 0);
}

void findLetters() {

    int found[26] = { 0 };

    for (int i = 0; i < wordCount; i++) {

        for (int j = 0; words[i][j] != '\0'; j++) {

            int id = words[i][j] - 'A';

            frequency[id]++;

            if (!found[id]) {
                found[id] = 1;
                letters[letterCount++] = words[i][j];
            }
        }
    }
}

void parseInput(char* input) {

    char* token = strtok(input, " +=\t\r\n");

    while (token != NULL && wordCount < MAX_WORDS) {

        strcpy(words[wordCount], token);
        wordCount++;

        token = strtok(NULL, " +=\t\r\n");
    }
}

int main() {

    char input[1024];

    for (int i = 0; i < 26; i++)
        values[i] = -1;

    for (int i = 0; i < 10; i++)
        used[i] = 0;

    if (fgets(input, sizeof(input), stdin) == NULL)
        return 1;

    parseInput(input);

    if (wordCount < 3 || wordCount > MAX_WORDS) {
        printf("Invalid input\n");
        return 1;
    }

    for (int i = 0; i < wordCount; i++) {

        if (strlen(words[i]) > 1)
            leading[words[i][0] - 'A'] = 1;
    }

    findLetters();

    if (letterCount > 10) {
        printf("No solution\n");
        return 0;
    }

    if (solveColumns()) {

        printSolution();

    }
    else {

        printf("No solution\n");
    }

    return 0;
}