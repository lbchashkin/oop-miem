#include "megatask.h"
#include <iostream>
#include <QJsonObject>
#include <QJsonArray>

template <class T>
bool sorting(T* array, int size, std::function<bool(T, T)> compare) {
    //Сортировка массива
    bool is_sorted = true;
    for (int i = 0; i<size-1; i++)
        for (int j = i+1; j<size; j++)
            if (compare(array[i], array[j])) {
                is_sorted = false;
                T tmp = array[i];
                array[i] = array[j];
                array[j] = tmp;
    }
    return !is_sorted;
}

template <class T>
bool vector_sorting(QVector<T>& array, std::function<bool(T, T)> compare) {
    //Сортировка QVector
    bool is_sorted = true;
    int size = array.size();
    for (int i = 0; i<size-1; i++)
        for (int j = i+1; j<size; j++)
            if (compare(array[i], array[j])) {
                is_sorted = false;
                array.swapItemsAt(i, j);
    }
    return !is_sorted;
}

std::ostream &operator<<(std::ostream &out, const MegaTask& task) {
    //Перегрузка оператора вывода
    int id = task.getParentId();
    out << "MegaTask" << std::endl;
    out << "Id: " << task.getId() << std::endl;
    if (id)
        out << "Parent Id: " << id << std::endl;
    out << "Name: " << task.getId() << std::endl;
    out << "Description: " << task.getDescription().toStdString() << std::endl;
    out << "Completion: " << task.getCompletion() << "%" << std::endl;
    out << "Number of Tasks: " << task.getSize() << std::endl;
    if (task.isDeleted())
        out << "DELETED" << std::endl;
    out << "Tasks id:" << std::endl;
    for (int i=0; i<task.getSize(); i++)
        out << QString("#%1 - id:%2").arg(i).arg(task[i]).toStdString() << std::endl;
    return out;
}

Task* MegaTask::getTaskFromBase(int id) const {
    //Получение задачи из базы
    Task* task = NULL;
    QString type = _base->getTypeById(id);
    if (type=="Task")
        task = new Task(_base->getTaskById(id));
    else if (type=="DeadlineTask") {
        task = new DeadlineTask(_base->getDeadlineTaskById(id));
    }
    else if (type=="MegaTask")
        task = new MegaTask(_base->getMegaTaskById(id));
    return task;
}

bool MegaTask::compare0(int id1, int id2) const {
    //Сравнение 2 задачи по имени
    bool result;
    Task* task1 = getTaskFromBase(id1);
    Task* task2 = getTaskFromBase(id2);
    if (task1 && task2) {
        if (task1->getName() > task2->getName())
            result = true;
        else
            result = false;
    }
    else
        result = false;
    if (task1)
        delete task1;
    if (task2)
        delete task2;
    return result;
}

bool MegaTask::compare1(int id1, int id2) const {
    //Сравнение 2 задач по проценту выполнения
    bool result;
    Task* task1 = getTaskFromBase(id1);
    Task* task2 = getTaskFromBase(id2);
    if (task1 && task2) {
        if (task1->getCompletion() > task2->getCompletion())
            result = true;
        else
            result = false;
    }
    else
        result = false;
    if (task1)
        delete task1;
    if (task2)
        delete task2;
    return result;
}

bool MegaTask::compare2(int id1, int id2) const {
    //Сравнение 2 задач по дате дедлайна
    bool result;
    Task* task1 = getTaskFromBase(id1);
    Task* task2 = getTaskFromBase(id2);
    if (task1 && task2) {
        if (task1->getType() == "DeadlineTask")
        {
            if (task2->getType() == "DeadlineTask") {
                result = ((DeadlineTask*)task1)->getDeadline() < ((DeadlineTask*)task2)->getDeadline();}
            else
                result = false;
        }
        else if (task2->getType() == "DeadlineTask")
            result = true;
        else
            result = false;
    }
    else
        result = false;
    if (task1)
        delete task1;
    if (task2)
        delete task2;
    return result;
}

MegaTask::MegaTask(const int id, TaskDB *base, const QString &name, const QString &description):
    Task(id, name, description) {
    //Конструктор инициализации
    _base = base;
}

MegaTask::MegaTask(const MegaTask &megatask):
    Task(megatask) {
    //Конструктор копирования
    _base = megatask._base;
    for (int i = 0; i<megatask.tasks.size(); i++)
        tasks.push_back(megatask.tasks[i]);
}

int MegaTask::getCompletion() const {
    //Получение значения процента выполнения
    float sum=0;
    for (int i = 0; i<tasks.size(); i++) {
        QString type = _base->getTypeById(tasks[i]);
        if (type=="Task")
            sum += _base->getTaskById(tasks[i]).getCompletion();
        else if (type=="DeadlineTask")
            sum += _base->getDeadlineTaskById(tasks[i]).getCompletion();
        else if (type=="MegaTask")
            sum += _base->getMegaTaskById(tasks[i]).getCompletion();
    }
    if (tasks.size())
        return std::round(sum/tasks.size());
    else
        return sum;
}

void MegaTask::setCompletion(const int percentage) {
    //Установка значения процента выполнения
    for (int i=0; i<tasks.size(); i++) {
        Task* task = getTaskFromBase(tasks[i]);
        task->setCompletion(percentage);
        if (task->getType() == "Task")
            _base->taskUpdate(*task);
        else if (task->getType() == "DeadlineTask")
            _base->taskUpdate(*(DeadlineTask*)task);
        else if (task->getType() == "MegaTask")
            _base->taskUpdate(*(MegaTask*)task);
        delete task;
    }
}

void MegaTask::sort(int by) {
    //Сортировка (by = 0/1/2)
    if (by==0)
        vector_sorting<int>(tasks, {[this](int id1, int id2){return this->compare0(id1, id2);}});
    else if (by==1)
        vector_sorting<int>(tasks, {[this](int id1, int id2){return this->compare1(id1, id2);}});
    else if (by==2)
        vector_sorting<int>(tasks, {[this](int id1, int id2){return this->compare2(id1, id2);}});
    else
        throw std::invalid_argument("Argument 'by' should be 0, 1 or 2");
}

QString MegaTask::getType() const {
    //Получение типа
    return "MegaTask";
}

int MegaTask::getSize() const {
    //Получение количества подзадач
    return tasks.size();
}

void MegaTask::operator+(Task &task) {
    //Добавление задачи
    if (!tasks.contains(task.getId()) and !task.getParentId())
    {
        try {
            _base->getTaskById(task.getId());
            tasks.push_back(task.getId());
            task._parentid = this->getId();
            _base->taskUpdate(task);
        }  catch (std::out_of_range) {
            throw std::invalid_argument("ID error");
        } catch(std::domain_error) {
            throw std::invalid_argument("ID error");
        }
    }
    else
        throw std::invalid_argument("ID error");
}

void MegaTask::operator-(Task &task) {
    //Удаление задачи
    if (tasks.contains(task.getId())) {
        task._parentid = 0;
        _base->taskUpdate(task);
        tasks.removeAll(task.getId());
    }
}

void MegaTask::operator+(DeadlineTask &task) {
    //Добавление задачи с дедлайном
    if (!tasks.contains(task.getId()) and !task.getParentId())
    {
        try {
            _base->getDeadlineTaskById(task.getId());
            tasks.push_back(task.getId());
            task._parentid = this->getId();
            _base->taskUpdate(task);
        }  catch (std::out_of_range) {
            throw std::invalid_argument("ID error");
        } catch(std::domain_error) {
            throw std::invalid_argument("ID error");
        }
    }
    else
        throw std::invalid_argument("ID error");
}

void MegaTask::operator-(DeadlineTask &task) {
    //Удаление задачи с дедлайном
    if (tasks.contains(task.getId())) {
        task._parentid = 0;
        _base->taskUpdate(task);
        tasks.removeAll(task.getId());
    }
}

void MegaTask::operator+(MegaTask &task) {
    //Добавление мегазадачи
    if (!tasks.contains(task.getId()) and !task.getParentId())
    {
        try {
            _base->getMegaTaskById(task.getId());
            int parentid = 0;
            Task* tmp = this;
            while (tmp->_parentid) {
                if (parentid)
                    delete tmp;
                tmp = getTaskFromBase(tmp->_parentid);
                parentid = tmp->getId();
            }
            if (parentid && parentid == getId())
                throw std::invalid_argument("ID error");
            tasks.push_back(task.getId());
            task._parentid = this->getId();
            _base->taskUpdate(task);
        }  catch (std::out_of_range) {
            throw std::invalid_argument("ID error");
        } catch(std::domain_error) {
            throw std::invalid_argument("ID error");
        }
    }
    else
        throw std::invalid_argument("ID error");
}

void MegaTask::operator-(MegaTask &task) {
    //Удаление мегазадачи
    if (tasks.contains(task.getId())) {
        task._parentid = 0;
        _base->taskUpdate(task);
        tasks.removeAll(task.getId());
    }
}

int MegaTask::operator[](int index) const {
    //Перегрузка []
    return tasks[index];
}

void MegaTask::operator>>(int completion) {
    //Установить процент выполнения всем задачам и удалить все задачи
    for (int i=0; i<tasks.size(); i++) {
        Task* task = getTaskFromBase(tasks[i]);
        task->operator>>(completion);
        if (task->getType() == "Task")
            _base->taskUpdate(*task);
        else if (task->getType() == "DeadlineTask")
            _base->taskUpdate(*(DeadlineTask*)task);
        else if (task->getType() == "MegaTask")
            _base->taskUpdate(*(MegaTask*)task);
        delete task;
    }
    _isdeleted = true;
}

void MegaTask::operator<<(int completion) {
    //Установить процент выполнения и снять удаление всех задач
    for (int i=0; i<tasks.size(); i++) {
        Task* task = getTaskFromBase(tasks[i]);
        task->operator<<(completion);
        if (task->getType() == "Task")
            _base->taskUpdate(*task);
        else if (task->getType() == "DeadlineTask")
            _base->taskUpdate(*(DeadlineTask*)task);
        else if (task->getType() == "MegaTask")
            _base->taskUpdate(*(MegaTask*)task);
        delete task;
    }
    _isdeleted = false;
}

QJsonObject MegaTask::toJsonObject() const {
    //Сохранение информации в QJsonObject
    QJsonObject object;
    object.insert("Type", getType());
    object.insert("Id", getId());
    object.insert("ParentId", getParentId());
    object.insert("Name", getName());
    object.insert("Description", getDescription());
    object.insert("isDeleted", isDeleted());
    QJsonArray array;
    for (int i=0; i<getSize(); i++)
        array.append(tasks[i]);
    object.insert("Tasks", array);
    return object;
}
