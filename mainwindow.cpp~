#include "mainwindow.h"
#include <QIcon>

QIcon icon(const QString name)
{
    return QIcon("icons/" + name + ".png");
}

QString getRootDir(QString filename)
{
    if(filename.isEmpty())
        return QDir::homePath();
    else
        return QFileInfo(filename).absolutePath();
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    resize(400, 300);
    setWindowTitle("QSum");
    setWindowIcon(icon("calc"));
    clipboard = qApp->clipboard();

    createBars();
    createActions();
    createComponents();

    connect(&sumlib, SIGNAL(started()), this,
            SLOT(calcStarted()));
    connect(&sumlib, SIGNAL(hashChanged()), this,
            SLOT(updateStatus()));
    connect(&sumlib, SIGNAL(finished()), this,
            SLOT(setFinalResult()));

    QMetaObject::connectSlotsByName(this);
}

MainWindow::~MainWindow()
{
    
}

void MainWindow::createBars()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    editMenu = menuBar()->addMenu(tr("&Edit"));
    helpMenu = menuBar()->addMenu(tr("&Help"));

    mainToolBar = addToolBar(tr("Main"));
    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::createActions()
{
    openAction = fileMenu->addAction(icon("open"), tr("&Open"));
    openAction->setStatusTip(tr("Open a file..."));
    openAction->setObjectName("openAction");

    fileMenu->addSeparator();

    quitAction = fileMenu->addAction(icon("quit"), tr("&Quit"));
    quitAction->setStatusTip(tr("Quit application"));
    quitAction->setObjectName("quitAction");

    sumAction = editMenu->addAction(icon("calc"), tr("&Sum"));
    sumAction->setStatusTip(tr("Start"));
    sumAction->setObjectName("sumAction");

    stopAction = editMenu->addAction(icon("pstop"), tr("&Stop"));
    stopAction->setStatusTip(tr("Stop current process"));
    stopAction->setObjectName("stopAction");
    stopAction->setEnabled(false);

    editMenu->addSeparator();

    copyAction = editMenu->addAction(icon("copy"), tr("&Copy"));
    copyAction->setStatusTip(tr("Copy result"));
    copyAction->setObjectName("copyAction");

    pasteAction = editMenu->addAction(icon("paste"), tr("&Paste"));
    pasteAction->setStatusTip(tr("Paste to compare"));
    pasteAction->setObjectName("pasteAction");

    editMenu->addSeparator();

    saveAction = editMenu->addAction(icon("save"), tr("&Save"));
    saveAction->setStatusTip(tr("Save result to file..."));
    saveAction->setObjectName("saveAction");

    aboutAction = helpMenu->addAction(icon("about"), tr("&About"));
    aboutAction->setStatusTip(tr("About..."));
    aboutAction->setObjectName("aboutAction");

    QList<QAction*> toolBarActions;
    toolBarActions << openAction << sumAction << stopAction
               << copyAction << pasteAction << saveAction;
    allActions << toolBarActions << quitAction << aboutAction;

    mainToolBar->addActions(toolBarActions);
    mainToolBar->insertSeparator(saveAction);
    mainToolBar->insertSeparator(copyAction);
}

void MainWindow::createComponents()
{
    QGridLayout *mLayout = new QGridLayout();

    QLabel *fileLabel = new QLabel(tr("File:"));
    fileText = new QLineEdit();
    fileText->setReadOnly(true);

    mLayout->addWidget(fileLabel, 0, 0, 1, 1);
    mLayout->addWidget(fileText, 0, 1, 1, 4);

    QLabel *typeLabel = new QLabel(tr("Algorithm Type:"));
    typeLabel->setScaledContents(true);
    typeCombo = new QComboBox();
    QStringList algorithms;
    algorithms << "md5" << "sha1" << "sha224" << "sha256" << "sha384" << "sha512";
    typeCombo->addItems(algorithms);

    mLayout->addWidget(typeLabel, 1, 0, 1, 1);
    mLayout->addWidget(typeCombo, 1, 1, 1, 4);

    QLabel *resultLabel = new QLabel(tr("Result:"));
    resultLabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    resultText = new QTextEdit();
    resultText->setReadOnly(true);
    resultText->setWordWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);

    mLayout->addWidget(resultLabel, 2, 0, 1, 1);
    mLayout->addWidget(resultText, 2, 1, 1, 4);

    QLabel *compareWithLabel = new QLabel(tr("Compare with:"));
    compareLabel = new QLabel();
    compareText = new QLineEdit();
    compareText->setObjectName("compareText");
    comparePixmap.load("icons/warning.png"); //&icon("warning").pixmap(24, 24);
    compareLabel->setPixmap(comparePixmap);

    mLayout->addWidget(compareWithLabel, 3, 0);
    mLayout->addWidget(compareText, 3, 1);
    mLayout->addWidget(compareLabel, 3, 2, 1, 4);

    showDetails = new QPushButton(icon("down"), "");
    showDetails->hide();
    showDetails->setObjectName("showDetails");
    hideDetails = new QPushButton(icon("up"), "");
    hideDetails->setObjectName("hideDetails");
    hideDetails->hide();
    progressBar = new QProgressBar();
    progressBar->hide();

    QHBoxLayout *progressLayout = new QHBoxLayout();
    progressLayout->addWidget(showDetails);
    progressLayout->addWidget(hideDetails);
    progressLayout->addWidget(progressBar);

    mLayout->addLayout(progressLayout, 4, 0, 1, 4);

    centralSplitter = new QSplitter(Qt::Vertical);

    QWidget *componentWidget = new QWidget();
    componentWidget->setLayout(mLayout);
    detailsTable = new DetailsTable();
    centralSplitter->addWidget(componentWidget);
    centralSplitter->addWidget(detailsTable);
    detailsTable->hide();

    setCentralWidget(centralSplitter);
}

void MainWindow::on_openAction_triggered(bool checked)
{
    Q_UNUSED(checked)
    QString path = fileText->text(), filename;
    filename = QFileDialog::getOpenFileName(this, tr("Select a file"),
                                            getRootDir(path));
    if(!filename.isEmpty()) fileText->setText(filename);
}

void MainWindow::on_copyAction_triggered(bool checked)
{
    Q_UNUSED(checked)
    if(!resultText->toPlainText().isEmpty())
        clipboard->setText(resultText->toPlainText());
}

void MainWindow::on_pasteAction_triggered(bool checked)
{
    Q_UNUSED(checked)
    if(!resultText->toPlainText().isEmpty())
        compareText->setText(clipboard->text());
}

void MainWindow::on_sumAction_triggered(bool checked)
{
    Q_UNUSED(checked)
    if(fileText->text().isEmpty()) return;
    sumlib.setFilename(fileText->text());
    sumlib.setAlgo(typeCombo->currentIndex());
    resultText->setText(tr("Working..."));
    progressBar->setMaximum(sumlib.size() / 100);
    sumlib.sum();
}

void MainWindow::calcStarted()
{
    foreach(QAction* action, allActions)
        action->setEnabled(false);

    stopAction->setEnabled(true);
    detailsTable->setFilename(fileText->text());
    detailsTable->setSize(sumlib.size());
    progressBar->show();
    showDetails->show();
    progressBar->setValue(0);
    on_compareText_textChanged(compareText->text());

    statusBar()->showMessage(tr("Process Started"));
}

void MainWindow::updateStatus()
{
    progressBar->setValue(sumlib.currentPos() / 100);
    detailsTable->updateDetails(&sumlib);
//    detailsTable->updateDetails(sumlib.currentPos(),
//                                sumlib.readRate(),
//                                sumlib.elapsed(),
//                                sumlib.estimated());
}

void MainWindow::setFinalResult()
{
    foreach(QAction* action, allActions)
        action->setEnabled(true);

    stopAction->setEnabled(false);
    resultText->setText(sumlib.result());
    progressBar->hide();
    detailsTable->hide();
    showDetails->hide();
    hideDetails->hide();
    on_compareText_textChanged(compareText->text());

    statusBar()->showMessage(tr("Process took %1").arg(Sum::toTime(sumlib.elapsed())));
}

void MainWindow::on_stopAction_triggered(bool checked)
{
    Q_UNUSED(checked)
    sumlib.stop(true);
    statusBar()->showMessage("Stopped");
}

void MainWindow::on_compareText_textChanged(const QString &text)
{
    QString result = resultText->toPlainText();
    if(text.isEmpty())
        comparePixmap.load("icons/warning.png");
    else if(!result.isEmpty() && !sumlib.stopped())
    {
        if(result == text)
            comparePixmap.load("icons/ok.png");
        else if(result != text)
            comparePixmap.load("icons/error.png");
    }
    compareLabel->setPixmap(comparePixmap);
}

void MainWindow::on_saveAction_triggered(bool checked)
{
    Q_UNUSED(checked)
    QString filename, result, saveFile;
    filename = sumlib.filename();
    result = sumlib.result();

    if(!filename.isEmpty() && !result.isEmpty() && !sumlib.stopped())
    {
        saveFile = QFileDialog::getSaveFileName(this, "Save", getRootDir(filename));
        if(saveFile.isEmpty()) return;

        QFile outFile(saveFile);
        if(outFile.open(QFile::WriteOnly | QFile::Text))
        {
            QTextStream out(&outFile);
            out << result << " " << QFileInfo(filename).fileName();
            statusBar()->showMessage(tr("Saved to %1 successfully").arg(saveFile));
        }
        else
            QMessageBox::critical(this, "QSum",
                                  tr("Cannot save result to file %1:\n%2")
                                      .arg(outFile.fileName(),
                                           outFile.errorString()));
    }
}

void MainWindow::on_showDetails_clicked()
{
    detailsTable->show();
    hideDetails->show();
    showDetails->hide();
}

void MainWindow::on_hideDetails_clicked()
{
    detailsTable->hide();
    hideDetails->hide();
    showDetails->show();
}

void MainWindow::on_aboutAction_triggered(bool checked)
{
    Q_UNUSED(checked)
    QMessageBox::about(this , "About QSum",
                       "<b>QSum</b> <br>Version: 0.0.1<br>"
                       "<br>License: GPL V2 or Later (See COPYING)</br><br>"
                       "<br>Checksums [md5, sha1, sha224, sha256, sha384, sha512]<br>"
                       "<br>This application uses <a href = 'http://polarssl.org/'>polarssl</a><br>"
                       "<br>Website:</br> <a href = 'http://www.programming-fr34ks.net'>www.programming-fr34ks.net</a>"
                       "<br>Copyright (c) 2012 Abdelrahman Ghanem</br> <a href = 'mailto:abom.jdev@gmail.com'>abom.jdev@gmail.com</a>");
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    event->ignore();
    on_quitAction_triggered(false);
}

void MainWindow::on_quitAction_triggered(bool checked)
{
    Q_UNUSED(checked)
    sumlib.stop(true);
    QApplication::exit();
}
