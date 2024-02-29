#ifndef TASKLIST_H
#define TASKLIST_H

#include <QListWidget>
#include <QObject>
enum class taskStatus;
class TMFactory;
class TaskList : public QListWidget
{
    Q_OBJECT
protected:
    taskStatus m_type;
    //TMFactory* m_tmFactory;

public:
    TaskList();
    TaskList(QWidget* parent, taskStatus type);
    virtual ~TaskList();
    taskStatus type() const; ///< function that returns m_type

protected:
    void initialize();
protected slots:
    void listItemClicked(QListWidgetItem* item);
    void listItemDoubleClicked(QListWidgetItem* item);
    void itemDrag(QListWidgetItem* item);
signals:
    void customItemClicked(QListWidgetItem*item);
    void customItemDoubleClicked(QListWidgetItem*item);
    void changeStatus(int task_status);
protected:
    virtual void dropEvent(QDropEvent *event) override;
};

#endif // TASKLIST_H
