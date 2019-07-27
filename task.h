#ifndef TASK_H
#define TASK_H

#include <QFile>

class Task
{
public:
    enum Result { SUCCESS, FAULT, NONE };

private:
    unsigned long id;
    QFile &file;
    bool started = false;
    bool finished = false;
    Result result = NONE;

    static unsigned long idCounter;

public:
    Task(QFile &file);
    void start();
    void finish(Result result);
    void cancel();

    bool isStarted() const;
    bool isFinished() const;

    Result getResult() const;

    QFile& getFile();
};

#endif // TASK_H
