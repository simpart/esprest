#include <WiFi.h>
#include "EspREST.hpp"

EspRESTResp::EspRESTResp (int sts_code, const char *resp) {
    try {
        status_code = sts_code;
        if (nullptr == resp) {
            throw std::exception();
        }
        std::string err;
        responce = json11::Json::parse(resp, err);
    } catch (std::exception e) {
        throw e;
    }
}

EspRESTClient::EspRESTClient (String u) {
    try {
        // get host
        int idxof = u.indexOf("https://");
        if (-1 == idxof) {
            throw std::exception();
        }
        //Serial.println(u.substring(strlen("https://")));
        String buff = u.substring(strlen("https://"));
        idxof = buff.indexOf("/");
        if (-1 == idxof) {
            host = buff;
        } else {
            host = buff.substring(0, idxof);
        }

        uri_base = u;

    } catch (std::exception e) {
        throw e;
    }
}

EspRESTResp EspRESTClient::send (E_Method mtd, String path) {
    return send(mtd, path, "");
}

EspRESTResp EspRESTClient::send (E_Method mtd, String path, String body) {
    try {
        if (WiFi.status() != WL_CONNECTED) {
            // warning: "wifi status is not connected."
            throw std::exception();
        }

        wclient.setInsecure();  //skip verification
        if (!wclient.connect(host.c_str(), 443)) {
            Serial.println("Connection failed!");
            throw std::exception();
        }
        
        /* HTTP request */
        // method
        String method[] = {"GET","POST","PUT","DELETE","OPTIONS"};
        wclient.println(method[(int)mtd] + " " + path + " HTTP/1.1");

        // header
        wclient.println("Host: " + host);
        wclient.println("Connection: close");
        if (0 != body.length()) {
            wclient.println("Content-Type: application/json");
            wclient.println("Content-Length: " + String(body.length()));
            // body
            wclient.println();
            wclient.println(body);
        }
        wclient.println();
        
        // responce
        delay(50);
        //ServResp ret;
        int sts_code;
        while (wclient.connected()) {
            String line = wclient.readStringUntil('\n');
            if (-1 != line.indexOf("HTTP")) {
                // get status code
                line     = line.substring(line.indexOf(" "), line.indexOf(" ")+4);
                sts_code = atoi(line.c_str());
            } else if (line == "\r") {
                //Serial.println("headers received");
                break;
            }
        }
        // if there are incoming bytes available
        // from the server, read them and print them:
        String resp = "";
        while (wclient.available()) {
            char buf = wclient.read();
            if ( (-1 == buf) || (255 == buf) ||
                 ((0 < buf) && (32 > buf)) ) {
                continue;
            }
            resp.concat(buf);
        }
        wclient.stop();

        //std::string err;
        //Serial.println(resp);
        return EspRESTResp(sts_code, resp.c_str());
        //return json11::Json::parse(resp.c_str(),err);

    } catch (std::exception e) {
        throw e;
    }
}

EspRESTResp EspRESTClient::get (String path) {
    return send(E_Method::GET, path);
}

EspRESTResp EspRESTClient::post (String path, json11::Json *prm) {
    String body = "";
    if (nullptr != prm) {
        body = prm->dump().c_str();
    }
    return send(E_Method::POST, path, body);
}

EspRESTResp EspRESTClient::put (String path, json11::Json *prm) {
    String body = "";
    if (nullptr != prm) {
        body = prm->dump().c_str();
    }
    return send(E_Method::PUT, path, body);
}

EspRESTResp EspRESTClient::del (String path) {
    return send(E_Method::DELETE, path);
}
