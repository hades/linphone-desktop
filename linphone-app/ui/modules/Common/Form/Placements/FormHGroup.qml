import QtQuick 2.7
import QtQuick.Layouts 1.3

import Common 1.0
import Common.Styles 1.0

// =============================================================================

RowLayout {
  property alias label: label.text
  property alias labelFont: label.font

  default property var _content: null
  property int maxWidth: FormHGroupStyle.content.maxWidth
  property bool fitLabel : false
  // ---------------------------------------------------------------------------

  spacing: FormHGroupStyle.spacing

  // ---------------------------------------------------------------------------

  Text {
    id: label
	computeFitWidth: parent.fitLabel
    Layout.preferredHeight: FormHGroupStyle.legend.height
    Layout.preferredWidth: fitLabel ? label.fitWidth : FormHGroupStyle.legend.width

    color: FormHGroupStyle.legend.colorModel.color
    elide: Text.ElideRight
    font.pointSize: FormHGroupStyle.legend.pointSize

    horizontalAlignment: Text.AlignRight
    verticalAlignment: Text.AlignVCenter

    TooltipArea {
      delay: 0
      text: parent.text
      visible: parent.truncated
    }
  }

  // ---------------------------------------------------------------------------

  Item {
    readonly property int currentHeight: _content ? _content.height : 0

    Layout.alignment: (
      currentHeight < FormHGroupStyle.legend.height
        ? Qt.AlignVCenter
        : Qt.AlignTop
    ) | Qt.AlignLeft

    Layout.fillWidth: true
    Layout.preferredHeight: currentHeight

    Loader {
      active: !!_content
      anchors.fill: parent

      sourceComponent: Item {
        id: content

        data: [ _content ]
        width: parent.width

        Component.onCompleted: _content.width = Qt.binding(function () {
          var contentWidth = content.width
          var wishedWidth = parent.parent.parent.maxWidth
          return contentWidth > wishedWidth ? wishedWidth : contentWidth
        })
      }
    }
  }
}
