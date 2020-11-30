#include "taskdb.h"

TaskDB::TaskDB()
{
    id = 0;
}

Task TaskDB::createNewTask(const QString &name, const QString &description)
{
    Task* task = new Task(name, description);
    task->_id = ++id;
    base.insert(id, task);
    return *task;
}

DeadlineTask TaskDB::createNewDeadlineTask(const QString &name, const QString &description, const QDate &date)
{
    DeadlineTask* task = new DeadlineTask(name, description, date);
    task->_id = ++id;
    base.insert(id, task);
    return *task;
}

MegaTask TaskDB::createNewMegaTask(const QString &name, const QString &description)
{
    MegaTask* task = new MegaTask(this, name, description);
    task->_id = ++id;
    base.insert(id, task);
    return *task;
}

void TaskDB::taskUpdate(const Task &task)
{
    if (base.contains(task.getId())) {
        delete base[task.getId()];
        base[task.getId()] = new Task(task);
    }
    else
        throw std::out_of_range("ID error");

}

void TaskDB::taskUpdate(const DeadlineTask &task)
{
    if (base.contains(task.getId())) {
        delete base[task.getId()];
        base[task.getId()] = new DeadlineTask(task);
    }
    else
        throw std::out_of_range("ID error");
}

void TaskDB::taskUpdate(const MegaTask &task)
{
    if (base.contains(task.getId())) {
        delete base[task.getId()];
        base[task.getId()] = new MegaTask(task);
    }
    else
        throw std::out_of_range("ID error");
}

void TaskDB::removeById(const int id)
{
    if (base.contains(id)) {
        Task* tmp = base[id];
        if (tmp->getParentId()) {
            ((MegaTask*)base[tmp->getParentId()])->operator-(*tmp);
            taskUpdate(*(MegaTask*)base[tmp->getParentId()]);
        }
        delete base[id];
        base.remove(id);
    }
    else
        throw std::out_of_range("ID error");
}

QString TaskDB::getTypeById(int id) const
{
    if (base.contains(id)) return base[id]->getType();
    else
        throw std::out_of_range("ID error");
}

Task TaskDB::getTaskById(int id) const
{
    if (base.contains(id)) {
        if (base[id]->getType() == "Task")
            return Task(*base[id]);
        else
            throw std::domain_error("Type error");
    }
    else
        throw std::out_of_range("ID Error");
}

DeadlineTask TaskDB::getDeadlineTaskById(int id) const
{
    if (base.contains(id)) {
        if (base[id]->getType() == "DeadlineTask")
            return DeadlineTask(*(DeadlineTask*)base[id]);
        else
            throw std::domain_error("Type error");
    }
    else
        throw std::out_of_range("ID Error");
}

MegaTask TaskDB::getMegaTaskById(int id) const
{
    if (base.contains(id)) {
        if (base[id]->getType() == "MegaTask")
            return MegaTask(*(MegaTask*)base[id]);
        else
            throw std::domain_error("Type error");
    }
    else
        throw std::out_of_range("ID Error");
}
