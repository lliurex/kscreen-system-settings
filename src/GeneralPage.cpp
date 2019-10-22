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
#include "ValidationForm.h"

//from Edupals http://github.com/edupals
#include <n4d.hpp>
#include <variant.hpp>

// From KDE
#include <KPluginFactory>
#include <KLocalizedString>

#include <iostream>
#include <fstream>

using namespace edupals;
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
    client = new Client("https://localhost",9779);
}

GeneralPage::~GeneralPage()
{
    delete client;
}

// 
// This function read values from N4d
//
void GeneralPage::load()
{
    systemConfigCheckBox->setChecked(true);
    //client->call();
}

//
// This function read values from UI and call n4d config
//
void GeneralPage::save()
{
        ValidationForm dialog(this);
        dialog.exec();
        if (dialog.result() == QDialog::DialogCode::Accepted ) {
                //client->call();
                cout << dialog.getUser() << endl;
                cout << dialog.getPassword() << endl;
        }
        else{
            
        }
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
    connect(newusersRadioButton, SIGNAL(toggled(bool)), SLOT(changed()));
    connect(allusersRadioButton, SIGNAL(toggled(bool)), SLOT(changed()));
}

#include "GeneralPage.moc"
