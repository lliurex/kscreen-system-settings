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
//#include <n4d.hpp>
#include <variant.hpp>

// From KDE

#include <KMessageWidget>

#include <QIcon>
#include <iostream>
#include <QGraphicsOpacityEffect>
#include <QThread>

using namespace std;
using namespace edupals;

ValidationForm::ValidationForm(QWidget *parent)
{
    this->parent = parent;
    setupUi(this);
    fillUi();
    notificationwidget = new KMessageWidget(this);
    notification->layout()->addWidget(notificationwidget);
    QGraphicsOpacityEffect * effect = new QGraphicsOpacityEffect(notificationwidget);
    effect->setOpacity(0);
    notificationwidget->setGraphicsEffect(effect);
    user->setText(QString(getenv("USER")));
    statusicon->setPixmap(QIcon::fromTheme("dialog-password").pixmap(65,65));
}

void ValidationForm::validateUser()
{
    ((QGraphicsOpacityEffect*)notificationwidget->graphicsEffect())->setOpacity(0);
    setEnableWidgets(false);
    an4d=new AsyncN4D(user->text().toStdString(), password->text().toStdString()) ;
    QObject::connect(an4d,SIGNAL(message(bool)), this, SLOT(n4dDone(bool)));
    an4d->start();
}

void ValidationForm::n4dDone(bool result)
{
     if (result){
        this->done(1);
    }
    else{
        notificationwidget->setText("Error de validacion");
        notificationwidget->setMessageType(KMessageWidget::MessageType::Error);
        notificationwidget->setCloseButtonVisible(false);
        ((QGraphicsOpacityEffect*)notificationwidget->graphicsEffect())->setOpacity(1);
    }
    an4d->exit(0);
    if (an4d->wait())
    {
        delete an4d;
    }
    setEnableWidgets(true);

}

string ValidationForm::getUser(){
    return user->text().toStdString();
}

string ValidationForm::getPassword(){
    return password->text().toStdString();
}

void ValidationForm::setEnableWidgets(bool state)
{
    user->setEnabled(state);
    password->setEnabled(state);
    actionButtons->setEnabled(state);
}

void ValidationForm::fillUi(){
    connect(actionButtons, SIGNAL(accepted()), SLOT(validateUser()));
    connect(actionButtons, SIGNAL(rejected()), SLOT(reject()));
}
