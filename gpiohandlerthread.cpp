/****************************************************************************
**
** Copyright (C) 2025 Robert Vetter.
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

#include "gpiohandlerthread.h"
#include <linux/gpio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/poll.h>

GPIOHandlerThread::GPIOHandlerThread(const QString &device, const QList<int> &offsetList, QObject *parent)
    :QThread(parent), device(device), offsetList(offsetList)
{

}

void GPIOHandlerThread::start(Priority priority)
{
    keepRunning = true;
    QThread::start(priority);
}

void GPIOHandlerThread::finish()
{
    keepRunning = false;
}

void GPIOHandlerThread::run()
{
    struct gpio_v2_line_request req;
    struct pollfd pfd;
    int fd;

    const QByteArray devNameArr = device.toLocal8Bit();

    fd = open(devNameArr.constData(), O_RDONLY);
    if (fd < 0)
    {
        qDebug("Unable to open %%s: %%s", devNameArr.constData(), strerror(errno));
        return;
    }

    memset(&req, 0, sizeof(req));
    req.config.flags = GPIO_V2_LINE_FLAG_INPUT| GPIO_V2_LINE_FLAG_EDGE_RISING | GPIO_V2_LINE_FLAG_EDGE_FALLING;
    req.config.num_attrs = 0;
    req.num_lines = offsetList.size();
    for(int i=0; i<offsetList.size() && i<GPIO_V2_LINES_MAX; i++) {
        req.offsets[i] = offsetList.at(i);
    }
    if (ioctl(fd, GPIO_V2_GET_LINE_IOCTL, &req) < 0) {
      qDebug("Error on chip io");
      close(fd);
      return;
    }

    pfd.fd = req.fd;
    pfd.events = POLLIN;

    while (keepRunning) {
        int ret = poll(&pfd, 1, 1000);
        if (ret < 0) {
            qDebug("poll() failed!");
            break;
        }
        if(ret == 0) {
            continue;
        }
        gpio_v2_line_event event;
        int read_ret = read(req.fd, &event, sizeof(event));
        if(read_ret>0) {
            qDebug("GPIO Event offset=%d, id=%d", event.offset, event.id);
        }
    }
}
