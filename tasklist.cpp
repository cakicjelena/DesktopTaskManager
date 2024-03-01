#include "tasklist.h"
#include "taskmodel.h"
#include "tmfactory.h"
#include "session.h"
#include "qdebug.h"
#include "QDropEvent"
taskStatus TaskList::type() const
{
    return m_type;
}


TaskList::TaskList() {}

TaskList::TaskList(QWidget* parent, taskStatus type):
    QListWidget(parent), m_type(type)
{
    initialize();
}

TaskList::~TaskList()
{

}

void TaskList::initialize()
{
    connect(this, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(listItemClicked(QListWidgetItem*)));
    connect(this, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(listItemDoubleClicked(QListWidgetItem*)));
    connect(this, SIGNAL(itemEntered(QListWidgetItem*)), this, SLOT(itemDrag(QListWidgetItem*)));
    //connect(this, SIGNAL(deleteDragItemSignal(QListWidgetItem*,taskStatus)), this, SLOT(deleteDragItem(QListWidgetItem*,taskStatus)));
    setDragEnabled(true);
    setAcceptDrops(true);
    setDragDropMode(QAbstractItemView::DragDrop);
    setDefaultDropAction(Qt::MoveAction);


}

void TaskList::listItemClicked(QListWidgetItem *item)
{

    emit customItemClicked(item);
}

void TaskList::listItemDoubleClicked(QListWidgetItem *item)
{
    qDebug()<<"Emituje signal";
    emit customItemDoubleClicked(item);
}

void TaskList::itemDrag(QListWidgetItem *item)
{
    qDebug()<<"Reaguje";
    emit customItemClicked(item);
    //setItemHidden(item,true);
}


void TaskList::dropEvent(QDropEvent *event)
{
    QListWidget::dropEvent(event);
    qDebug()<<"Task moved";
    emit changeStatus((int)m_type);

}
