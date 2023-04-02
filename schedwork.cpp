#ifndef RECCHECK
#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#endif

// add or remove necessary headers as you please
#include "schedwork.h"

using namespace std;

// a constant that can be used to indicate an INVALID 
// worker ID if that is useful to your implementation.
// Feel free to not use or delete.
static const Worker_T INVALID_ID = (unsigned int)-1;

// Add prototypes for any helper functions here
bool buildSchedule(
    const AvailabilityMatrix& avail, 
    Worker_T startWorker,
    const size_t dailyNeed, 
    const size_t maxShifts,
    std::vector<size_t>& shiftsWorked, 
    DailySchedule& sched, 
    size_t day,
    size_t schedCol);

bool recurse(
    const AvailabilityMatrix& avail, 
    Worker_T startWorker,
    const size_t dailyNeed, 
    const size_t maxShifts,
    std::vector<size_t>& shiftsWorked, 
    DailySchedule& sched, 
    size_t day,
    size_t schedCol
);

// Add your implementation of schedule() and other helper functions here

bool schedule(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched
)
{
    if(avail.size() == 0U){
        return false;
    }
    sched.clear();
    
    // initialize
    sched.resize(avail.size()); // resize number of rows to n
    for (size_t r = 0; r < sched.size(); r++) {
        sched[r].resize(dailyNeed); // resize number of cols to d
        for (size_t c = 0; c < dailyNeed; c++) {
            sched[r][c] = INVALID_ID;
        }
    }

    std::vector<size_t> shiftsWorked(avail[0].size(), 0U);

    return buildSchedule(avail, 0U, dailyNeed, maxShifts, shiftsWorked, sched, 0U, 0U);
}


bool buildSchedule(
    const AvailabilityMatrix& avail, 
    Worker_T startWorker, 
    const size_t dailyNeed, 
    const size_t maxShifts, 
    std::vector<size_t>& shiftsWorked, 
    DailySchedule& sched,
    size_t day,
    size_t schedCol)
{
    // Base case: we've gone off the end of the matrix, so we reached a valid solution
    if (day == sched.size()) {
        return true;
    }

    size_t k = avail[day].size();
    for (Worker_T worker = startWorker; worker < k; worker++) {
        
        // check if the worker can work this day (is available and hasn't already worked maxShifts)
        if (!avail[day][worker]) continue;
        if (shiftsWorked[worker] == maxShifts) continue;

        // set worker at the current location and increase their shift count.
        sched[day][schedCol] = worker;
        shiftsWorked[worker] += 1;

        // try building the rest of the schedule with the current worker selected
        bool status = recurse(avail, worker, dailyNeed, maxShifts, shiftsWorked, sched, day, schedCol);
        if (status) return true;

        // reset if recursion returns false, and try next worker.
        sched[day][schedCol] = INVALID_ID;
        shiftsWorked[worker] -= 1;
    }

    // if we've iterated through all possible workers for that day and haven't already returned true,
    // no solution exists given the previous entries
    return false;
}

/**
 *   Helper function to correctly advance to the next sched and avail location
 *   (if we're at the end of the sched matrix, move to the next day and reset the column and next worker to 0.)
*/
bool recurse(
    const AvailabilityMatrix& avail, 
    Worker_T currWorker,
    const size_t dailyNeed, 
    const size_t maxShifts,
    std::vector<size_t>& shiftsWorked, 
    DailySchedule& sched, 
    size_t day,
    size_t schedCol
)
{
    // we're on the last column of sched
    if (schedCol == sched[0].size() - 1) {
        // advance down one row, make sure to reset nextWorker to 0 and sched column to 0
        return buildSchedule(avail, 0U, dailyNeed, maxShifts, shiftsWorked, sched, day+1, 0U);
    }
    // else just go to the next column for both avail and sched.
    return buildSchedule(avail, currWorker+1, dailyNeed, maxShifts, shiftsWorked, sched, day, schedCol+1);
}