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


#include "ValidationForm.h"

// From Edupals
#include <n4d.hpp>
#include <variant.hpp>

// From KDE

#include <KMessageWidget>

#include <iostream>

using namespace std;
using namespace edupals;


ValidationForm::ValidationForm(QWidget *parent)
{
    this->parent = parent;
    setupUi(this);
    fillUi();
    notificationwidget = new KMessageWidget(this);
    notification->layout()->addWidget(notificationwidget);

}

void ValidationForm::validateUser(){
    
    n4d::Client client("https://localhost",9779);

    bool result = client.validate_user(user->text().toStdString(), password->text().toStdString());
    if (bool(result)){
        this->done(1);
    }
    else{
        notificationwidget->setText("Error de validacion");
        notificationwidget->setMessageType(KMessageWidget::MessageType::Error);
    }
    
}

void ValidationForm::fillUi(){
    connect(actionButtons,SIGNAL(accepted()),SLOT(validateUser()));
    connect(actionButtons,SIGNAL(rejected()),SLOT(reject()));
}