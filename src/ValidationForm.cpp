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

using namespace std;


ValidationForm::ValidationForm(QWidget *parent)
{
    setupUi(this);
    fillUi();
}

void ValidationForm::validateUser(){
    user = "Raul";
    password = "Rodrigo";
    cout << "Hola" << endl;
}

void ValidationForm::fillUi(){
    connect(actionButtons,SIGNAL(accepted()),SLOT(validateuser()));
}