#ifndef TASK_H
#define TASK_H

#include <QString>

class TaskDB;
class MegaTask;

class Task
{
public:
    friend TaskDB;
    friend MegaTask;
    Task(const Task& task);
    void setName(const QString& name);
    void setDescription(const QString& description);
    virtual void setCompletion(const int percentage);
    QString getName() const;
    QString getDescription() const;
    virtual int getCompletion() const;
    int getId() const;
    bool isDeleted() const;
    virtual QString getType() const;
    virtual void operator>>(int completion);
    virtual void operator<<(int completion);
    virtual ~Task() {};
    int getParentId() const;
protected:
    Task(const QString& name="", const QString& description="");
private:
    int _id;
    QString _name;
    QString _description;
    int _completion;
    int _parentid;
    bool _isdeleted;
};

#endif // TASK_H
