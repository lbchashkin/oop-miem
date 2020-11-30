#ifndef DEADLINETASK_H
#define DEADLINETASK_H
#include "task.h"
#include <QDate>

class DeadlineTask: public Task
{
public:
    friend TaskDB;
    DeadlineTask(const DeadlineTask& task);
    QDate getDeadline() const;
    void setDeadline(const QDate& date);
    unsigned int getDaysToDeadline() const;
    virtual QString getType() const;
    virtual ~DeadlineTask() {};
private:
    DeadlineTask(const QString& name="", const QString& description="", const QDate& date=QDate::currentDate());
    QDate _deadline;

};

#endif // DEADLINETASK_H
