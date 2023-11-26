#include <WiFiClientSecure.h>
#include "json11.hpp"

#ifndef __ESPREST_HPP__
#define __ESPREST_HPP__

enum class E_Method {
    GET=0,
    POST,
    PUT,
    DELETE,
    OPTIONS,
};

class EspRESTResp {
    public:
        EspRESTResp (int, const char *);
        int status_code = -1;
        json11::Json responce;
};

class EspRESTClient {
    private:
        WiFiClientSecure wclient;
        String host;
        String uri_base;
        EspRESTResp send (E_Method, String);
        EspRESTResp send (E_Method, String, String);

    public:
        EspRESTClient (String);
        EspRESTResp get (String);
        EspRESTResp post (String, json11::Json *);
        EspRESTResp put (String, json11::Json *);
        EspRESTResp del (String);
};

#endif
