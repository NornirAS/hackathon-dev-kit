#include <Arduino.h>
#include <WiFi.h>
#include <TFT_eSPI.h>
#include <ArduinoWebsockets.h>
#include <ArduinoJson.h>

const char* WIFI_SSID = "Nornir HIVE";
const char* WIFI_PASS = "TallinOslo1";

using namespace websockets;

WebsocketsClient ws_channel;

const String token = "aToken_ccd2cc6b315eee220dc6e908d2030d47e52e5be92e49c46e1b09ae8799faa645";
const String ws_hostname = "wss://websocket.oslohive.com/";
const String ws_service = "bjorn/testhenrik";
const String ws_ghost_id = "1";
const String ws_action = "/channel";
const String ws_url = ws_hostname + ws_service + "/" + ws_ghost_id +  ws_action;

const char cert[] PROGMEM = \
"-----BEGIN CERTIFICATE-----\n" \
"MIIFIzCCBAugAwIBAgISAwhA2f2dNcYD+dx1gk3mY1T7MA0GCSqGSIb3DQEBCwUA\n" \
"MDIxCzAJBgNVBAYTAlVTMRYwFAYDVQQKEw1MZXQncyBFbmNyeXB0MQswCQYDVQQD\n" \
"EwJSMzAeFw0yMjExMTEwODU4MzFaFw0yMzAyMDkwODU4MzBaMBkxFzAVBgNVBAMM\n" \
"Dioub3Nsb2hpdmUuY29tMIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEA\n" \
"wBKIbZFRh1mv4MpImozZpsSvJEDBazqJ1KkjSPj6JU3ep7lp1iyk28YV5WtAu252\n" \
"TRUN9iW2RhmnPc54PlIaXy0Ptnz3tuEGHOpb9ictbG3O28FjMVPXI6Op9xsu1knE\n" \
"a7KvqsqFYHQsnFH9xa0rPW3gseCUYFSkuz21TkHeptjuk3w39Brl9x6BOxd4LLod\n" \
"7dHXrhy0dOPrNUd7YTlGzC4O5IwAbRwtmIbFF/pV/xTEmTGJvzIWFEB8DbN5EHXT\n" \
"BckoUzyRa7O53KVCIE2/KI8wuLntdc6L+lQz6hLIFtTl5OrsbLI0QwbvVueYxflp\n" \
"Lp5g7xImHupRmqZR3dwLFQIDAQABo4ICSjCCAkYwDgYDVR0PAQH/BAQDAgWgMB0G\n" \
"A1UdJQQWMBQGCCsGAQUFBwMBBggrBgEFBQcDAjAMBgNVHRMBAf8EAjAAMB0GA1Ud\n" \
"DgQWBBTfgTmcgnhiqkz/Oz5coFwOwOpSIjAfBgNVHSMEGDAWgBQULrMXt1hWy65Q\n" \
"CUDmH6+dixTCxjBVBggrBgEFBQcBAQRJMEcwIQYIKwYBBQUHMAGGFWh0dHA6Ly9y\n" \
"My5vLmxlbmNyLm9yZzAiBggrBgEFBQcwAoYWaHR0cDovL3IzLmkubGVuY3Iub3Jn\n" \
"LzAZBgNVHREEEjAQgg4qLm9zbG9oaXZlLmNvbTBMBgNVHSAERTBDMAgGBmeBDAEC\n" \
"ATA3BgsrBgEEAYLfEwEBATAoMCYGCCsGAQUFBwIBFhpodHRwOi8vY3BzLmxldHNl\n" \
"bmNyeXB0Lm9yZzCCAQUGCisGAQQB1nkCBAIEgfYEgfMA8QB3AHoyjFTYty22IOo4\n" \
"4FIe6YQWcDIThU070ivBOlejUutSAAABhGYgar0AAAQDAEgwRgIhAObOASLKervh\n" \
"/7/qnhjbqRmUpF3xeL0vmlcXaeBTYyDwAiEAwqhni62ufYHVo++0LD6/GocJUiWM\n" \
"INua9vOWiygzYhcAdgCt9776fP8QyIudPZwePhhqtGcpXc+xDCTKhYY069yCigAA\n" \
"AYRmIGrBAAAEAwBHMEUCIQDWw4rM9LrR3phn34OAfsUtTf5VG3G+PAVpJDNC+uXi\n" \
"xAIgGJfb/xs5qc2nka1Dl8r7jqyIBjbruVTezzenLkZ4KbwwDQYJKoZIhvcNAQEL\n" \
"BQADggEBAJjV4Or17dKzVCfW1CfbhnH1cFWDfQLL3mYsPnocLHOi+ZZj+wItIq/Y\n" \
"o065DcD7plNUqu2S/iRXpP26m/H7fxd0aW6LHmvRa9WUkjHWO9vCoaEBZWkBVdno\n" \
"+z1KcIuKXYF86wYoXm7TCio6ofyoNvWS5taM7pRPfn53ddqonndaMPF+6ZLiv6si\n" \
"k6MKGDuUJxVCZTOS/J/C4NGTmPq2AWK+OFNJL5fWAj7Z3KnAWsSb8IAoGX2Dz2M3\n" \
"amF/r9GdaLKSC9A2YauLFVtJHzHo2LNqZ2QbCzjFeybxdXC7cT1Q3d51riO5qYHF\n" \
"9LQ233cttSdmerhSJ7STe3t3/Lv0J24=\n" \
"-----END CERTIFICATE-----\n";

// ----------------------------------------------------------------------------
// Connecting to the WiFi network
// ----------------------------------------------------------------------------

void initWiFi() {
    Serial.print("Connecting to ");
    Serial.println(WIFI_SSID);

    WiFi.begin(WIFI_SSID, WIFI_PASS);

    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }

    Serial.println();
    Serial.println("WiFi Connected!");
    Serial.println(WiFi.localIP().toString().c_str());

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
    message["NAME"] = "HELLO";
    message["TESTID"] = "123";
    String payload = "";
    serializeJson(message, payload);
    ws_channel.send(payload);
}

// ----------------------------------------------------------------------------
// Handle incoming messages
// ----------------------------------------------------------------------------

void handleReceivedMessage(String message) {
    DynamicJsonDocument doc(1024);
    DeserializationError error = deserializeJson(doc, message);
    if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return;
    }
    // To access data schem doc["RTW"]
    // Access data elements doc["RTW"]["elementName"]
    String name = doc["RTW"]["NAME"];
    String id = doc["RTW"]["TESTID"];
    Serial.println(name + " - " + id);
}

// ----------------------------------------------------------------------------
// Handle incoming events
// ----------------------------------------------------------------------------

void onEventsCallback(WebsocketsEvent event, String data) {
    if(event == WebsocketsEvent::ConnectionOpened) {
        Serial.println("Connnection Opened");
    } else if(event == WebsocketsEvent::ConnectionClosed) {
        Serial.println("Connnection Closed");
    } else if(event == WebsocketsEvent::GotPing) {
        Serial.println("Got a Ping!");
    } else if(event == WebsocketsEvent::GotPong) {
        Serial.println("Got a Pong!");
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
        Serial.println("Connected!");
        sendToken();
        Serial.println("Token sendt!");
        delay(1000);
        sendServiceURL();
    } else {
        Serial.println("Not Connected!");
        Serial.println("Please restart the device");
    }
}

void setup() {
    Serial.begin(115200);
    delay(1000);

    Serial.println("Connecting to");
    Serial.println(WIFI_SSID);

    initWiFi();
    delay(1000);

    initWSChannel();
}

void loop() {
    // let the websockets client check for incoming messages
    if(ws_channel.available()) {
        ws_channel.poll();
    }
    
    sendData();
    delay(1000);
}