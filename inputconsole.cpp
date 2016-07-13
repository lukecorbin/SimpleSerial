
/****************************************************************************
**
** Copyright (C) 2012 Denis Shienkov <denis.shienkov@gmail.com>
** Copyright (C) 2012 Laszlo Papp <lpapp@kde.org>
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtSerialPort module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "inputconsole.h"

#include <QScrollBar>
#include <QtCore/QDebug>
#include <QMenu>


InputConsole::InputConsole(QWidget *parent)
    : QPlainTextEdit(parent)
{

    document()->setMaximumBlockCount(100);
    QPalette p = palette();
    p.setColor(QPalette::Base, Qt::black);
    p.setColor(QPalette::Text, Qt::red);
    p.setColor(QPalette::HighlightedText, Qt::yellow);
//    p.setColor(QPalette::Inactive, QPalette::Highlight, p.color(QPalette::Active, QPalette::Highlight));
    p.setColor(QPalette::Active, QPalette::Background,Qt::black);
    p.setColor(QPalette::Normal, QPalette::Background,Qt::black);
    setPalette(p);

    setReadOnly(false);

    QFont f;
    f.setBold(true);
    setFont(f);

    QSizePolicy sizePolicy;
    sizePolicy.setHorizontalPolicy(QSizePolicy::Expanding);
    sizePolicy.setVerticalPolicy(QSizePolicy::Expanding);
    setSizePolicy(sizePolicy);

    moveCursor (QTextCursor::End);

    setFixedHeight(50);
    insertPlainText("");
    moveCursor (QTextCursor::End);

}

//void InputConsole::putData(const QByteArray &data)
//{

//    moveCursor (QTextCursor::End);
//    insertPlainText (data);
//    moveCursor (QTextCursor::End);

//    QScrollBar *bar = verticalScrollBar();
//    bar->setValue(bar->maximum());


//}


void InputConsole::keyPressEvent(QKeyEvent *e)
{
    if (e->text() == "\r" ) {

        emit getData( toPlainText().toLocal8Bit()  );
        setPlainText("");

    }
    else
    {
        QPlainTextEdit::keyPressEvent(e);
    }
}



