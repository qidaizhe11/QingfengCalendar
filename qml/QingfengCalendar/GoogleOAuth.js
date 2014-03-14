var client_id = "135859676517-padra6sp9cd3ahqs0rs1j5l508e05nu1.apps.googleusercontent.com";
var client_secret = "3M70A_ZLApKTz46uve9P0H5T";
var redirect_uri = "http://localhost";

function urlChanged(url) {
    console.log("urlChanged!!!");
    var m_url = url.toString();

    console.log("m_url = ", url);

    var access_token = "";
    var code = "";

    if (m_url.indexOf(redirect_uri) > -1) {
        var query = m_url.substring(m_url.indexOf("?") + 1);
        var vars = query.split("&");
        for (var i = 0; i < vars.length; ++i) {
            var pair = vars[i].split("=");
            if (pair[0] === "access_token") {
                access_token = pair[1];
            }
            if (pair[0] === "code") {
                code = pair[1];
            }
        }
    }

    console.log("Found access_token = ", access_token);
    console.log("Found code = ", code);

    if (access_token !== "") {
        google_oauth.access_token = access_token;
    }
    if (code !== "") {
        requestPermanentToken(code);
    }
}

function requestPermanentToken(code) {
    var req = new XMLHttpRequest();

    req.open("POST", "https://accounts.google.com/o/oauth2/token", true);
    req.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
    req.send("client_id=" + client_id +
             "&redirect_uri=" + redirect_uri +
             "&client_secret=" + client_secret +
             "&grant_type=authorization_code" +
             "&code=" + code);

    req.onreadystatechange = function() {
        console.log("req.readyState", req.readyState);
        if (req.readyState == 4 && req.status == 200) {
            console.log("responseText", req.responseText);
            var result = eval("(" + req.responseText + ")");
            google_oauth.access_token = result["access_token"];
            google_settings.refreshToken = result["refresh_token"];
        } else if (req.readyState == 4) {
            console.log("Status, headers: ", req.status,
                        req.getAllResponseHeaders());
        }
    }
}

function refreshAccessToken(refresh_token) {
    console.log("called refreshAccessToken", refresh_token);
    console.log("Before refresh, access token: ", google_settings.accessToken);
    var req = new XMLHttpRequest();

    req.open("POST", "https://accounts.google.com/o/oauth2/token", true);
    req.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");

    req.send("client_id=" + client_id +
             "&client_secret=" + client_secret +
             "&grant_type=refresh_token" +
             "&refresh_token=" + refresh_token);

    req.onreadystatechange = function() {
        console.log("req.readyState", req.readyState);

        if (req.readyState == 4 && req.status == 200) {
            console.log("responseText", req.responseText);
            var result = eval("(" + req.responseText + ")");
            google_oauth.access_token = result["access_token"];
        } else if (req.readyState == 4) {
            console.log("Status: ", req.status);
            console.log("Status Text: ", req.statusText);
            console.log("Headers: ", req.getAllResponseHeaders());
        }
    }
}
