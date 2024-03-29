import QtQuick 2.7
import QtQuick.Layouts 1.3

import Common.Styles 1.0
import Common 1.0

// =============================================================================

Column {
	property alias title: title.text
	property int orientation: Qt.Horizontal
	property bool addButton : false
	signal addButtonClicked;
	property bool removeButton : false
	signal removeButtonClicked;
	
	// ---------------------------------------------------------------------------
	
	spacing: FormStyle.spacing
	
	// ---------------------------------------------------------------------------
	
	ColumnLayout {
		spacing: FormStyle.header.spacing
		visible: parent.title.length > 0
		width: parent.width
		
		Row{
			spacing:10
			Text {
				id: title
				anchors.verticalCenter: parent.verticalCenter
				
				color: FormStyle.header.title.colorModel.color
				font {
					bold: true
					pointSize: FormStyle.header.title.pointSize
				}
			}
			ActionButton {
				visible:addButton
				anchors.verticalCenter: parent.verticalCenter
				isCustom: true
				backgroundRadius: 90
				colorSet: FormStyle.header.add
				
				scale:0.8
				onClicked:addButtonClicked()
			}
			ActionButton {
				visible:removeButton
				anchors.verticalCenter: parent.verticalCenter
				isCustom: true
				backgroundRadius: 90
				
				colorSet: FormStyle.header.deleteAction
				scale:0.8
				onClicked:removeButtonClicked()
			}
		}
		
		Rectangle {
			Layout.fillWidth: true
			Layout.preferredHeight: FormStyle.header.separator.height
			
			color: FormStyle.header.separator.colorModel.color
		}
		
		Item {
			height: FormStyle.header.bottomMargin
		}
	}
}
