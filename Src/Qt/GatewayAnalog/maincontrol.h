#ifndef MAINCONTROL_H
#define MAINCONTROL_H

#include <QObject>
#include <QTimer>

class MainControl : public QObject
{
    Q_OBJECT
private:
    explicit MainControl(QObject *parent = 0);

public:
    static MainControl *getObject();

protected slots:
    void updateTimerTimeoutSlot();

protected:
    QTimer *updateTimer;
};

#endif // MAINCONTROL_H
