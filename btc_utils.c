#include <string.h>
#include <stdio.h>
#include <argp.h>
#include <stdbool.h>
#include "seed_gen.h"

const char *argp_program_version = "v0.0.0";
const char *argp_program_bug_address = "https://github.com/dni/btc-utils/issues";
static char doc[] = "dni's bitcoin utilities.";
static char args_doc[] = "seed_gen";
static struct argp_option options[] = {
    { "seed", 's', "SEED", 0, "Seed for address opertions."},
    { 0 }
};

struct arguments {
    char *seed;
    enum { NO_COMMAND, SEED_GEN } command;
};


static error_t parse_opt(int key, char *arg, struct argp_state *state) {
    struct arguments *arguments = state->input;
    switch (key) {
        case 's': arguments->seed = arg; break;
        case ARGP_KEY_ARG:
                  if (state->arg_num >= 1) argp_usage(state);
                  if (strcmp(arg, "seed_gen") == 0) {
                      arguments->command = SEED_GEN;
                  } else {
                      arguments->command = NO_COMMAND;
                  }
                  break;
        default: return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

static struct argp argp = { options, parse_opt, args_doc, doc, 0, 0, 0 };

int main(int argc, char *argv[])
{
    struct arguments arguments;

    argp_parse(&argp, argc, argv, 0, 0, &arguments);
    switch (arguments.command) {
        case SEED_GEN:
            seed_gen();
            break;
        case NO_COMMAND:
            printf("No command.\n");
            break;
        default:
            return 1;
    }

}
