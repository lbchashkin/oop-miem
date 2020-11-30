#ifndef MEGATASK_H
#define MEGATASK_H
#include "task.h"
#include "deadlinetask.h"
#include "taskdb.h"
#include <QVector>
#include <cmath>
#include <memory>

class MegaTask: public Task
{
public:
    friend TaskDB;
    MegaTask(const MegaTask& megatask);
    virtual int getCompletion() const;
    virtual void setCompletion(const int percentage);
    void sort(int by=0);
    virtual QString getType() const;
    void operator+(Task& task);
    void operator-(Task& task);
    void operator+(DeadlineTask& task);
    void operator-(DeadlineTask& task);
    void operator+(MegaTask& task);
    void operator-(MegaTask& task);
    int operator[](int index);
    virtual void operator>>(int completion);
    virtual void operator<<(int completion);
    virtual ~MegaTask() {};
private:
    MegaTask(TaskDB *base, const QString& name="", const QString& description="");
    bool compare0(int id1, int id2) const;
    bool compare1(int id1, int id2) const;
    bool compare2(int id1, int id2) const;
    QVector<int> tasks;
    Task* getTaskFromBase(int id) const;
    TaskDB* _base;
};

template <class T>
bool sorting(T* array, int size, bool (*compare)(T, T));

template <class T>
bool vector_sorting(QVector<T> array, bool (*compare)(T, T));

#endif // MEGATASK_H
