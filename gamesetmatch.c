#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "gauss.h"
#include "hashmap.h"
#include "ts_fg.h"

#define WIN_IDX  0
#define LOS_IDX  1

#define DEFAULT_MU    25.0
#define DEFAULT_SIGMA (25.0 / 3.0)
#define DEFAULT_BETA  (25.0 / 6.0)
#define DEFAULT_TAU   (DEFAULT_SIGMA / 100.0)


char *usage = "Usage: %s [options] filename.csv\n"
			  "-m --mu=MU         set starting mu    (default %f)\n"
			  "-s --sigma=SIGMA   set starting sigma (default %f)\n"
			  "-b --beta=BETA     set starting beta  (default %f)\n"
			  "-t --tau=TAU       set starting tau   (default %f)\n"
			  "-h --help          this help\n";

char *progname = NULL;

hashmap_t* player_map;
static double beta           = DEFAULT_BETA;
static double tau            = DEFAULT_TAU;
static double starting_mu    = DEFAULT_MU;
static double starting_sigma = DEFAULT_SIGMA;
static char*  filename       = NULL;

typedef struct player {
	char    id[9];
	gauss_t skill;
} player_t;

static player_t* find_or_create_player(char *id){
	player_t* player = hashmap_find(player_map, id, 8);
	if(!player){
		player = calloc(sizeof(player_t), 1);
		memcpy(&(player->id), id, 8);
		player->skill = gauss_init_std(starting_mu, starting_sigma);
		hashmap_add(player_map, &(player->id), 8, player);
	}
	return player;
}

void print_usage(){
	printf(usage, progname, DEFAULT_MU, DEFAULT_SIGMA, DEFAULT_BETA, DEFAULT_TAU);

}

void parse_opts(int argc, char **argv){
	static struct option long_options[] =
        {
          {"beta",    required_argument, 0, 'b'},
          {"mu",      required_argument, 0, 'm'},
          {"sigma",   required_argument, 0, 's'},
          {"tau",     required_argument, 0, 't'},
          {"help",    no_argument,       0, 'h'},
          {0, 0, 0, 0}
        };

    int c;
    int index;
    while (1){
    	  c = getopt_long (argc, argv, "b:m:s:h", long_options, &index);
    	  if(c == -1) break;
    	  switch(c){
    	  	
    	  	case 'b':
    	  	beta = atof(optarg);
    	  	if(beta == 0.0){
    	  		fprintf(stderr, "'%s' is not a valid value for beta!\n", optarg);
    	  		print_usage();
    	  		exit(1);
    	  	}
    	  	break;

    	  	case 'm':
    	  	starting_mu = atof(optarg);
    	  	if(starting_mu == 0.0){
    	  		fprintf(stderr, "'%s' is not a valid value for mu!\n", optarg);
    	  		print_usage();
    	  		exit(1);
    	  	}
    	  	break;

    	  	case 's':
    	  	starting_sigma = atof(optarg);
    	  	if(starting_sigma == 0.0){
    	  		fprintf(stderr, "'%s' is not a valid value for sigma!\n", optarg);
    	  		print_usage();
    	  		exit(1);
    	  	}
    	  	break;


    	  	case 'h':
    	  	print_usage();
    	  	exit(1);
    	  	break;



    	  }


    }
    if(optind < argc){
    	filename = argv[optind];
    }
    if(!filename){
    	fprintf(stderr, "Missing csv filename!\n");
    	print_usage();
    	exit(1);
    }
}



int main(int argc, char **argv){
	
	char *line       = NULL;
	size_t line_size = 0;
	progname = argv[0];
	player_t* winner;
	player_t* loser;
	char winner_key[64];
	char loser_key[64];
	player_t* players[2];
	gauss_t   skills[2];
	int match_id;

	ts_fg_t* trueskill = ts_fg_new();
	int first = 1;
	player_map = hashmap_new();
	parse_opts(argc, argv);
	FILE *fp = fopen(filename, "r");
	if(!fp) {
		fprintf(stderr, "Couldn't open CSV file '%s'!\n", filename);
		exit(1);
	}
	
	while(getline(&line, &line_size, fp) != -1){
		if(first) {
			first = 0;
			printf("\"match_id\",\"winner_key\",\"winner_mu\",\"winner_sigma\",\"loser_key\",\"loser_mu\",\"loser_sigma\"\n");
			continue;
		}

		// parse line

		sscanf(line, "\"%d\",\"%8s\",\"%8s\"", &match_id, winner_key, loser_key);
		
		// get players from hash map

		players[WIN_IDX] = find_or_create_player(winner_key);
		players[LOS_IDX] = find_or_create_player(loser_key);

		// print out result before updating true skill

		printf("\"%d\",\"%s\",%f,%f,\"%s\",%f,%f\n", match_id, 
			winner_key, players[WIN_IDX]->skill.mean, players[WIN_IDX]->skill.stddev,
			loser_key,  players[LOS_IDX]->skill.mean, players[LOS_IDX]->skill.stddev);

		// update trueskill

		skills[WIN_IDX]  = players[WIN_IDX]->skill;
		skills[LOS_IDX]  = players[LOS_IDX]->skill; 

		ts_fg_run(trueskill, skills, 2, beta, tau);

		players[WIN_IDX]->skill = skills[WIN_IDX];
		players[LOS_IDX]->skill = skills[LOS_IDX];

	}
}