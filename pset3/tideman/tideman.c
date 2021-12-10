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

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
// Added
void sort_candidates_names(void);
int search_candidates_names(string name);
bool will_cycle(int winner, int loser);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // Sort candidates array by name
    sort_candidates_names();

    // Search for candidates name
    int index = search_candidates_names(name);

    // Updates ranks array
    if (index >= 0)
    {
        ranks[rank] = index;
        return true;
    }

    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // Looks through ranks array starting from first rank
    for (int rank = 0; rank < candidate_count; rank++)
    {
        int chosen = ranks[rank];

        // Updates only when other candidates are of lower rank
        for (int i = rank + 1; i < candidate_count; i++)
        {
                preferences[chosen][ranks[i]]++;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // Algorithm 1: Looks for reverse order too
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < i; j++)
        {
            if (preferences[i][j] > preferences[j][i])
            {
                pair_count++;
                pairs[pair_count - 1].winner = i;
                pairs[pair_count - 1].loser = j;
            }
            else if (preferences[i][j] < preferences[j][i])
            {
                pair_count++;
                pairs[pair_count - 1].winner = j;
                pairs[pair_count - 1].loser = i;
            }
        }
    }
    return;

    // Algorithm 2: Does not look for reverse order
    // for (int i = 0; i < candidate_count; i++)
    // {
    //     for (int j = 0; j < candidate_count; j++)
    //     {
    //         if (preferences[i][j] > preferences[j][i])
    //         {
    //             pair_count++;
    //             pairs[pair_count - 1].winner = i;
    //             pairs[pair_count - 1].loser = j;
    //         }
    //     }
    // }
    // return;

}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // BUBBLE SORT //
    int swap_count = -1;
    while (swap_count != 0)
    {
        swap_count = 0;
        for (int i = 0; i < pair_count - 1; i++)
        {
            int highscore1 = preferences[pairs[i].winner][pairs[i].loser];
            int highscore2 = preferences[pairs[i + 1].winner][pairs[i + 1].loser];

            // if current pair's strength of victory is lesser than the next pair, shift it behind
            if (highscore1 < highscore2)
            {
                // swap values
                pair tmp;
                tmp = pairs[i];

                pairs[i] = pairs[i + 1];
                pairs[i + 1] = tmp;

                swap_count++;
            }
        }
    }
    // BUBBLE SORT END //

    // For Debug; Printf
    // for (int i = 0; i < pair_count; i++)
    // {
    //     printf("Win: %s", candidates[pairs[i].winner]);
    //     printf(" ");
    //     printf("Lose: %s", candidates[pairs[i].loser]);
    //     printf("\n");
    // }

    // return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // Do for all pairs
    for (int i = 0; i < pair_count; i++)
    {
        int winner = pairs[i].winner;
        int loser = pairs[i].loser;

        if (i == 0)
        {
            printf("Lock: %s -> %s\n", candidates[winner], candidates[loser]);
            locked[winner][loser] = true;
        }
        else
        {

            // Test if adding this pair will cause a cycle
            //printf("Check case: %s -> %s\n", candidates[winner], candidates[loser]);
            if (will_cycle(winner, loser) == false)
            {
                printf("Lock: %s -> %s\n", candidates[winner], candidates[loser]);
                locked[winner][loser] = true;
            }
            else
                printf("Did not lock: %s -> %s\n", candidates[winner], candidates[loser]);

        }
    }

    // for debug; printf
    // for (int i = 0; i < candidate_count; i++)
    // {
    //     for (int j = 0; j < candidate_count; j++)
    //     {
    //         printf("%i", (int)locked[i][j]);
    //     }
    //     printf("\n");
    // }
    // return;
}

// Print the winner of the election
void print_winner(void)
{
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        int lost = 0;

        for (int j = 0; j < candidate_count; j++)
        {

            if (i != j && locked[j][i] == true)
                lost++;
        }

        if (!lost)
        {
            printf("%s\n", candidates[i]);
            return;
        }
    }
}

// BUBBLE SORT
// sort candidates by name (from A to Z)
void sort_candidates_names(void)
{
    int swap_count = -1;
    while (swap_count != 0)
    {
        swap_count = 0;
        for (int i = 0; i < candidate_count - 1; i++)
        {
            // if candidate's name is alphabetically behind (greater in ASCII) the next candidate's name
            if (strcmp(candidates[i], candidates[i + 1]) > 0)
            {
                // swap values
                string tmp = candidates[i];
                candidates[i] = candidates[i + 1];
                candidates[i + 1] = tmp;

                swap_count++;
            }
        }
    }
}

// BINARY SEARCH
// search candidates' name, returns index when matched
int search_candidates_names(string name)
{
    int start = 0;
    int end = candidate_count - 1;
    int mid;

    while (start <= end)
    {
        mid = (start + end) / 2;
        int comparison = strcmp(name, candidates[mid]);

        if (comparison == 0)
            return mid;

        else if (comparison < 0)
        {
            end = mid - 1;
        }

        else // comparison > 0
        {
            start = mid + 1;
        }

    }
    return -1;
}

bool will_cycle(int winner, int loser)
{

    //printf("Winner: %s, Loser: %s\n", candidates[winner], candidates[loser]);

    // base case
    if (locked[loser][winner] == true)
    {
        //printf("Found2: %s -> %s\n", candidates[loser], candidates[winner]);
        return true;
    }

    // Prepare a variable so that it weed out a true from all test cases.
    int value = false;

    for (int i = 0; i < candidate_count; i++)
    {
        //printf("Checking: %s -> %s\n", candidates[loser], candidates[i]);

        if (locked[loser][i] == true)
        {
            //printf("Found1: %s -> %s\n", candidates[loser], candidates[i]);
            value = will_cycle(winner, i);
            if (value == true)
                return true;
        }
    }

    // Collate results of all test scenarios.
    // If there is a "true" in one of them, will return true.
    return (false || value);
}