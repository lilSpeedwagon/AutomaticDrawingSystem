#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <QObject>
#include <QQueue>
#include "task.h"
#include <QDebug>
#include "logclient.h"
#include "utils.h"

class Scheduler : public QObject, LogClient
{
    Q_OBJECT
private:
    QQueue<Task> queue;
    bool isWorking = false;
    int success_counter, fault_counter, task_counter;

    void process();
public:
    explicit Scheduler();
    int tasksInQueue() const;

signals:
    void executeTask(Task& task);

public slots:
    void start();
    void stop();
    void addTask(Task& task);
    void removeTask();
    void clear();

    void taskFinished(Task&);
};

#endif // SCHEDULER_H
