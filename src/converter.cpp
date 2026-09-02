#include "converter.h"
#include "ui_converter.h"

Converter::Converter(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Converter)
{
    ui->setupUi(this);

    assignCodecs("CP1251 to UTF-8");

    connect(ui->convertTypeComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(assignCodecs(QString)));
}

Converter::~Converter()
{
    delete ui;
}

void Converter::assignCodecs(QString convertType)
{
    if(convertType == "CP1251 to UTF-8")
    {
        codec1 = QTextCodec::codecForLocale();
        codec2 = QTextCodec::codecForName("UTF-8");
    }
    else
    {
        codec1 = QTextCodec::codecForName("UTF-8");
        codec2 = QTextCodec::codecForLocale();
    }
}

void Converter::on_selectPathFromToolButton_clicked()
{
    dirFrom = QFileDialog::getExistingDirectory(0,
                                            "Choose directory",
                                            qApp->applicationDirPath(),
                                            QFileDialog::ShowDirsOnly
                                            | QFileDialog::DontResolveSymlinks);
    if(!dirFrom.isEmpty())
        ui->pathFromLineEdit->setText(dirFrom);
}

void Converter::on_selectPathToToolButton_clicked()
{
    dirTo = QFileDialog::getExistingDirectory(0,
                                            "Choose directory",
                                            qApp->applicationDirPath(),
                                            QFileDialog::ShowDirsOnly
                                            | QFileDialog::DontResolveSymlinks);
    if(!dirTo.isEmpty())
        ui->pathToLineEdit->setText(dirTo);
}

void Converter::on_convertPushButton_clicked()
{
    if(dirFrom == dirTo)
        QMessageBox::warning(0, "Change directories",
                             "Directories 'from' and 'to' should be different!");
    else
    {
        QDir dir;
        QStringList listFiles;
        dir.setPath(dirFrom);
        foreach(QFileInfo item, dir.entryInfoList())
        {
            if(item.isFile())
            {
                listFiles << item.absoluteFilePath();
                listFileNames << item.fileName();
            }
        }
        if(listFiles.isEmpty())
            QMessageBox::warning(0, "No files",
                                 "Directory 'from' is empty!");
        else
        {
            for(int i = 0; i < listFiles.size(); ++i)
            {
                QString filePath = listFiles.at(i);
                QString fileContent = readFile(filePath);
                QByteArray byteArrayContent;
                byteArrayContent.insert(0, fileContent);
                fileContent = codec1->toUnicode(byteArrayContent);
                fileContent = codec2->fromUnicode(fileContent);
                writeFile(fileContent, listFileNames.at(i));
            }
        }
    }
}

QString Converter::readFile(QString filePath)
{
    QFile file(filePath);
    QString line;
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream stream(&file);
        line = stream.readAll();
        file.close();
        return line;
    }
    else
        QMessageBox::warning(0, "Error",
                             "Error reading file");

    return 0;
}

void Converter::writeFile(QString fileContent, QString fileName)
{

    fileName = dirTo + "/" + fileName;
    QFile file(fileName);
    if(file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream stream(&file);

        stream << fileContent;

        file.close();
    }
    else
        QMessageBox::warning(0, "Error",
                             "Error writing file");
}
