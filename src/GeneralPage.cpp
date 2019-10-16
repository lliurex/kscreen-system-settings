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

#include "GeneralPage.h"

#include <KPluginFactory>
#include <KLocalizedString>

#include <iostream>
#include <fstream>
using namespace std;


K_PLUGIN_FACTORY(KScreenSystemKCMFactory,
                 registerPlugin<GeneralPage>();
                )


GeneralPage::GeneralPage(QWidget *parent, const QVariantList &args)
        : KCModule(0,parent, args)
{
    setButtons(Apply);
    setupUi(this);
    fillUi();
}

GeneralPage::~GeneralPage()
{
}

// 
// This function read values from N4d
//
void GeneralPage::load()
{
    systemConfigCheckBox->setChecked(true);
}

//
// This function read values from UI and call n4d config
//
void GeneralPage::save()
{

}

//
// This function write default values into n4d
//
void GeneralPage::defaults()
{

}

void GeneralPage::toggleOptions()
{
    systemConfigLayout->parentWidget()->setEnabled(systemConfigCheckBox->isChecked());
}

void GeneralPage::fillUi()
{
    connect(systemConfigCheckBox, SIGNAL(stateChanged(int)), SLOT(changed()));
    connect(systemConfigCheckBox, SIGNAL(stateChanged(int)), SLOT(toggleOptions()));
    connect(newusersRadioButton, SIGNAL(stateChanged(int)), SLOT(changed()));
    connect(allusersRadioButton, SIGNAL(stateChanged(int)), SLOT(changed()));
}

#include "GeneralPage.moc"