#include "scheduler.h"

Scheduler::Scheduler()
{
    setName("Scheduler");
}

void Scheduler::start() {
    log("start executing tasks");
    task_counter = queue.size();
    success_counter = fault_counter = 0;
    process();
}

void Scheduler::stop()  {
    isWorking = false;
    log("execution cancelled");
    log("Tasks " + Utils::toStr(task_counter) + ", success " + Utils::toStr(success_counter) + ", fault " + Utils::toStr(fault_counter));
}

void Scheduler::addTask(Task& task) {
    queue.enqueue(task);
}

void Scheduler::process()  {
    isWorking = true;

    if (!queue.empty()) {
        Task nextTask = queue.dequeue();
        nextTask.start();
        executeTask(nextTask);
    } else {
        isWorking = false;
        log("execution tasks done");
        log("Tasks " + Utils::toStr(task_counter) + ", success " + Utils::toStr(success_counter) + ", fault " + Utils::toStr(fault_counter));
    }
}

void Scheduler::removeTask()    {

}

void Scheduler::clear() {
    stop();

}

int Scheduler::tasksInQueue() const  {
    return queue.size();
}

void Scheduler::taskFinished(Task task)    {
    Task::Result result = task.getResult();
    switch (result) {

    case Task::SUCCESS:
        success_counter++;
        log("task " + task.getFile().fileName() + " successfully finished.");
        break;

    case Task::FAULT:
        fault_counter++;
        log("task " + task.getFile().fileName() + " fault.");
        break;

    case Task::CANCEL:
        fault_counter++;
        log("task " + task.getFile().fileName() + " was cancelled.");
        break;

    default:
        log("task " + task.getFile().fileName() + " finished with unhandling code.");
        break;
    }

    process();
}
