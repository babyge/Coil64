/* system_functions.h - header text to Coil64 - Radio frequency inductor and choke calculator
Copyright (C) 2019 Kustarev V.

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

#ifndef SYSTEM_FUNCTIONS_H
#define SYSTEM_FUNCTIONS_H

#include <QCoreApplication>
#include <QStandardPaths>
#include <QProcess>
#include <QSettings>
#include <QString>
#include <QSysInfo>
#include <QMessageBox>
#include <QLocale>
#include <QDate>
#include <QFile>
#include <QDir>


#include "definitions.h"


#ifdef __linux__
#include <sys/utsname.h>
#endif

enum _StyleGUI {
    _DefaultStyle,
    _SunnyStyle,
    _DarkStyle
};

#pragma pack(push,1)
    struct _OptionStruct{
      unsigned int dwAccuracy;
      int indexCapacityMultiplier;
      int indexInductanceMultiplier;
      int indexFrequencyMultiplier;
      int indexLengthMultiplier;
      double dwCapacityMultiplier;
      double dwInductanceMultiplier;
      double dwFrequencyMultiplier;
      double dwLengthMultiplier;
      QString ssCapacityMeasureUnit;
      QString ssInductanceMeasureUnit;
      QString ssFrequencyMeasureUnit;
      QString ssLengthMeasureUnit;
      QString mainFontFamily;
      QString textFontFamily;
      int mainFontSize;
      int textFontSize;
      bool isAutomaticUpdate;
      bool isEnglishLocale;
      bool isConfirmExit;
      bool isConfirmClear;
      bool isConfirmDelete;
      bool isAWG;
      bool isInsertImage;
      bool isSaveOnExit;
      bool isAdditionalResult;
      bool isPCBcoilSquare;
      bool isLastShowingFirst;
      int styleGUI;
      int upDateInterval;
      QDate firstDate;
    };
#pragma pack(pop)

QString getOSVersion();
bool isAppPortable();
void defineAppSettings(QSettings *&settings);
QString defineSavePath();
void showWarning(QString title, QString msg);
void showInfo(QString title, QString msg);
QStringList translateInstalling(QStringList *lang);
QLocale getLanguageLocale (QString lang);
void completeOptionsStructure(_OptionStruct *opt);
QString formatLength(double length, double lengthMultiplyer);

#endif // SYSTEM_FUNCTIONS_H
