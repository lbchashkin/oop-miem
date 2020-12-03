#include <iostream>
#include "taskdb.h"

using namespace std;

void testTaskDefaultConstructor(Task &task)
{
    //Тестирование конструктора по умолчанию для Task
    assert(task.getParentId() == 0);
    assert(task.getName() == QString("Task #%1").arg(task.getId()));
    assert(task.getDescription().isEmpty());
    assert(task.getCompletion() == 0);
    assert(!task.isDeleted());
}

void testTaskInitializationConstructor(Task &task, QString name, QString description)
{
    //Тестирование конструктора инициализации для Task
    assert(task.getParentId() == 0);
    assert(task.getName() == name);
    assert(task.getDescription() == description);
    assert(task.getCompletion() == 0);
    assert(!task.isDeleted());
}

void testDeadlineTaskDefaultConstructor(DeadlineTask &task)
{
    //Тестирование конструктора по умолчанию для DeadlineTask
    assert(task.getParentId() == 0);
    assert(task.getName() == QString("Task #%1").arg(task.getId()));
    assert(task.getDescription().isEmpty());
    assert(task.getCompletion() == 0);
    assert(task.getDeadline() == QDate::currentDate());
    assert(!task.isDeleted());
}

void testDeadlineTaskInitializationConstructor(DeadlineTask &task, QString name, QString description, QDate date)
{
    //Тестирование конструктора инициализации для DeadlineTask
    assert(task.getParentId() == 0);
    assert(task.getName() == name);
    assert(task.getDescription() == description);
    assert(task.getCompletion() == 0);
    assert(task.getDeadline() == date);
    assert(!task.isDeleted());
}

void testTaskSetters(Task& task)
{
    //Тестирование сеттеров для Task
    task.setName("My task");
    assert(task.getName() == "My task");
    task.setDescription("Some description");
    assert(task.getDescription() == "Some description");
    task.setCompletion(90);
    assert(task.getCompletion() == 90);
    task >> 100;
    assert(task.getCompletion() == 100);
    assert(task.isDeleted());
    task << 95;
    assert(task.getCompletion() == 95);
    assert(!task.isDeleted());
}

void testDeadlineTaskSetters(DeadlineTask& task)
{
    //Тестирование сеттеров для DeadlineTask
    task.setName("My task");
    assert(task.getName() == "My task");
    task.setDescription("Some description");
    assert(task.getDescription() == "Some description");
    task.setCompletion(90);
    assert(task.getCompletion() == 90);
    task >> 100;
    assert(task.getCompletion() == 100);
    assert(task.isDeleted());
    task << 95;
    assert(task.getCompletion() == 95);
    assert(!task.isDeleted());
    task.setDeadline(QDate(2020, 12, 31));
    assert(task.getDeadline() == QDate(2020, 12, 31));
}

void testTaskCopyConstructor(Task& task)
{
    //Тестирование копирующего конструктора для Task
    Task task2(task);
    assert(task2.getId() == task.getId());
    assert(task2.getParentId() == task.getParentId());
    assert(task2.getName() == task.getName());
    assert(task2.getDescription() == task.getDescription());
    assert(task2.getCompletion() == task.getCompletion());
    assert(task2.isDeleted() == task.isDeleted());
}

void testDeadlineTaskCopyConstructor(DeadlineTask& task)
{
    //Тестирование копирующего конструктора для DeadlineTask
    DeadlineTask task2(task);
    assert(task2.getId() == task.getId());
    assert(task2.getParentId() == task.getParentId());
    assert(task2.getName() == task.getName());
    assert(task2.getDescription() == task.getDescription());
    assert(task2.getCompletion() == task.getCompletion());
    assert(task2.getDeadline() == task.getDeadline());
    assert(task2.isDeleted() == task.isDeleted());
}

void testTaskUpdate(TaskDB& base, Task task)
{
    //Тестирование обновления задачи в базе
    task.setName("New name");
    task.setDescription("New Description");
    task>>100;
    base.taskUpdate(task);
    assert(base.getTypeById(task.getId()) == "Task");
    Task task2 = base.getTaskById(task.getId());
    assert(task2.getId() == task.getId());
    assert(task2.getParentId() == task.getParentId());
    assert(task2.getName() == task.getName());
    assert(task2.getDescription() == task.getDescription());
    assert(task2.getCompletion() == task.getCompletion());
    assert(task2.isDeleted() == task.isDeleted());
}

Task* getTask(TaskDB& base, int id)
{
    //Получение задачи из базы
    if (base.isId(id)) {
        QString type = base.getTypeById(id);
        if (type == "Task")
            return new Task(base.getTaskById(id));
        else if (type == "DeadlineTask")
            return new DeadlineTask(base.getDeadlineTaskById(id));
        else
            return new MegaTask(base.getMegaTaskById(id));
    }
    else
        return NULL;
}

void testMegaTaskSetters(MegaTask& task, int id[], TaskDB& base)
{
    //Тестирование сеттеров для MegaTask
    for (int i=0; i<task.getSize(); i++)
        assert(task[i] == id[i]);
    task.setName("My task");
    assert(task.getName() == "My task");
    task.setDescription("Some description");
    assert(task.getDescription() == "Some description");
    task.setCompletion(90);
    for (int i=0; i<task.getSize(); i++) {
        Task* tmp = getTask(base, task[i]);
        if (tmp) {
            if (tmp->getType()!="MegaTask")
                assert(tmp->getCompletion() == 90);
        }
        else
            assert(false);
        delete tmp;
    }
    assert(task.getCompletion() == 60);
    task >> 100;
    for (int i=0; i<task.getSize(); i++) {
        Task* tmp = getTask(base, task[i]);
        if (tmp) {
            if (tmp->getType()!="MegaTask") {
                assert(tmp->getCompletion() == 100);
                assert(tmp->isDeleted());
            }
        }
        else
            assert(false);
        delete tmp;
    }
    assert(task.getCompletion() == 67);
    assert(task.isDeleted());
    task << 95;
    for (int i=0; i<task.getSize(); i++) {
        Task* tmp = getTask(base, task[i]);
        if (tmp) {
            if (tmp->getType()!="MegaTask") {
                assert(tmp->getCompletion() == 95);
                assert(!tmp->isDeleted());
            }
        }
        else
            assert(false);
        delete tmp;
    }
    assert(task.getCompletion() == 63);
    assert(!task.isDeleted());
}

void testTask()
{
    //Тестирование Task и DeadlineTask
    TaskDB base;
    Task task1 = base.createNewTask();
    assert(task1.getType() == "Task");
    testTaskDefaultConstructor(task1);
    testTaskSetters(task1);
    testTaskUpdate(base, task1);
    assert(base.isId(task1.getId()));
    base.removeById(task1.getId());
    assert(!base.isId(task1.getId()));
    Task task2 = base.createNewTask("Task", "...");
    testTaskInitializationConstructor(task2, "Task", "...");
    base.removeById(task2.getId());
    DeadlineTask deadlinetask1 = base.createNewDeadlineTask();
    testDeadlineTaskDefaultConstructor(deadlinetask1);
    base.removeById(deadlinetask1.getId());
    DeadlineTask deadlinetask2 = base.createNewDeadlineTask("Deadline Task", "Test task", QDate(2020, 11, 25));
    testDeadlineTaskInitializationConstructor(deadlinetask2, "Deadline Task", "Test task", QDate(2020, 11, 25));
    base.removeById(deadlinetask2.getId());
    cout << "All Task/DeadlineTask tests are passed" << endl;
}

bool compareMegaTasks(MegaTask& task1, MegaTask& task2)
{
    //Сравнение мегазадач
    if (task1.getId() != task2.getId())
        return false;
    if (task1.getParentId() != task2.getParentId())
        return false;
    if (task1.getSize() != task2.getSize())
        return false;
    if (task1.getCompletion() != task2.getCompletion())
        return false;
    if (task1.getDescription() != task2.getDescription())
        return false;
    if (task1.getName() != task2.getName())
        return false;
    for (int i=0; i<task1.getSize(); i++)
        if (task1[i] != task2[i])
            return false;
    return true;
}


void testSorting() {
    //Тестирование сортировки задач
    TaskDB base;
    Task task1 = base.createNewTask("AAA");
    task1.setCompletion(50);
    Task task2 = base.createNewTask("BBB");
    task2.setCompletion(80);
    Task task3 = base.createNewTask("CCC");
    task3.setCompletion(20);
    MegaTask megatask = base.createNewMegaTask();
    megatask+task3;
    megatask+task2;
    megatask+task1;
    megatask.sort(0);
    assert(megatask[0] == task1.getId());
    assert(megatask[1] == task2.getId());
    assert(megatask[2] == task3.getId());
    megatask.sort(1);
    assert(megatask[0] == task3.getId());
    assert(megatask[1] == task1.getId());
    assert(megatask[2] == task2.getId());
    DeadlineTask task4 = base.createNewDeadlineTask("", "", QDate(2020, 11, 25));
    DeadlineTask task5 = base.createNewDeadlineTask("", "", QDate(2020, 11, 20));
    megatask+task5;
    megatask+task4;
    megatask.sort(2);
    assert(megatask[0] == task4.getId());
    assert(megatask[1] == task5.getId());
    assert(megatask[2] == task2.getId());
    assert(megatask[3] == task3.getId());
    assert(megatask[4] == task1.getId());
}

void testMegaTask()
{
    //Тестирование класса MegaTask
    TaskDB base;
    MegaTask tasks = base.createNewMegaTask();
    testTaskDefaultConstructor(tasks);
    assert(tasks.getType() == "MegaTask");
    MegaTask tasks2 = base.createNewMegaTask("MEGATASK", "Important");
    testTaskInitializationConstructor(tasks2, "MEGATASK", "Important");
    Task task1 = base.createNewTask();
    DeadlineTask task2 = base.createNewDeadlineTask();
    tasks2+tasks;
    assert(tasks2.getSize() == 1);
    assert(tasks.getParentId());
    tasks2+task1;
    assert(tasks2.getSize() == 2);
    tasks2+task2;
    assert(tasks2.getSize() == 3);
    MegaTask copy(tasks2);
    assert(compareMegaTasks(copy, tasks2));
    int id[] = {tasks.getId(), task1.getId(), task2.getId()};
    testMegaTaskSetters(tasks2, id, base);
    assert(tasks.getParentId());
    tasks2-tasks;
    assert(tasks2.getSize() == 2);
    assert(tasks2[0] == task1.getId());
    assert(tasks2[1] == task2.getId());
    assert(!tasks.getParentId());
    base.taskUpdate(tasks2);
    MegaTask megatask = base.getMegaTaskById(tasks2.getId());
    assert(compareMegaTasks(megatask, tasks2));
    testSorting();
    cout << "All MegaTask tests are passed" << endl;
}

int main()
{
    setlocale(LC_ALL, "RUS");
    testTask();
    testMegaTask();
    TaskDB base;
    Task task1 = base.createNewTask("My task");
    DeadlineTask task2 = base.createNewDeadlineTask("Deadline task", "", QDate(2020, 12, 20));
    MegaTask megatask = base.createNewMegaTask();
    megatask+task1;
    megatask+task2;
    base.taskUpdate(megatask);
    base.printTasks();
    return 0;
}
