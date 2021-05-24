/****************************************************************************
**
** Copyright (C) 2021 Robert Vetter.
**
** This file is part of the MagicstompFrenzy - an editor for Yamaha Magicstomp
** effect processor
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

MainWidget::MainWidget(QWidget *parent)
    :QWidget(parent)
{
    QPalette plt = this->palette();
    plt.setColor(QPalette::Window, Qt::black);
    plt.setColor(QPalette::WindowText, QColor(255,165,0));
    this->setPalette(plt);

    programNumberLabel = new QLabel(this);
    QFont fnt = programNumberLabel->font();
    fnt.setPixelSize(144);
    fnt.setBold(true);
    programNumberLabel->setFont(fnt);

    QLabel *title = new QLabel("MagicstompSwitcher");
    plt.setColor(QPalette::WindowText, QColor(112, 169, 255));
    title->setPalette(plt);

    QListView *listView = new QListView();
    listView->setStyleSheet("background-color: black; color: yellow;");

    QHBoxLayout *mainLyt = new QHBoxLayout();

    QVBoxLayout *vLyt = new QVBoxLayout();
    vLyt->addWidget(programNumberLabel);
    vLyt->addWidget(title);

    mainLyt->addLayout(vLyt);
    mainLyt->addWidget(listView);
    this->setLayout(mainLyt);
}

void MainWidget::setCurrentProgram(unsigned char val)
{
    programNumberLabel->setText(QString::number(val+1));
}

void MainWidget::onPatchNameChanged(unsigned int id, const QString &name)
{
    qDebug() << "New Patch name:" << name << " ,id=" << id;
}

void MainWidget::onMsDisconnected(unsigned int id)
{

}
