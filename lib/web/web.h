#ifndef WEB_H
#define WEB_H
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "main.h"

ESP8266WebServer server(80);



void handleRoot() {
    String html = "<html><body>";
    html += "<h1>Current Values</h1>";
    html += "<p>Reference Heading: <span id='referenceHeading'>" + String(referenceHeading) + "</span></p>";
    html += "<p>Current Heading: <span id='currentHeading'>" + String(currentHeading) + "</span></p>";
    html += "<p>Heading: <span id='heading'>" + String(heading) + "</span></p>";
    html += "<p>Error: <span id='error'>" + String(error) + "</span></p>";
    html += "<p>Position: <span id='position'>" + String(position) + "</span></p>";
    html += "<script>";
    html += "function fetchData() {";
    html += "  fetch('/data').then(response => response.json()).then(data => {";
    html += "    document.getElementById('referenceHeading').innerText = data.referenceHeading;";
    html += "    document.getElementById('currentHeading').innerText = data.currentHeading;";
    html += "    document.getElementById('heading').innerText = data.heading;";
    html += "    document.getElementById('error').innerText = data.error;";
    html += "    document.getElementById('position').innerText = data.position;";
    html += "  });";
    html += "}";
    html += "setInterval(fetchData, 1000);"; // Fetch data every second
    html += "</script>";
    html += "</body></html>";
    server.send(200, "text/html", html);
}

void handleData() {
    String json = "{";
    json += "\"referenceHeading\":" + String(referenceHeading) + ",";
    json += "\"currentHeading\":" + String(currentHeading) + ",";
    json += "\"heading\":" + String(heading) + ",";
    json += "\"error\":" + String(error) + ",";
    json += "\"position\":" + String(position);
    json += "}";
    server.send(200, "application/json", json);
}

void setupWebServer() {
    WiFi.softAP("ESP8266_AP", "password123");
    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(IP);

    server.on("/", handleRoot);
    server.on("/data", handleData);
    server.begin();
    Serial.println("HTTP server started");
}

#endif // WEB_H