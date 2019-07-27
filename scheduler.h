#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <QObject>
#include <QQueue>
#include "task.h"
#include <QDebug>

class Scheduler : public QObject
{
    Q_OBJECT
private:
    QQueue<Task> queue;
    bool isWorking = false;
    void process();
public:
    explicit Scheduler();

signals:
    void executeTask(Task& task);

public slots:
    void start();
    void stop();
    void addTask(Task& task);
    void removeTask();
    void clear();

    int tasksInQueue() const;
};

#endif // SCHEDULER_H
