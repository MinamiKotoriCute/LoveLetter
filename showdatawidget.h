#ifndef SHOWDATAWIDGET_H
#define SHOWDATAWIDGET_H

#include <QWidget>
#include <QDialog>
#include <QJsonObject>
#include <QJsonArray>

class QTreeWidget;

class ShowDataWidget : public QDialog
{
    Q_OBJECT
public:
    explicit ShowDataWidget(QWidget *parent = 0);

    void setData(QJsonObject json);
    void setData(QJsonArray json);

signals:
    void needData();

public slots:

private:
    QTreeWidget *treeWidget;
};

#endif // SHOWDATAWIDGET_H
