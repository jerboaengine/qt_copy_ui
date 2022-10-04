#ifndef CCOPIER_H
#define CCOPIER_H

#include <QThread>
#include <QDir>
#include <QMutex>

class CCopier : public QThread
{
    Q_OBJECT
private:
    QDir sourcePath;
    QDir destinationPath;
    void run() override;
    QFileInfoList getSrcDirContentInfoList(const QDir dir);
    void throwStringMessage(const QString mesage);
    bool isNeedStopFlag = false;
    QMutex stopFalgMutex;

public:
    explicit CCopier(QObject *parent = nullptr);
    void copyFolder(const QString src, const QString dst);
    void stop();
    void dispatchStopAndError(const QString mes);

signals:
    void error(const QString reason);
    void changeProgress(int val);
    void objectsCountUpdate(int val);
    void successfully();
    void countingStep();
    void copyStep();

private:
    bool isNeedStop();
    void setStopFlag(bool val);
};

#endif // CCOPIER_H
