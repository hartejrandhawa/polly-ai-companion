// SAAD MAHMOOD

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "../include/polly/Display.h"
#include "crow.h"
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <thread> // Include for sleep

extern crow::websocket::connection *current_ws_conn;

void Display::pollyFeels(const std::string &emotion) {
  if (current_ws_conn) {
    crow::json::wvalue msg;
    msg["emotion"] = emotion;
    current_ws_conn->send_text(crow::json::dump(msg));
  }
}
