/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of the Nedrysoft component system (https://github.com/nedrysoft/componentsystem)
 *
 * A cross-platform plugin system for Qt applications.
 *
 * Created by Adrian Carpenter on 03/12/2020.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "ComponentViewerDialog.h"

#include "Component.h"
#include "ComponentDetailsDialog.h"
#include "IComponentManager.h"
#include "ComponentSystemFontAwesome.h"
#include "ui_ComponentViewerDialog.h"

#include <QTreeWidgetItem>

Nedrysoft::ComponentSystem::ComponentViewerDialog::ComponentViewerDialog(QWidget *parent) :
        QDialog(parent),
        ui(new Nedrysoft::ComponentSystem::Ui::ComponentViewerDialog) {

    ui->setupUi(this);

    m_fontAwesome = new Nedrysoft::ComponentSystem::FontAwesome;

    auto minusIcon = m_fontAwesome->icon("fas fa-minus", 16, Qt::darkRed);
    auto crossIcon = m_fontAwesome->icon("fas fa-times", 16, Qt::darkRed);
    auto tickIcon = m_fontAwesome->icon("fas fa-check", 16, Qt::darkGreen);

    ui->componentsTreeWidget->setHeaderLabels(
            QStringList() << tr("Name") << tr("Load") << tr("Version") << tr("Vendor"));

    ui->componentsTreeWidget->setColumnWidth(0, 300);
    ui->componentsTreeWidget->setColumnWidth(1, 50);
    ui->componentsTreeWidget->setColumnWidth(2, 300);
    ui->componentsTreeWidget->setColumnWidth(3, 200);

    QMap<QString, QMap<QString, Nedrysoft::ComponentSystem::Component *> > categoryMap;

    auto componentLoader = Nedrysoft::ComponentSystem::getObject<Nedrysoft::ComponentSystem::ComponentLoader>();

    auto components = componentLoader->components();

    for (auto component : components) {
        categoryMap[component->category()][component->name()] = component;
    }

    QMapIterator<QString, QMap<QString, Nedrysoft::ComponentSystem::Component *> > categoryIterator(categoryMap);

    while (categoryIterator.hasNext()) {
        categoryIterator.next();

        auto categoryItem = new QTreeWidgetItem;

        categoryItem->setText(0, categoryIterator.key());

        QMapIterator<QString, Nedrysoft::ComponentSystem::Component *> componentIterator(categoryIterator.value());

        while (componentIterator.hasNext()) {
            componentIterator.next();

            auto component = componentIterator.value();

            auto componentItem = new QTreeWidgetItem;

            if (component->loadStatus() == Nedrysoft::ComponentSystem::ComponentLoader::Loaded) {
                componentItem->setIcon(0, tickIcon);
            } else {
                if (component->loadStatus() == Nedrysoft::ComponentSystem::ComponentLoader::Disabled) {
                    componentItem->setIcon(0, minusIcon);
                } else {
                    componentItem->setIcon(0, crossIcon);
                }
            }

            componentItem->setText(0, component->name());

            if (!component->canBeDisabled()) {
                componentItem->setData(1, Qt::CheckStateRole, Qt::Checked);
                componentItem->setDisabled(true);
            } else {
                if (component->loadStatus() == Nedrysoft::ComponentSystem::ComponentLoader::Disabled) {
                    componentItem->setCheckState(1, Qt::Unchecked);
                } else {
                    componentItem->setCheckState(1, Qt::Checked);
                }
            }

            componentItem->setText(2, component->versionString());
            componentItem->setText(3, component->vendor());

            componentItem->setData(
                    0,
                    Qt::UserRole,
                    QVariant::fromValue<Nedrysoft::ComponentSystem::Component *>(component) );

            categoryItem->addChild(componentItem);
        }

        ui->componentsTreeWidget->addTopLevelItem(categoryItem);
    }

    ui->componentsTreeWidget->expandAll();
}

Nedrysoft::ComponentSystem::ComponentViewerDialog::~ComponentViewerDialog() {
    delete ui;

    delete m_fontAwesome;
}

void Nedrysoft::ComponentSystem::ComponentViewerDialog::on_componentsTreeWidget_itemDoubleClicked(
        QTreeWidgetItem *item,
        int column) {

    Q_UNUSED(column)

    auto component = item->data(0, Qt::UserRole).value<Nedrysoft::ComponentSystem::Component *>();

    if (component) {
        ComponentDetailsDialog detailsDialog(component);

        detailsDialog.exec();
    }
}

auto Nedrysoft::ComponentSystem::ComponentViewerDialog::disabledComponents() -> QStringList {
    QStringList disabledComponentList;

    for (auto categoryIndex = 0; categoryIndex < ui->componentsTreeWidget->topLevelItemCount(); categoryIndex++) {
        auto categoryItem = ui->componentsTreeWidget->topLevelItem(categoryIndex);

        for (auto itemIndex = 0; itemIndex < categoryItem->childCount(); itemIndex++) {
            auto componentItem = categoryItem->child(itemIndex);

            if (componentItem->checkState(1) == Qt::Unchecked) {
                Nedrysoft::ComponentSystem::Component *component = componentItem->data(0,
                        Qt::UserRole).value<Nedrysoft::ComponentSystem::Component *>();

                if (component) {
                    disabledComponentList.append(( component->name() + "." + component->vendor()).toLower());
                }
            }
        }
    }

    return disabledComponentList;
}
