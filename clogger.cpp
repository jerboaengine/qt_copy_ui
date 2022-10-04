#include "clogger.h"
#include <QDateTime>

CLogger::CLogger(QObject *parent)
    : QObject{parent}
{

}

QString CLogger::getCurrentTimeString() {
    return QDateTime::currentDateTime().toString("[dd.MM.yyyy hh:mm:ss]");
}

void CLogger::onHandleMessage(const QString message) {
    const QString out = getCurrentTimeString() + " >> " + message;
    qDebug() << out;
}
