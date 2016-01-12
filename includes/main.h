#ifndef MAIN_H
#define MAIN_H

#include <signal.h>

#include "Master.h"
#include "Scheduler.h"

int main(int, char**);
void exit_handler(int);
void scheduler_realtime();
void scheduler_standard();

#endif // MAIN_H