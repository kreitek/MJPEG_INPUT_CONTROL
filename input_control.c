#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <linux/videodev2.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <signal.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <getopt.h>
#include <pthread.h>
#include <syslog.h>

#include "control.h"
#include "./mjpg-streamer/utils.h"
#include "./mjpg-streamer/mjpg_streamer.h"

#define INPUT_PLUGIN_NAME "Input motor"

/* private functions and variables to this plugin */
static pthread_t   worker;
static globals     *pglobal;
static int plugin_number;

void *worker_thread(void *);
void worker_cleanup(void *);
void help(void);
int input_cmd(int plugin_number, unsigned int control_id, unsigned int group, int value);

static int delay = 250;

/*** plugin interface functions ***/
/******************************************************************************
Description.: This function initializes the plugin. It parses the commandline-
              parameter and stores the default and parsed values in the
              appropriate variables.
Input Value.: param contains among others the command-line string
Return Value: 0 if everything is fine
              1 if "--help" was triggered, in this case the calling programm
              should stop running and leave.
******************************************************************************/
int input_init(input_parameter *param, int id)
{
    /* keep a pointer to the global variables */
    pglobal = param->global;

    plugin_number = id;

    param->argv[0] = INPUT_PLUGIN_NAME;
	
	control_inic();

    /* parse the parameters */
    reset_getopt();
    while(1) {
        int option_index = 0, c = 0;
        static struct option long_options[] = {
            {"h", no_argument, 0, 0
            },
            {"help", no_argument, 0, 0},
            {0, 0, 0, 0}
        };

        /* parsing all parameters according to the list above is sufficent */
        c = getopt_long_only(param->argc, param->argv, "", long_options, &option_index);

        /* no more options to parse */
        if(c == -1) break;

        /* unrecognized option */
        if(c == '?') {
            help();
            return 1;
        }

        /* dispatch the given options */
        switch(option_index) {
            /* h, help */
        case 0:
        case 1:
            DBG("case 0,1\n");
            help();
            return 1;
            break;

        default:
            DBG("default case\n");
            help();
            return 1;
        }
    }

    return 0;
}

/******************************************************************************
Description.: Stops the execution of worker thread
Input Value.: -
Return Value: always 0
******************************************************************************/
int input_stop(void)
{
    DBG("will cancel input thread\n");
    pthread_cancel(worker);

    return 0;
}

/******************************************************************************
Description.: spins of a worker thread
Input Value.: -
Return Value: always 0
******************************************************************************/
int input_run(void)
{
    pthread_create(&worker, 0, worker_thread, NULL);
    pthread_detach(worker);

    return 0;
}

/******************************************************************************
Description.: process commands, allows to set certain runtime configurations
              and settings like pan/tilt, colors, saturation etc.
Input Value.: * cmd specifies the command, a complete list is maintained in
                the file "input.h"
              * value is used for commands that make use of a parameter.
Return Value: depends in the command, for most cases 0 means no errors and
              -1 signals an error. This is just rule of thumb, not more!
******************************************************************************/
int input_cmd(int plugin_number, unsigned int control_id, unsigned int group, int value)
{
    return proc_control(control_id,  group, value);
}

/*** private functions for this plugin below ***/
/******************************************************************************
Description.: print a help message to stderr
Input Value.: -
Return Value: -
******************************************************************************/
void help(void)
{
    fprintf(stderr, " ---------------------------------------------------------------\n" \
    " Help for input plugin..: "INPUT_PLUGIN_NAME"\n");
}

void *worker_thread(void *arg)
{
    /* set cleanup handler to cleanup allocated ressources */
    pthread_cleanup_push(worker_cleanup, NULL);

    while(!pglobal->stop) {
	/* Stop the actually command before MAX_TIME_BEFORE_STOP_MOTORS seconds without received one*/
	if (now_in_msec() >= time4NextStop)
	{
		proc_control(STOP_MOTORS, 0, 0);
	}
#ifdef DEBUG
	else
		if (time4NextStop != LONG_MAX)
	        	fprintf(stderr, "Don't stop now...\n");
		else
	        	fprintf(stderr, "Stop are necessary...\n");
#endif

        usleep(1000 * delay);
    }

    IPRINT("leaving input thread, calling cleanup function now\n");
    pthread_cleanup_pop(1);

    return NULL;
}

/******************************************************************************
Description.:
Input Value.:
Return Value:
******************************************************************************/
void worker_cleanup(void *arg)
{
    static unsigned char first_run = 1;

    if(!first_run) {
        DBG("already cleaned up ressources\n");
        return;
    }

    first_run = 0;
}
