#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <unistd.h> 

#define TIMER_SIG1 SIGUSR1

// Global variables to track timer ticks
int timer1_ticks = 0;

// Timer callback function
void timer_callback(int sig) 
{
    printf("Time handler getting execuited\n");
    if (sig == TIMER_SIG1) 
    {
        printf("Timer 1 Ticks: %d\n", ++timer1_ticks);
    } 
}

void set_timer1(int delay_in_sec, int interval_in_sec)
{
    // Set up signal handler for timer 1
    struct sigaction sa1;
    sa1.sa_flags = SA_SIGINFO;
    sa1.sa_sigaction = timer_callback;
    sigemptyset(&sa1.sa_mask);
    if (sigaction(TIMER_SIG1, &sa1, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    // Create timer 1
    timer_t timerid1;
    struct sigevent sev1;
    struct itimerspec its1;
    sev1.sigev_notify = SIGEV_SIGNAL;
    sev1.sigev_signo = TIMER_SIG1;
    sev1.sigev_value.sival_ptr = &timerid1;
    if (timer_create(CLOCK_REALTIME, &sev1, &timerid1) == -1) {
        perror("timer_create");
        exit(EXIT_FAILURE);
    }
       
    // Set timer 1 expiration time and interval
    its1.it_value.tv_sec = delay_in_sec; 
    its1.it_value.tv_nsec = 0;
    its1.it_interval.tv_sec = interval_in_sec;
    its1.it_interval.tv_nsec = 0;

    // Start timer 1
    if (timer_settime(timerid1, 0, &its1, NULL) == -1) {
        perror("timer_settime");
        exit(EXIT_FAILURE);
    }
    printf("Timer 1 started. Waiting for ticks...\n");

}

int main() {
    
    set_timer1(10,0);       //delay for 10 seconds
    
    // Print elapsed time every second
    int elapsed_seconds = 0;
    while (1) {
        sleep(1); // Sleep for 1 second
        elapsed_seconds++;
        printf("Elapsed time: %d seconds\n", elapsed_seconds);
    }

    return 0;
}
