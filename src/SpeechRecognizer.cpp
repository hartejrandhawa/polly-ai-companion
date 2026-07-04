#include "../include/polly/SpeechRecognizer.h"
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/buffer.h>
#include <nlohmann/json.hpp>
#include <iostream>
#include <string>
#include <memory>

// Constructor
SpeechRecognizer::SpeechRecognizer(const std::string &apiKey) : apiKey(apiKey) {}

// This function will be used by curl to write the response from the Google Cloud Speech API
size_t SpeechRecognizer::curlWriteCallback(void *contents, size_t size, size_t nmemb, std::string *s) {
  size_t newLength = size * nmemb;
  try {
    s->append((char *) contents, newLength);
    return newLength;
  } catch (std::bad_alloc &e) {
    std::cerr << "Memory allocation error in curlWriteCallback: " << e.what() << std::endl;
    return 0;
  }
}

std::string SpeechRecognizer::base64Encode(const std::vector<int16_t> &buffer) {
  BIO *bio, *b64;
  BUF_MEM *bufferPtr;

  b64 = BIO_new(BIO_f_base64());
  bio = BIO_new(BIO_s_mem());
  bio = BIO_push(b64, bio);

  BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL); // Ignore newlines - write everything in one line
  BIO_write(bio, buffer.data(), buffer.size() * sizeof(int16_t));
  BIO_flush(bio);
  BIO_get_mem_ptr(bio, &bufferPtr);
  BIO_set_close(bio, BIO_NOCLOSE);

  // Copy memory buffer
  std::string encodedAudio(bufferPtr->data, bufferPtr->length);

  BIO_free_all(bio);
  BUF_MEM_free(bufferPtr);

  return encodedAudio;
}

// This function will perform speech recognition using the Google Cloud Speech API
std::string SpeechRecognizer::performSpeechRecognition(std::vector<int16_t> audioBuffer) {
  CURL *curl = curl_easy_init();
  if (!curl) {
    std::cerr << "Failed to initialize CURL." << std::endl;
    throw std::runtime_error("CURL initialization failed.");
  }

  std::string readBuffer;
  std::string encodedAudio = base64Encode(audioBuffer);
  std::string jsonPayload =
          R"({"config":{"encoding":"LINEAR16","sampleRateHertz":44100,"languageCode":"en-US"},"audio":{"content":")" +
          encodedAudio + R"("}})";
  std::string requestUrl = "https://speech.googleapis.com/v1p1beta1/speech:recognize?key=" + apiKey;

  try {
    struct curl_slist *headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: application/json");

    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_URL, requestUrl.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonPayload.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, -1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curlWriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
      std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
    }
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
  } catch (...) {
    curl_easy_cleanup(curl);
    throw;
  }

  return readBuffer;
}

// This function will parse the JSON response and get the transcribed text from it
std::string SpeechRecognizer::getTranscribedText(const std::string &jsonResponse) {
  try {
    nlohmann::json parsedJson = nlohmann::json::parse(jsonResponse);
    if (parsedJson.contains("results") && !parsedJson["results"].empty()) {
      auto &alternatives = parsedJson["results"][0]["alternatives"];
      if (!alternatives.empty() && alternatives[0].contains("transcript")) {
        return alternatives[0]["transcript"].get<std::string>();
      }
    }
    throw std::runtime_error("No transcription available in the response.");
  } catch (nlohmann::json::exception &e) {
    std::cerr << "Error parsing JSON response: " << e.what() << std::endl;
    throw;
  }
}
