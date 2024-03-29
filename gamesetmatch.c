#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "gauss.h"
#include "hashmap.h"
#include "ts_fg.h"

#define WIN_IDX  0
#define LOS_IDX  1

#define MAX_PLAYERS   64
#define MAX_MATCH_ID 256
#define MAX_PLAYER_ID 256

#define DEFAULT_MU    25.0
#define DEFAULT_SIGMA (25.0 / 3.0)
#define DEFAULT_BETA  (25.0 / 6.0)
#define DEFAULT_TAU   (DEFAULT_SIGMA / 100.0)


char *usage = "Usage: %s [options] filename.csv\n"
			  "\n"
			  "Options:\n"
			  "-m --mu     MU     set starting mu    (default %f)\n"
			  "-s --sigma  SIGMA  set starting sigma (default %f)\n"
			  "-b --beta   BETA   set starting beta  (default %f)\n"
			  "-t --tau    TAU    set starting tau   (default %f)\n"
			  "   --ignore        ignore first line  (default false)\n"
			  "-h --help         this help\n";

char *progname = NULL;

hashmap_t* player_map;
static double beta           = DEFAULT_BETA;
static double tau            = DEFAULT_TAU;
static double starting_mu    = DEFAULT_MU;
static double starting_sigma = DEFAULT_SIGMA;
static int    ignore_first   = 0;
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
          {"beta",   required_argument, 0, 'b'},
          {"mu",     required_argument, 0, 'm'},
          {"sigma",  required_argument, 0, 's'},
          {"tau", 	 required_argument, 0, 't'},
          {"ignore", no_argument,       &ignore_first, 1},
          {"help",   no_argument,       0, 'h'},
          {0, 0, 0, 0}
        };

    int c;
    int index;
    while (1){
    	  c = getopt_long (argc, argv, "b:m:s:t:h", long_options, &index);
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


    	  	case 't':
    	  	tau = atof(optarg);
    	  	if(tau == 0.0){
    	  		fprintf(stderr, "'%s' is not a valid value for tau!\n", optarg);
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

	player_t* players[MAX_PLAYERS];
	gauss_t   skills[MAX_PLAYERS];
	char* match_id;
	char* player_ids[MAX_PLAYERS];



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
			if(ignore_first) continue;
		}

		char *tok;
		int first_tok = 1;
		int player_n = 0;
		for(tok = strtok(line, ",\n"); tok; tok = strtok(NULL, ",\n")){
			if(first_tok){
				match_id  = tok;
				first_tok = 0;
				continue;
			}
			if(strlen(tok)==0) continue;
			player_ids[player_n] = tok;
			players[player_n]    = find_or_create_player(tok);
			player_n++;
			if(player_n >= MAX_PLAYERS){
				fprintf(stderr, "Too many players! Max is %d\n", MAX_PLAYERS);
				exit(1);
			}
		}

		int i;
		printf("%s", match_id);
		for(i=0; i < player_n; i++){
			printf(",%s,%f,%f", player_ids[i], players[i]->skill.mean, players[i]->skill.stddev);
			skills[i] = players[i]->skill;
		}
		printf("\n");

		ts_fg_run(trueskill, skills, player_n, beta, tau);

		for(i=0; i < player_n; i++){
			players[i]->skill = skills[i];
		}
	}
}
