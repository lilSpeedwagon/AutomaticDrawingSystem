#include "scheduler.h"

Scheduler::Scheduler()
{

}

void Scheduler::start() {
    qDebug() << "Scheduler: start";
    isWorking = true;
    process();
    qDebug() << "Scheduler: done";
}

void Scheduler::stop()  {
    isWorking = false;
}

void Scheduler::addTask(Task& task) {
    queue.enqueue(task);
}

void Scheduler::process()  {
    while(isWorking) {
        if (!queue.empty()) {
            Task nextTask = queue.dequeue();
            executeTask(nextTask);
        } else {
            isWorking = false;
        }
    }

}

void Scheduler::removeTask()    {

}

void Scheduler::clear() {

}

int Scheduler::tasksInQueue() const  {
    return queue.size();
}
