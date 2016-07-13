

#ifndef INPUTCONSOLE_H
#define INPUTCONSOLE_H

#include <QPlainTextEdit>

class InputConsole : public QPlainTextEdit
{
    Q_OBJECT

signals:
    void getData(const QByteArray &data);

public:
    explicit InputConsole(QWidget *parent = 0);

    void putData(const QByteArray &data);


protected:
    virtual void keyPressEvent(QKeyEvent *e);
    //virtual void mousePressEvent(QMouseEvent *e);
    //virtual void mouseDoubleClickEvent(QMouseEvent *e);
    //virtual void contextMenuEvent(QContextMenuEvent *e);

private:


};

#endif // INPUTCONSOLE_H
