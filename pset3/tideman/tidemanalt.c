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
bool is_inside(int a, int b[]);
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

bool is_inside(int a, int b[])
{

    for (int i = 0; i < pair_count; i++)
    {
        if (a == b[i])
        {
            return true;
        }
    }
    return false;
}
// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i]) == 0)
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
        for (int j = 0; j < candidate_count; j++)
        {
            int x = ranks[i];
            int y = ranks[j];

            if (i < j)
            {

            preferences[x][y]++;

            }
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    pair_count = 0;
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)

        if (i < j && preferences[i][j] > preferences[j][i])
        {
            pairs[pair_count].winner = i;
            pairs[pair_count].loser = j;
            pair_count++;
        }
        else if (i < j && preferences[i][j] < preferences[j][i])
        {
            pairs[pair_count].winner = j;
            pairs[pair_count].loser = i;
            pair_count++;
        }
    }


    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    int str[pair_count];

    for(int i = 0; i < pair_count; i++)
    {
        str[i] = preferences[pairs[i].winner][pairs[i].loser] - preferences[pairs[i].loser][pairs[i].winner];

    }

    int swap_counter = -1;

    while (swap_counter != 0)
    {
        swap_counter = 0;

        for (int i = 0; i < pair_count - 1; i++)
        {
            if (str[i] < str[i+1])
            {
                int swap = str[i];
                str[i] = str[i+1];
                str[i+1] = swap;

                swap = pairs[i].winner;
                pairs[i].winner = pairs[i+1].winner;
                pairs[i+1].winner = swap;

                swap = pairs[i].loser;
                pairs[i].loser = pairs[i+1].loser;
                pairs[i+1].loser = swap;



                swap_counter++;


            }
        }
    }




    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        locked[pairs[i].winner][pairs[i].loser] = true;
    }



    for (int i = 0; i < pair_count; i++)
    {
        printf("%i %i\n", pairs[i].winner, pairs[i].loser);

        for (int j = 0; j < pair_count; j++)
        {
            if (pairs[i].loser == pairs[j].winner && i > j)
            {
                //printf("check1\n");
                if((preferences[pairs[j].loser][pairs[i].winner] - preferences[pairs[i].winner][pairs[j].loser]) > (preferences[pairs[i].winner][pairs[i].loser] - preferences[pairs[i].loser][pairs[j].winner]))
                {
                    locked[pairs[i].winner][pairs[i].loser] = false;

                    //printf("check2\n");
                }
            }

            if (pairs[i].winner == pairs[j].loser && i > j)
            {
                //printf("check3\n");
                if((preferences[pairs[i].loser][pairs[j].winner] - preferences[pairs[j].winner][pairs[i].loser]) > (preferences[pairs[i].winner][pairs[i].loser] - preferences[pairs[i].loser][pairs[j].winner]))
                {
                    locked[pairs[i].winner][pairs[i].loser] = false;

                    //printf("check4\n");
                }


            }
        }
    }


    return;
}

// Print the winner of the election
void print_winner(void)
{
    // TODO
    return;
}

