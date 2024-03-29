/*
 * Copyright (c) 2010-2020 Belledonne Communications SARL.
 *
 * This file is part of linphone-desktop
 * (see https://www.linphone.org).
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
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <QClipboard>
#include <QGuiApplication>
#include <QMimeData>

#include "Clipboard.hpp"

// =============================================================================

Clipboard::Clipboard (QObject *parent) : QObject(parent) {
  QObject::connect(QGuiApplication::clipboard(), &QClipboard::dataChanged, this, &Clipboard::textChanged);
}

QString Clipboard::getText () const {
  return QGuiApplication::clipboard()->text(QClipboard::Clipboard);
}

void Clipboard::setText (const QString &text) {
  QGuiApplication::clipboard()->setText(text, QClipboard::Clipboard);
}

void Clipboard::backup () {
	if (QGuiApplication::clipboard() != nullptr) {
		const QMimeData * clipboardData = QGuiApplication::clipboard()->mimeData();
		mMimeCopy = new QMimeData();
		foreach(const QString & format, clipboardData->formats())
		mMimeCopy->setData(format, clipboardData->data(format));
	}
}

void Clipboard::restore () {
	if (QGuiApplication::clipboard() != nullptr && mMimeCopy != nullptr)
		QGuiApplication::clipboard()->setMimeData(mMimeCopy);
}

QString Clipboard::getChatFormattedText () const {
	QString text = getText();
	if (text.isEmpty())
		return text;
#ifdef __linux__
	QString cr = "\n";
#endif
#ifdef WIN32
	QString cr = "\n\r";
#endif
#ifdef __APPLE__
	QString cr = "\n";
#endif
	return text.replace(cr,"\u2028");
}


