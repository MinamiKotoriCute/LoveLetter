#ifndef GAMEBASE_H
#define GAMEBASE_H

#include <QObject>

class GameBase : public QObject
{
    Q_OBJECT
public:
    explicit GameBase(QObject *parent = 0);

    void executeAction();

signals:
    void notification();

public slots:
};

#endif // GAMEBASE_H
