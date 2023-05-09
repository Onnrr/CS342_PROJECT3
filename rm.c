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
int Allocation[MAXP][MAXR] = {0};
int Need[MAXP][MAXR] = {0};
int threads[MAXP] = {0};

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t cv = PTHREAD_COND_INITIALIZER;

//..... other definitions/variables .....
//.....
//.....

// end of global variables


// only call inside lock
int getThread(int selfId) {
    int res = -1;
    for (int i = 0; i < N; i++) {
        if (threads[i] == selfId)
            res = i;
    }

    return res;
}

int rm_thread_started(int tid) {
    int ret = 0;

    pthread_mutex_lock(&lock);
    if (tid < N) {
        // thread is alive
        threads[tid] = pthread_self();
    }
    else {
        ret = -1;
    }
    pthread_mutex_unlock(&lock);
    return (ret);
}


int rm_thread_ended() {
    int ret = 0;

    pthread_mutex_lock(&lock);
    int id = getThread(pthread_self());

    if (threads[id] == -1) {
        ret = -1;
    }
    else {
        threads[id] = 0;
    }
    pthread_mutex_unlock(&lock);
    return (ret);
}


int rm_claim (int claim[]) {
    int ret = 0;

    pthread_mutex_lock(&lock);
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
    pthread_mutex_unlock(&lock);
    return(ret);
}


int rm_init(int p_count, int r_count, int r_exist[],  int avoid) {
    int i;
    int ret = 0;
    
    pthread_mutex_lock(&lock);
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
    pthread_mutex_unlock(&lock);

    return  (ret);
}


int rm_request (int request[])
{
    int ret = 0;
    /*
    if (DA) {
        int finish[N];
        for (int i = 0; i < N; i++) {
            finish[i] = 0;
        }
        int work[M];
        for (int i = 0; i < M; i++) {
            work[i] = ExistingRes[i];
        }
        
        int found;
        int i, j;
        do {
            found = 0;
            for (i = 0; i < N; i++) {
                if (finish[i] == 0) {
                    int can_allocate = 1;
                    for (j = 0; j < N; j++) {
                        if (Need[i][j] > work[j]) {
                            can_allocate = 0;
                            break;
                        }
                    }

                    if (can_allocate) {
                        found = 1;
                        finish[i] = 1;
                        for (j = 0; j < M; j++) {
                            work[j] += Allocation[i][j];
                        }
                    }
                }
            }
        } while (found);

        // Check if all processes are finished
        for (i = 0; i < N; i++) {
            if (finish[i] == 0) {
                // TODO Sleep
            }
        }

    }
    else {
        int available = 0;
        while (available == 0) {
            available = 1;
            for (int i = 0; i < M; i++) {
                if (ExistingRes[i] < request[i]) {
                    available = 0;
                    break;
                }
            }
            if (available == 0) {
                pthread_cond_wait(&cv, &lock);
            }
            else {
                for (int i = 0; i < M; i++) {
                    ExistingRes[i] -= request[i];
                }
                pthread_mutex_unlock(&lock);
                return 0;
            }
            pthread_mutex_unlock(&lock);
        }
    }
    */
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
    pthread_mutex_lock(&lock);

    printf("##########################\n");
    printf("%s\n", hmsg);
    printf("##########################\n");

    printf("\nExist:\n%4s", "");
    for (int i = 0; i < M; i++) {
        char temp[5] = "";
        sprintf(temp, "R%d", i);
        printf("%4s", temp);
    }
    printf("\n%4s", "");
    for (int i = 0; i < M; i++) {
        printf("%4d", ExistingRes[i]);
    }

    printf("\n\nAvailable:\n%4s", "");
    for (int i = 0; i < M; i++) {
        char temp[5] = "";
        sprintf(temp, "R%d", i);
        printf("%4s", temp);
    }
    printf("\n%4s", "");
    for (int i = 0; i < M; i++) {
        int sum = 0;
        for (int j = 0; j < N; j++) {
            sum = sum + Allocation[j][i]; 
        }
        printf("%4d", (ExistingRes[i] - sum));
    }

    printf("\n\nAllocation:\n%4s", "");
    for (int i = 0; i < M; i++) {
        char temp[5] = "";
        sprintf(temp, "R%d", i);
        printf("%4s", temp);
    }
    for (int i = 0; i < N; i++) {
        char temp[5] = "";
        sprintf(temp, "T%d:", i);
        printf("\n%-4s", temp);
        for (int j = 0; j < M; j++) {
            printf("%4d", Allocation[i][j]);
        }
    }
    
    //request ????

    printf("\n\nMaxDemand:\n%4s", "");
    for (int i = 0; i < M; i++) {
        char temp[5] = "";
        sprintf(temp, "R%d", i);
        printf("%4s", temp);
    }
    for (int i = 0; i < N; i++) {
        char temp[5] = "";
        sprintf(temp, "T%d:", i);
        printf("\n%-4s", temp);
        for (int j = 0; j < M; j++) {
            printf("%4d", MaxDemand[i][j]);
        }
    }

    printf("\n\nNeed:\n%4s", "");
    for (int i = 0; i < M; i++) {
        char temp[5] = "";
        sprintf(temp, "R%d", i);
        printf("%4s", temp);
    }
    for (int i = 0; i < N; i++) {
        char temp[5] = "";
        sprintf(temp, "T%d:", i);
        printf("\n%-4s", temp);
        for (int j = 0; j < M; j++) {
            printf("%4d", Need[i][j]);
        }
    }
    printf("\n\n");
    pthread_mutex_unlock(&lock);
    return;
}
