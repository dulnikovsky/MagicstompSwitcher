/****************************************************************************
**
** Copyright (C) 2021 Robert Vetter.
**
** This file is part of the MagicstompSwitcher
**
** THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF
** ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY
** IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR
** PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.
**
** GNU General Public License Usage
** This file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version . The licenses are
** as published by the Free Software Foundation and appearing in the file LICENSE
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**/

#include "mainwidget.h"
#include <QPalette>
#include <QLabel>
#include <QFont>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListView>
#include <QDebug>

#include "currentpatchesmodel.h"

MainWidget::MainWidget(QWidget *parent)
    :QWidget(parent)
{
    cpModel = new CurrentPatchesModel(this);

    QPalette plt = this->palette();
    plt.setColor(QPalette::Window, Qt::black);
    plt.setColor(QPalette::WindowText, QColor(255,165,0));
    this->setPalette(plt);

    programNumberLabel = new QLabel(this);
    QFont fnt = programNumberLabel->font();
    fnt.setPointSize(72);
    fnt.setBold(true);
    programNumberLabel->setFont(fnt);

    QLabel *title = new QLabel("MagicstompSwitcher");
    title->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    plt.setColor(QPalette::WindowText, QColor(112, 169, 255));
    title->setPalette(plt);

    QListView *listView = new QListView();
    listView->setModel(cpModel);
    listView->setStyleSheet("background-color: black; color: Chartreuse");

    QVBoxLayout *mainLyt = new QVBoxLayout();

    QHBoxLayout *hLyt = new QHBoxLayout();
    hLyt->addWidget(programNumberLabel);
    hLyt->addWidget(listView);

    mainLyt->addLayout(hLyt);
    mainLyt->addWidget(title);
    this->setLayout(mainLyt);
}

void MainWidget::setCurrentProgram(unsigned char val)
{
    QString strVal;
    if(val < 9)
        strVal.append(' ');
    programNumberLabel->setText(strVal + QString::number(val+1));
}

void MainWidget::onCurrentPatchChanged(unsigned int id, const QString &name, bool isRequesting)
{
    cpModel->onCurrentPatchChanged(id, name, isRequesting);
}

void MainWidget::onMsDisconnected(unsigned int id)
{
    cpModel->onMSRemoved(id);
}
