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

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "console.h"
#include "settingsdialog.h"
#include "inputconsole.h"

#include <QMessageBox>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QToolButton>
#include <QComboBox>
#include <QLabel>
#include <QSplitter>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    globalFont = QFont( "Courier New", 11, QFont::Normal );

    serial = new QSerialPort(this);
    settings = new SettingsDialog;

    cmbPorts = new QComboBox;
    ui->mainToolBar->addWidget(cmbPorts);

    cmdBaud = new QComboBox;
    ui->mainToolBar->addWidget(cmdBaud);
    cmdBaud->addItem(QLatin1String("9600"), QSerialPort::Baud9600);
    cmdBaud->addItem(QLatin1String("19200"), QSerialPort::Baud19200);
    cmdBaud->addItem(QLatin1String("38400"), QSerialPort::Baud38400);
    cmdBaud->addItem(QLatin1String("115200"), QSerialPort::Baud115200);
    cmdBaud->addItem(QLatin1String("Custom"));

    enumeratePorts();

    console = new Console();
    console->setFont(globalFont);
    console->setFrameShadow(QFrame::Sunken);
    console->setStyleSheet(
                "QTextEdit::disabled {background-color:gray;border-width: 1px ; border-color:black;border-style: outset;}"
                "QTextEdit::enabled {background-color:black;border-width: 1px ; border-color:black;border-style: outset;}");
    QSizePolicy sp1 = QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    console->setSizePolicy(sp1);

    iconsole = new InputConsole();
    iconsole->setSizePolicy( QSizePolicy(QSizePolicy::Expanding,QSizePolicy::MinimumExpanding) );
    iconsole->setFont(globalFont);
    iconsole->setEnabled(false);
    //iconsole->setMaximumHeight(100);
    iconsole->setMinimumHeight(50);
    iconsole->setFrameShadow(QFrame::Sunken);
    iconsole->setStyleSheet(
                "QPlainTextEdit::disabled {background-color:gray;border-width: 1px ; border-color: red;border-style: outset;}"
                "QPlainTextEdit::enabled {background-color:black;border-width: 1px ; border-color: red;border-style: outset;}");

    // Hex Window Setup
    hexwin = new QTextEdit();
    hexwin->setVisible(false);
    hexwin->setMaximumWidth(85);
    hexwin->setMinimumWidth(65);
    hexwin->setReadOnly(true);
    QSizePolicy sp = QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);
    sp.setHorizontalStretch(0);
    hexwin->setSizePolicy(sp);
    //hexwin->setSizeAdjustPolicy( QTextEdit::AdjustToContents);
    hexwin->setFont(globalFont);
    hexwin->setFrameShadow(QFrame::Sunken);
    hexwin->setStyleSheet(
                "QTextEdit::disabled {background-color:gray;border-width: 1px ; border-color: red;border-style: outset;}"
                "QTextEdit::enabled {background-color:black;border-width: 1px ; border-color: black;border-style: outset;}");

    QSplitter* hexsplit = new QSplitter(Qt::Horizontal);
    hexsplit->addWidget(console);
    hexsplit->addWidget(hexwin);
    hexsplit->setChildrenCollapsible(false);
//    hexsplit->setStyleSheet(
//                "QSplitter::handle {width: 1px;height: 1px;background-color:black;border-width: 1px ; border-color:red;border-style: outset;}"
//                "QSplitter::handle:horizontal {width: 14px;}"
//                "QSplitter::handle:vertical {width: 14px;}"
//                );


    QSplitter* hzsplit = new QSplitter(Qt::Vertical);
    hzsplit->addWidget(hexsplit);
    hzsplit->addWidget(iconsole);
    hzsplit->setChildrenCollapsible(false);
//    hzsplit->setStyleSheet(
//                "QSplitter::handle {width: 1px;height: 1px;background-color:white;border-width: 1px ; border-color: red;border-style: outset;}"
//                "QSplitter::handle:horizontal {width: 14px;}"
//                "QSplitter::handle:vertical {width: 14px;}"
//                );

    ui->horizontalLayout->addWidget(hzsplit);

    ui->actionConnect->setEnabled(true);
    ui->actionDisconnect->setEnabled(false);
    ui->actionQuit->setEnabled(true);
    ui->actionConfigure->setEnabled(true);

    initActionsConnections();

    connect(serial, SIGNAL(error(QSerialPort::SerialPortError)), this,
            SLOT(handleError(QSerialPort::SerialPortError)));

    connect(serial, SIGNAL(readyRead()), this, SLOT(readData()));
    connect(iconsole, SIGNAL(getData(QByteArray)), this, SLOT(writeData(QByteArray)));
    connect(cmbPorts, SIGNAL(currentIndexChanged(QString)),this, SLOT(changePort(QString)));
    connect(settings, SIGNAL( signalClosing()),this, SLOT(updatePorts()));

}

MainWindow::~MainWindow()
{
    delete settings;
    delete ui;
}

void MainWindow::openSerialPort()
{
    SettingsDialog::Settings p = settings->settings();
    serial->setPortName(p.name);
    if (serial->open(QIODevice::ReadWrite)) {
        if (serial->setBaudRate(p.baudRate)
                && serial->setDataBits(p.dataBits)
                && serial->setParity(p.parity)
                && serial->setStopBits(p.stopBits)
                && serial->setFlowControl(p.flowControl)) {
            iconsole->setEnabled(true);
            console->setEnabled(true);
            console->setLocalEchoEnabled(p.localEchoEnabled);
            ui->actionConnect->setEnabled(false);
            ui->actionDisconnect->setEnabled(true);
            ui->actionConfigure->setEnabled(false);
            ui->statusBar->showMessage(tr("Connected to %1 : %2, %3, %4, %5, %6")
                                       .arg(p.name).arg(p.stringBaudRate).arg(p.stringDataBits)
                                       .arg(p.stringParity).arg(p.stringStopBits).arg(p.stringFlowControl));

        } else {
            serial->close();
            QMessageBox::critical(this, tr("Error"), serial->errorString());

            ui->statusBar->showMessage(tr("Open error"));
        }
    } else {
        QMessageBox::critical(this, tr("Error"), serial->errorString());

        ui->statusBar->showMessage(tr("Configure error"));
    }

}

void MainWindow::closeSerialPort()
{
    if (this->serial->isOpen())
    {
        serial->close();
    }

    //console->setEnabled(false);
    iconsole->setEnabled(false);
    ui->actionConnect->setEnabled(true);
    ui->actionDisconnect->setEnabled(false);
    ui->actionConfigure->setEnabled(true);
    ui->statusBar->showMessage(tr("Disconnected"));
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About Simple Terminal"),
                       tr("The <b>Simple Terminal</b> example demonstrates how to "
                          "use the Qt Serial Port module in modern GUI applications "
                          "using Qt, with a menu bar, toolbars, and a status bar."));
}

void MainWindow::writeData(const QByteArray &data)
{
    console->putInputData(data);
    serial->write(data);
}

void MainWindow::readData()
{
    //    QPalette p = palette();
    //    p.setColor(QPalette::Base, Qt::black);
    //    p.setColor(QPalette::Text, Qt::cyan);
    //    p.setColor(QPalette::HighlightedText, Qt::yellow);

    //    console->setPalette(p);
    //    hexwin->setPalette(p);

    QByteArray data = serial->readAll();
    console->putData(data);
    if(ui->actionEnable_Hex_Window->isChecked())
    {
        foreach (char c,data)
        {
            char hexchar[10];
            sprintf_s(hexchar,"0x%02X",c);
            QByteArray hexdata = QByteArray(hexchar);


            QPalette p = palette();
            p.setColor(QPalette::Base, Qt::black);
            p.setColor(QPalette::Text, Qt::cyan);
            p.setColor(QPalette::HighlightedText, Qt::yellow);

            hexwin->setPalette(p);

            hexwin->append(hexdata);
            //hexwin->adjustSize();
        }
    }
}

void MainWindow::changePort(QString selection)
{
    if(cmbPorts->count() == 0 && selection.count() < 2)
        return;
    settings->currentSettings.name = cmbPorts->currentData().toString();
    settings->currentSettings.name = selection;
    if(serial->isOpen())
    {
        serial->setPortName(selection);
        closeSerialPort();

    }
    openSerialPort();
}

void MainWindow::changeBaud()
{

    settings->currentSettings.baudRate = cmdBaud->currentData().toInt();
    settings->currentSettings.stringBaudRate = cmdBaud->currentData().toString();
    serial->setBaudRate(cmdBaud->currentData().toInt());
}


void MainWindow::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        QMessageBox::critical(this, tr("Critical Error"), serial->errorString());
        closeSerialPort();
    }
}

void MainWindow::displayhex()
{
    if(ui->actionEnable_Hex_Window->isChecked())
    {
        hexwin->setVisible(true);
    }
    else
    {
        hexwin->setVisible(false);
    }
}

void MainWindow::refreshPorts()
{
    settings->fillPortsInfo();
    settings->updateSettings();
    enumeratePorts();
}

void MainWindow::updatePorts()
{
    int index = cmbPorts->findText(settings->currentSettings.name);
    if( index >= 0 ) // TODO:  fix this
    {
        cmbPorts->setCurrentIndex( index );
    }

    index = cmdBaud->findData(settings->currentSettings.baudRate);
    if( index >= 0 ) // TODO:  fix this
    {
        cmdBaud->setCurrentIndex( index );
    }
    else
    {
        cmdBaud->addItem(QString(settings->currentSettings.baudRate),settings->currentSettings.baudRate);
        index = cmbPorts->findData(settings->currentSettings.name);
        cmbPorts->setCurrentIndex( index );
    }
}

void MainWindow::enumeratePorts()
{
    if (this->serial->isOpen())
    {
        closeSerialPort();
    }

    disconnect(cmbPorts, SIGNAL(currentIndexChanged(QString)),this, SLOT(changePort(QString)));
    cmbPorts->setEnabled(false);
    cmbPorts->clear();
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        serlist.append(info); //
        cmbPorts->addItem(info.portName());
    }
    cmbPorts->setDisabled(false);
    connect(cmbPorts, SIGNAL(currentIndexChanged(QString)),this, SLOT(changePort(QString)));
}

void MainWindow::initActionsConnections()
{
    connect(ui->actionConnect, SIGNAL(triggered()), this, SLOT(openSerialPort()));
    connect(ui->actionDisconnect, SIGNAL(triggered()), this, SLOT(closeSerialPort()));
    connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui->actionConfigure, SIGNAL(triggered()), settings, SLOT(show()));
    connect(ui->actionClear, SIGNAL(triggered()), console, SLOT(clear()));
    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(about()));
    connect(ui->actionAboutQt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    connect(ui->actionEnable_Hex_Window, SIGNAL(toggled(bool)), this, SLOT(displayhex()));
    connect(ui->actionRefreshPorts, SIGNAL(triggered(bool)), this, SLOT(refreshPorts()));
}
