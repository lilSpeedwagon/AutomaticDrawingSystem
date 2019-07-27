#include "task.h"

unsigned long Task::idCounter = 0;

Task::Task(QFile &file) : file(file)
{
    id = idCounter++;
}

bool Task::isStarted() const    {
    return started;
}

bool Task::isFinished() const   {
    return finished;
}

void Task::start()  {
    started = true;
}

void Task::finish(Result result) {
    this->result = result;
    finished = true;
}

void Task::cancel() {
    started = false;
    finished = false;
}

Task::Result Task::getResult() const  {
    return result;
}

QFile& Task::getFile()   {
    return file;
}
