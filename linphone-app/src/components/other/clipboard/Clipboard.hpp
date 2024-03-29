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

#ifndef CLIPBOARD_H_
#define CLIPBOARD_H_

#include <QObject>

// =============================================================================

class Clipboard : public QObject {
  Q_OBJECT;

  Q_PROPERTY(QString text READ getText WRITE setText NOTIFY textChanged);
  
public:
  Clipboard (QObject *parent = Q_NULLPTR);
  Q_INVOKABLE void backup();
  Q_INVOKABLE void restore();
  Q_INVOKABLE QString getChatFormattedText() const;

signals:
  void textChanged ();

private:

  QString getText () const;
  void setText (const QString &text);
  QMimeData *mMimeCopy;
};

#endif // ifndef CLIPBOARD_H_
