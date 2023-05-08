#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include "rm.h"


// global variables

int DA;  // indicates if deadlocks will be avoided or not
int N;   // number of processes
int M;   // number of resource types
int ExistingRes[MAXR]; // Existing resources vector
int MaxDemand[MAXP][MAXR] = {0};
int threads[MAXP] = {0};

//..... other definitions/variables .....
//.....
//.....

// end of global variables

int rm_thread_started(int tid)
{
    int ret = 0;

    if (tid < N) {
        // thread is alive
        threads[tid] = pthread_self();
    }
    else {
        ret = -1;
    }

    return (ret);
}


int rm_thread_ended()
{
    int ret = 0;
    int id = getThread(pthread_self());
    if (threads[id] == -1) {
        ret = -1;
    }
    else {
        threads[id] = 0;
    }
    return (ret);
}


int rm_claim (int claim[])
{
    int ret = 0;

    if (DA) {
        int id = getThread(pthread_self());
        for (int i = 0; i < M; i ++) {
            if (claim[i] > ExistingRes[i]) {
                ret = -1;
                break;
            }

            MaxDemand[id][i] = claim[i];
        }
    }
    else {
        ret = -1;
    }
    return(ret);
}


int rm_init(int p_count, int r_count, int r_exist[],  int avoid)
{
    int i;
    int ret = 0;
    
    DA = avoid;
    N = p_count;
    M = r_count;

    if (N > MAXP || M > MAXR || N < 0 || M < 0 || DA < 0) {
        ret = -1;
    }

    // initialize (create) resources
    for (i = 0; i < M; ++i) {
        if (r_exist[i] < 0) {
            ret = -1;
        }
        ExistingRes[i] = r_exist[i];
    }
    // resources initialized (created)

    return  (ret);
}


int rm_request (int request[])
{
    int ret = 0;
    
    return(ret);
}


int rm_release (int release[])
{
    int ret = 0;

    return (ret);
}


int rm_detection()
{
    int ret = 0;
    
    return (ret);
}


void rm_print_state (char hmsg[])
{
    return;
}

int getThread(int selfId) {
    int res = -1;
    for (int i = 0; i < N; i++) {
        if (threads[i] == selfId)
            res = threads[i];
    }

    return res;
}