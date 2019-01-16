/* -*- c++ -*- */
/*
 * Gqrx SDR: Software defined radio receiver powered by GNU Radio and Qt
 *           http://gqrx.dk/
 *
 * Copyright 2011-2013 Alexandru Csete OZ9AEC.
 *
 * Gqrx is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * Gqrx is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Gqrx; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */
#include <QApplication>
#include <QDebug>
#include <QDesktopServices>
#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <QString>
#include <QStringList>
#include <QStyleFactory>
#include <QtGlobal>

#include "mainwindow.h"
#include "gqrx.h"

#include <iostream>

int main(int argc, char *argv[])
{
    QString         cfg_file;
    std::string     conf;
    std::string     style;
    bool            edit_conf = false;
    int             return_code;

    QApplication app(argc, argv);
    QCoreApplication::setOrganizationName(GQRX_ORG_NAME);
    QCoreApplication::setOrganizationDomain(GQRX_ORG_DOMAIN);
    QCoreApplication::setApplicationName(GQRX_APP_NAME);
    QCoreApplication::setApplicationVersion(VERSION);

    // setup controlport via environment variables
    // see http://lists.gnu.org/archive/html/discuss-gnuradio/2013-05/msg00270.html
    // Note: tried using gr::prefs().save() but that doesn't have effect until the next time
    if (qputenv("GR_CONF_CONTROLPORT_ON", "False"))
        qDebug() << "Controlport disabled";
    else
        qDebug() << "Failed to disable controlport";


    if (!conf.empty())
    {
        cfg_file = QString::fromStdString(conf);
        qDebug() << "User specified config file:" << cfg_file;
    }
    else
    {
        cfg_file = "/default.conf";
        qDebug() << "No user supplied config file. Using" << cfg_file;
    }


    // Mainwindow will check whether we have a configuration
    // and open the config dialog if there is none or the specified
    // file does not exist.
    MainWindow w(cfg_file, edit_conf);

    if (w.configOk)
    {
        w.show();
        return_code = app.exec();
    }
    else
    {
        return_code = 1;
    }

    return  return_code;
}
