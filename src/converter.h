#ifndef CONVERTER_H
#define CONVERTER_H

#include <QWidget>
#include <QTextCodec>
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>

namespace Ui {
class Converter;
}

class Converter : public QWidget
{
    Q_OBJECT

public:
    explicit Converter(QWidget* parent = 0);
    ~Converter();

private:
    Ui::Converter* ui;
    QTextCodec* codec1;
    QTextCodec* codec2;
    QString dirFrom;
    QString dirTo;
    QStringList listFileNames;

    QString readFile(QString filePath);
    void writeFile(QString fileContent, QString fileName);

private slots:
    void assignCodecs(QString convertType);
    void on_selectPathFromToolButton_clicked();
    void on_selectPathToToolButton_clicked();
    void on_convertPushButton_clicked();
};

#endif // CONVERTER_H
