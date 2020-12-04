#include "deadlinetask.h"

DeadlineTask::DeadlineTask(const int id, const QString &name, const QString &description, const QDate &date):
    Task(id, name, description) {
    //Конструктор инициализации
    setDeadline(date);
}

DeadlineTask::DeadlineTask(const DeadlineTask &task):
    Task(task) {
    //Конструктор копирования
    setDeadline(task.getDeadline());
}

QDate DeadlineTask::getDeadline() const {
    //Получение дедлайна
    return _deadline;
}

void DeadlineTask::setDeadline(const QDate &date) {
    //Установка дедлайна
    _deadline = date;
}

unsigned int DeadlineTask::getDaysToDeadline() const {
    //Количество дней до дедлайна
    if (_deadline > QDate::currentDate())
        return QDate::currentDate().daysTo(_deadline);
    else
        return 0;
}

QString DeadlineTask::getType() const {
    //Получение типа
    return "DeadlineTask";
}

std::ostream &operator<<(std::ostream &out, const DeadlineTask &task) {
    //Перегрузка оператора вывода
    int id = task.getParentId();
    out << "DeadlineTask" << std::endl;
    out << "Id: " << task.getId() << std::endl;
    if (id)
        out << "Parent Id: " << id << std::endl;
    out << "Name: " << task.getId() << std::endl;
    out << "Description: " << task.getDescription().toStdString() << std::endl;
    out << "Completion: " << task.getCompletion() << "%" << std::endl;
    out << "Deadline: " << task.getDeadline().toString().toLocal8Bit().toStdString() << std::endl;
    if (task.isDeleted())
        out << "DELETED" << std::endl;
    return out;
}

QJsonObject DeadlineTask::toJsonObject() const {
    //Сохранение информации в QJsonOnject
    QJsonObject object;
    object.insert("Type", getType());
    object.insert("Id", getId());
    object.insert("ParentId", getParentId());
    object.insert("Name", getName());
    object.insert("Description", getDescription());
    object.insert("Completion", getCompletion());
    object.insert("Deadline", getDeadline().toString("dd.MM.yyyy"));
    object.insert("isDeleted", isDeleted());
    return object;
}
