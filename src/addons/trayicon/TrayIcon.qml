// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick 2.15
import QtQml.Models 2.15
import Qt.labs.platform 1.1

SystemTrayIcon {
    visible: true
    icon.name: trayicon.imAddonIcon
    // icon.source: "image://icon/" + trayicon.imAddonIcon

    menu: Menu {
        id: contextMenu

        // Instantiator {
        //     model: trayicon.properties

        //     delegate: MenuItem {
        //         text: modelData.label
        //         icon.name: modelData.icon
        //         onTriggered: trayicon.menuTriggered(modelData.key)
        //     }

        //     onObjectAdded: function(index, object) {
        //         contextMenu.insertItem(index, object)
        //     }
        //     onObjectRemoved: function(index, object) {
        //         contextMenu.removeItem(object)
        //     }
        // }

        // MenuSeparator {}

        // MenuItem {
        //     text: qsTr("Reload Configuration")
        //     onTriggered: trayicon.reloadTriggered()
        // }

        // MenuItem {
        //     text: qsTr("Settings")
        //     onTriggered: trayicon.configureTriggered()
        // }
    }
}
