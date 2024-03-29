pragma Singleton
import QtQml 2.2
import QtQuick 2.7

import Units 1.0
import ColorsList 1.0

// =============================================================================

QtObject {
	property string sectionName : 'Conversation'
	property QtObject bar: QtObject {
		property var backgroundColor: ColorsList.add(sectionName+'_bar_bg', 'e')
		property var searchIconColor: ColorsList.add(sectionName+'_bar_search', 'c')
		property int avatarSize: 60
		property int groupChatSize: 80
		property string groupChatIcon: 'chat_room_custom'
		property var groupChatColor: ColorsList.add(sectionName+'_bar_groupChat', 'g')
		
		property int height: 80
		property int leftMargin: 40
		property int rightMargin: 10
		property int spacing: 20
		
		property QtObject status: QtObject {
			property string adminStatusIcon: 'admin_selected_custom'
			property int adminStatusIconSize: 24
			property var adminStatusColor: ColorsList.add(sectionName+'_bar_status', 's')
			property var adminTextColor : ColorsList.add(sectionName+'_description_admin_status', 'af')
		}
		
		property QtObject actions: QtObject {
			property int spacing: 20
			
			property QtObject call: QtObject {
				property int iconSize: 40
				property string name : 'call'
				property string icon : 'call_custom'
				property var backgroundNormalColor : ColorsList.addImageColor(sectionName+'_'+name+'_bg_n', icon, 's_n_b_bg')
				property var backgroundHoveredColor : ColorsList.addImageColor(sectionName+'_'+name+'_bg_h', icon, 's_h_b_bg')
				property var backgroundPressedColor : ColorsList.addImageColor(sectionName+'_'+name+'_bg_p', icon, 's_p_b_bg')
				property var foregroundNormalColor : ColorsList.addImageColor(sectionName+'_'+name+'_fg_n', icon, 's_n_b_fg')
				property var foregroundHoveredColor : ColorsList.addImageColor(sectionName+'_'+name+'_fg_h', icon, 's_h_b_fg')
				property var foregroundPressedColor : ColorsList.addImageColor(sectionName+'_'+name+'_fg_p', icon, 's_p_b_fg')
			}
			
			property QtObject chat: QtObject {
				property int iconSize: 40
				property string name : 'chat'
				property string icon : 'chat_custom'
				property var backgroundNormalColor : ColorsList.addImageColor(sectionName+'_'+name+'_bg_n', icon, 's_n_b_bg')
				property var backgroundHoveredColor : ColorsList.addImageColor(sectionName+'_'+name+'_bg_h', icon, 's_h_b_bg')
				property var backgroundPressedColor : ColorsList.addImageColor(sectionName+'_'+name+'_bg_p', icon, 's_p_b_bg')
				property var foregroundNormalColor : ColorsList.addImageColor(sectionName+'_'+name+'_fg_n', icon, 's_n_b_fg')
				property var foregroundHoveredColor : ColorsList.addImageColor(sectionName+'_'+name+'_fg_h', icon, 's_h_b_fg')
				property var foregroundPressedColor : ColorsList.addImageColor(sectionName+'_'+name+'_fg_p', icon, 's_p_b_fg')
			}
			
			property QtObject groupChat: QtObject {
				property int iconSize: 40
				property string name : 'groupChat'
				property string icon : 'group_chat_custom'
				property var backgroundNormalColor : ColorsList.addImageColor(sectionName+'_'+name+'_bg_n', icon, 's_n_b_bg')
				property var backgroundHoveredColor : ColorsList.addImageColor(sectionName+'_'+name+'_bg_h', icon, 's_h_b_bg')
				property var backgroundPressedColor : ColorsList.addImageColor(sectionName+'_'+name+'_bg_p', icon, 's_p_b_bg')
				property var backgroundUpdatingColor : ColorsList.addImageColor(sectionName+'_'+name+'_bg_u', icon, 's_p_b_bg')
				property var foregroundNormalColor : ColorsList.addImageColor(sectionName+'_'+name+'_fg_n', icon, 's_n_b_fg')
				property var foregroundHoveredColor : ColorsList.addImageColor(sectionName+'_'+name+'_fg_h', icon, 's_h_b_fg')
				property var foregroundPressedColor : ColorsList.addImageColor(sectionName+'_'+name+'_fg_p', icon, 's_p_b_fg')
				property var foregroundUpdatingColor : ColorsList.addImageColor(sectionName+'_'+name+'_fg_u', icon, 's_p_b_fg')
			}
			
			property QtObject videoCall: QtObject {
				property int iconSize: 40
				property string name : 'videoCall'
				property string icon : 'video_call_custom'
				property var backgroundNormalColor : ColorsList.addImageColor(sectionName+'_'+name+'_bg_n', icon, 's_n_b_bg')
				property var backgroundHoveredColor : ColorsList.addImageColor(sectionName+'_'+name+'_bg_h', icon, 's_h_b_bg')
				property var backgroundPressedColor : ColorsList.addImageColor(sectionName+'_'+name+'_bg_p', icon, 's_p_b_bg')
				property var foregroundNormalColor : ColorsList.addImageColor(sectionName+'_'+name+'_fg_n', icon, 's_n_b_fg')
				property var foregroundHoveredColor : ColorsList.addImageColor(sectionName+'_'+name+'_fg_h', icon, 's_h_b_fg')
				property var foregroundPressedColor : ColorsList.addImageColor(sectionName+'_'+name+'_fg_p', icon, 's_p_b_fg')
			}
			
			property QtObject del: QtObject {
				property int iconSize: 40
				property QtObject deleteHistory: QtObject {
					property int iconSize: 40
					property string name : 'deleteHistory'
					property string icon : 'delete_custom'
					property var backgroundNormalColor : ColorsList.addImageColor(sectionName+'_'+name+'_bg_n', icon, 'l_n_b_bg')
					property var backgroundHoveredColor : ColorsList.addImageColor(sectionName+'_'+name+'_bg_h', icon, 'l_h_b_bg')
					property var backgroundPressedColor : ColorsList.addImageColor(sectionName+'_'+name+'_bg_p', icon, 'l_p_b_bg')
					property var foregroundNormalColor : ColorsList.addImageColor(sectionName+'_'+name+'_fg_n', icon, 'l_n_b_fg')
					property var foregroundHoveredColor : ColorsList.addImageColor(sectionName+'_'+name+'_fg_h', icon, 'l_h_b_fg')
					property var foregroundPressedColor : ColorsList.addImageColor(sectionName+'_'+name+'_fg_p', icon, 'l_p_b_fg')
				}
			}
			
			property QtObject edit: QtObject {
				property int iconSize: 40
				property QtObject addContact: QtObject {
					property int iconSize: 40
					property string name : 'addContact'
					property string icon : 'contact_add_custom'
					property var backgroundNormalColor : ColorsList.addImageColor(sectionName+'_'+name+'_bg_n', icon, 'l_n_b_bg')
					property var backgroundHoveredColor : ColorsList.addImageColor(sectionName+'_'+name+'_bg_h', icon, 'l_h_b_bg')
					property var backgroundPressedColor : ColorsList.addImageColor(sectionName+'_'+name+'_bg_p', icon, 'l_p_b_bg')
					property var foregroundNormalColor : ColorsList.addImageColor(sectionName+'_'+name+'_fg_n', icon, 'l_n_b_fg')
					property var foregroundHoveredColor : ColorsList.addImageColor(sectionName+'_'+name+'_fg_h', icon, 'l_h_b_fg')
					property var foregroundPressedColor : ColorsList.addImageColor(sectionName+'_'+name+'_fg_p', icon, 'l_p_b_fg')
				}
				property QtObject viewContact: QtObject {
					property int iconSize: 40
					property string name : 'viewContact'
					property string icon : 'contact_view_custom'
					property var backgroundNormalColor : ColorsList.addImageColor(sectionName+'_'+name+'_bg_n', icon, 'l_n_b_bg')
					property var backgroundHoveredColor : ColorsList.addImageColor(sectionName+'_'+name+'_bg_h', icon, 'l_h_b_bg')
					property var backgroundPressedColor : ColorsList.addImageColor(sectionName+'_'+name+'_bg_p', icon, 'l_p_b_bg')
					property var foregroundNormalColor : ColorsList.addImageColor(sectionName+'_'+name+'_fg_n', icon, 'l_n_b_fg')
					property var foregroundHoveredColor : ColorsList.addImageColor(sectionName+'_'+name+'_fg_h', icon, 'l_h_b_fg')
					property var foregroundPressedColor : ColorsList.addImageColor(sectionName+'_'+name+'_fg_p', icon, 'l_p_b_fg')
				}
			}
			property QtObject openMenu: QtObject {
					property int iconSize: 40
					property string name : 'other'
					property string icon : 'menu_vdots_custom'
					property var backgroundNormalColor : ColorsList.addImageColor(sectionName+'_'+name+'_bg_n', icon, 'l_n_b_bg')
					property var backgroundHoveredColor : ColorsList.addImageColor(sectionName+'_'+name+'_bg_h', icon, 'l_h_b_bg')
					property var backgroundPressedColor : ColorsList.addImageColor(sectionName+'_'+name+'_bg_p', icon, 'l_p_b_bg')
					property var backgroundUpdatingColor : ColorsList.addImageColor(sectionName+'_'+name+'_bg_u', icon, 'l_u_b_bg')
					property var foregroundNormalColor : ColorsList.addImageColor(sectionName+'_'+name+'_fg_n', icon, 'l_n_b_fg')
					property var foregroundHoveredColor : ColorsList.addImageColor(sectionName+'_'+name+'_fg_h', icon, 'l_h_b_fg')
					property var foregroundPressedColor : ColorsList.addImageColor(sectionName+'_'+name+'_fg_p', icon, 'l_p_b_fg')
					property var foregroundUpdatingColor : ColorsList.addImageColor(sectionName+'_'+name+'_fg_u', icon, 'l_u_b_fg')
				}
		}
		
		property QtObject contactDescription : QtObject {
			property QtObject subtitle: QtObject {
				property var colorModel: ColorsList.add(sectionName+'_description_subtitle', 'n')
				property int pointSize: Units.dp * 10
				property int weight: Font.Light
			}
			
			property QtObject title: QtObject {
				property var colorModel: ColorsList.add(sectionName+'_description_title', 'j')
				property int pointSize: Units.dp * 11
				property int weight: Font.Normal
				property QtObject status : QtObject{
					property var colorModel : ColorsList.add(sectionName+'_description_title_status', 'g')
					property int pointSize : Units.dp * 9
				}
			}
		}
	}
	
	property QtObject filters: QtObject {
		property var backgroundColor: ColorsList.add(sectionName+'_filters_bg', 'q')
		property var iconColor: ColorsList.add(sectionName+'_filters_icon', 'c')
		property int height: 51
		property int leftMargin: 40
		property int pointSize: Units.dp * 9
		
		property QtObject border: QtObject {
			property var colorModel: ColorsList.add(sectionName+'_filters_border', 'g10')
			property int bottomWidth: 1
			property int topWidth: 0
		}
	}
	property QtObject menu: QtObject{
		property var separatorColor: ColorsList.add(sectionName+'_separator', 'u')
	}
	
}
