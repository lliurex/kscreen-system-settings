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

#include <iostream>

using namespace std;


ValidationForm::ValidationForm(QWidget *parent)
{
    this->parent = parent;
    setupUi(this);
    fillUi();
}

void ValidationForm::validateUser(){
    
    n4d::Client client("https://server",9779);
    vector<variant::Variant> params = {"1",2,false};
    
    variant::Variant result = client.validate_user(user->text().toStdString(), password->text().toStdString());

    if (bool(result[0])){
        this->done(1);
    }
    else{
        cout << "Ha fallado la autenticacion" << endl;
    }
    
}

void ValidationForm::fillUi(){
    connect(actionButtons,SIGNAL(accepted()),SLOT(validateUser()));
    connect(actionButtons,SIGNAL(rejected()),SLOT(reject()));
}