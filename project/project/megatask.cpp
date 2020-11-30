#include "megatask.h"

template <class T>
bool sorting(T* array, int size, std::function<bool(int, int)> compare)
{
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
bool vector_sorting(QVector<T> array, std::function<bool(int, int)> compare)
{
    bool is_sorted = true;
    int size = array.size();
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

Task* MegaTask::getTaskFromBase(int id) const {
    Task* task = NULL;
    QString type = _base->getTypeById(id);
    if (type=="Task")
        task = new Task(_base->getTaskById(id));
    else if (type=="DeadlineTask")
        task = new Task(_base->getDeadlineTaskById(id));
    else if (type=="MegaTask")
        task = new Task(_base->getMegaTaskById(id));
    return task;
}

bool MegaTask::compare0(int id1, int id2) const
{
    bool result;
    Task* task1 = getTaskFromBase(id1);
    Task* task2 = getTaskFromBase(id2);
    if (task1 && task2) {
        if (task1->getName() < task2->getName())
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

bool MegaTask::compare1(int id1, int id2) const
{
    bool result;
    Task* task1 = getTaskFromBase(id1);
    Task* task2 = getTaskFromBase(id2);
    if (task1 && task2) {
        if (task1->getCompletion() < task2->getCompletion())
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

bool MegaTask::compare2(int id1, int id2) const
{
    bool result;
    Task* task1 = getTaskFromBase(id1);
    Task* task2 = getTaskFromBase(id2);
    if (task1 && task2) {
        if (task1->getType() == "DeadlineTask")
        {
            if (task2->getType() == "DeadlineTask")
                result = ((DeadlineTask*)task1)->getDeadline() < ((DeadlineTask*)task1)->getDeadline() ? true : false;
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

MegaTask::MegaTask(TaskDB *base, const QString &name, const QString &description):
    Task(name, description)
{
    _base = base;
}

MegaTask::MegaTask(const MegaTask &megatask):
    Task(megatask)
{
    _base = megatask._base;
    for (int i = 0; i<megatask.tasks.size(); i++)
        tasks.push_back(megatask.tasks[i]);
}

int MegaTask::getCompletion() const
{
    int sum=0;
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

void MegaTask::setCompletion(const int percentage)
{
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

void MegaTask::sort(int by)
{
    if (by==0)
        vector_sorting<int>(tasks, {[this](int id1, int id2){return this->compare0(id1, id2);}});
    else if (by==1)
        vector_sorting<int>(tasks, {[this](int id1, int id2){return this->compare1(id1, id2);}});
    else if (by==2)
        vector_sorting<int>(tasks, {[this](int id1, int id2){return this->compare2(id1, id2);}});
    else
        throw std::invalid_argument("Argument 'by' should be 0, 1 or 2");
}

QString MegaTask::getType() const
{
    return "MegaTask";
}

void MegaTask::operator+(Task &task)
{
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

void MegaTask::operator-(Task &task)
{
    if (tasks.contains(task.getId()))
        tasks.removeAll(task.getId());
}

void MegaTask::operator+(DeadlineTask &task)
{
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

void MegaTask::operator-(DeadlineTask &task)
{
    if (tasks.contains(task.getId()))
        tasks.removeAll(task.getId());
}

void MegaTask::operator+(MegaTask &task)
{
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

void MegaTask::operator-(MegaTask &task)
{
    if (tasks.contains(task.getId()))
        tasks.removeAll(task.getId());
}

int MegaTask::operator[](int index)
{
    return tasks[index];
}

void MegaTask::operator>>(int completion)
{
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

void MegaTask::operator<<(int completion)
{
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
    _isdeleted = false;
}
