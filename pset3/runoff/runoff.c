#include <cs50.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

// Max voters and candidates
#define MAX_VOTERS 100
#define MAX_CANDIDATES 9

// preferences[i][j] is jth preference for voter i
int preferences[MAX_VOTERS][MAX_CANDIDATES];

// Candidates have name, vote count, eliminated status
typedef struct
{
    string name;
    int votes;
    bool eliminated;
}
candidate;

// Array of candidates
candidate candidates[MAX_CANDIDATES];

// Numbers of voters and candidates
int voter_count;
int candidate_count;

// Function prototypes
bool vote(int voter, int rank, string name);
void tabulate(void);
bool print_winner(void);
int find_min(void);
bool is_tie(int min);
void eliminate(int min);
// Added
// void sort_candidates_votes(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: runoff [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX_CANDIDATES)
    {
        printf("Maximum number of candidates is %i\n", MAX_CANDIDATES);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
        candidates[i].eliminated = false;
    }

    voter_count = get_int("Number of voters: ");
    if (voter_count > MAX_VOTERS)
    {
        printf("Maximum number of voters is %i\n", MAX_VOTERS);
        return 3;
    }

    // Keep querying for votes
    for (int i = 0; i < voter_count; i++)
    {

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            // Record vote, unless it's invalid
            if (!vote(i, j, name))
            {
                printf("Invalid vote.\n");
                return 4;
            }
        }

        printf("\n");
    }

    // Keep holding runoffs until winner exists
    while (true)
    {
        // Calculate votes given remaining candidates
        tabulate();

        // Check if election has been won
        bool won = print_winner();
        if (won)
        {
            break;
        }

        // Eliminate last-place candidates
        int min = find_min();
        bool tie = is_tie(min);

        // If tie, everyone wins
        if (tie)
        {
            for (int i = 0; i < candidate_count; i++)
            {
                if (!candidates[i].eliminated)
                {
                    printf("%s\n", candidates[i].name);
                }
            }
            break;
        }

        // Eliminate anyone with minimum number of votes
        eliminate(min);

        // Reset vote counts back to zero
        for (int i = 0; i < candidate_count; i++)
        {
            candidates[i].votes = 0;
        }
    }
    return 0;
}

// Record preference if vote is valid
bool vote(int voter, int rank, string name)
{
    // Linear search candidate list for user-given name
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i].name) == 0)
        {
            // update voting preferences
            preferences[voter][rank] = i;
            return true;
        }
    }
    return false;
}

// Tabulate votes for non-eliminated candidates
void tabulate(void)
{
    // Retrieve information from each voter
    for (int i = 0; i < voter_count; i++)
    {
        // Retrieve the top rank from each voter, if cannot find candidate, move to next rank
        for (int j = 0; j < candidate_count; j++)
        {
            bool found = false;
            // Iterate over candidates to look for match
            for (int k = 0; k < candidate_count; k++)
            {
                if (preferences[i][j] == k)
                {
                    if (!(candidates[k].eliminated))
                    {
                        candidates[k].votes++;
                        found = true;
                    }
                    break;
                }
            }
            if (found)
            {
                break;
            }
        }
    }
    return;
}

// Print the winner of the election, if there is one
bool print_winner(void)
{
    // Find the min number of votes to get majority
    float half = voter_count / 2.0;

    // Find any candidate that has majority
    for (int i = 0; i < candidate_count; i++)
    {
        // Return True if found
        if (candidates[i].votes > half)
        {
            printf("%s\n", candidates[i].name);
            return true;
        }
    }

    return false;
}

// Return the minimum number of votes any remaining candidate has
int find_min(void)
{

    // Initialise minimum
    int min = candidates[0].votes;

    // Search for any candidates with lower votes
    for (int i = 0; i < candidate_count; i++)
    {
        if (!candidates[i].eliminated)
        {
            if (candidates[i].votes < min)
            {
                min = candidates[i].votes;
            }
        }
    }

    return min;
}

// Return true if the election is tied between all candidates, false otherwise
bool is_tie(int min)
{
    // Check if all candidates have min votes
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].votes > min)
        {
            return false;
        }
    }
    return true;
}

// Eliminate the candidate (or candidates) in last place
void eliminate(int min)
{
    // Search all candidates that has minimum votes but not yet eliminated
    for (int i = 0; i < candidate_count; i++)
    {
        if (!candidates[i].eliminated && candidates[i].votes == min)
        {
            candidates[i].eliminated = true;
        }
    }

    return;
}

// void sort_candidates_votes(void)
// {
//     // BUBBLE SORT
//     int swap_count = -1;
//     while (swap_count != 0)
//     {
//         swap_count = 0;
//         for (int i = 0; i < candidate_count - 1; i++)
//         {
//             if (candidates[i].votes < candidates[i + 1].votes)
//             {
//                 // swap values
//                 candidate tmp = candidates[i];
//                 candidates[i] = candidates[i + 1];
//                 candidates[i + 1] = tmp;

//                 swap_count++;
//             }
//         }
//     }
// }
