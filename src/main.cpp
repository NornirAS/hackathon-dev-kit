#include <Arduino.h>
#include <WiFi.h>
#include <TFT_eSPI.h>
#include <ArduinoWebsockets.h>
#include <ArduinoJson.h>
// ----------------------------------------------------------------------------
// Configuration file
// ----------------------------------------------------------------------------

// TO DO : Customise your settings in the config file 

#include <config/AppConfig.h>  

// Display
TFT_eSPI tft = TFT_eSPI();

// TO DO : Add Values or sensors 

//Example : 

// String status = "LOCKED";

// ----------------------------------------------------------------------------
// Prepare token before send to Hive
// ----------------------------------------------------------------------------

String tokenToJsonString(String token) {
    StaticJsonDocument<256> doc;
    JsonObject token_json = doc.to<JsonObject>();
    token_json["token"] = token;
    String domain_token = "";
    serializeJson(token_json, domain_token);
    return domain_token;
}

// ----------------------------------------------------------------------------
// Display setup
// ----------------------------------------------------------------------------

void initDisplay() {
    tft.init();
    tft.setRotation(0);
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(1);
}

// ----------------------------------------------------------------------------
// Connecting to the WiFi network
// ----------------------------------------------------------------------------

void initWiFi() {
    tft.print("Connecting to ");
    tft.println(WIFI_SSID);

    WiFi.begin(WIFI_SSID, WIFI_PASS);

    while (WiFi.status() != WL_CONNECTED) {
        tft.print(".");
        delay(500);
    }
    tft.println();
    tft.println("WiFi Connected!");
    tft.println(WiFi.localIP().toString().c_str());

    delay(1000);
}

// ----------------------------------------------------------------------------
// Send token
// ----------------------------------------------------------------------------

void sendToken() {
    StaticJsonDocument<256> doc;
    JsonObject message = doc.to<JsonObject>();
    message["token"] = token;
    String payload = "";
    serializeJson(message, payload);
    ws_channel.send(payload);
}

// ----------------------------------------------------------------------------
// Send service url
// ----------------------------------------------------------------------------

void sendServiceURL() {
    StaticJsonDocument<256> doc;
    JsonObject message = doc.to<JsonObject>();
    message["url"] = ws_service + "/" + ws_ghost_id;
    String payload = "";
    serializeJson(message, payload);
    ws_channel.send(payload);
}

// ----------------------------------------------------------------------------
// Send data
// ----------------------------------------------------------------------------

void sendData() {
    StaticJsonDocument<256> doc;
    JsonObject message = doc.to<JsonObject>();

// TO DO : Send Data or sensor values. 

//Example :

//  message["STATUS"] = status;
//  String payload = "";
//  serializeJson(message, payload);
//  ws_channel.send(payload);

}

// ----------------------------------------------------------------------------
// Perform Actions 
// ----------------------------------------------------------------------------

// TO DO : Perform Actions if needed by your solution .

// Example :

// void do_lock() {
//     status = "LOCKED";
//     tft.fillScreen(TFT_RED);
//     sendData();
// }

// void do_unlock() {
//     status = "UNLOCKED";
//     tft.fillScreen(TFT_GREEN);
//     sendData();
//}

// ----------------------------------------------------------------------------
// Handle incoming messages
// ----------------------------------------------------------------------------

void handleReceivedMessage(String message) {
    DynamicJsonDocument doc(1024);
    DeserializationError error = deserializeJson(doc, message);
    if (error) {
        tft.print(F("deserializeJson() failed: "));
        tft.println(error.f_str());
        return;
    }

//To do : Perform actions when data is coming from the Hive.
// Example :

    // bool lock = doc["RTW"]["LOCK"].as<bool>();
    // if (lock) {
    //     do_lock();
    // }
    // bool unlock = doc["RTW"]["UNLOCK"].as<bool>();
    // if (unlock) {
    //     do_unlock();
    // }
}

// ----------------------------------------------------------------------------
// Handle incoming events
// ----------------------------------------------------------------------------

void onEventsCallback(WebsocketsEvent event, String data) {
    if(event == WebsocketsEvent::ConnectionOpened) {
        tft.println("Connnection Opened");
    } else if(event == WebsocketsEvent::ConnectionClosed) {
        tft.println("Connnection Closed");
    } else if(event == WebsocketsEvent::GotPing) {
        tft.println("Got a Ping!");
    } else if(event == WebsocketsEvent::GotPong) {
        tft.println("Got a Pong!");
    }
}

// ----------------------------------------------------------------------------
// Init WebSocket connection for reading
// ----------------------------------------------------------------------------

void initWSChannel() {
    ws_channel.onMessage([&](WebsocketsMessage message) {
        handleReceivedMessage(message.data());
    });
    ws_channel.onEvent(onEventsCallback);

    ws_channel.setCACert(cert);

    bool connected = ws_channel.connect(ws_url);
    if(connected) {
        tft.println("Connected!");
        sendToken();
        tft.println("Token sendt!");
        delay(1000);
        sendServiceURL();
    } else {
        tft.println("Not Connected!");
        tft.println("Please restart the device");
    }
}


void setup() {
    pinMode(0, INPUT_PULLUP);
    pinMode(35, INPUT);

    Serial.begin(115200);
    delay(1000);

    initDisplay();

    tft.println("Connecting to");
    tft.println(WIFI_SSID);

    initWiFi(); 
    delay(1000);

    initWSChannel();

    tft.fillScreen(TFT_RED);
}

void loop() {
    // let the websockets client check for incoming messages
    if(ws_channel.available()) {
        ws_channel.poll();
    }

// TO DO : Read Data from the sensors and perform actions   

//Example :

    // if(digitalRead(0)==0) {
    //     do_lock();
    // }
    // if(digitalRead(35)==0) {
    //     do_unlock();
    // }
    // delay(100);
}
