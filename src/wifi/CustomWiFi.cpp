#include "CustomWiFi.h"

CustomWiFi::CustomWiFi() {
  // this->pin = 13;
}

void CustomWiFi::setup() {
  Serial.println("CustomWiFi::setupWiFi");
  Serial3.begin(115200);
  WiFi.init(&Serial3);
  this->connect();

  String message = F("");
  message += F("Firmware ");
  message += getFirmwareVersion();
  message += F("  SSID ");
  message += this->getWiFiSSID();
  message += F("  Status ");
  message += this->getWiFiStatus();
  message += F("  MAC ");
  message += this->getMacAddress();
  message += F("  IP ");
  message += this->getIP();
  message += F("  Subnet Mask ");
  message += this->getSubnetMask();
  message += F("  Gateway ");
  message += this->getGatewayIP();
  message += F("  RSSI ");
  message += this->getRSSI();
  Serial.println(message);
}

void CustomWiFi::connect() {
  Serial.println("CustomWiFi::connect");
  Serial.print("Wi-Fi status: ");
  Serial.println(this->getWiFiStatus());
  char ssid[] = "";
  char pwd[] = "";
  if(WiFi.status() != WL_CONNECTED) {
    WiFi.begin(ssid, pwd);
  }
}

String CustomWiFi::getFirmwareVersion() {
  Serial.println(F("getFirmwareVersion()"));
  return WiFi.firmwareVersion();
}

String CustomWiFi::getWiFiStatus() {
  Serial.println(F("getWiFiStatus()"));
  uint8_t status = WiFi.status();
  if(status == WL_CONNECTED) {
    return F("Connected");
  } else if(status == WL_NO_SHIELD) {
    return F("No WiFi");
  } else if(status == WL_IDLE_STATUS) {
    return F("Idle");
  } /*else if(status == WL_NO_SSID_AVAIL) {
    return F("No SSID Available");
  } else if(status == WL_SCAN_COMPLETED) {
    return F("Scan Completed");
  } */else if(status == WL_CONNECT_FAILED) {
    return F("Connect Failed");
  } /*else if(status == WL_CONNECTION_LOST) {
    return F("Connection Lost");
  } */else if(status == WL_DISCONNECTED) {
    return F("Disconnected");
  } else {
    return F("Error");
  }
}

String CustomWiFi::getWiFiSSID() {
  Serial.println(F("getWiFiSSID()"));
  return WiFi.SSID();
}

String CustomWiFi::getMacAddress() {
  Serial.println(F("getMacAddress()"));
  byte mac[6]={0,0,0,0,0,0};
  WiFi.macAddress(mac);
  String result = F("");
  result += String(mac[5], HEX);
  result += F(":");
  result += String(mac[4], HEX);
  result += F(":");
  result += String(mac[3], HEX);
  result += F(":");
  result += String(mac[2], HEX);
  result += F(":");
  result += String(mac[1], HEX);
  result += F(":");
  result += String(mac[0], HEX);
  return result;
}

String CustomWiFi::getIP() {
  Serial.println(F("getIP()"));
  IPAddress ip = WiFi.localIP();
  String result = F("");
  result += ip[0];
  result += F(".");
  result += ip[1];
  result += F(".");
  result += ip[2];
  result += F(".");
  result += ip[3];
  return result;
}

String CustomWiFi::getSubnetMask() {
  Serial.println(F("getSubnetMask()"));
  IPAddress subnet = WiFi.subnetMask();
  String result = F("");
  result += subnet[0];
  result += F(".");
  result += subnet[1];
  result += F(".");
  result += subnet[2];
  result += F(".");
  result += subnet[3];
  return result;
}

String CustomWiFi::getGatewayIP() {
  Serial.println(F("getGatewayIP()"));
  IPAddress gateway = WiFi.gatewayIP();
  String result = F("");
  result += gateway[0];
  result += F(".");
  result += gateway[1];
  result += F(".");
  result += gateway[2];
  result += F(".");
  result += gateway[3];
  return result;
}

int32_t CustomWiFi::getRSSI() {
  Serial.println(F("getRSSI()"));
  return WiFi.RSSI();
}

String CustomWiFi::getNTPDate() {
  // TODO refactor
  Serial.println("DEBUG #1");
  String now = F("");
  // NTP
  // char timeServer[] = "time.nist.gov";
  char timeServer[] = "a.ntp.br";
  unsigned int localPort = 2390;
  const int NTP_PACKET_SIZE = 48;
  const int UDP_TIMEOUT = 2000;
  byte packetBuffer[NTP_PACKET_SIZE];
  WiFiEspUDP Udp;
  Udp.begin(localPort);
  Serial.println("DEBUG #2");
  sendNTPpacket(timeServer, packetBuffer, NTP_PACKET_SIZE, Udp);
  Serial.println("DEBUG #3");
  unsigned long startMs = millis();
  while (!Udp.available() && (millis() - startMs) < UDP_TIMEOUT) {Serial.println("DEBUG #3.1");
  }
  Serial.print("Packet=");
  Serial.println(Udp.parsePacket());
  Serial.println("DEBUG #4");
  if (Udp.parsePacket()) {
    Serial.println("packet received");
    // We've received a packet, read the data from it into the buffer
    Udp.read(packetBuffer, NTP_PACKET_SIZE);

    // the timestamp starts at byte 40 of the received packet and is four bytes,
    // or two words, long. First, esxtract the two words:

    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
    // combine the four bytes (two words) into a long integer
    // this is NTP time (seconds since Jan 1 1900):
    unsigned long secsSince1900 = highWord << 16 | lowWord;
    Serial.print("Seconds since Jan 1 1900 = ");
    Serial.println(secsSince1900);

    // now convert NTP time into everyday time:
    Serial.print("Unix time = ");
    // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
    const unsigned long seventyYears = 2208988800UL;
    // subtract seventy years:
    unsigned long epoch = secsSince1900 - seventyYears;
    // print Unix time:
    Serial.println(epoch);
    now += epoch;
    now += F("T");

    // print the hour, minute and second:
    Serial.print("The UTC time is ");       // UTC is the time at Greenwich Meridian (GMT)
    Serial.print((epoch  % 86400L) / 3600); // print the hour (86400 equals secs per day)
    now += (epoch  % 86400L) / 3600;
    now += F(":");
    Serial.print(':');
    if (((epoch % 3600) / 60) < 10) {
      // In the first 10 minutes of each hour, we'll want a leading '0'
      Serial.print('0');
      now += F("0");
    }
    Serial.print((epoch  % 3600) / 60); // print the minute (3600 equals secs per minute)
    now += (epoch  % 3600) / 60;
    now += F(":");
    Serial.print(':');
    if ((epoch % 60) < 10) {
      // In the first 10 seconds of each minute, we'll want a leading '0'
      Serial.print('0');
      now += F("0");
    }
    Serial.println(epoch % 60); // print the second
    now += epoch % 60;
    return now;
  }
  Serial.println("DEBUG #99");
  return now;
}

void CustomWiFi::sendNTPpacket(char *ntpSrv, byte packetBuffer[], int NTP_PACKET_SIZE, WiFiEspUDP Udp) {
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)

  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;

  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  Udp.beginPacket(ntpSrv, 123); //NTP requests are to port 123

  Udp.write(packetBuffer, NTP_PACKET_SIZE);

  Udp.endPacket();
}
