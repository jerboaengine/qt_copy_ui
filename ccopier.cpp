#include "ccopier.h"
#include <QDirIterator>

CCopier::CCopier(QObject *parent)
    : QThread{parent}
{

}

void CCopier::run() {
    try {
        const int sourcePathLength = sourcePath.path().length() - sourcePath.dirName().length();
        const QFileInfoList fileInfoList = getSrcDirContentInfoList(sourcePath);
        if (isNeedStop()) {
            return;
        }

        emit copyStep();

        QString newPath = destinationPath.absolutePath()
                        + QDir::separator()
                        + sourcePath.dirName();

        if (!destinationPath.mkpath(newPath)) {
            throwStringMessage("unable to create root directory: " + newPath);
        }
        unsigned int iterationCount = 0;
        unsigned int listSize = fileInfoList.size();
        for (const QFileInfo &fileInfo : fileInfoList) {
            if (isNeedStop()) {
                return;
            }
            newPath = destinationPath.absolutePath()
                    + QDir::separator()
                    + fileInfo.absoluteFilePath().mid(sourcePathLength);

            if(fileInfo.isDir()) {
                if (!destinationPath.mkpath(newPath)) {
                    throwStringMessage("unable to create directory: " + newPath);
                }
            } else if(fileInfo.isFile()) {
                if (!QFile::copy(fileInfo.absoluteFilePath(), newPath)) {
                    throwStringMessage("unable to create file: " + newPath);
                }
            }
            emit changeProgress((double)++iterationCount / listSize * 100);
        }

        emit successfully();
    } catch (const QString &exeptionString) {
        dispatchStopAndError(exeptionString);
    } catch (...) {
        dispatchStopAndError("unknown error");
    }
}

QFileInfoList CCopier::getSrcDirContentInfoList(const QDir dir) {
    emit countingStep();
    QFileInfoList contentList;
    QDirIterator it(dir.path(),
                    QDir::AllEntries |
                    QDir::Hidden |
                    QDir::System |
                    QDir::NoDotAndDotDot,
                    QDirIterator::Subdirectories);
    while (it.hasNext()) {
        if (isNeedStop()) {
            return QFileInfoList();
        }
        it.next();
        contentList.append(it.fileInfo());
        emit objectsCountUpdate(contentList.length());
    }
    return contentList;
}

void CCopier::copyFolder(const QString src, const QString dst) {
    if (isRunning()) {
        emit error("previous task is not completed...");
        return;
    }

    try {
        setStopFlag(false);

        sourcePath = src;
        destinationPath = dst;

        if (src.isEmpty() || !sourcePath.exists()) {
            throwStringMessage("source path does not exist");
        }
        if (dst.isEmpty() || !destinationPath.exists()) {
            throwStringMessage("destination path does not exist");
        }

        if (sourcePath == destinationPath) {
            throwStringMessage("source path and destination must not match");
        }

        start();
    } catch (QString exeptionString) {
        dispatchStopAndError(exeptionString);
    } catch (...) {
        dispatchStopAndError("unknown error");
    }
}

void CCopier::throwStringMessage(const QString mesage) {
    throw mesage;
}

void CCopier::dispatchStopAndError(const QString mes) {
    emit error(mes);
}

void CCopier::stop() {
    setStopFlag(true);
}

void CCopier::setStopFlag(bool val) {
    stopFalgMutex.lock();
    isNeedStopFlag = val;
    stopFalgMutex.unlock();
}

bool CCopier::isNeedStop() {
    bool flagValue;
    stopFalgMutex.lock();
    flagValue = isNeedStopFlag;
    stopFalgMutex.unlock();
    return flagValue;
}

