#ifndef TASKDB_H
#define TASKDB_H
#include "task.h"
#include "deadlinetask.h"
#include "megatask.h"
#include <QMap>

class TaskDB
{
public:
    TaskDB();
    Task createNewTask(const QString& name="", const QString& description="");
    DeadlineTask createNewDeadlineTask(const QString& name="", const QString& description="", const QDate& date=QDate::currentDate());
    MegaTask createNewMegaTask(const QString& name="", const QString& description="");
    void taskUpdate(const Task& task);
    void taskUpdate(const DeadlineTask& task);
    void taskUpdate(const MegaTask& task);
    void removeById(const int id);
    bool isId(const int id) const;
    bool toJson(QString filename) const;
    bool fromJson(QString filename);
    QString getTypeById(int id) const;
    Task getTaskById(int id) const;
    DeadlineTask getDeadlineTaskById(int id) const;
    MegaTask getMegaTaskById(int id) const;
    void printTasks() const;
    ~TaskDB();
private:
    Task* getTaskFromQJsonObject(QJsonObject object);
    int id;
    QMap<int, Task*> base;
};

#endif // TASKDB_H
