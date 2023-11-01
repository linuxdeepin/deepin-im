// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef UTILFUNCS_H
#define UTILFUNCS_H

#include <QApplication>
#include <QDir>
#include <QFont>

/*************************************
公共函数 该公共空间内存放与项目无关的通用函数

**************************************/
class QKeyEvent;

namespace utilFuncs {

void pause(const unsigned int &msec);
void moveToWindowCenter(QWidget *widget);
bool createDir(const QString &dir);
bool removeDir(const QString &dir);
bool createFile(const QString &filePath,
                const QString &text,
                QIODevice::OpenMode model = QIODevice::WriteOnly);
QJsonDocument readJson(const QString &filePath);
QString readFile(const QString &filePath);
QString getImagePixel(const QString &imagePath);
QStringList getDirFileNames(const QString &dirPath,
                            const QDir::Filters &flag = QDir::Files,
                            const QStringList &suffix = QStringList());
QPair<int, QString> startPopen(const QString &cmd, const QString &model = QString("r"));
int fontSize(const QString &str, const QFont &font = qApp->font());
QString getKeyValue(const QKeyEvent *event);
QString getKeyValue(const int &key);
QString transFirstUpper(const QString &str); // 字符串转换成首字母大写

}; // namespace utilFuncs

#endif // PUBLISHERFUNC_H
