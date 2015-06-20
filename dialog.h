#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QCloseEvent>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

private slots:
    void on_pushButton_clicked();

    void on_xdValue_valueChanged(double arg1);

    void on_xwValue_valueChanged(double arg1);

    void on_xfValue_valueChanged(double arg1);

private:
    Ui::Dialog *ui;
    double P1;
    double P2;
    double xd,xw,xf;
    double t;
    double ax,bx,cx,ay,by,cy;
    double pressure;
    double a1,a2,a3,am;
    double R,L,W;
    double xn,xm;
    double temperature;
    double Rmin;
    QString tld,jld;
protected:
    void readSettings();
    void writeSettings();
    void closeEvent(QCloseEvent *e);
};

#endif // DIALOG_H
