#ifndef TASKLISTWIDGETITEM_H
#define TASKLISTWIDGETITEM_H

#include <QListWidgetItem>
#include <QObject>

class TaskListWidgetItem : public QListWidgetItem
{

public:
    TaskListWidgetItem();
    virtual ~TaskListWidgetItem();

protected:
    void initialize();
};

#endif // TASKLISTWIDGETITEM_H
