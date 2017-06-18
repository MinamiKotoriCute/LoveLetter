#ifndef AGENT_H
#define AGENT_H

#include <QObject>

#include "qlearning.h"
#include "lovelettergame.h"

class Agent : public QObject
{
    Q_OBJECT
public:
    explicit Agent(QObject *parent = 0);

signals:

public slots:

private:
    qln::State getState();
    qln::Action getAction();
    void move();

    qln::QLearning *qlearning;
    LoveLetterGame *game;

    qln::Action lastAction;
    qln::State lastState;
};

#endif // AGENT_H
