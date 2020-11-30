#include "deadlinetask.h"

DeadlineTask::DeadlineTask(const QString &name, const QString &description, const QDate &date):
    Task(name, description)
{
    setDeadline(date);
}

DeadlineTask::DeadlineTask(const DeadlineTask &task):
    Task(task)
{
    setDeadline(task.getDeadline());
}

QDate DeadlineTask::getDeadline() const
{
    return _deadline;
}

void DeadlineTask::setDeadline(const QDate &date)
{
    _deadline = date;
}

unsigned int DeadlineTask::getDaysToDeadline() const
{
    if (_deadline > QDate::currentDate())
        return QDate::currentDate().daysTo(_deadline);
    else
        return 0;
}

QString DeadlineTask::getType() const
{
    return "DeadlineTask";
}
