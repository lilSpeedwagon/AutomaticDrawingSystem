#ifndef TASK_H
#define TASK_H

#include <QFile>

class Task
{
public:
    enum Result { SUCCESS, FAULT, NONE };

private:
    unsigned long id;
    QFile file;
    bool started = false;
    bool finished = false;
    Result result = NONE;

    static unsigned long idCounter;

public:
    Task() {}
    Task(QFile const& file);
    Task(Task const& task);
    ~Task() {}
    void start();
    void finish(Result result);
    void cancel();

    bool isStarted() const;
    bool isFinished() const;

    Result getResult() const;

    QFile& getFile();
};
Q_DECLARE_METATYPE(Task)

#endif // TASK_H
