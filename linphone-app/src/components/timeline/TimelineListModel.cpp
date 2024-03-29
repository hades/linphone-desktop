/*
 * Copyright (c) 2010-2023 Belledonne Communications SARL.
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

#include "TimelineListModel.hpp"

#include "components/core/CoreManager.hpp"
#include "components/core/CoreHandlers.hpp"
#include "components/calls/CallsListModel.hpp"
#include "components/settings/AccountSettingsModel.hpp"
#include "components/settings/SettingsModel.hpp"
#include "utils/Utils.hpp"


#include "TimelineModel.hpp"
#include "TimelineListModel.hpp"

#include <QDebug>
#include <QElapsedTimer>


// =============================================================================

TimelineListModel::TimelineListModel (QObject *parent) : ProxyListModel(parent) {
	mSelectedCount = 0;
	CoreHandlers* coreHandlers= CoreManager::getInstance()->getHandlers().get();
	connect(coreHandlers, &CoreHandlers::chatRoomRead, this, &TimelineListModel::onChatRoomRead);
	connect(coreHandlers, &CoreHandlers::chatRoomStateChanged, this, &TimelineListModel::onChatRoomStateChanged);
	connect(coreHandlers, &CoreHandlers::messagesReceived, this, &TimelineListModel::update);
	
	QObject::connect(coreHandlers, &CoreHandlers::callStateChanged, this, &TimelineListModel::onCallStateChanged);
	QObject::connect(coreHandlers, &CoreHandlers::callCreated, this, &TimelineListModel::onCallCreated);
	
	connect(CoreManager::getInstance()->getSettingsModel(), &SettingsModel::hideEmptyChatRoomsChanged, this, &TimelineListModel::update);
	connect(CoreManager::getInstance()->getAccountSettingsModel(), &AccountSettingsModel::defaultRegistrationChanged, this, &TimelineListModel::update);
	updateTimelines ();
}

TimelineListModel::TimelineListModel(const TimelineListModel* model){
	mSelectedCount = model->mSelectedCount;
	CoreHandlers* coreHandlers= CoreManager::getInstance()->getHandlers().get();
	connect(coreHandlers, &CoreHandlers::chatRoomRead, this, &TimelineListModel::onChatRoomRead);
	connect(coreHandlers, &CoreHandlers::chatRoomStateChanged, this, &TimelineListModel::onChatRoomStateChanged);
	connect(coreHandlers, &CoreHandlers::messagesReceived, this, &TimelineListModel::update);
	
	QObject::connect(coreHandlers, &CoreHandlers::callStateChanged, this, &TimelineListModel::onCallStateChanged);
	QObject::connect(coreHandlers, &CoreHandlers::callCreated, this, &TimelineListModel::onCallCreated);
	
	connect(CoreManager::getInstance()->getSettingsModel(), &SettingsModel::hideEmptyChatRoomsChanged, this, &TimelineListModel::update);
	connect(CoreManager::getInstance()->getAccountSettingsModel(), &AccountSettingsModel::defaultRegistrationChanged, this, &TimelineListModel::update);
	for(auto item : model->mList) {
		auto newItem = qobject_cast<TimelineModel*>(item)->clone();
		connect(newItem.get(), SIGNAL(selectedChanged(bool)), this, SLOT(onSelectedHasChanged(bool)));
		connect(newItem.get(), &TimelineModel::chatRoomDeleted, this, &TimelineListModel::onTimelineDeleted);
		connect(newItem.get(), &TimelineModel::dataChanged, this, &TimelineListModel::onTimelineDataChanged);
		mList << newItem;
	}
}

TimelineListModel::~TimelineListModel(){
}

TimelineListModel* TimelineListModel::clone() const{
	return new TimelineListModel(this);
}
// -----------------------------------------------------------------------------

void TimelineListModel::reset(){
	updateTimelines ();
}

void TimelineListModel::update(){
	updateTimelines ();
}

void TimelineListModel::selectAll(const bool& selected){
	for(auto it = mList.begin() ; it != mList.end() ; ++it)
		it->objectCast<TimelineModel>()->setSelected(selected);
}

// -----------------------------------------------------------------------------

bool TimelineListModel::removeRows (int row, int count, const QModelIndex &parent) {
	QVector<QSharedPointer<TimelineModel> > oldTimelines;
	oldTimelines.reserve(count);
	int limit = row + count - 1;
	
	if (row < 0 || count < 0 || limit >= mList.count())
		return false;
	
	beginRemoveRows(parent, row, limit);
	
	for (int i = 0; i < count; ++i){
		auto timeline = mList.takeAt(row).objectCast<TimelineModel>();
		timeline->disconnectChatRoomListener();
		oldTimelines.push_back(timeline);
	}
	
	endRemoveRows();
	
	for(auto timeline : oldTimelines)
		if(timeline->mSelected) {
			timeline->setSelected(false);
				
		}
	emit countChanged();
	emit dataChanged(index(row), index(limit));
	return true;
}


// -----------------------------------------------------------------------------

QSharedPointer<TimelineModel> TimelineListModel::getTimeline(std::shared_ptr<linphone::ChatRoom> chatRoom, const bool &create){
	if(chatRoom){
		for(auto it = mList.begin() ; it != mList.end() ; ++it){
			auto timeline = it->objectCast<TimelineModel>();
			if( timeline->getChatRoomModel()->getChatRoom() == chatRoom){
				return timeline;
			}
		}
		if(create){			
			QSharedPointer<TimelineModel> model = TimelineModel::create(this, chatRoom);
			if(model){
				connect(model.get(), SIGNAL(selectedChanged(bool)), this, SLOT(onSelectedHasChanged(bool)));
				add(model);
				return model;
			}
		}
	}
	return nullptr;
}

QVariantList TimelineListModel::getLastChatRooms(const int& maxCount) const{
	QVariantList contacts;
	QMultiMap<qint64, ChatRoomModel*> sortedData;
	QDateTime currentDateTime = QDateTime::currentDateTime();
	bool doTest = true;
	
	for(auto timeline : mList){
		auto chatRoom = timeline.objectCast<TimelineModel>()->getChatRoomModel();
		if(chatRoom && chatRoom->isCurrentAccount() && chatRoom->isOneToOne() && !chatRoom->haveEncryption()) {
			sortedData.insert(chatRoom->mLastUpdateTime.secsTo(currentDateTime),chatRoom);
		}
	}
	do{
		int count = 0;
		for(auto contact : sortedData){
			if(!doTest || Utils::hasCapability(contact->getFullPeerAddress(),  LinphoneEnums::FriendCapabilityGroupChat)  ) {
				++count;
				contacts << QVariant::fromValue(contact);
				if(count >= maxCount)
					return contacts;
			}
		}
		doTest = false;
	}while( contacts.size() == 0 && sortedData.size() > 0);// no friends capability have been found : take contacts without testing capabilities.
	
	return contacts;
}

QSharedPointer<ChatRoomModel> TimelineListModel::getChatRoomModel(std::shared_ptr<linphone::ChatRoom> chatRoom, const bool& create){
	if(chatRoom ){
		for(auto timeline : mList){
			auto model = timeline.objectCast<TimelineModel>()->mChatRoomModel;
			if(model->getChatRoom() == chatRoom)
				return model;
		}
		if(create){
			QSharedPointer<TimelineModel> model = TimelineModel::create(this, chatRoom);
			if(model){
				connect(model.get(), SIGNAL(selectedChanged(bool)), this, SLOT(onSelectedHasChanged(bool)));
				add(model);
				return model->mChatRoomModel;
			}
		}
	}
	return nullptr;
}

QSharedPointer<ChatRoomModel> TimelineListModel::getChatRoomModel(ChatRoomModel * chatRoom){
	for(auto timeline : mList){
		auto model = timeline.objectCast<TimelineModel>()->mChatRoomModel;
		if(model == chatRoom)
			return model;
	}
	return nullptr;
}

TimelineModel * TimelineListModel::getFirstSelected() {
	if(mSelectedCount == 0)
		return nullptr;
	for(auto timeline : mList){
		auto model = timeline.objectCast<TimelineModel>();
		if(model->mSelected)
			return model.get();
	}
	return nullptr;
}

//-------------------------------------------------------------------------------------------------

void TimelineListModel::setSelectedCount(int selectedCount){
	if(mSelectedCount != selectedCount) {
		mSelectedCount = selectedCount;
		if( mSelectedCount <= 1)// Do not send signals when selection is higher than max : this is a transition state
			emit selectedCountChanged(mSelectedCount);
	}
}

void TimelineListModel::onSelectedHasChanged(bool selected){
	if( mSelectedCount == 1){//swap
		setSelectedCount(0);
		for(auto it = mList.begin() ; it != mList.end() ; ++it)
			if(it->get() != sender())
				it->objectCast<TimelineModel>()->setSelected(false);
		if(!selected){
			if( this ==  CoreManager::getInstance()->getTimelineListModel()) {// Clean memory only if the selection is about the main list.
				auto timeline = qobject_cast<TimelineModel*>(sender());
				timeline->getChatRoomModel()->resetData();// Cleanup leaving chat room
			}
		}else{
			setSelectedCount(1);
			emit selectedChanged(qobject_cast<TimelineModel*>(sender()));
		}
	}else if( mSelectedCount <1){//Select
		if(selected){
			setSelectedCount(1);
			emit selectedChanged(qobject_cast<TimelineModel*>(sender()));
		}
	}else{// Do nothing
	}
}

void TimelineListModel::updateTimelines () {
	QElapsedTimer timer, stepsTimer;
	timer.start();
	
	stepsTimer.start();
	CoreManager *coreManager = CoreManager::getInstance();
	std::list<std::shared_ptr<linphone::ChatRoom>> allChatRooms = coreManager->getCore()->getChatRooms();

// Clean terminated chat rooms and conferences from timeline.
	allChatRooms.remove_if([](std::shared_ptr<linphone::ChatRoom> chatRoom){
		if( ChatRoomModel::isTerminated(chatRoom) && chatRoom->getUnreadMessagesCount() > 0)
			chatRoom->markAsRead();
		if(chatRoom->getState() == linphone::ChatRoom::State::Deleted)
			return true;
		if(!chatRoom->hasCapability((int)linphone::ChatRoom::Capabilities::Basic)){
			auto conferenceAddress = chatRoom->getConferenceAddress();
			if( conferenceAddress && conferenceAddress->getDomain() == Constants::LinphoneDomain) {
				QString conferenceAddressStr = Utils::coreStringToAppString(conferenceAddress->asStringUriOnly());
				if( conferenceAddressStr.contains("conf-id"))
					return true;
			}
		}
		return false;
	}); 
	qInfo() << "Timelines cleaning :" << stepsTimer.restart() << "ms.";
//Remove no more chat rooms
	auto itTimeline = mList.begin();
	while(itTimeline != mList.end()) {
		auto itDbTimeline = allChatRooms.begin();
		if(*itTimeline) {
			auto chatRoomModel = itTimeline->objectCast<TimelineModel>()->getChatRoomModel();
			if(chatRoomModel) {
				auto timeline = chatRoomModel->getChatRoom();
				if( timeline ) {
					while(itDbTimeline != allChatRooms.end() && *itDbTimeline != timeline ){
						++itDbTimeline;
					}
				}else
					itDbTimeline = allChatRooms.end();
			}else
				itDbTimeline = allChatRooms.end();
		}else
			itDbTimeline = allChatRooms.end();
		if( itDbTimeline == allChatRooms.end()){
			int index = itTimeline - mList.begin();
			if(index>0){
				--itTimeline;
				removeRow(index);// This will call removeRows()
				++itTimeline;
			}else{
				removeRow(0);// This will call removeRows()
				itTimeline = mList.begin();
			}
		}else
			++itTimeline;
	}
	qInfo() << "Timelines removing expired :" << stepsTimer.restart() << "ms.";
	// Add new.
// Call logs optimization : store all the list and check on it for each chat room instead of loading call logs on each chat room. See TimelineModel()
	std::list<std::shared_ptr<linphone::CallLog>> callLogs = coreManager->getCore()->getCallLogs();
	QList<QSharedPointer<TimelineModel>> models;
	QMap<QString,QMap<QString, std::shared_ptr<linphone::CallLog>>> optimizedCallLogs;
	for(auto callLog : callLogs){
		QString localAddress = Utils::coreStringToAppString(callLog->getLocalAddress()->asStringUriOnly());
		QString peerAddress = Utils::coreStringToAppString(callLog->getRemoteAddress()->asStringUriOnly());
		auto itLocal = optimizedCallLogs.find(localAddress);
		if(itLocal == optimizedCallLogs.end()){
			optimizedCallLogs[localAddress][peerAddress] = callLog;
		}else{
			auto itPeer = itLocal->find(peerAddress);
			if(itPeer == itLocal->end())
				optimizedCallLogs[localAddress][peerAddress] = callLog;
		}
	}
	qInfo() << "Timelines optimization for build :" << stepsTimer.restart() << "ms.";
	for(auto dbChatRoom : allChatRooms){
		auto haveTimeline = getTimeline(dbChatRoom, false);
		if(!haveTimeline && dbChatRoom){// Create a new Timeline if needed
			QSharedPointer<TimelineModel> model = TimelineModel::create(this, dbChatRoom, optimizedCallLogs);
			if( model){
				connect(model.get(), SIGNAL(selectedChanged(bool)), this, SLOT(onSelectedHasChanged(bool)));
				models << model;
			}
		}
	}
	qInfo() << "Timelines adding :" << stepsTimer.restart() << "ms for " << models.size() << " new timelines";
	add(models);
	qInfo() << "Timelines adding GUI :" << stepsTimer.restart() << "ms.";
	qInfo() << "Timelines initialized in:" << timer.elapsed() << "ms.";
}

void TimelineListModel::add (QSharedPointer<TimelineModel> timeline){
	auto chatRoomModel = timeline->getChatRoomModel();
	auto chatRoom = chatRoomModel->getChatRoom();
	connect(timeline.get(), &TimelineModel::chatRoomDeleted, this, &TimelineListModel::onTimelineDeleted);
	connect(timeline.get(), &TimelineModel::dataChanged, this, &TimelineListModel::onTimelineDataChanged);
	
	ProxyListModel::add(timeline);
	emit countChanged();
}

void TimelineListModel::add (QList<QSharedPointer<TimelineModel>> timelines){
	for(auto timeline : timelines){
		auto chatRoomModel = timeline->getChatRoomModel();
		auto chatRoom = chatRoomModel->getChatRoom();
		connect(timeline.get(), &TimelineModel::chatRoomDeleted, this, &TimelineListModel::onTimelineDeleted);
		connect(timeline.get(), &TimelineModel::dataChanged, this, &TimelineListModel::onTimelineDataChanged);
	}
	if(timelines.size() > 0) {
		ProxyListModel::add(timelines);
		emit countChanged();
	}
}

void TimelineListModel::removeChatRoomModel(QSharedPointer<ChatRoomModel> model){
	if(!model || (model->getChatRoom()->isEmpty() && (model->isReadOnly() || !model->isGroupEnabled()))){
		auto itTimeline = mList.begin();
		while(itTimeline != mList.end()) {
			auto timeline = itTimeline->objectCast<TimelineModel>();
			if(timeline->mChatRoomModel == model){
				if(model)
					model->markAsToDelete();
				remove(*itTimeline);// This will call removeRows()
				return;
			}else
				++itTimeline;
		}
	}
}

void TimelineListModel::select(ChatRoomModel * chatRoomModel){
	if(chatRoomModel) {
		auto timeline = getTimeline(chatRoomModel->getChatRoom(), false);
		if(timeline){
			if(timeline->isUpdating())
				timeline->delaySelected();
			else
				timeline->setSelected(true);
		}
	}
}

void TimelineListModel::onChatRoomRead(const std::shared_ptr<linphone::ChatRoom> &chatRoom){
	auto timeline = getTimeline(chatRoom, false);
	if(timeline) {
		if(timeline->getChatRoomModel()){
			timeline->getChatRoomModel()->enableMarkAsRead(true);
			timeline->getChatRoomModel()->resetMessageCount();
			timeline->getChatRoomModel()->enableMarkAsRead(false);
		}
	}
}

void TimelineListModel::onChatRoomStateChanged(const std::shared_ptr<linphone::ChatRoom> &chatRoom,linphone::ChatRoom::State state){
	if( state == linphone::ChatRoom::State::Created
			&& !getTimeline(chatRoom, false)){// Create a new Timeline if needed
		if( chatRoom->hasCapability((int)linphone::ChatRoom::Capabilities::Conference) && !chatRoom->ephemeralEnabled()) {
			int ephemeralTime = CoreManager::getInstance()->getSettingsModel()->getCreateEphemeralChatRooms();
			if( ephemeralTime>0){
				chatRoom->setEphemeralLifetime(ephemeralTime);
				chatRoom->enableEphemeral(true);
			}
		}
		QSharedPointer<TimelineModel> model = TimelineModel::create(this, chatRoom);
		if(model){
			connect(model.get(), SIGNAL(selectedChanged(bool)), this, SLOT(onSelectedHasChanged(bool)));
			add(model);
		}
	}else if(state == linphone::ChatRoom::State::Deleted || state == linphone::ChatRoom::State::Terminated){
		auto timeline = getTimeline(chatRoom, false);
		if(timeline) {
			if(timeline->getChatRoomModel())
				timeline->getChatRoomModel()->resetMessageCount();
			if(state == linphone::ChatRoom::State::Deleted){
				remove(timeline);// This will call removeRows()
			}
		}
	}else if(state == linphone::ChatRoom::State::CreationFailed){
		auto timeline = getTimeline(chatRoom, false);
		if(timeline) {
			remove(timeline);// This will call removeRows()
		}
	}
}

void TimelineListModel::onCallStateChanged (const std::shared_ptr<linphone::Call> &call, linphone::Call::State state) {
}

void TimelineListModel::onCallCreated(const std::shared_ptr<linphone::Call> &call){
	std::shared_ptr<linphone::Core> core = CoreManager::getInstance()->getCore();
	std::shared_ptr<linphone::ChatRoomParams> params = core->createDefaultChatRoomParams();
	std::list<std::shared_ptr<linphone::Address>> participants;
	if( !call->getConference() && false ){
	// Find all chat rooms with local address. If not, create one.
		bool isOutgoing = (call->getDir() == linphone::Call::Dir::Outgoing) ;
		bool found = false;
		auto callLog = call->getCallLog();
		auto callLocalAddress = callLog->getLocalAddress()->clone();
		callLocalAddress->clean();
		auto currentParams = call->getCurrentParams();
		bool isEncrypted = currentParams->getMediaEncryption() != linphone::MediaEncryption::None;
		bool createSecureChatRoom = false;
		SettingsModel * settingsModel = CoreManager::getInstance()->getSettingsModel();
		
		if( settingsModel->getSecureChatEnabled() && 
			(!settingsModel->getStandardChatEnabled() || (settingsModel->getStandardChatEnabled() && isEncrypted))
			){
			params->enableEncryption(true);
			createSecureChatRoom = true;
		}
		participants.push_back(callLog->getRemoteAddress()->clone());
		participants.back()->clean();// Not cleaning allows chatting to a specific device but the current design is not adapted to show them
		auto chatRoom = core->searchChatRoom(params, callLocalAddress
											 , nullptr//callLog->getRemoteAddress()
											 , participants);
		if(chatRoom){
			for(auto item : mList){
				auto timeline = item.objectCast<TimelineModel>();
				if( chatRoom == timeline->mChatRoomModel->getChatRoom()){
					found = true;
					if(isOutgoing)// If outgoing, we switch to this chat room
						timeline->setSelected(true);
				}
			}
		}
		if(!found){// Create a default chat room
			QVariantList participants;
			auto remoteAddress = callLog->getRemoteAddress()->clone();
			remoteAddress->clean();
			participants << Utils::coreStringToAppString(remoteAddress->asStringUriOnly());
			CoreManager::getInstance()->getCallsListModel()->createChatRoom("", (createSecureChatRoom?1:0),  callLocalAddress, participants, isOutgoing);
		}
	}
}

void TimelineListModel::onTimelineDeleted(){
	remove(sender());// This will call removeRows()
}

void TimelineListModel::onTimelineDataChanged(){
	int row = -1;
	get(sender(), &row);
	if(row >= 0){
		emit dataChanged(index(row),index(row));
	}
}