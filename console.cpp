
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


/** This handles the console ****/
#include "console.h"

#include <QScrollBar>
#include <QtCore/QDebug>
#include <QMenu>

Console::Console(QWidget *parent)
    : QTextEdit(parent)
{

    document()->setMaximumBlockCount(100);
    QPalette p = (this->palette());
//    p.setColor(QPalette::Base, Qt::black);
//    //p.setColor(QPalette::Text, Qt::cyan);
//    p.setColor(QPalette::HighlightedText, Qt::yellow);
//    p.setBrush(QPalette::Base,Qt::black);
//    p.setColor(QPalette::Window, Qt::green); // background

    //    pal.setColor(QPalette::Base, Qt::black);
        p.setColor(backgroundRole(), Qt::black);
    //    pal.setColor(QPalette::Text, Qt::green);
    //    pal.setColor(QPalette::Active,QPalette::Background, (Qt::black));
    //    pal.setColor(QPalette::Disabled,QPalette::Background, (Qt::gray));
    //p.setBrush(QPalette::Normal ,QPalette::Background, Qt::black);
    setPalette(p);
    setAutoFillBackground(true);

    setReadOnly(false);
    setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);

    QSizePolicy sizePolicy;
    sizePolicy.setHorizontalPolicy(QSizePolicy::Expanding);
    sizePolicy.setVerticalPolicy(QSizePolicy::Expanding);
    setSizePolicy(sizePolicy);

}

void Console::putData(const QByteArray &data)
{

    //    moveCursor (QTextCursor::End);
    //    insertPlainText (data);
    //    moveCursor (QTextCursor::End);

    // QTextCursor prev_cursor = textCursor();
    moveCursor (QTextCursor::End);
    QTextCursor cursor( textCursor() );

    QTextCharFormat format;
    format.setFontWeight( QFont::DemiBold );
    format.setForeground( QBrush( QColor("cyan")) );
    cursor.setCharFormat( format );

    moveCursor (QTextCursor::End);
    cursor.insertText( data );
    moveCursor (QTextCursor::End);

    QScrollBar *bar = verticalScrollBar();
    bar->setValue(bar->maximum());
}

void Console::putInputData(const QByteArray &data)
{

    moveCursor (QTextCursor::End);
    QTextCursor cursor( textCursor() );

    QTextCharFormat format;
    format.setFontWeight( QFont::DemiBold );
    format.setForeground( QBrush( QColor( "red" ) ) );
    cursor.setCharFormat( format );

    moveCursor (QTextCursor::End);
    cursor.insertText( data );
    moveCursor (QTextCursor::End);

    QScrollBar *bar = verticalScrollBar();
    bar->setValue(bar->maximum());
}




void Console::setLocalEchoEnabled(bool set)
{
    localEchoEnabled = set;
}




