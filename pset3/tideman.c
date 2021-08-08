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
bool makes_circle(int cycle_start, int loser);
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

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
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(candidates[i], name) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }

    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int k = i; k < candidate_count; k++)
        {
            if (i != k)
            {
                preferences[ranks[i]][ranks[k]]++;
            }
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int k = i; k < candidate_count; k++)
        {
            if (i != k)
            {
                if (preferences[i][k] > preferences[k][i])
                {
                    pairs[pair_count].winner = i;
                    pairs[pair_count].loser = k;

                    pair_count++;
                }

                if (preferences[i][k] < preferences[k][i])
                {
                    pairs[pair_count].winner = k;
                    pairs[pair_count].loser = i;

                    pair_count++;
                }
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    /*printf("--Before Sorting--\n");
    for (int i = 0; i < pair_count; i++)
    {
        int diff = preferences[pairs[i].winner][pairs[i].loser] - preferences[pairs[i].loser][pairs[i].winner];
        printf("%i. %i - %i   -   strength: %i\n", i, pairs[i].winner, pairs[i].loser, diff);
    }

    printf("--After Sorting--\n");*/


    for (int i = 0; i < pair_count; i++)
    {
        int diff = preferences[pairs[i].winner][pairs[i].loser] - preferences[pairs[i].loser][pairs[i].winner];
        int next_diff = preferences[pairs[i + 1].winner][pairs[i + 1].loser] - preferences[pairs[i + 1].loser][pairs[i + 1].winner];

        if (next_diff > diff)
        {
            pair temp_pair = pairs[i];
            pairs[i] = pairs[i + 1];
            pairs[i + 1] = temp_pair;
        }


    }

    /*for (int i = 0; i < pair_count; i++)
    {
        int diff = preferences[pairs[i].winner][pairs[i].loser] - preferences[pairs[i].loser][pairs[i].winner];
        printf("%i. %i - %i   -   strength: %i\n", i, pairs[i].winner, pairs[i].loser, diff);
    }*/
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
/*void lock_pairs(void)
{
    bool is_locked_1 = false;
    bool is_locked_2 = false;
    for (int i = 0; i < pair_count; i++)
    {
        for (int j = 0; j < pair_count; j++)
        {
            if (locked[pairs[i].loser][j])
            {
                is_locked_1 = true;
            }
            if (locked[j][pairs[i].winner])
            {
                is_locked_2 = true;
            }
        }
        if (!is_locked_1 || !is_locked_2)
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
        is_locked_1 = false;
        is_locked_2 = false;
    }
    return;
}*/

void lock_pairs()
{
    for (int i = 0; i < pair_count; i++)
    {
        if (!makes_circle(pairs[i].winner, pairs[i].loser))
        {
            // Lock the pair unless it makes a circle
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }
}

// Print the winner of the election
/*void print_winner(void)
{
    bool is_winner = true;
    for (int i = 0; i < pair_count; i++)
    {
        for (int k = 0; k < pair_count; k++)
        {
            if (locked[k][i])
            {
                is_winner = false;
            }
        }

        if (is_winner)
        {
            printf("%s\n", candidates[i]);
            is_winner = true;
        }
    }
    return;
}*/

bool makes_circle(int cycle_start, int loser)
{
    if (loser == cycle_start)
    {
        // If the current loser is the cycle start
        // The entry makes a circle
        return true;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        if (locked[loser][i])
        {
            if (makes_circle(cycle_start, i))
            {
                // Forward progress through the circle
                return true;
            }
        }
    }
    return false;
}

void print_winner(void)
{
    // Winner is the candidate with no arrows pointing to them
    for (int i = 0; i < candidate_count; i++)
    {
        int false_count = 0;
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[j][i] == false)
            {
                false_count++;
                if (false_count == candidate_count)
                {
                    printf("%s\n", candidates[i]);
                }
            }
        }
    }
    return;
}