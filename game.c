#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define BOARD_SIZE 8
#define MAX_PLAYERS 5

//To pause the game for a while and let the user see the output
void delay(int number_of_seconds)
{
	int milli_seconds = 1000 * number_of_seconds;
	clock_t start_time = clock();
	while(clock() < start_time + milli_seconds);
}

void display(int[], int);
int destination(int, int, int[], int[]);

int main (int argc, char* argv[])
{
	// Improper command line input exits the program
	if (argc != 2)
	{
		printf("Wrong input format!\nPlease enter command in the following format :\n$ %s <	input>\n\t where input -> name of config file\n", argv[0]);
		return 1;	
	}

	//read the config file into 
	char* config = argv[1];
	FILE *fptr;
	fptr = fopen(config, "r");
	if(fptr == NULL)
	{
		printf("Cannot open file \n");
		exit(0);
	}

	int transitions = 0;
	int sources[BOARD_SIZE * BOARD_SIZE], destinations[BOARD_SIZE * BOARD_SIZE];
	fscanf(fptr, "%d", &transitions);
	for(int i = 0; i < transitions; i++)
	{
		fscanf(fptr, "%d", &sources[i]);
		fscanf(fptr, "%d", &destinations[i]);
	}
	for(int i = 0; i < transitions; i++)
	{
		printf("Transition %d : %d -> %d\n", i, sources[i], destinations[i]);
	}
	//The actual game
	int n = 0, dice = 0, x = 0;
	int playerPositions[MAX_PLAYERS];
	for (int i = 0; i < MAX_PLAYERS; ++i)
	{
		playerPositions[i] = 1;
	}
	printf("Enter the number of players(maximum of 5) : ");
	scanf("%d", &n);
	int winner = MAX_PLAYERS;
	while(winner == MAX_PLAYERS) {
		for (int i = 0; i < n; i++)
		{
			display(playerPositions, n);
			dice = (rand()%6) + 1;
			printf("\nPlayer %d\nPress a number to roll the dice : ", i);
			scanf("%d", &x);
			printf("\nDice : %d\n", dice);
			if(playerPositions[i] + dice > BOARD_SIZE * BOARD_SIZE)
			{
				printf("You cannot make this move. Skipping move...\n");
				delay(1000);
				continue;
			}
			for(int j = 0; j < dice; j++){
				playerPositions[i]++;
				display(playerPositions, n);
				printf("\nPlayer %d scored %d on the dice\n", i, dice);
				delay(500);
			}
			playerPositions[i] = destination(playerPositions[i], transitions, sources, destinations);
			printf("Press a number to continue : \n");
			scanf("%d", &x);

			if(playerPositions[i] == BOARD_SIZE * BOARD_SIZE)
			{
				winner = i;
				break;
			}
		}
	}
	system("clear");
	printf("Winner is Player %d\n", winner);
	return 0;
}
int destination(int source, int transitions, int sources[], int destinations[])
{
	int isNone = 0;
	printf("The following are the applicable transitions for your move :\n");
	for(int i = 0; i < transitions; i++)
	{
		if(source == sources[i])
		{
			isNone = 1;
			printf("%d -> %d\n", sources[i], destinations[i]);
			source = destinations[i];
		}
	}
	if(isNone==0)
		printf("None\n");
	return source;
}

void display(int playerPositions[], int numberOfPlayers)
{
	system("clear");
	printf("\nBoard : \n");
	int offset = BOARD_SIZE - 1;
	for(int k = 1, i = 0; k <= BOARD_SIZE * BOARD_SIZE; k++)
	{
		if(((k-1)/BOARD_SIZE)%2==1)
		{
			i = k + offset;
			offset -=2;
		}
		else
		{
			i = k;
			offset = BOARD_SIZE - 1;
		}
		printf("\t%d ", i);
		for(int j = 0; j < numberOfPlayers; j++)
		{
			if(i == playerPositions[j])
				printf("P%d", j);
			else
				printf("");
		}
		if(k%BOARD_SIZE==0)
			printf("\n");
	}	

}