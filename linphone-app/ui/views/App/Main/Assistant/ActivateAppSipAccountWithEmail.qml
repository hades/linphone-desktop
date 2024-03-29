import QtQuick 2.7

import Common 1.0
import Linphone 1.0
import Utils 1.0

import App.Styles 1.0

// =============================================================================

AssistantAbstractView {
  property var assistantModel

  backEnabled: false

  title: qsTr('activateAppSipAccount').replace('%1', Qt.application.name.toUpperCase())

  mainAction: requestBlock.execute
  mainActionEnabled: !requestBlock.loading
  mainActionLabel: qsTr('confirmAction')

  Column {
    anchors.centerIn: parent
    spacing: ActivateAppSipAccountWithEmailStyle.spacing
    width: parent.width

    Text {
      color: ActivateAppSipAccountWithEmailStyle.activationSteps.colorModel.color
      font.pointSize: ActivateAppSipAccountWithEmailStyle.activationSteps.pointSize
      horizontalAlignment: Text.AlignHCenter
      text: qsTr('activationSteps').replace('%1', assistantModel.email)
      width: parent.width
      wrapMode: Text.WordWrap
    }

    RequestBlock {
      id: requestBlock

      action: assistantModel.activate
      width: parent.width
      loading: assistantModel.isProcessing
    }
  }

  // ---------------------------------------------------------------------------
  // Assistant.
  // ---------------------------------------------------------------------------

  Connections {
    target: assistantModel

    onActivateStatusChanged: {
      requestBlock.setText(error)
      if (!error.length) {
        function quitToHome (window) {
          window.unlockView()
          window.setView('Home')
        }
        var codecInfo = VideoCodecsModel.getCodecInfo('H264')
        if (codecInfo.downloadUrl) {
          Utils.openCodecOnlineInstallerDialog(window, codecInfo, quitToHome)
        } else {
          quitToHome(window)
        }
      }
    }
  }
}
