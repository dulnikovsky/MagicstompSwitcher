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

MainWidget::MainWidget(QWidget *parent)
    :QWidget(parent)
{
    QPalette plt = this->palette();
    plt.setColor(QPalette::Window, Qt::black);
    plt.setColor(QPalette::WindowText, Qt::yellow);
    this->setPalette(plt);

    programNumberLabel = new QLabel(this);
    QFont fnt = programNumberLabel->font();
    fnt.setPixelSize(144);
    fnt.setBold(true);
    programNumberLabel->setFont(fnt);

    QVBoxLayout *mainLyt = new QVBoxLayout();
    mainLyt->addWidget(programNumberLabel);
    this->setLayout(mainLyt);
}

void MainWidget::setCurrentProgram(unsigned char val)
{
    programNumberLabel->setText(QString::number(val+1));
}
