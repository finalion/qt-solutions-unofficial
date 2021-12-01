/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of Qt Creator.
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
****************************************************************************/

#include "qtlockedfile.h"

#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

namespace SharedTools {

bool QtLockedFile::lock(LockMode mode, bool block)
{
    if (!isOpen()) {
        qWarning("QtLockedFile::lock(): file is not opened");
        return false;
    }

    if (mode == NoLock)
        return unlock();

    if (mode == m_lock_mode)
        return true;

    if (m_lock_mode != NoLock)
        unlock();

    struct flock fl;
    fl.l_whence = SEEK_SET;
    fl.l_start = 0;
    fl.l_len = 0;
    fl.l_type = (mode == ReadLock) ? F_RDLCK : F_WRLCK;
    int cmd = block ? F_SETLKW : F_SETLK;
    int ret = fcntl(handle(), cmd, &fl);

    if (ret == -1) {
        if (errno != EINTR && errno != EAGAIN)
            qWarning("QtLockedFile::lock(): fcntl: %s", strerror(errno));
        return false;
    }


    m_lock_mode = mode;
    return true;
}


bool QtLockedFile::unlock()
{
    if (!isOpen()) {
        qWarning("QtLockedFile::unlock(): file is not opened");
        return false;
    }

    if (!isLocked())
        return true;

    struct flock fl;
    fl.l_whence = SEEK_SET; //偏移量的起始位置
    fl.l_start = 0;         //加锁的起始偏移
    fl.l_len = 0;           //上锁字节，值为0时，则表示锁的区域从起点开始直至最大的可能位置
    fl.l_type = F_UNLCK;    //锁的类型 F_UNLCK为解锁  共享锁（F_RDLCK,读锁）互斥锁（F_WDLCK,写锁）

    /* fcntl系统调用可以用来对已打开的文件描述符进行各种控制操作以改变已打开文件的的各种属性
     * 文件记录锁是fcntl函数的主要功能。
     * 记录锁：实现只锁文件的某个部分，并且可以灵活的选择是阻塞方式还是立刻返回方式
     * F_SETLKW 阻塞方式上锁，当希望设置的锁因为其他锁而被阻止设置时，该命令会等待相冲突的锁被释放。
     * F_SETLK 非阻塞方式
     */
    int ret = fcntl(handle(), F_SETLKW, &fl);

    if (ret == -1) {
        qWarning("QtLockedFile::lock(): fcntl: %s", strerror(errno));
        return false;
    }

    m_lock_mode = NoLock;
    remove();
    return true;
}

QtLockedFile::~QtLockedFile()
{
    if (isOpen())
        unlock();
}

} // namespace SharedTools
