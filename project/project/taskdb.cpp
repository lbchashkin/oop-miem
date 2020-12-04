#include "taskdb.h"
#include <iostream>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

TaskDB::TaskDB() {
    //Конструктор по умолчанию
    id = 0;
}

Task TaskDB::createNewTask(const QString &name, const QString &description) {
    //Создание задачи
    Task* task = new Task(++id, name, description);
    base.insert(id, task);
    return *task;
}

DeadlineTask TaskDB::createNewDeadlineTask(const QString &name, const QString &description, const QDate &date) {
    //Создание задачи с дедлайном
    DeadlineTask* task = new DeadlineTask(++id, name, description, date);
    base.insert(id, task);
    return *task;
}

MegaTask TaskDB::createNewMegaTask(const QString &name, const QString &description) {
    //Создание мегазадачи
    MegaTask* task = new MegaTask(++id, this, name, description);
    base.insert(id, task);
    return *task;
}

void TaskDB::taskUpdate(const Task &task) {
    //Обновление задачи
    if (base.contains(task.getId())) {
        delete base[task.getId()];
        base[task.getId()] = new Task(task);
    }
    else
        throw std::out_of_range("ID error");

}

void TaskDB::taskUpdate(const DeadlineTask &task) {
    //Обновление задачи с дедлайном
    if (base.contains(task.getId())) {
        delete base[task.getId()];
        base[task.getId()] = new DeadlineTask(task);
    }
    else
        throw std::out_of_range("ID error");
}

void TaskDB::taskUpdate(const MegaTask &task) {
    //Обновление мегазадачи
    if (base.contains(task.getId())) {
        delete base[task.getId()];
        base[task.getId()] = new MegaTask(task);
    }
    else
        throw std::out_of_range("ID error");
}

void TaskDB::removeById(const int id) {
    //Удаление по id
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

bool TaskDB::isId(const int id) const {
    //Является ли значение id
    return base.contains(id);
}

bool TaskDB::toJson(QString filename) const {
    //Сохранение в JSON
    if (!filename.endsWith(".json"))
        return false;
    else {
        QFile file(filename);
        if (file.open(QFile::WriteOnly)) {
            bool result = true;
            QJsonArray array;
            for (auto value: base)
                array.append(value->toJsonObject());
            QJsonDocument document;
            document.setArray(array);
            if (file.write(document.toJson()) == -1)
                result = false;
            file.close();
            return result;
        }
        return false;
    }
}

bool TaskDB::fromJson(QString filename) {
    //Загрузка из JSON
    for (auto value: base)
        delete value;
    base.clear();
    QFile file(filename);
    if (file.open(QFile::ReadOnly) and filename.endsWith(".json")) {
        QJsonDocument document = QJsonDocument::fromJson(file.readAll());
        QJsonArray array = document.array();
        for (auto value: array) {
            QJsonObject object = value.toObject();
            Task* task = getTaskFromQJsonObject(object);
            base.insert(task->getId(), task);
        }
        file.close();
        return true;
    }
    else
        return false;
}

QString TaskDB::getTypeById(int id) const {
    //Получение типа по id
    if (base.contains(id)) return base[id]->getType();
    else
        throw std::out_of_range("ID error");
}

Task TaskDB::getTaskById(int id) const {
    //Получение задачи по id
    if (base.contains(id)) {
        if (base[id]->getType() == "Task")
            return Task(*base[id]);
        else
            throw std::domain_error("Type error");
    }
    else
        throw std::out_of_range("ID Error");
}

DeadlineTask TaskDB::getDeadlineTaskById(int id) const {
    //Получение задачи с дедлайном по id
    if (base.contains(id)) {
        if (base[id]->getType() == "DeadlineTask")
            return DeadlineTask(*(DeadlineTask*)base[id]);
        else
            throw std::domain_error("Type error");
    }
    else
        throw std::out_of_range("ID Error");
}

MegaTask TaskDB::getMegaTaskById(int id) const {
    //Получение мегазадачи по id
    if (base.contains(id)) {
        if (base[id]->getType() == "MegaTask")
            return MegaTask(*(MegaTask*)base[id]);
        else
            throw std::domain_error("Type error");
    }
    else
        throw std::out_of_range("ID Error");
}

void TaskDB::printTasks() const {
    //Вывод задач на экран
    std::cout << "Base" << std::endl;
    for (auto value: base) {
        QString type = value->getType();
        if (type == "Task")
            std::cout << *value;
        else if (type == "DeadlineTask")
            std::cout << *(DeadlineTask*)value;
        else
            std::cout << *(MegaTask*)value;
    }
}

TaskDB::~TaskDB() {
    //Деструктор
    for (auto value: base)
        delete value;
    base.clear();
}

Task *TaskDB::getTaskFromQJsonObject(QJsonObject object) {
    //QJsonObject -> Task*
    Task* result = NULL;
    if (object["Type"] == "Task") {
        result = new Task(object["Id"].toInt(), object["Name"].toString(), object["Description"].toString());
        result->_parentid = object["ParentId"].toInt();
        if (object["isDeleted"].toBool())
            result->operator>>(object["Completion"].toInt());
        else
            result->operator<<(object["Completion"].toInt());
    }
    else if (object["Type"] == "DeadlineTask") {
        QDate date = QDate::fromString(object["Deadline"].toString(), "dd.MM.yyyy");
        result = new DeadlineTask(object["Id"].toInt(), object["Name"].toString(), object["Description"].toString(), date);
        result->_parentid = object["ParentId"].toInt();
        if (object["isDeleted"].toBool())
            result->operator>>(object["Completion"].toInt());
        else
            result->operator<<(object["Completion"].toInt());
    }
    else if (object["Type"] == "MegaTask") {
        result = new MegaTask(object["Id"].toInt(), this, object["Name"].toString(), object["Description"].toString());
        result->_parentid = object["ParentId"].toInt();
        for (auto id: object["Tasks"].toArray())
            ((MegaTask*)result)->tasks.append(id.toInt());
        if (object["isDeleted"].toBool())
            result->operator>>(object["Completion"].toInt());
        else
            result->operator<<(object["Completion"].toInt());
    }
    return result;
}
