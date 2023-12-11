// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "immodel.h"

#include <qabstractitemmodel.h>
#include <qdebug.h>

using namespace dcc_im_plugin::operation;

QDebug operator<<(QDebug debug, const IMData &v)
{
    QDebugStateSaver saver(debug);
    debug.nospace() << "("
                    << "name:" << v.name << ","
                    << "type:" << v.type << ")";
    return debug;
}

IMModel::IMModel(QObject *obj)
    : QAbstractListModel(obj)
{
}

QVariant IMModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_IMDataList.size()) {
        return QVariant();
    }

    const IMData &imEntry = m_IMDataList.at(index.row());

    switch (role) {
    case Qt::DisplayRole:
    case IMNameRole:
        return imEntry.name;
    case IMTypeRole:
        return imEntry.type;
    default:
        return QVariant();
    }
}

int IMModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0;
    }

    return m_IMDataList.count();
}

QHash<int, QByteArray> IMModel::roleNames() const
{
    return { { IMNameRole, "name" }, { IMTypeRole, "type" } };
}

bool IMModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);

    endInsertRows();
    return true;
}

void IMModel::move(int from, int to)
{
    if (from < 0 || to < 0 || from >= m_IMDataList.size() || to >= m_IMDataList.size()) {
        return;
    }
    beginMoveRows(QModelIndex(), from, from, QModelIndex(), to > from ? to + 1 : to);
    m_IMDataList.move(from, to);
    endMoveRows();
    Q_EMIT imListChanged(m_IMDataList);
}

void IMModel::remove(int index)
{
    if (index < 0 || index >= m_IMDataList.size()) {
        return;
    }
    beginRemoveRows(QModelIndex(), index, index);
    m_IMDataList.removeAt(index);
    endRemoveRows();
    Q_EMIT imListChanged(m_IMDataList);
}

void setIMList(const IMDataList &imDataList)
{
    /* beginResetModel(); */
    /* m_IMDataList = imDataList; */
    /* endResetModel(); */
}

void IMModel::clear()
{
    while (rowCount()) {
        removeRow(0);
    }
    m_IMDataList.clear();
}
