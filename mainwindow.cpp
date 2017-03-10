#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::exit()
{
    close();
    qApp->quit();
}

void MainWindow::on_patchButton_clicked()
{
    if(searchKernelExtensionFile(&kernelFile))
    {
        //Display Warning Message
        int answer = QMessageBox::question(this, "Warning", "This will patch the kernel configuration file.\nAre you sure you want to procede ?", QMessageBox::Yes | QMessageBox::No);

        if (answer == QMessageBox::Yes)
        {

        }
        else
        {
            return;
        }
    }
    else
    {
        return;
    }
}

void MainWindow::on_restoreButton_clicked()
{
    //Display Warning Message
    int answer = QMessageBox::question(this, "Warning", "This will restore the old configuration.\nAre you sure you want to procede ?", QMessageBox::Yes | QMessageBox::No);

    if (answer == QMessageBox::Yes)
    {

    }
    else
    {
        return;
    }
}

bool MainWindow::init()
{
    bool isInitOk = true;

    //Search for compatibility
    if(isCompatibleVersion(getMBPModelVersion()))
    {
        isInitOk = true;
    }
    else
    {
        QMessageBox::information(this,"Mac not compatible","Sorry, your Mac is not compatible.\nThe application will close");
        isInitOk = false;
    }

    return isInitOk;
}

QString MainWindow::getMBPModelVersion()
{
    QString MBPModelVersion;
    QProcess process;

    //Execute commande line
    process.start("sysctl -n hw.model");

    //Wait forever until finished
    process.waitForFinished(-1);

    //Get command line output
    MBPModelVersion = process.readAllStandardOutput();

    //Remove carriage return ("\n") from string
    MBPModelVersion = MBPModelVersion.simplified();

    return MBPModelVersion;
}


//Parse system directory searching for AppleGraphicsPowerManagement.kext file
bool MainWindow::searchKernelExtensionFile(QFile* kernelExtensionFile)
{
    bool isFileFound;
    QDir appPath("../");
    QStringList listOfFiles;

    //Print Current app directory
    qDebug() << "Current Dir :" <<appPath.absolutePath();

    //Recursively search for "Info.plist" file in appPath
    QDirIterator it(appPath.absolutePath(),
                    QStringList() << "*.plist",
                    QDir::NoSymLinks | QDir::Files,
                    QDirIterator::Subdirectories);


    //Check if the file was found
    if(it.hasNext())
    {
        //Print files found
        qDebug () << "Files found :";
        while(it.hasNext())
        {
            it.next();
            listOfFiles.push_back(it.filePath());
        }
    }

    if(listOfFiles.length() <= 1 && listOfFiles.length() > 0)
    {
        //qDebug() << "Moins de 1";
        kernelExtensionFile->setFileName(listOfFiles.at(0));
        isFileFound = true;
    }
    else
    {
        //qDebug () << "No file was found...";
        isFileFound = false;
    }

    //Start search manuallyand only allow loading of the perfect named file (or kext)
    if(!isFileFound)
    {
        QMessageBox::information(this,"File not found","Any corresponding file was found, please search for the file");

        QString dir = QFileDialog::getOpenFileName(this, tr("Open Info.plist file"),
                                                   "Info.plist",
                                                   "Property List Files (Info.plist)");

        if(!(dir.isNull()))
        {
            kernelExtensionFile->setFileName(dir);
            isFileFound = true;
        }
        else
        {
            isFileFound = false;
        }
    }

    return isFileFound;
}

bool MainWindow::isCompatibleVersion(QString modelVersion)
{
    //Compare version with compatible versions of MBPs
    bool isCompatibleVersion;
    QString MBPModelVersion;

    MBPModelVersion = getMBPModelVersion();

    //TODO : Search in a list
    if(MBPModelVersion == "MacBookPro6,2")
    {
        isCompatibleVersion = true;
    }
    else
    {
        isCompatibleVersion = false;
    }

    return isCompatibleVersion;
}

void MainWindow::backupKernelExtension()
{
    //Save File to current location adding .bak extension
}

void MainWindow::patchKernelExtensionFile(QFile *kernelFile)
{
    //Modify Kernel Extension File to add fix explained here :
    //https://forums.macrumors.com/threads/gpu-kernel-panic-in-mid-2010-whats-the-best-fix.1890097/

}

int MainWindow::loadKernelExtension(QFile *kernelFile)
{
    //Use Kext Utility or command lines utils to load the file in Kernel
    //kextload

    //See here : http://osxdaily.com/2015/06/24/load-unload-kernel-extensions-mac-os-x/
    int Status;

    return Status;
}

int MainWindow::restoreOldKernelExtension(QFile *kernelFile)
{
    //Restore.bak extension
    int Status;

    return Status;
}


