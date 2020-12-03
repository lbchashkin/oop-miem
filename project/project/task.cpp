#include "task.h"

Task::Task(const int id, const QString &name, const QString &description)
{
    //Конструктор инициализации
    _id = id;
    setName(name);
    setDescription(description);
    setCompletion(0);
    _isdeleted = false;
    _parentid = 0;
}

int Task::getParentId() const
{
    //Получение id родителя
    return _parentid;
}

Task::Task(const Task &task)
{
    //Конструктор копирования
    _id = task.getId();
    setName(task.getName());
    setDescription(task.getDescription());
    setCompletion(task.getCompletion());
    _isdeleted = task.isDeleted();
    _parentid = task.getParentId();
}

bool Task::isDeleted() const
{
    //Получение флага удаления задачи
    return _isdeleted;
}

QString Task::getType() const
{
    //Получение типа
    return "Task";
}

void Task::operator>>(int completion)
{
    //>> - выставить процент выполнения и удалить
    setCompletion(completion);
    _isdeleted = true;
}

void Task::operator<<(int completion)
{
    //<< - выставить процент выполнения и снять удаление
    setCompletion(completion);
    _isdeleted = false;
}

void Task::setName(const QString &name)
{
    //Установить имя
    if (name.isEmpty())
        _name = QString("Task #%1").arg(_id);
    else
        _name = name;
}

void Task::setDescription(const QString &description)
{
    //Установить описание
    _description = description;
}

void Task::setCompletion(const int percentage)
{
    //Установить процент выполнения
    if (percentage <= 0)
        _completion = 0;
    else if (percentage >= 100)
        _completion = 100;
    else
        _completion = percentage;
}

QString Task::getName() const
{
    //Получить имя
    return _name;
}

QString Task::getDescription() const
{
    //Получить описание
    return _description;
}

int Task::getCompletion() const
{
    //Получить процент выполнения
    return _completion;
}

int Task::getId() const
{
    //Получить id
    return _id;
}

std::ostream &operator<<(std::ostream &out, const Task &task)
{
    //Перегрузка оператора вывода
    int id = task.getParentId();
    out << "Task" << std::endl;
    out << "Id: " << task.getId() << std::endl;
    if (id)
        out << "Parent Id: " << id << std::endl;
    out << "Name: " << task.getName().toStdString() << std::endl;
    out << "Description: " << task.getDescription().toStdString() << std::endl;
    out << "Completion: " << task.getCompletion() << "%" << std::endl;
    if (task.isDeleted())
        out << "DELETED" << std::endl;
    return out;
}
