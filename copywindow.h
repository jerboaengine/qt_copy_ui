#ifndef COPYWINDOW_H
#define COPYWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QProgressBar>
#include "ccopier.h"
#include "clogger.h"

class CopyWindow : public QMainWindow
{
    Q_OBJECT

public:
    CopyWindow(QWidget *parent = nullptr);
    ~CopyWindow();

private:
    QLineEdit *srcEdit;
    QLineEdit *dstEdit;

    QPushButton *srcButOpenDialog;
    QPushButton *dstButOpenDialog;

    QPushButton *startStopButton;

    QProgressBar *progressBar;

    CCopier copier;
    CLogger logger;

private slots:
    void onShowSelectSrcDialog();
    void onShowSelectDstDialog();
    void startProgress();
    void stopProgress();
    void onHandleStartStopButtonPress();
    void objectsCountUpdate(int val);
    void changeProgress(int val);
    void successfully();
    void countingStep();
    void copyStep();
};
#endif // COPYWINDOW_H
