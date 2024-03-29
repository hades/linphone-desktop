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

#ifndef SIP_ADDRESS_OBSERVER_H_
#define SIP_ADDRESS_OBSERVER_H_

#include "components/presence/Presence.hpp"

#include <QSharedPointer>
#include <QDateTime>

// =============================================================================

class ContactModel;

class SipAddressObserver : public QObject {
	friend class SipAddressesModel;

	Q_OBJECT;

	Q_PROPERTY(QString peerAddress READ getPeerAddress CONSTANT);
	Q_PROPERTY(QString localAddress READ getLocalAddress CONSTANT);

	Q_PROPERTY(ContactModel *contact READ getContact NOTIFY contactChanged);
	Q_PROPERTY(Presence::PresenceStatus presenceStatus READ getPresenceStatus NOTIFY presenceStatusChanged);
	Q_PROPERTY(QDateTime presenceTimestamp READ getPresenceTimestamp NOTIFY presenceTimestampChanged);

	Q_PROPERTY(bool isOneToOne MEMBER isOneToOne CONSTANT);
	Q_PROPERTY(bool haveEncryption MEMBER haveEncryption CONSTANT);
	Q_PROPERTY(int securityLevel MEMBER securityLevel CONSTANT);
public:
	SipAddressObserver (const QString &peerAddress, const QString &localAddress);

	bool isOneToOne = true;
	bool haveEncryption = false;
	int securityLevel = 1;

signals:
	void contactChanged (QSharedPointer<ContactModel>);
	void presenceStatusChanged (const Presence::PresenceStatus &presenceStatus);
	void presenceTimestampChanged (const QDateTime &presenceTimestamp);

private:
	QString getPeerAddress () const {
		return mPeerAddress;
	}

	QString getLocalAddress () const {
		return mLocalAddress;
	}

	// ---------------------------------------------------------------------------

	ContactModel *getContact () const {
		return mContact.get();
	}

	void setContact (QSharedPointer<ContactModel> contact);

	// ---------------------------------------------------------------------------

	Presence::PresenceStatus getPresenceStatus () const {
		return mPresenceStatus;
	}
	QDateTime getPresenceTimestamp() const{
		return mPresenceTimestamp;
	}

	void setPresenceStatus (const Presence::PresenceStatus &presenceStatus);
	void setPresenceTimestamp (const QDateTime &presenceTimestamp);

	// ---------------------------------------------------------------------------
	QString mPeerAddress;
	QString mLocalAddress;

	QSharedPointer<ContactModel> mContact;
	Presence::PresenceStatus mPresenceStatus = Presence::PresenceStatus::Offline;
	QDateTime mPresenceTimestamp;
};

Q_DECLARE_METATYPE(SipAddressObserver *);

#endif // SIP_ADDRESS_OBSERVER_H_
