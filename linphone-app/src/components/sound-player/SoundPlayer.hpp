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

#ifndef SOUND_PLAYER_H_
#define SOUND_PLAYER_H_

#include <memory>

#include <QMutex>
#include <QObject>

// =============================================================================

class QTimer;

namespace linphone {
	class Player;
}

class SoundPlayer : public QObject {
	class Handlers;
	
	Q_OBJECT
	
	Q_PROPERTY(QString source READ getSource WRITE setSource NOTIFY sourceChanged)
	Q_PROPERTY(QString baseName READ getBaseName NOTIFY sourceChanged)
	Q_PROPERTY(PlaybackState playbackState READ getPlaybackState WRITE setPlaybackState NOTIFY playbackStateChanged)
	Q_PROPERTY(int duration READ getDuration NOTIFY sourceChanged)
	Q_PROPERTY(bool isRinger MEMBER mIsRinger)
	Q_PROPERTY(QDateTime creationDateTime READ getCreationDateTime NOTIFY sourceChanged)
	
public:
	enum PlaybackState {
		PlayingState,
		PausedState,
		StoppedState,
		ErrorState
	};
	Q_ENUM(PlaybackState);
	
	SoundPlayer (QObject *parent = Q_NULLPTR);
	~SoundPlayer ();
	
	bool open();
	Q_INVOKABLE void pause ();
	Q_INVOKABLE bool play ();
	Q_INVOKABLE void stop ();
	
	Q_INVOKABLE void seek (int offset);
	
	Q_INVOKABLE int getPosition () const;
	Q_INVOKABLE bool hasVideo() const;// Call it after playing a video because the detection is not outside this scope.
	
	int getDuration () const;
	QDateTime getCreationDateTime() const;
	QString getBaseName() const;
	std::shared_ptr<linphone::Player> getLinphonePlayer()const;
	
	QString getSource () const;
	void setSource (const QString &source);
	
signals:
	void sourceChanged (const QString &source);
	
	void paused ();
	void playing ();
	void stopped ();
	
	void playbackStateChanged (PlaybackState playbackState);
	
private:
	void buildInternalPlayer ();
	void rebuildInternalPlayer ();
	
	void stop (bool force);
	
	void handleEof ();
	
	void setError (const QString &message);
	
	PlaybackState getPlaybackState () const;
	void setPlaybackState (PlaybackState playbackState);
	
	
	
	QString mSource;
	PlaybackState mPlaybackState = StoppedState;
	bool mIsRinger = false;
	
	bool mForceClose = false;
	QMutex mForceCloseMutex;
	
	QTimer *mForceCloseTimer = nullptr;
	
	std::shared_ptr<linphone::Player> mInternalPlayer;
	std::shared_ptr<Handlers> mHandlers;
};

#endif // SOUND_PLAYER_H_
