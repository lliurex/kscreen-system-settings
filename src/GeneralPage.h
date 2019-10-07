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

#ifndef GENERALPAGE_H
#define GENERALPAGE_H

#include <KCModule>

#include "ui_generalPage.h"

class ErrorOverlay;
class GeneralPage : public KCModule, private Ui_generalPage
{
    Q_OBJECT

public:
    GeneralPage(QWidget *parent, const QVariantList &args);
    virtual ~GeneralPage();
    void fillUi();
    void load() Q_DECL_OVERRIDE;
    void save() Q_DECL_OVERRIDE;
    void defaults() Q_DECL_OVERRIDE;
};

#endif /* GENERALPAGE_H */


