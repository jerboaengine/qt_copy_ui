#ifndef CLOGGER_H
#define CLOGGER_H

#include <QObject>

class CLogger : public QObject
{
    Q_OBJECT
public:
    explicit CLogger(QObject *parent = nullptr);

public slots:
    void onHandleMessage(const QString message);

private:
    QString getCurrentTimeString();
};

#endif // CLOGGER_H
