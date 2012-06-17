#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include "detailstable.h"
#include "sum.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void on_openAction_triggered(bool checked);
    void on_quitAction_triggered(bool checked);
    void on_sumAction_triggered(bool checked);
    void on_stopAction_triggered(bool checked);
    void on_copyAction_triggered(bool checked);
    void on_pasteAction_triggered(bool checked);
    void on_saveAction_triggered(bool checked);
    void on_aboutAction_triggered(bool checked);
    void calcStarted();
    void updateStatus();
    void setFinalResult();
    void on_compareText_textChanged(const QString &text);
    void on_showDetails_clicked();
    void on_hideDetails_clicked();

private:
    void createBars();
    void createActions();
    void createComponents();

    Sum sumlib;
    QAction *openAction;
    QAction *quitAction;
    QAction *sumAction;
    QAction *stopAction;
    QAction *copyAction;
    QAction *pasteAction;
    QAction *saveAction;
    QAction *aboutAction;
    QList<QAction*> allActions;

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *helpMenu;
    QToolBar *mainToolBar;

    QLineEdit *fileText;
    QComboBox *typeCombo;
    QTextEdit *resultText;
    QLineEdit *compareText;
    QPixmap comparePixmap;
    QLabel *compareLabel;
    QPushButton *showDetails;
    QPushButton *hideDetails;
    QProgressBar *progressBar;
    DetailsTable *detailsTable;
    QClipboard *clipboard;
    QSplitter *centralSplitter;
};

#endif // MAINWINDOW_H
