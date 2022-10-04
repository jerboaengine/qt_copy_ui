#include "copywindow.h"
#include <QFileDialog>

#define WINDOW_WIDTH 400
#define WINDOW_HEIGHT 300

#define EDIT_WIDTH 200
#define EDIT_HEIGHT 30

#define BUTTON_WIDTH 200
#define BUTTON_HEIGHT 30

#define P_BAR_WIDTH WINDOW_WIDTH
#define P_BAR_HEIGHT 60

#define P_BAR_Y_POS EDIT_HEIGHT + BUTTON_HEIGHT

#define START_BUTTON_TEXT "start copying"
#define STOP_BUTTON_TEXT "stop copying"
#define START_STOP_BUTTON_Y_POS P_BAR_Y_POS + P_BAR_HEIGHT
#define START_STOP_BUTTON_X_POS (WINDOW_WIDTH - BUTTON_WIDTH) / 2

CopyWindow::CopyWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setFixedSize(WINDOW_WIDTH, WINDOW_HEIGHT);

    srcEdit = new QLineEdit(this);
    srcEdit->setPlaceholderText("source path");
    srcEdit->resize(EDIT_WIDTH, EDIT_HEIGHT);
    srcEdit->setText("/Users/ivan/work/native-android_");

    dstEdit = new QLineEdit(this);
    dstEdit->setPlaceholderText("destination path");
    dstEdit->resize(EDIT_WIDTH, EDIT_HEIGHT);
    dstEdit->move(EDIT_WIDTH, 0);
    dstEdit->setText("/Volumes/TOSHIBA_EXT/tmp");

    srcButOpenDialog = new QPushButton(this);
    srcButOpenDialog->setText("select source path");
    srcButOpenDialog->resize(BUTTON_WIDTH, BUTTON_HEIGHT);
    srcButOpenDialog->move(0, EDIT_HEIGHT);
    connect(srcButOpenDialog, SIGNAL(clicked()), this, SLOT(onShowSelectSrcDialog()));

    dstButOpenDialog = new QPushButton(this);
    dstButOpenDialog->setText("select destination path");
    dstButOpenDialog->resize(BUTTON_WIDTH, BUTTON_HEIGHT);
    dstButOpenDialog->move(BUTTON_WIDTH, EDIT_HEIGHT);
    connect(dstButOpenDialog, SIGNAL(clicked()), this, SLOT(onShowSelectDstDialog()));

    progressBar = new QProgressBar(this);
    progressBar->resize(P_BAR_WIDTH, P_BAR_HEIGHT);
    progressBar->move(0, P_BAR_Y_POS);
    progressBar->setValue(0);
    progressBar->setStyleSheet("QProgressBar{"
                               "border: 1px solid transparent;text-align: center;"
                               "color:rgba(255,255,250,255);"
                               "background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 rgba(200, 200, 200, 200), stop:1 rgba(0, 0, 0, 200));}"
                               "QProgressBar::chunk {background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 rgba(255,100,0,255), stop: 1 rgba(255,0,0,255));}");

    startStopButton = new QPushButton(this);
    startStopButton->setText(START_BUTTON_TEXT);
    startStopButton->resize(BUTTON_WIDTH, BUTTON_HEIGHT);
    startStopButton->move(START_STOP_BUTTON_X_POS, START_STOP_BUTTON_Y_POS);
    connect(startStopButton, SIGNAL(clicked()), this, SLOT(onHandleStartStopButtonPress()));

    connect(&copier, SIGNAL(objectsCountUpdate(int)), this, SLOT(objectsCountUpdate(int)));
    connect(&copier, SIGNAL(changeProgress(int)), this, SLOT(changeProgress(int)));
    connect(&copier, SIGNAL(error(QString)), &logger, SLOT(onHandleMessage(QString)));
    connect(&copier, SIGNAL(finished()), this, SLOT(stopProgress()));
    connect(&copier, SIGNAL(started()), this, SLOT(startProgress()));
    connect(&copier, SIGNAL(successfully()), this, SLOT(successfully()));
    connect(&copier, SIGNAL(countingStep()), this, SLOT(countingStep()));
    connect(&copier, SIGNAL(copyStep()), this, SLOT(copyStep()));
}

void CopyWindow::onShowSelectSrcDialog() {
    srcEdit->setText(QFileDialog::getExistingDirectory(
                this,
                "source path",
                QDir::currentPath()));
}

void CopyWindow::onShowSelectDstDialog() {
    dstEdit->setText(QFileDialog::getExistingDirectory(
                this,
                "destination path",
                QDir::currentPath()));
}

void CopyWindow::startProgress() {
    startStopButton->setText(STOP_BUTTON_TEXT);
}

void CopyWindow::stopProgress() {
    startStopButton->setText(START_BUTTON_TEXT);
}

void CopyWindow::onHandleStartStopButtonPress() {
    if (copier.isRunning()) {
        copier.stop();
    } else {
        progressBar->setValue(0);
        copier.copyFolder(srcEdit->text(), dstEdit->text());
    }
}

void CopyWindow::objectsCountUpdate(int val) {
    progressBar->setMaximum(val);
}

void CopyWindow::changeProgress(int val) {
    progressBar->setValue(val);
}

void CopyWindow::successfully() {

}

void CopyWindow::countingStep() {
    progressBar->setFormat("number of objects: %m");
}

void CopyWindow::copyStep() {
    progressBar->setFormat("%p%");
    progressBar->setMaximum(100);
}

CopyWindow::~CopyWindow()
{
    copier.stop();
    copier.wait();
}

