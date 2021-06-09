
#include "currentpatchesmodel.h"
#include <QFont>
#include <QApplication>
#include <QColor>

QVariant CurrentPatchesModel::data(const QModelIndex &index, int role) const
{
    if( role == Qt::DisplayRole)
    {
        return msMap.value(index.internalId()).curPatchName;
    }
    else if( role == Qt::FontRole )
    {
        QFont font = qApp->font();
        font.setPointSize(28);
        return QVariant::fromValue<QFont>(font);
    }
    else if( role == Qt::ForegroundRole )
    {
        if(msMap.value(index.internalId()).is_Requesting)
            return QColor(Qt::red);
    }
    return QVariant();
}

QModelIndex CurrentPatchesModel::index(int row, int column, const QModelIndex &) const
{
    auto iter = msMap.constBegin();
    if(iter == msMap.constEnd())
        return QModelIndex();

    int rowcounter = 0;
    while( rowcounter < row && iter != msMap.constEnd())
    {
        rowcounter++;
        iter++;
    }

    return createIndex(row, column, iter.key());
}

void CurrentPatchesModel::onCurrentPatchChanged(unsigned int msId, const QString &name, bool isRequesting)
{
    struct MSData msdata;
    msdata.curPatchName = name;
    msdata.is_Requesting = isRequesting;
    if( ! msMap.contains(msId))
    {
        beginResetModel();
        msMap.insert(msId, msdata);
        endResetModel();
    }
    else
    {
       auto iter = msMap.insert(msId, msdata);
       int rowcounter = 0;
       auto iterSearch = msMap.begin();
       while( iterSearch != iter && iterSearch != msMap.end())
       {
           rowcounter++;
           iterSearch++;
       }
       emit dataChanged( index(rowcounter, 0, QModelIndex()), index(rowcounter, 0, QModelIndex()));
    }
}


void CurrentPatchesModel::onMSRemoved(unsigned int msId)
{
    beginResetModel();
    msMap.remove(msId);
    endResetModel();
}
