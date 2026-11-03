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

long long getNumber(char* word) {
    long long result = 0;

    for (int i = 0; word[i] != '\0'; i++) {
        int digit = values[word[i] - 'A'];
        result = result * 10 + digit;
    }

    return result;
}

int checkSolution() {
    long long sum = 0;

    for (int i = 0; i < wordCount - 1; i++) {
        sum += getNumber(words[i]);
    }

    long long result = getNumber(words[wordCount - 1]);

    return sum == result;
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

int solve(int index) {

    if (index == letterCount) {
        return checkSolution();
    }

    char current = letters[index];
    int id = current - 'A';

    for (int d = 0; d <= 9; d++) {

        if (used[d])
            continue;

        if (d == 0 && leading[id])
            continue;

        values[id] = d;
        used[d] = 1;

        if (solve(index + 1))
            return 1;

        values[id] = -1;
        used[d] = 0;
    }

    return 0;
}

void findLetters() {
    int found[26] = { 0 };

    for (int i = 0; i < wordCount; i++) {
        for (int j = 0; words[i][j] != '\0'; j++) {

            int id = words[i][j] - 'A';

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

        token = strtok(NULL, " +=\n");
    }
}

int main() {
    char input[1024];

    for (int i = 0; i < 26; i++) {
        values[i] = -1;
    }

    for (int i = 0; i < 10; i++) {
        used[i] = 0;
    }

    if (fgets(input, sizeof(input), stdin) == NULL)
        return 1;

    parseInput(input);

    if (wordCount < 3 || wordCount > MAX_WORDS) {
        printf("Invalid input\n");
        return 1;
    }

    for (int i = 0; i < wordCount; i++) {
        if (strlen(words[i]) > 1) {
            leading[words[i][0] - 'A'] = 1;
        }
    }

    findLetters();

    if (letterCount > 10) {
        printf("No solution\n");
        return 0;
    }

    if (solve(0)) {
        printSolution();
    }
    else {
        printf("No solution\n");
    }

    return 0;
}