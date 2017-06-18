#ifndef LOVELETTER_H
#define LOVELETTER_H

#include <QObject>

class LoveLetter : public QObject
{
    Q_OBJECT
public:
    explicit LoveLetter(QObject *parent = 0);

    void executeAction(QVector<int> action);

    void prepare(int playerNumber);
    void playerAction(int action1, int action2 = -1, int action3 = -1);
    QVector<int> playerHandCard() const;

signals:
    void notification();

    void playerTurn(int playerID, int remainCardNumer, int card1, int card2, QVector<int> canChoosePlayer);
    void playerDied(int playerID);

public slots:

private:
    void shuffleCards();
    void dealCards();
    void noticePlayer();
    void drawNewCard(int playerID = -1);
    void playerDying(int playerID);

    int m_playerNumber;
    QVector<int> m_cards;
    int m_turn;
    QVector<int> m_playerHandCard;
    QVector<bool> m_isPlayerInvincible;
    int m_playerNewCard;
};

#endif // LOVELETTER_H
