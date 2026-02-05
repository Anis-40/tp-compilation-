#include <stdio.h>
#include <string.h>

#define MAX_STATES 20
#define MAX_TRANS 50
#define MAX_SYMBOLS 10

// Structure for a transition
typedef struct {
    int from;
    int to;
    char symbol; // 'e' represents epsilon
} Transition;

// Global variables
int nStates, nTransitions, nFinalStates;
int finalStates[MAX_STATES];
Transition transitions[MAX_TRANS];

// Epsilon closure matrix
int eclosure[MAX_STATES][MAX_STATES];

// Alphabet
char alphabet[MAX_SYMBOLS];
int alphaSize = 0;


void readNFA() {
    int i;

    printf("Enter number of states: ");
    scanf("%d", &nStates);

    printf("Enter number of transitions: ");
    scanf("%d", &nTransitions);

    printf("Enter transitions (from to symbol), use 'e' for epsilon:\n");
    for (i = 0; i < nTransitions; i++) {
        scanf("%d %d %c", &transitions[i].from,
                           &transitions[i].to,
                           &transitions[i].symbol);

        // Store alphabet 
        if (transitions[i].symbol != 'e') {
            int exists = 0;
            for (int j = 0; j < alphaSize; j++)
                if (alphabet[j] == transitions[i].symbol)
                    exists = 1;
            if (!exists)
                alphabet[alphaSize++] = transitions[i].symbol;
        }
    }

    printf("Enter number of final states: ");
    scanf("%d", &nFinalStates);

    printf("Enter final states:\n");
    for (i = 0; i < nFinalStates; i++)
        scanf("%d", &finalStates[i]);
}


void computeEpsilonClosure() {
    int i, j, k;

    
    for (i = 0; i < nStates; i++)
        for (j = 0; j < nStates; j++)
            eclosure[i][j] = 0;

    // Each state reaches itself
    for (i = 0; i < nStates; i++)
        eclosure[i][i] = 1;

    // Direct epsilon transitions
    for (i = 0; i < nTransitions; i++) {
        if (transitions[i].symbol == 'e')
            eclosure[transitions[i].from][transitions[i].to] = 1;
    }

    // Warshall algorith
    for (k = 0; k < nStates; k++)
        for (i = 0; i < nStates; i++)
            for (j = 0; j < nStates; j++)
                if (eclosure[i][k] && eclosure[k][j])
                    eclosure[i][j] = 1;
}


void computeNewTransitions() {
    printf("\nNew Transitions (without epsilon):\n");

    for (int s = 0; s < nStates; s++) {
        for (int a = 0; a < alphaSize; a++) {
            char sym = alphabet[a];

            for (int p = 0; p < nStates; p++) {
                if (eclosure[s][p]) {
                    for (int t = 0; t < nTransitions; t++) {
                        if (transitions[t].from == p &&
                            transitions[t].symbol == sym) {

                            int q = transitions[t].to;

                            for (int r = 0; r < nStates; r++) {
                                if (eclosure[q][r]) {
                                    printf("%d --%c--> %d\n", s, sym, r);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}


void computeNewFinalStates() {
    printf("\nNew Final States:\n");

    for (int s = 0; s < nStates; s++) {
        for (int i = 0; i < nStates; i++) {
            if (eclosure[s][i]) {
                for (int f = 0; f < nFinalStates; f++) {
                    if (i == finalStates[f]) {
                        printf("State %d is final\n", s);
                    }
                }
            }
        }
    }
}


void displayAutomaton() {
    printf("\n--- NFA without Epsilon Transitions ---\n");
    computeNewTransitions();
    computeNewFinalStates();
}


int main() {
    readNFA();
    computeEpsilonClosure();
    displayAutomaton();
    return 0;
}