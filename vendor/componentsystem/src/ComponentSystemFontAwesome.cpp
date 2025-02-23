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


#include "ComponentSystemFontAwesome.h"

#include <QFile>
#include <QFontDatabase>
#include <QPainter>
#include <QRegularExpression>

constexpr auto BaseHex = 16;

Nedrysoft::ComponentSystem::FontAwesome::FontAwesome() :
        m_regularId(QFontDatabase::addApplicationFont(":/Nedrysoft/ComponentSystem/FontAwesome/Free-Regular.otf")),
        m_solidId(QFontDatabase::addApplicationFont(":/Nedrysoft/ComponentSystem/FontAwesome/Free-Solid.otf")),
        m_brandsId(QFontDatabase::addApplicationFont(":/Nedrysoft/ComponentSystem/FontAwesome/Free-Brands.otf")) {

    if (QFontDatabase::applicationFontFamilies(m_regularId).count()) {
        m_regularName = QFontDatabase::applicationFontFamilies(m_regularId).at(0);
    }

    if (QFontDatabase::applicationFontFamilies(m_solidId).count()) {
        m_solidName = QFontDatabase::applicationFontFamilies(m_solidId).at(0);
    }

    if (QFontDatabase::applicationFontFamilies(m_brandsId).count()) {
        m_brandsName = QFontDatabase::applicationFontFamilies(m_brandsId).at(0);
    }

    QFile scssFile(":/Nedrysoft/ComponentSystem/FontAwesome/_variables.scss");

    if (scssFile.open(QFile::ReadOnly)) {
        auto scssContent = QString::fromUtf8(scssFile.readAll());

        if (!scssContent.isEmpty()) {
            auto scssExpression = QRegularExpression(R"(\$fa-var-([0-9|a-z|\-]*)\:\W\\([0-9|a-f]{1,4});)");

            auto scssMatchIterator = scssExpression.globalMatch(scssContent);

            while (scssMatchIterator.hasNext()) {
                auto scssMatch = scssMatchIterator.next();

                auto capturedTexts = scssMatch.capturedTexts();

                if (capturedTexts.count() == 3) {
                    m_glyphMap["fa-" + capturedTexts.at(1)] = capturedTexts.at(2);
                }
            }
        }
    }

    m_styleString = QString(R"(
        <style>
            .far {
                font-family:'%1';
                font-weight:900
            }

            .fas {
                font-family:'%2';
                font-weight:400
            }

            .fab {
                font-family:'%3';
                font-weight:400
            }

            .body {
                position: absolute;
                top: 50%;
            }
        </style>
    )").arg(m_regularName, m_solidName, m_brandsName);
}

auto Nedrysoft::ComponentSystem::FontAwesome::regularName() -> QString {
    return m_regularName;
}

auto Nedrysoft::ComponentSystem::FontAwesome::solidName() -> QString {
    return m_solidName;
}

auto Nedrysoft::ComponentSystem::FontAwesome::brandsName() -> QString {
    return m_brandsName;
}

auto Nedrysoft::ComponentSystem::FontAwesome::richText(QString string) -> QString {
    auto expression = QRegularExpression(R"(\[(far|fas|fab) ([a-z|\-|0-9]*)\])");
    auto match = QRegularExpressionMatch();
    auto searchIndex = 0;

    while (string.indexOf(expression, searchIndex, &match) >= 0) {
        if (!match.hasMatch()) {
            break;
        }

        if (match.capturedTexts().count() == 3) {
            auto richTextString = QString();
            auto iconFont = match.capturedTexts().at(1);
            auto iconId = match.capturedTexts().at(2);
            auto iconCode = QString();

            if (m_glyphMap.contains(iconId)) {
                iconCode = m_glyphMap[iconId];
            } else {
                if (( iconId.size() >= 1 ) && (( iconId.size() <= 4 ))) {
                    bool ok = false;

                    iconId.toInt(&ok, BaseHex);

                    if (ok) {
                        iconCode = iconId;
                    }
                }
            }

            if (iconCode.isNull()) {
                richTextString = QString("");
            } else {
                richTextString = QString(R"(<span class="%1">&#x%2;</span>)").arg(iconFont, iconCode);
            }

            string.replace(match.capturedTexts().at(0), richTextString);
        } else {
            searchIndex += match.captured().length();
        }
    }

    return QString("<html>%1<body>%2</body></html>").arg(m_styleString, string);
}

auto Nedrysoft::ComponentSystem::FontAwesome::icon(QString glyphName, int pointSize, QColor colour) -> QIcon {
    QPixmap pixmap(pointSize, pointSize);

    pixmap.fill(Qt::transparent);

    auto expression = QRegularExpression(R"((far|fas|fab) ([a-z|\-|0-9]*))");
    auto match = QRegularExpressionMatch();
    auto searchIndex = 0;

    while (glyphName.indexOf(expression, searchIndex, &match) >= 0) {
        if (!match.hasMatch()) {
            break;
        }

        if (match.capturedTexts().count() == 3) {
            auto iconFont = match.capturedTexts().at(1);
            auto iconId = match.capturedTexts().at(2);
            auto iconCode = 0;
            QFont::Weight fontWeight;
            QString fontName;

            if (m_glyphMap.contains(iconId)) {
                iconCode = m_glyphMap[iconId].toInt(nullptr, 16);
            } else {
                if (( iconId.size() >= 1 ) && (( iconId.size() <= 4 ))) {
                    iconCode = iconId.toInt(nullptr, BaseHex);
                }
            }

            if (iconFont == "fab") {
                fontName = brandsName();
                fontWeight = QFont::Normal;
            } else if (iconFont == "fas") {
                fontName = solidName();
                fontWeight = QFont::Bold;
            } else {
                fontName = regularName();
                fontWeight = QFont::Normal;
            }

            auto *painter = new QPainter(&pixmap);

            painter->setPen(colour);
            painter->setFont(QFont(fontName, pointSize, fontWeight));
            painter->drawText(QRect(0, 0, pointSize, pointSize), Qt::AlignHCenter | Qt::AlignVCenter, QChar(iconCode));
            painter->end();

            delete painter;

            break;
        }
    }

    return QIcon(pixmap);
}
