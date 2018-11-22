#include "widget.h"
#include "ui_widget.h"
#include <QtXml>
#include <QFile>
#include <QTreeWidget>
#include <QDebug>
#include <QFileDialog>
#include <QCheckBox>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::FramelessWindowHint );

    QStringList strList;
    strList.append("参数");
    strList.append("值");
    strList.append("类型");
    strList.append("偏移量");
    ui->treeWidget->setHeaderLabels(strList);



}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open XML File"), "./", tr("XML Files (*.xml)"));
        if (!fileName.isEmpty())
        {
            openXML(fileName);
        }

}

void Widget::saveFile()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save XML File"), "./ManagedDevice.xml", tr("XML Files (*.xml)"));
    if (!fileName.isEmpty())
    {
        saveXML(fileName);
    }
}

void Widget::openXML(QString fileName)
{
    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly))
    {
        QDomDocument dom("MngedDev");
#if 0
        if (dom.setContent(&file))
        {
            ui->treeWidget->clear();
            QDomElement docElem = dom.documentElement();
            listDom(docElem, NULL);
        }
#else
        QString errorStr;
        int errorLine;
        int errorCol;
        if (!dom.setContent(&file, true, &errorStr, &errorLine, &errorCol))
        {
            //如果出错，则会进入这里。errorStr得到的是出错说明
            //errorLine和errorCol则是出错的行和列
            qDebug() << errorStr << "line: " << errorLine << "col: " << errorCol;
        }
        else
        {
            ui->treeWidget->clear();
            QDomElement docElem = dom.documentElement();
            listDom(docElem);
            ui->treeWidget->expandAll();
        }
#endif

    }
    file.close();
}

void Widget::listDom(QDomElement& docElem)
{
    //分组节点
    QDomNode groupnode = docElem.firstChild();
    while (!groupnode.isNull())
    {

        QDomElement elementGrp = groupnode.toElement(); // try to convert the node to an element.

        if (elementGrp.tagName() == QStringLiteral("Group"))
        {

            //组名
            QString GroupName = elementGrp.attribute("name");
            QString Value = elementGrp.attribute("value");
            QString Type = elementGrp.attribute("Type");
            QString Offset = elementGrp.attribute("Offset");

//            str <<strGroupName<<strGroupName1;     //源地址
//             ui->treeWidget->addTopLevelItem(top);
            QTreeWidgetItem *groupitem = new QTreeWidgetItem(ui->treeWidget);
//            str <<QString(strGroupName)<<QString(strGroupName1);     //源地址
//            sourceAddr=new QTreeWidgetItem(ui->treeWidget,str);

            if(Type=="1")
            {
                QCheckBox *ckBox = new QCheckBox;
                    ckBox->setText("CheckBox");
                    ckBox->setChecked(true);
                ui->treeWidget->setItemWidget(groupitem,1,ckBox);   //设置ckBox为item的第二列

                QCheckBox *cpy = (QCheckBox*)ui->treeWidget->itemWidget(groupitem,1);
//                QMessageBox::information(this,"title",cpy->text());     //获取item中Checkbox的文本值
            }

            groupitem->setText(0, GroupName);
//            groupitem->setData(2, Qt::UserRole, strGroupName1);


            if(Type!="1")
            {
             groupitem->setText(1,Value);
            }
            groupitem->setText(2,Type);
            groupitem->setText(3,Offset);

//            groupitem->();
//            groupitem->setIcon(0, QIcon(QStringLiteral(":/Widget/Resources/image/group.png")));
//            groupitem->setWhatsThis(0, "Group");
            groupitem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable); //设置不可编辑




            //设备节点
            QDomNode devicenode = groupnode.firstChild();
            while (!devicenode.isNull())
            {
                QDomElement elementDev = devicenode.toElement();
                if (elementDev.tagName() == QStringLiteral("Parameter"))
                {

                    QString GroupName = elementDev.attribute("name");
                    QString Value = elementDev.attribute("value");
                    QString Type = elementDev.attribute("Type");
                    QString Offset = elementDev.attribute("Offset");


                    QTreeWidgetItem *deviceitem = new QTreeWidgetItem(groupitem);
                    deviceitem->setFlags(deviceitem->flags() | Qt::ItemIsEditable); //设置可编辑


                    if(Type=="1")
                    {
                        QCheckBox *ckBox = new QCheckBox;
                            ckBox->setText("CheckBox");
                            ckBox->setChecked(true);
                        ui->treeWidget->setItemWidget(deviceitem,1,ckBox);   //设置ckBox为item的第二列

                        QCheckBox *cpy = (QCheckBox*)ui->treeWidget->itemWidget(deviceitem,1);
        //                QMessageBox::information(this,"title",cpy->text());     //获取item中Checkbox的文本值
                    }

                    deviceitem->setText(0, GroupName);
        //            groupitem->setData(2, Qt::UserRole, strGroupName1);


                    if(Type!="1")
                    {
                     deviceitem->setText(1,Value);
                    }
                    deviceitem->setText(2,Type);
                    deviceitem->setText(3,Offset);

                    }

                devicenode = devicenode.nextSibling();
            }

        }

        //下一个分组节点
        groupnode = groupnode.nextSibling(); 
    }

    return;
}

void Widget::newGroup()
{
    QTreeWidgetItem *newgroup = new QTreeWidgetItem(ui->treeWidget);
    newgroup->setText(0, QStringLiteral("新建组"));
    newgroup->setIcon(0, QIcon(QStringLiteral(":/Widget/Resources/image/group.png")));
    newgroup->setWhatsThis(0, "Group");
}

void Widget::delGroup()
{
    QTreeWidgetItem *curritem = ui->treeWidget->currentItem();
    if (curritem != NULL)
    {
#if 0  //全部项都可删除
        QTreeWidgetItem *curritemparent = curritem->parent();
        if (curritemparent == NULL)
        {
            int index = ui->treeWidget->indexOfTopLevelItem(curritem);
            if (index != -1)
            {
                ui->treeWidget->takeTopLevelItem(index);
            }
        }
        else
        {
            int countChild = curritemparent->childCount();
            for (int i = 0; i < countChild; i++)
            {
                if (curritemparent->child(i) == curritem)
                {
                    curritemparent->takeChild(i);
                    break;
                }
            }
        }
#else  //只可删除组
        QString strItemType = curritem->whatsThis(0);
        if (strItemType == QStringLiteral("Group"))
        {
            int index = ui->treeWidget->indexOfTopLevelItem(curritem);
            if (index != -1)
            {
                ui->treeWidget->takeTopLevelItem(index);
            }
        }
#endif
    }
}

void Widget::saveXML(QString fileName)
{
    QTreeWidgetItem *rootItem = ui->treeWidget->invisibleRootItem(); //树的根节点
    int countGroup = rootItem->childCount();
    if (countGroup <= 0)  //列表树没有项
    {
        return;
    }

    QFile db(fileName);
    if (!db.open(QIODevice::WriteOnly | QIODevice::Truncate)) return;

    //只写方式打开，并清空以前的信息
    QDomDocument doc;
    QDomProcessingInstruction instruction;
    instruction = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
    doc.appendChild(instruction);

    QDomElement root = doc.createElement("ManagedDevice");
    doc.appendChild(root); //添加根元素

    for (int i = 0; i < countGroup; i++)
    {
        QTreeWidgetItem *groupitem = rootItem->child(i);
        QString groupname = groupitem->text(0);
        QDomElement elementGrp = doc.createElement("Group");
        QDomAttr elementGrpAttr = doc.createAttribute("name");
        elementGrpAttr.setValue(groupname);
        elementGrp.setAttributeNode(elementGrpAttr);
        root.appendChild(elementGrp);

        int countDev = groupitem->childCount();
        for (int j = 0; j < countDev; j++)
        {
            QTreeWidgetItem *deviceitem = groupitem->child(j);
            QDomElement elementDev = doc.createElement("Device");

            //Device项属性name
            QString devicename = deviceitem->text(0);
            QDomAttr elementDevNameAttr = doc.createAttribute("name");
            elementDevNameAttr.setValue(devicename);
            elementDev.setAttributeNode(elementDevNameAttr);

            //Device项属性ip
            QString deviceip = deviceitem->data(0, Qt::UserRole).toString();
            QDomAttr elementDevIpAttr = doc.createAttribute("ip");
            elementDevIpAttr.setValue(deviceip);
            elementDev.setAttributeNode(elementDevIpAttr);

            elementGrp.appendChild(elementDev);

            int countChn = deviceitem->childCount();
            for (int k = 0; k < countChn; k++)
            {
                QTreeWidgetItem *channelitem = deviceitem->child(k);
                QString channelname = channelitem->text(0);
                QDomElement elementChn = doc.createElement("Channel");
                QDomAttr elementChnAttr = doc.createAttribute("name");
                elementChnAttr.setValue(channelname);
                elementChn.setAttributeNode(elementChnAttr);
                elementDev.appendChild(elementChn);

                QString strRtsp = channelitem->data(0, Qt::UserRole).toString();
                QDomElement elementRtsp = doc.createElement("RtspUrl");
                elementChn.appendChild(elementRtsp);
                QDomText textnodeRtsp = doc.createTextNode(strRtsp);
                elementRtsp.appendChild(textnodeRtsp);
            }
        }
    }

    QTextStream out(&db);
    out.setCodec("UTF-8");
    doc.save(out, 4);
    db.close();

    QMessageBox tmpMsgBox(QMessageBox::Information, QStringLiteral("提示"), QStringLiteral("保存Xml文件成功！"), QMessageBox::Ok);
    tmpMsgBox.exec();
}

void Widget::on_pushButton_2_clicked()
{
    QTreeWidgetItem *TEMP;
    TEMP=ui->treeWidget->itemAt(1,5);
    ui->textBrowser->append(TEMP->text(0));
    ui->textBrowser->append(TEMP->text(1));
    ui->textBrowser->append(TEMP->text(2));
    ui->textBrowser->append(TEMP->text(3));
}
