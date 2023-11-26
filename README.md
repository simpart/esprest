# ESP32 REST Client Library for Arduino

This is a REST client library designed for ESP32 devices running on the Arduino platform. The library provides functionality to interact with RESTful web services, supporting methods like GET and POST. Responses are obtained in the `json11::Json` format.

## Features

- Designed for ESP32 on Arduino
- Supports only `application/json` format
- Retrieve responses in `json11::Json` format by calling methods like GET and POST
- Requires prior integration of the [json11 library](https://github.com/dropbox/json11)

## Usage

1. **Install json11 Library:**
   Before using this REST client library, make sure to integrate the [json11 library](https://github.com/dropbox/json11) into your Arduino project.

2. **Include the Library:**
   Include the ESP32 REST Client Library in your Arduino sketch.

```
#include <Arduino.h>
#include <WiFi.h>
#include "EspREST.hpp"

EspRESTClient rest = EspRESTClient("https://api.example.com/");

void setup() {
    try {
        Serial.begin(115200);
        
        WiFi.begin("wifi_ssid", "wifi_pass");
        while (WiFi.status() != WL_CONNECTED) {
            delay(100);
            Serial.print(".");
        }
        Serial.println("wifi connected");
        
        EspRESTResp resp = rest.get("/rest/v1.1/account");  // GET sample
        Serial.println(resp.status_code);
        Serial.println(resp.responce.dump().c_str());       // responce is json11::Json object
    } catch (...) {
        Serial.println("setup error");
    }
    
}

void loop() {
  delay(100);
}
```


## Contribution
Feel free to contribute by submitting bug reports, feature requests, or pull requests. Your feedback is highly appreciated!

## License
MIT LIcense
