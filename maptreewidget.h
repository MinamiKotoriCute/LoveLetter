#ifndef MAPTREEWIDGET_H
#define MAPTREEWIDGET_H

#include <QDialog>
#include <QWidget>
#include <QTreeWidget>
#include <QDebug>
#include <QJsonObject>

#include "qlearning.h"

class MapTreeWidget : public QDialog
{
    Q_OBJECT
public:
    explicit MapTreeWidget(QWidget *parent = 0);

    void setData(qln::StateActionSpace sas);

    void setData(QJsonObject jsonObject);


signals:

public slots:

private:
    QString index2QString(qln::Index index);

    QTreeWidget *treeWidget;
};

#endif // MAPTREEWIDGET_H
