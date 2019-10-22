/*
    KScreen System
    Copyright (C) 2019  Raul Rodrigo Segura <raurodse@gmail.com>
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef VALIDATIONFORM
#define VALIDATIONFORM

#include "ui_validation.h"

// From KDE
#include <KMessageWidget>

#include <QtWidgets/QDialog>
#include <QtWidgets/QWidget>
#include <QThread>
#include <n4d.hpp>
#include <iostream>

class AsyncN4D: public QThread
{
    Q_OBJECT

    public:
        std::string user;
        std::string password;


        AsyncN4D(std::string user,std::string password)
        {
            this->user=user;
            this->password=password;
        }
        void run()
        {
            edupals::n4d::Client client("https://localhost",9779);
            bool result = client.validate_user(user, password);
            emit message(result);
        }

    signals:
        void message(bool);
};


class ValidationForm : public QDialog, public Ui_validationForm
{
    Q_OBJECT

public:
    ValidationForm(QWidget *parent);
    virtual ~ValidationForm(){};
    void fillUi();
private:
    QWidget *parent;
    KMessageWidget *notificationwidget;
    AsyncN4D* an4d;
    void setEnableWidgets(bool status);
private slots:
    void validateUser();
    void n4dDone(bool status);
};

#endif
           
