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
#include <json.hpp>
#include <filesystem.hpp>

// From KDE
#include <KPluginFactory>
#include <KLocalizedString>
#include <KMessageWidget>

#include <QIcon>
#include <QMetaObject>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <filesystem.hpp>

using namespace edupals;
using namespace std;
namespace fs=std::experimental::filesystem;


K_PLUGIN_FACTORY(KScreenSystemKCMFactory,
                 registerPlugin<GeneralPage>();
                )


GeneralPage::GeneralPage(QWidget *parent, const QVariantList &args)
        : KCModule(0,parent, args)
{
    setButtons(Apply);
    setupUi(this);
    fillUi();
   /* client = new n4d::Client("https://localhost",9779);*/
   client = new n4d::Client("https://localhost:9779");

	
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
    bool status = client->running();

    if ( status)
    {
	   /*
	    vector<variant::Variant> args = {"SRV_IP"};
	    variant::Variant remote = client->call("VariablesManager","get_variable",args);
	    */
	    try{
            	variant::Variant remote = client->get_variable("SRV_IP",false);
    		address="https://"+ remote.get_string()+":9779";
	    }
	    catch (...){
	    	address="https://localhost:9779";
	    }
	    
    	    client = new n4d::Client(address);
	    vector<variant::Variant> arguments = {"config"}; 
	    variant::Variant result = client->call("MonitorSettings","getSettings",arguments);
	    try{
			
		if (result["mode"].get_string() == "allusers"){  
		    systemConfigCheckBox->setChecked(true);
		    allusersRadioButton->setChecked(true);
		}
		else if(result["mode"].get_string() == "newusers")
		{
			systemConfigCheckBox->setChecked(true);
			newusersRadioButton->setChecked(true);
		}
		else{
			systemConfigCheckBox->setChecked(false);
			newusersRadioButton->setChecked(true);
			toggleOptions();
		}
	    }
	    catch(...){
	     	mainwidget->setEnabled(false);
		KMessageWidget *notificationwidget = new KMessageWidget(this);
		notificationwidget->setText("An unexpected error has ocurred.");
		notificationwidget->setMessageType(KMessageWidget::MessageType::Error);
		notifications->layout()->addWidget(notificationwidget);	

	}
    }
    else{
	      
	    mainwidget->setEnabled(false);
	    KMessageWidget *notificationwidget = new KMessageWidget(this);
            notificationwidget->setText("N4D service is down.");
            notificationwidget->setMessageType(KMessageWidget::MessageType::Error);
            notifications->layout()->addWidget(notificationwidget);	
	
}
    user = "";
    password = "";
}

//
// This function read values from UI and call n4d config
//
void GeneralPage::save()
{

    if (user == "" )
    {
    	ValidationForm dialog(this);
    	dialog.exec();
    
    	if (dialog.result() == QDialog::DialogCode::Accepted)
    	{
	    user = dialog.getUser();
	    password = dialog.getPassword();
    	}
    }
    if (user != "" )
    {
        /*n4d::auth::Credential credential(user,password);*/
	client = new n4d::Client(address,user,password);    
	vector<variant::Variant> mode = {getMode()};
        client->call("MonitorSettings","saveMode", mode);
	string settings_path = string(getenv("HOME")) + "/.local/share/kscreen";
	string outputs_path = string(getenv("HOME"))+"/.local/share/kscreen/outputs";
	string control_path = string(getenv("HOME"))+"/.local/share/kscreen/control/configs";
        variant::Variant result;
	bool ok = true;

	if (!process_dir(settings_path, result, client, "config")) ok = false;
	if (!process_dir(outputs_path, result, client, "outputs")) ok = false;
	if (!process_dir(control_path, result, client, "control")) ok = false;
	/*
        bool ok = false;
        try{
            ok = result["status"].get_boolean();
        }
        catch (...){
            ok = false;
        }
	*/
        if(ok)
        {
            system("kscreensystemsettings_updateresolution&");
            fstream fs(  string(getenv("HOME")) + "/.config/kscreensystem" , fstream::out);
            fs << result.get_string() << endl;
            fs.close();
        }

    }
    else{
	    QMetaObject::invokeMethod(this, "changed", Qt::QueuedConnection, Q_ARG(bool, true));
    }
}

string GeneralPage::getMode(){
    if (!systemConfigCheckBox->isChecked())
    {
        return "nobody";
    }
    else
    {
        if(newusersRadioButton->isChecked()){
            return "newusers";
        }
        else{
            return "allusers";
        }
    }
}


bool GeneralPage::process_dir( string path, variant::Variant &result, n4d::Client *client, string filetype ){

	bool ok = true;
        auto files = filesystem::glob(path + "/*");
	for (auto file : files) 
        {
	    if (fs::is_directory(fs::path(file))) continue;
            fstream fb;
	    fb.open(file.string(),ios::in);
            if(fb.is_open()){
                variant::Variant configuration = json::load(fb);
                vector<variant::Variant> arguments = {configuration,variant::Variant(file.filename()), filetype};
                try{
			result = client->call("MonitorSettings","saveResolution",arguments);
		}catch(...){
			ok=false;
		}
            }   
        }
	return ok;
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
