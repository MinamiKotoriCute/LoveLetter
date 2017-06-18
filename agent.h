#ifndef AGENT_H
#define AGENT_H

#include <QObject>

#include "qlearning.h"
#include "lovelettergame.h"
#include "maptreewidget.h"

class Agent : public QObject
{
    Q_OBJECT
public:
    explicit Agent(QObject *parent = 0);

signals:

public slots:

private:
    void send(QVector<int> action);

    qln::State getState();
    qln::Action getAction();
    void executeAction();

    qln::QLearning *qlearning;
    LoveLetterGame *game;

    int playerNumber;
    QList<qln::Action> lastAction;
    QList<qln::State> lastState;
    MapTreeWidget mapTreeWidget;
};

#endif // AGENT_H
