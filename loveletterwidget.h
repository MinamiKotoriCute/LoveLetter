#ifndef LOVELETTERWIDGET_H
#define LOVELETTERWIDGET_H

#include <QWidget>
#include <QGroupBox>
#include <QTextEdit>

class QPushButton;
class LoveLetterGame;


class LoveLetterWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LoveLetterWidget(QWidget *parent = 0);

signals:

public slots:

private:
    void send(QVector<int> action);

    int _playerNumber;
    QVector<QPushButton *> buttons;
    LoveLetterGame *game;
    QList<QGroupBox *> groups;
    QTextEdit *textEdit;
};

#endif // LOVELETTERWIDGET_H
