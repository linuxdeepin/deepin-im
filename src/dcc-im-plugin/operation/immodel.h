// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef IMMODEL_H
#define IMMODEL_H

#include <qlist.h>
#include <QAbstractListModel>
#include <QDataStream>

namespace dcc_im_plugin {
namespace operation {

struct IMData
{
    QString name;
    QString type;

    IMData& operator=(const IMData &imData)
    {
      if (this == &imData)
        return *this;
      this->name = imData.name;
      this->type = imData.type;
      return *this;
    }
    friend QDebug operator<<(QDebug debug, const IMData &v);
};
using IMDataList = QList<IMData>;

enum ItemDataRole {
    IMNameRole = Qt::UserRole + 1,
    IMTypeRole,
};

class IMModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit IMModel(QObject *obj = nullptr);
    ~IMModel() = default;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QHash<int, QByteArray> roleNames() const override;

    bool insertRows(int row, int count, const QModelIndex &parent) override;

    int count() const { return rowCount(); }

    void setIMList(const IMDataList &imDataList);

    void clear();

public Q_SLOTS:
    void move(int from, int to);
    void remove(int index);

Q_SIGNALS:
    void imListChanged(const IMDataList &imDataList);

private:
    IMDataList m_IMDataList;
};
} // namespace operation
} // namespace dcc_im_plugin
#endif // !IMMODEL_H
