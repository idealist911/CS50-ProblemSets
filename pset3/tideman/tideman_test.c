#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count = 5;
int candidate_count = 5;

// FUNCTION PROTOTYPES
void lock_pairs(void);
bool will_cycle(int winner, int loser);


int main(void)
{
    candidates[0] = "a";
    candidates[1] = "b";
    candidates[2] = "c";
    candidates[3] = "d";
    candidates[4] = "e";

    pairs[0].winner = 0;
    pairs[0].loser = 1;
    pairs[1].winner = 1;
    pairs[1].loser = 2;
    pairs[2].winner = 1;
    pairs[2].loser = 3;
    pairs[3].winner = 3;
    pairs[3].loser = 4;
    pairs[4].winner = 4;
    pairs[4].loser = 0;

    lock_pairs();
}

void lock_pairs(void)
{
    // TODO
    for (int i = 0; i < pair_count; i++)
    {
        int winner = pairs[i].winner;
        int loser = pairs[i].loser;


        /* How can I tell that there is a cyclic graph? */
        printf("Check case: %s -> %s\n", candidates[winner], candidates[loser]);
        if (will_cycle(winner, loser) == false)
        {
            printf("Lock: %s -> %s\n", candidates[winner], candidates[loser]);
            locked[winner][loser] = true;
        }
        else
            printf("rej: %s, %s\n", candidates[winner], candidates[loser]);
    }

    // for debug; printf
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            printf("%i", (int)locked[i][j]);
        }
        printf("\n");
    }
    return;
}

bool will_cycle(int winner, int loser)
{

    printf("Winner: %s, Loser: %s\n", candidates[winner], candidates[loser]);

    // base case
    if (locked[loser][winner] == true)
    {
        printf("Found2: %s -> %s\n", candidates[loser], candidates[winner]);
        return true;
    }

    bool value = false;

    for (int i = 0; i < candidate_count; i++)
    {
        printf("Checking: %s -> %s\n", candidates[loser], candidates[i]);

        if (locked[loser][i] == true)
        {
            printf("Found1: %s -> %s\n", candidates[loser], candidates[i]);
            value = will_cycle(winner, i);
        }
    }

    return (false || value);
}