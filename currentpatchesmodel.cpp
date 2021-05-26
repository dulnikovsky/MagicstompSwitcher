
#include "currentpatchesmodel.h"
#include <QFont>
#include <QApplication>

QVariant CurrentPatchesModel::data(const QModelIndex &index, int role) const
{
    if( role == Qt::DisplayRole)
    {
        return nameList.at(index.row());
    }
    else if( role == Qt::FontRole )
    {
        QFont font = qApp->font();
        font.setPointSize(32);
        return QVariant::fromValue<QFont>(font);
    }
    return QVariant();
}

void CurrentPatchesModel::onCurrentPatchChanged(unsigned int msId, const QString &name)
{
    int pos = idList.indexOf(msId);
    if( pos < 0)
    {
        beginInsertRows(QModelIndex(), idList.size(), idList.size());
        idList.append(msId);
        nameList.append(name);
        endInsertRows();
    }
    else
    {
       nameList[pos] = name;
       emit dataChanged( createIndex(pos, 0), createIndex(pos, 0));
    }
}

void CurrentPatchesModel::onMSRemoved(unsigned int msId)
{
    int pos = idList.indexOf(msId);
    if( pos >= 0)
    {
        beginRemoveRows(QModelIndex(), pos, pos);
        idList.removeAt(pos);
        nameList.removeAt(pos);
        endRemoveRows();
    }
}
