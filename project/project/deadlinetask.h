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
    virtual QJsonObject toJsonObject() const;
    virtual ~DeadlineTask() {};
private:
    DeadlineTask(const int id, const QString& name="", const QString& description="", const QDate& date=QDate::currentDate());
    QDate _deadline;

};

std::ostream& operator<<(std::ostream& out, const DeadlineTask &task);

#endif // DEADLINETASK_H
