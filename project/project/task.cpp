#include "task.h"

Task::Task(const QString &name, const QString &description)
{
    setName(name);
    setDescription(description);
    setCompletion(0);
    _isdeleted = false;
    _parentid = 0;
}

int Task::getParentId() const
{
    return _parentid;
}

Task::Task(const Task &task)
{
    _id = task.getId();
    setName(task.getName());
    setDescription(task.getDescription());
    setCompletion(task.getCompletion());
    _isdeleted = task.isDeleted();
    _parentid = task.getParentId();
}

bool Task::isDeleted() const
{
    return _isdeleted;
}

QString Task::getType() const
{
    return "Task";
}

void Task::operator>>(int completion)
{
    setCompletion(completion);
    _isdeleted = true;
}

void Task::operator<<(int completion)
{
    setCompletion(completion);
    _isdeleted = false;
}

void Task::setName(const QString &name)
{
    if (name.isEmpty())
        _name = QString("Task #%1").arg(_id);
    else
        _name = name;
}

void Task::setDescription(const QString &description)
{
    _description = description;
}

void Task::setCompletion(const int percentage)
{
    if (percentage <= 0)
        _completion = 0;
    else if (percentage >= 100)
        _completion = 100;
    else
        _completion = percentage;
}

QString Task::getName() const
{
    return _name;
}

QString Task::getDescription() const
{
    return _description;
}

int Task::getCompletion() const
{
    return _completion;
}

int Task::getId() const
{
    return _id;
}

