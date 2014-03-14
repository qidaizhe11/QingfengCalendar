import QtQuick 2.1
import QtQuick.Controls 1.0
import QtWebKit 3.0
import "GoogleOAuth.js" as OAuth

Rectangle {
    id: google_oauth

//    width: parent.width
//    height: parent.height
    width: 960
    height: 600

    color: "#343434"

    property string oauth_link:
        "https://accounts.google.com/o/oauth2/auth?" +
        "client_id=" + OAuth.client_id +
        "&redirect_uri=" + OAuth.redirect_uri +
        "&response_type=code" +
        "&scope=https://www.googleapis.com/auth/calendar";

    property string access_token: ""
    property bool authorized: access_token != ""

    signal loginDone()

    onAccess_tokenChanged: {
        console.log("onAccess_tokenChanged.");
        if (access_token != "") {
            console.log("access_token = ", access_token);
            loginDone();
        }
    }

    function login() {
        login_view.url = oauth_link;
    }

    function refreshAccessToken(refresh_token) {
        OAuth.refreshAccessToken(refresh_token)
    }

    Item {
        id: top_tool_bar

        anchors.left: parent.left
        anchors.top: parent.top
        height: 60

        Button {
            id: close_button
            text: "Close"
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            onClicked: google_oauth.visible = false
        }
    }

    WebView {
        id: login_view

        anchors.fill: parent

        url: ""
        onUrlChanged: OAuth.urlChanged(url)

//        onLoadFinished: {
//            console.log("onLoadFinished.")
//            web_view_mousearea.cursorShape = Qt.ArrowCursor;
//        }

//        onLoadStarted: {
//            console.log("onLoadStarted.")
//            web_view_mousearea.cursorShape = Qt.WaitCursor;
//        }

        onLoadingChanged: {
            switch (loadRequest.status) {
            case WebView.LoadSucceededStatus:
                console.log("onLoadSucceeded.");
                return
            case WebView.LoadStartedStatus:
                console.log("onLoadStarted.");
                break
            case WebView.LoadFailedStatus:
                console.log("onLoadFailed.");
                break
            }
        }
    } // web_viw
}
