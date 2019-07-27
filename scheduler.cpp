#include "scheduler.h"

Scheduler::Scheduler()
{
    setName("Scheduler");
}

void Scheduler::start() {
    isWorking = true;
    process();
}

void Scheduler::stop()  {
    isWorking = false;
    log("execution cancelled");
}

void Scheduler::addTask(Task& task) {
    queue.enqueue(task);
}

void Scheduler::process()  {
    log("start executing tasks");
    task_counter = queue.size();
    success_counter = fault_counter = 0;

    while(isWorking) {
        if (!queue.empty()) {
            Task nextTask = queue.dequeue();            
            executeTask(nextTask);
        } else {
            isWorking = false;
        }
    }
    log("execution tasks done");
    log("Tasks " + Utils::toStr(task_counter) + ", success " + Utils::toStr(success_counter) + ", fault " + Utils::toStr(fault_counter));
}

void Scheduler::removeTask()    {

}

void Scheduler::clear() {

}

int Scheduler::tasksInQueue() const  {
    return queue.size();
}

void Scheduler::taskFinished(Task& task)    {
    if (task.getResult() == Task::SUCCESS)  {
        success_counter++;
    }
    if (task.getResult() == Task::FAULT)    {
        fault_counter++;
    }
}
