#include "dialog.h"
#include "ui_dialog.h"
#include <qmath.h>
#include <QMessageBox>
#include <QSettings>
#include <QTextEdit>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    setWindowTitle("两相组成计算泡点温度t,理论板数n");


    readSettings();
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_pushButton_clicked()
{
    ax=ui->axValue->value();
    bx=ui->bxValue->value();
    cx=ui->cxValue->value();
    ay=ui->ayValue->value();
    by=ui->byValue->value();
    cy=ui->cyValue->value();
    xd=ui->xdValue->value();
    xw=ui->xwValue->value();
    xf=ui->xfValue->value();
    pressure=ui->pressureValue->value();
    for(t=1;t<=500;t+=0.01){
        P1=qPow(10,(ax-bx/(cx+t)));
        P2=qPow(10,(ay-by/(cy+t)));
        if(qFabs(P1*xd+P2*(1-xd)-pressure)<0.5){
            ui->labelT1->setText(QVariant(t).toString());
            ui->labelP11->setText(QVariant(P1).toString());
            ui->labelP21->setText(QVariant(P2).toString());
            a1=P1/P2;
            ui->labela1->setText(QVariant(a1).toString());
            break;
        }
    }
    if(t==501){
        QMessageBox msgBox;
        msgBox.setWindowTitle("错误!");
        msgBox.setText("XD:1-500摄氏度范围内没有泡点温度（很有可能参数输入有误）");
        msgBox.exec();
        t=1;
    }
    for(t=1;t<=500;t+=0.01){
        P1=qPow(10,(ax-bx/(cx+t)));
        P2=qPow(10,(ay-by/(cy+t)));
        if(qFabs(P1*xw+P2*(1-xw)-pressure)<0.5){
            ui->labelT2->setText(QVariant(t).toString());
            ui->labelP12->setText(QVariant(P1).toString());
            ui->labelP22->setText(QVariant(P2).toString());
            a2=P1/P2;
            ui->labela2->setText(QVariant(a2).toString());
            break;
        }
    }
    if(t==501){
        QMessageBox msgBox;
        msgBox.setWindowTitle("错误!");
        msgBox.setText("XW:1-500摄氏度范围内没有泡点温度（很有可能参数输入有误）");
        msgBox.exec();
        t=1;
    }
    for(t=1;t<=500;t+=0.01){
        P1=qPow(10,(ax-bx/(cx+t)));
        P2=qPow(10,(ay-by/(cy+t)));
        if(qFabs(P1*xf+P2*(1-xf)-pressure)<0.5){
            ui->labelT3->setText(QVariant(t).toString());
            ui->labelP13->setText(QVariant(P1).toString());
            ui->labelP23->setText(QVariant(P2).toString());
            a3=P1/P2;
            ui->labela3->setText(QVariant(a3).toString());
            break;
        }
    }
    if(t==501){
        QMessageBox msgBox;
        msgBox.setWindowTitle("错误!");
        msgBox.setText("XF:1-500摄氏度范围内没有泡点温度（很有可能参数输入有误）");
        msgBox.exec();
        t=1;
    }
    am=qPow(a1*a2*a3,1.0/3.0);
    ui->labelam->setText(QVariant(am).toString());
    Rmin=1/(am-1)*(xd/xf-am*(1-xd)/(1-xf));


    QTextEdit *jldMSG=new QTextEdit;
    QTextEdit *tldMSG=new QTextEdit;
    jldMSG->setReadOnly(true);
    tldMSG->setReadOnly(true);
    jldMSG->setWindowFlags(Qt::WindowStaysOnTopHint);
    tldMSG->setWindowFlags(Qt::WindowStaysOnTopHint);
    jldMSG->setMaximumWidth(500);
    jldMSG->setMinimumWidth(500);
    tldMSG->setMaximumWidth(500);
    tldMSG->setMinimumWidth(500);
    tldMSG->setWindowTitle("提馏");
    jldMSG->setWindowTitle("精馏");

    double x,y;
    int n=1;

    R=ui->RValue->value()*Rmin;
    L=ui->LValue->value();
    W=ui->WValue->value();
    if(ui->RValue->value()==0 || L==0 || W==0){
        QMessageBox msgBox;
        msgBox.setWindowTitle("错误!");
        msgBox.setText("参数错误");
        msgBox.exec();
        return;
    }

    y=xd;
    x=y/(am-(am-1)*y);
    for(double j=1;j<=500;j+=0.01){
        if(qFabs(qPow(10,(ax-bx/(cx+j)))*x+qPow(10,ay-by/(cy+j))*(1-x)-pressure)<0.5){
            temperature=j;
            break;
        }
    }
    jld= "y"+QVariant(n).toString()+"="+QVariant(y).toString()+
            "\tx"+QVariant(n).toString()+"="+QVariant(x).toString()+"\t泡点温度t:"+QVariant(temperature).toString()+"\n";
    while(x>xf){
        y=R/(R+1)*x+xd/(R+1);
        x=y/(am-(am-1)*y);
        for(double j=1;j<=500;j+=0.01){
            if(qFabs(qPow(10,(ax-bx/(cx+j)))*x+qPow(10,ay-by/(cy+j))*(1-x)-pressure)<0.5){
                temperature=j;
                break;
            }
        }
        ++n;
        jld=jld+"y"+QVariant(n).toString()+"="+QVariant(y).toString()+
                "\tx"+QVariant(n).toString()+"="+QVariant(x).toString()+"\t泡点温度t:"+QVariant(temperature).toString()+"\n";
    }
    xn=x;
    ui->labeljldn->setText(QVariant(n).toString());

    y=R/(R+1)*xn+xd/(R+1);
    x=y/(am-(am-1)*y);
    for(double j=1;j<=500;j+=0.01){
        if(qFabs(qPow(10,(ax-bx/(cx+j)))*x+qPow(10,ay-by/(cy+j))*(1-x)-pressure)<0.5){
            temperature=j;
            break;
        }
    }
    n=1;
    tld="y"+QVariant(n).toString()+"="+QVariant(y).toString()+
            "\tx"+QVariant(n).toString()+"="+QVariant(x).toString()+"\t泡点温度t:"+QVariant(temperature).toString()+"\n";
    while(x>xw){
        y=L/(L-W)*x-W/(L-W)*xw;
        x=y/(am-(am-1)*y);
        for(double j=1;j<=500;j+=0.01){
            if(qFabs(qPow(10,(ax-bx/(cx+j)))*x+qPow(10,ay-by/(cy+j))*(1-x)-pressure)<0.5){
                temperature=j;
                break;
            }
        }
        ++n;
        tld=tld+"y"+QVariant(n).toString()+"="+QVariant(y).toString()+
                "\tx"+QVariant(n).toString()+"="+QVariant(x).toString()+"\t泡点温度t:"+QVariant(temperature).toString()+"\n";
    }
    xm=x;
    ui->labeltld->setText(QVariant(n).toString());
    jldMSG->setText(jld);
    tldMSG->setText(tld);
    jldMSG->show();
    tldMSG->show();
}

void Dialog::on_xdValue_valueChanged(double arg1)
{
    ui->ydValue->display(1-ui->xdValue->value());
}

void Dialog::on_xwValue_valueChanged(double arg1)
{
    ui->ywValue->display(1-ui->xwValue->value());
}

void Dialog::on_xfValue_valueChanged(double arg1)
{
    ui->yfValue->display(1-ui->xfValue->value());
}

void Dialog::readSettings()
{
    QSettings settings("CUMT_chemical","calculate");
    ui->axValue->setValue(settings.value("ax").toDouble());
    ui->bxValue->setValue(settings.value("bx").toDouble());
    ui->cxValue->setValue(settings.value("cx").toDouble());
    ui->ayValue->setValue(settings.value("ay").toDouble());
    ui->byValue->setValue(settings.value("by").toDouble());
    ui->cyValue->setValue(settings.value("cy").toDouble());
    ui->RValue->setValue(settings.value("R").toDouble());
    ui->WValue->setValue(settings.value("W").toDouble());
    ui->LValue->setValue(settings.value("L").toDouble());
    ui->pressureValue->setValue(settings.value("pressure").toDouble());
    ui->xdValue->setValue(settings.value("xd").toDouble());
    ui->xwValue->setValue(settings.value("xw").toDouble());
    ui->xfValue->setValue(settings.value("xf").toDouble());
}

void Dialog::writeSettings()
{
    QSettings settings("CUMT_chemical","calculate");
    settings.clear();
    settings.setValue("ax",ui->axValue->value());
    settings.setValue("bx",ui->bxValue->value());
    settings.setValue("cx",ui->cxValue->value());
    settings.setValue("ay",ui->ayValue->value());
    settings.setValue("by",ui->byValue->value());
    settings.setValue("cy",ui->cyValue->value());
    settings.setValue("R",ui->RValue->value());
    settings.setValue("L",ui->LValue->value());
    settings.setValue("W",ui->WValue->value());
    settings.setValue("pressure",ui->pressureValue->value());
    settings.setValue("xd",ui->xdValue->value());
    settings.setValue("xw",ui->xwValue->value());
    settings.setValue("xf",ui->xfValue->value());
}

void Dialog::closeEvent(QCloseEvent*e)
{
    writeSettings();
    e->accept();
}
