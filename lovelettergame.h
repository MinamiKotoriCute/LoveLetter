#ifndef LOVELETTERGAME_H
#define LOVELETTERGAME_H

#include <QObject>
#include <QVector>

class LoveLetterGame : public QObject
{
    Q_OBJECT
public:
    explicit LoveLetterGame(QObject *parent = 0);

    int playerNumber() const;
    void setPlayerNumber(int playerNumber);

    void prepare();

    int turn() const;

    QVector<int> currentState() const;

    QList<bool> isDead() const;

signals:
    void notification(QVector<int> state, QVector<QVector<int>> actions);
    void gameOver(int winner);
    void playerDead(int playerId);

public slots:
    void executeAction(QVector<int> action);

private:
    void settle();
    void nextRound();
    void playerDying(int playerId);

    int _playerNumber;
    int _alivePlayerNumber;
    QList<int> _cards;
    QList<int> _handCards;
    QVector<bool> _isInvincible;
    QList<bool> _isDead;
    int _turn;
    int _extraHandCard;
};

#endif // LOVELETTERGAME_H
