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

#ifndef APP_H_
#define APP_H_

#include <memory>

#include "single-application/singleapplication.h"

#include <QApplication>

// =============================================================================

class QCommandLineParser;
class QQmlApplicationEngine;
class QQuickWindow;
class QSystemTrayIcon;

namespace linphone {
  class Config;
}

class ColorListModel;
class DefaultTranslator;
class ImageListModel;
class Notifier;


class App : public SingleApplication {
  Q_OBJECT

  Q_PROPERTY(QString configLocale READ getConfigLocale WRITE setConfigLocale NOTIFY configLocaleChanged)
  Q_PROPERTY(QLocale locale READ getLocale CONSTANT)
  Q_PROPERTY(QVariantList availableLocales READ getAvailableLocales CONSTANT)
  Q_PROPERTY(QString qtVersion READ getQtVersion CONSTANT)

  Q_PROPERTY(bool autoStart READ getAutoStart WRITE setAutoStart NOTIFY autoStartChanged)  

public:
  App (int &argc, char *argv[]);
  ~App ();

  void stop();
  void initContentApp ();
  QStringList cleanParserKeys(QCommandLineParser * parser, QStringList keys);// Get all options from parser and remove the selected keys. Return the result that can be passed to parser process.
  void processArguments(QHash<QString,QString> args);

  QString getCommandArgument ();

  QString getFetchConfig (QString filePath, bool * error);
  QString getFetchConfig (QCommandLineParser *parser);// Return file path of fetch-config
  void useFetchConfig(const QString& filePath);	// Check if the fetch is auto or not and make gui request if needed.
  Q_INVOKABLE bool setFetchConfig (QString filePath);	// return true if filepath has been set.

  #ifdef Q_OS_MACOS
    bool event (QEvent *event) override;
  #endif // ifdef Q_OS_MACOS

  QQmlApplicationEngine *getEngine () {
    return mEngine;
  }

  Notifier *getNotifier () const {
    return mNotifier;
  }

  ColorListModel *getColorListModel () const {
	return mColorListModel;
	}
	ImageListModel *getImageListModel () const {
	return mImageListModel;
	}
	
  QLocale getLocale () const;
	
   //static ColorListModel *getColorListModel () const {
    //return App::getInstance()-getColorListModel();
  //}

  QSystemTrayIcon *getSystemTrayIcon () const {
    return mSystemTrayIcon;
  }

  QQuickWindow *getMainWindow () const;

  bool hasFocus () const;

  bool isOpened () const {
    return mIsOpened;
  }

  static App *getInstance () {
    return static_cast<App *>(QApplication::instance());
  }

  static constexpr int RestartCode = 1000;
  static constexpr int DeleteDataCode = 1001;

  Q_INVOKABLE void restart () {
    exit(RestartCode);
  }

  Q_INVOKABLE QQuickWindow *getCallsWindow () const;
  Q_INVOKABLE QQuickWindow *getSettingsWindow () const;

  Q_INVOKABLE static void smartShowWindow (QQuickWindow *window);
  bool mCheckForUpdateUserInitiated;
  Q_INVOKABLE static void checkForUpdates(bool force = false);

// Check module availability when no dependencies are needed (else use SettingsModel)
  Q_INVOKABLE static bool isPdfAvailable();
  Q_INVOKABLE static bool isLinux();
  bool autoStartEnabled();
#ifdef Q_OS_LINUX
  Q_INVOKABLE void exportDesktopFile();
  
  QString getApplicationPath() const;
  bool generateDesktopFile(const QString& confPath, bool remove, bool openInBackground);
#endif
  
public slots:
  void stateChanged(Qt::ApplicationState);

signals:
  void configLocaleChanged (const QString &locale);

  void autoStartChanged (bool enabled);

  void opened (bool status);
  void requestFetchConfig(QString filePath);
  
private:
  void createParser ();

  void registerTypes ();
  void registerSharedTypes ();
  void registerToolTypes ();
  void registerSharedToolTypes ();
  void registerUninstalledModules ();

  void setTrayIcon ();
  void createNotifier ();

  void initLocale (const std::shared_ptr<linphone::Config> &config);

  QString getConfigLocale () const;
  void setConfigLocale (const QString &locale);

  QVariantList getAvailableLocales () const {
    return mAvailableLocales;
  }

  bool getAutoStart () const {
    return mAutoStart;
  }

  void setAutoStart (bool enabled);

  void openAppAfterInit (bool mustBeIconified = false);

  void setOpened (bool status) {
    if (mIsOpened != status) {
      mIsOpened = status;
      emit opened(mIsOpened);
    }
  }
  static QString getStrippedApplicationVersion();// x.y.z but if 'z-*' then x.y.z-1
  static void checkForUpdate ();

  static QString getQtVersion () {
    return qVersion();
  }

  QVariantList mAvailableLocales;
  QLocale mLocale;

  bool mAutoStart = false;

  QCommandLineParser *mParser = nullptr;

  QQmlApplicationEngine *mEngine = nullptr;

  DefaultTranslator *mTranslator = nullptr;
  DefaultTranslator *mDefaultTranslator = nullptr;
  Notifier *mNotifier = nullptr;

  QQuickWindow *mCallsWindow = nullptr;
  QQuickWindow *mSettingsWindow = nullptr;

  ColorListModel * mColorListModel;
  ImageListModel * mImageListModel;

  QSystemTrayIcon *mSystemTrayIcon = nullptr;

  bool mIsOpened = false;
};

#endif // APP_H_
