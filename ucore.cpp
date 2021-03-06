/* ucore.cpp - source text to Coil64 - Radio frequency inductor and choke calculator
Copyright (C) 2020 Kustarev V.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses
*/

#include "ucore.h"
#include "ui_ucore.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
UCore::UCore(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UCore)
{
    ui->setupUi(this);
    fOpt = new _OptionStruct;
    dv = new QDoubleValidator(0.0, MAX_DOUBLE, 380);
    ui->lineEdit_a->setValidator(dv);
    ui->lineEdit_b->setValidator(dv);
    ui->lineEdit_c->setValidator(dv);
    ui->lineEdit_d->setValidator(dv);
    ui->lineEdit_e->setValidator(dv);
    ui->lineEdit_f->setValidator(dv);
    ui->lineEdit_s->setValidator(dv);
    ui->lineEdit_mu->setValidator(dv);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
UCore::~UCore()
{
    bool isReverse = ui->checkBox_isReverce->isChecked();
    if (isReverse)
        N = loc.toDouble(ui->lineEdit_N->text());
    else
        ind = loc.toDouble(ui->lineEdit_N->text())*fOpt->dwInductanceMultiplier;
    A = loc.toDouble(ui->lineEdit_a->text())*fOpt->dwLengthMultiplier;
    B = loc.toDouble(ui->lineEdit_b->text())*fOpt->dwLengthMultiplier;
    C = loc.toDouble(ui->lineEdit_c->text())*fOpt->dwLengthMultiplier;
    D = loc.toDouble(ui->lineEdit_d->text())*fOpt->dwLengthMultiplier;
    E = loc.toDouble(ui->lineEdit_e->text())*fOpt->dwLengthMultiplier;
    F = loc.toDouble(ui->lineEdit_f->text())*fOpt->dwLengthMultiplier;
    s = loc.toDouble(ui->lineEdit_s->text())*fOpt->dwLengthMultiplier;
    mu = loc.toDouble(ui->lineEdit_mu->text());

    QSettings *settings;
    defineAppSettings(settings);
    settings->beginGroup( "UCore" );
    settings->setValue("pos", this->pos());
    settings->setValue("size", this->size());
    settings->setValue("isReverse", ui->checkBox_isReverce->isChecked());
    settings->setValue("currStdCore", ui->comboBox->currentIndex());
    settings->setValue("ind", ind);
    settings->setValue("N", N);
    settings->setValue("A", A);
    settings->setValue("B", B);
    settings->setValue("C", C);
    settings->setValue("D", D);
    settings->setValue("E", E);
    settings->setValue("F", F);
    settings->setValue("s", s);
    settings->setValue("mu", mu);
    settings->endGroup();

    delete settings;
    delete fOpt;
    delete dv;
    delete ui;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UCore::getOpt(_OptionStruct gOpt)
{
    *fOpt = gOpt;
    ui->label_N_m->setText(qApp->translate("Context", fOpt->ssInductanceMeasureUnit.toUtf8()));
    ui->label_01->setText(qApp->translate("Context", fOpt->ssLengthMeasureUnit.toUtf8()));
    ui->label_02->setText(qApp->translate("Context", fOpt->ssLengthMeasureUnit.toUtf8()));
    ui->label_03->setText(qApp->translate("Context", fOpt->ssLengthMeasureUnit.toUtf8()));
    ui->label_04->setText(qApp->translate("Context", fOpt->ssLengthMeasureUnit.toUtf8()));
    ui->label_05->setText(qApp->translate("Context", fOpt->ssLengthMeasureUnit.toUtf8()));
    ui->label_06->setText(qApp->translate("Context", fOpt->ssLengthMeasureUnit.toUtf8()));
    ui->label_07->setText(qApp->translate("Context", fOpt->ssLengthMeasureUnit.toUtf8()));
    QSettings *settings;
    defineAppSettings(settings);
    settings->beginGroup( "UCore" );
    QRect screenGeometry = qApp->primaryScreen()->availableGeometry();
    int x = (screenGeometry.width() - this->width()) / 2;
    int y = (screenGeometry.height() - this->height()) / 2;
    QPoint pos = settings->value("pos", QPoint(x, y)).toPoint();
    QSize size = settings->value("size", this->minimumSize()).toSize();
    bool isReverse = settings->value("isReverse", false).toBool();
    currStdCore = settings->value("currStdCore", 0).toInt();

    N = settings->value("N", 0).toDouble();
    ind = settings->value("ind", 0).toDouble();
    A = settings->value("A", 0).toDouble();
    B = settings->value("B", 0).toDouble();
    C = settings->value("C", 0).toDouble();
    D = settings->value("D", 0).toDouble();
    E = settings->value("E", 0).toDouble();
    F = settings->value("F", 0).toDouble();
    s = settings->value("s", 0).toDouble();
    mu = settings->value("mu", 100).toDouble();
    settings->endGroup();
    if (isReverse)
        ui->lineEdit_N->setText(loc.toString(N));
    else
        ui->lineEdit_N->setText(loc.toString(ind / fOpt->dwInductanceMultiplier));
    ui->lineEdit_a->setText(loc.toString(A / fOpt->dwLengthMultiplier));
    ui->lineEdit_b->setText(loc.toString(B / fOpt->dwLengthMultiplier));
    ui->lineEdit_c->setText(loc.toString(C / fOpt->dwLengthMultiplier));
    ui->lineEdit_d->setText(loc.toString(D / fOpt->dwLengthMultiplier));
    ui->lineEdit_e->setText(loc.toString(E / fOpt->dwLengthMultiplier));
    ui->lineEdit_f->setText(loc.toString(F / fOpt->dwLengthMultiplier));
    ui->lineEdit_s->setText(loc.toString(s / fOpt->dwLengthMultiplier));
    ui->lineEdit_mu->setText(loc.toString(mu));

    ui->label_s->setText(tr("Slot size")+" s:");
    ui->label_mu->setText(tr("Magnetic permeability")+" µ:");
    ui->comboBox->setCurrentIndex(currStdCore);
    on_comboBox_currentIndexChanged(currStdCore);
    resize(size);
    move(pos);
    ui->checkBox_isReverce->setChecked(isReverse);
    on_checkBox_isReverce_clicked();
    delete settings;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UCore::getCurrentLocale(QLocale locale)
{
    this->loc = locale;
    this->setLocale(loc);
    dv->setLocale(loc);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UCore::on_checkBox_isReverce_clicked()
{
    if (ui->checkBox_isReverce->isChecked()){
        ui->lineEdit_N->setText(loc.toString(N, 'f', fOpt->dwAccuracy));
        QString tmpTxt = tr("Number of turns") + " N:";
        ui->label_N->setText(tmpTxt);
        ui->label_N_m->setVisible(false);
    } else {
        ui->lineEdit_N->setText(loc.toString(ind / fOpt->dwInductanceMultiplier, 'f', fOpt->dwAccuracy));
        QString tmpTxt = tr("Inductance") + " L:";
        ui->label_N->setText(tmpTxt);
        ui->label_N_m->setVisible(true);
    }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UCore::on_pushButton_close_clicked()
{
    this->close();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UCore::on_comboBox_currentIndexChanged(int index)
{
    switch (index) {
    case 0:{
        ui->image->setPixmap(QPixmap(":/images/res/UU-core.png"));
        ui->label_s->setVisible(false);
        ui->lineEdit_s->setVisible(false);
        ui->label_07->setVisible(false);
        ui->label_f->setVisible(false);
        ui->lineEdit_f->setVisible(false);
        ui->label_06->setVisible(false);
    }
        break;
    case 1:{
        ui->image->setPixmap(QPixmap(":/images/res/UR-core.png"));
        ui->label_s->setVisible(true);
        ui->lineEdit_s->setVisible(true);
        ui->label_07->setVisible(true);
        ui->label_f->setVisible(true);
        ui->lineEdit_f->setVisible(true);
        ui->label_06->setVisible(true);
    }
        break;
    case 2:{
        ui->image->setPixmap(QPixmap(":/images/res/UY-core.png"));
        ui->label_s->setVisible(true);
        ui->lineEdit_s->setVisible(true);
        ui->label_07->setVisible(true);
        ui->label_f->setVisible(false);
        ui->lineEdit_f->setVisible(false);
        ui->label_06->setVisible(false);
    }
        break;
    default:
        break;
    }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UCore::on_pushButton_help_clicked()
{
    QDesktopServices::openUrl(QUrl("https://coil32.net/ferrite-u-core.html"));
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UCore::on_pushButton_calculate_clicked()
{
    int index = ui->comboBox->currentIndex();
    if ((ui->lineEdit_N->text().isEmpty())||(ui->lineEdit_a->text().isEmpty())||(ui->lineEdit_b->text().isEmpty())
            ||(ui->lineEdit_c->text().isEmpty()) ||(ui->lineEdit_d->text().isEmpty()) ||(ui->lineEdit_e->text().isEmpty())){
        showWarning(tr("Warning"), tr("One or more inputs are empty!"));
        return;
    }
    bool ok1, ok2, ok3, ok4, ok5, ok6, ok7, ok9;
    A = loc.toDouble(ui->lineEdit_a->text(), &ok1)*fOpt->dwLengthMultiplier;
    B = loc.toDouble(ui->lineEdit_b->text(), &ok2)*fOpt->dwLengthMultiplier;
    C = loc.toDouble(ui->lineEdit_c->text(), &ok3)*fOpt->dwLengthMultiplier;
    D = loc.toDouble(ui->lineEdit_d->text(), &ok4)*fOpt->dwLengthMultiplier;
    E = loc.toDouble(ui->lineEdit_e->text(), &ok5)*fOpt->dwLengthMultiplier;
    if (index == 0){
        F = 0;
    } else if (index == 1){
        F = loc.toDouble(ui->lineEdit_f->text(), &ok6)*fOpt->dwLengthMultiplier;
        if (!ok6){
            showWarning(tr("Warning"), tr("One or more inputs have an illegal format!"));
            return;
        }
        if (F == 0){
            showWarning(tr("Warning"), tr("One or more inputs are equal to null!"));
            return;
        }
    } else if (index == 2){
        F = -1;
    }
    if (index > 0){
        s = loc.toDouble(ui->lineEdit_s->text(), &ok7)*fOpt->dwLengthMultiplier;
        if (!ok7){
            showWarning(tr("Warning"), tr("One or more inputs have an illegal format!"));
            return;
        }
    } else {
        s = 0;
    }
    mu = loc.toDouble(ui->lineEdit_mu->text(), &ok9);
    if((!ok1)||(!ok2)||(!ok3)||(!ok4)||(!ok5)||(!ok9)){
        showWarning(tr("Warning"), tr("One or more inputs have an illegal format!"));
        return;
    }
    if ((A == 0)||(B == 0)||(C == 0)||(D == 0)||(E == 0)||(mu == 0)){
        showWarning(tr("Warning"), tr("One or more inputs are equal to null!"));
        return;
    }
    if (B < D){
        showWarning(tr("Warning"), "B < D");
        return;
    }
    _CoilResult result;
    if (ui->checkBox_isReverce->isChecked()){
        N = loc.toDouble(ui->lineEdit_N->text(), &ok1);
        if (!ok1){
            showWarning(tr("Warning"), tr("One or more inputs have an illegal format!"));
            return;
        }
        if (N == 0){
            showWarning(tr("Warning"), tr("One or more inputs are equal to null!"));
            return;
        }
        ind = findUCore_I(N,A,B,C,D,E,F,s,mu,&result);
    } else {
        ind = loc.toDouble(ui->lineEdit_N->text(), &ok1)*fOpt->dwInductanceMultiplier;
        if (!ok1){
            showWarning(tr("Warning"), tr("One or more inputs have an illegal format!"));
            return;
        }
        if (ind == 0){
            showWarning(tr("Warning"), tr("One or more inputs are equal to null!"));
            return;
        }
        N = findUCore_N(ind,A,B,C,D,E,F,s,mu,&result);
    }
    QString sResult = "<hr><h2>" +QCoreApplication::applicationName() + " " + QCoreApplication::applicationVersion() +
            " - " + windowTitle() + "</h2><br/>";
    if (fOpt->isInsertImage){
        switch (index) {
        case 0:
            sResult += "<img src=\":/images/res/UU-core.png\">";
            break;
        case 1:
            sResult += "<img src=\":/images/res/UR-core.png\">";
            break;
        case 2:
            sResult += "<img src=\":/images/res/UY-core.png\">";
             break;
        default:
            break;
        }
    }
    sResult += "<p><u>" + tr("Input data") + ":</u><br/>";
    if (ui->checkBox_isReverce->isChecked())
        sResult += ui->label_N->text() + " " + ui->lineEdit_N->text() + "<br/>";
    else
        sResult += ui->label_N->text() + " " + ui->lineEdit_N->text() + " " + ui->label_N_m->text() + "<br/>";
    sResult += ui->label_a->text() + " " + ui->lineEdit_a->text() + " " + ui->label_01->text() + "<br/>";
    sResult += ui->label_b->text() + " " + ui->lineEdit_b->text() + " " + ui->label_02->text() + "<br/>";
    sResult += ui->label_c->text() + " " + ui->lineEdit_c->text() + " " + ui->label_03->text() + "<br/>";
    sResult += ui->label_d->text() + " " + ui->lineEdit_d->text() + " " + ui->label_04->text() + "<br/>";
    sResult += ui->label_e->text() + " " + ui->lineEdit_e->text() + " " + ui->label_05->text() + "<br/>";
    if (index == 1)
        sResult += ui->label_f->text() + " " + ui->lineEdit_f->text() + " " + ui->label_06->text() + "<br/>";
    if (index > 0)
        sResult += ui->label_s->text() + " " + ui->lineEdit_s->text() + " " + ui->label_07->text() + "<br/>";
    sResult += ui->label_mu->text() + " " + ui->lineEdit_mu->text() + "<br/>";
    sResult += "<hr>";
    sResult += "<p><u>" + tr("Result") + ":</u><br/>";
    if (ui->checkBox_isReverce->isChecked()){
        sResult += tr("Inductance") + " L = " + loc.toString(ind / fOpt->dwInductanceMultiplier, 'f', fOpt->dwAccuracy) + " "
                + qApp->translate("Context", fOpt->ssInductanceMeasureUnit.toUtf8());
    } else {
        sResult += tr("Number of turns of the coil") + " N = " + loc.toString(N, 'f', fOpt->dwAccuracy);
    }
    sResult += "<br/><br/>" + tr("Effective magnetic path length") + " (l<sub>e</sub>): "
            + loc.toString(result.N/fOpt->dwLengthMultiplier, 'f', fOpt->dwAccuracy)
            + "&nbsp;" + qApp->translate("Context", fOpt->ssLengthMeasureUnit.toUtf8()) + "<br/>";
    sResult += tr("Effective area of magnetic path") + " (A<sub>e</sub>): "
            + loc.toString(result.sec/(fOpt->dwLengthMultiplier * fOpt->dwLengthMultiplier), 'f', fOpt->dwAccuracy)
            + "&nbsp;" + qApp->translate("Context", fOpt->ssLengthMeasureUnit.toUtf8()) + "<sup>2</sup><br/>";
    sResult += tr("Effective volume") + " (V<sub>e</sub>): "
            + loc.toString(result.N * result.sec/(fOpt->dwLengthMultiplier * fOpt->dwLengthMultiplier * fOpt->dwLengthMultiplier), 'f', fOpt->dwAccuracy)
            + "&nbsp;" + qApp->translate("Context", fOpt->ssLengthMeasureUnit.toUtf8()) + "<sup>3</sup><br/>";
    sResult += "</p><hr>";
    emit sendResult(sResult);
}
