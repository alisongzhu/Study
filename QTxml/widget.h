#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QFile>
#include <QtXml>
#include <QTreeWidgetItem>
#include <QFileDialog>
#include <QMessageBox>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();


    void listDom(QDomElement& docElem);
    void openXML(QString fileName);
    void saveXML(QString fileName);



private slots:
    void on_pushButton_clicked();

//    void openFile();
    void saveFile();
    void newGroup();
    void delGroup();

    void on_pushButton_2_clicked();

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
