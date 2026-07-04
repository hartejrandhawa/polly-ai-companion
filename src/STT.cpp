#include "../include/polly/STT.h"
#include "../include/polly/SpeechRecognizer.h"
#include <iostream>
#include <fstream>
#include <vector>
#include "portaudio.h"
#include <curl/curl.h>
#include <limits>
#include "nlohmann/json.hpp"

const int STT::kSampleRate = 44100;
const int STT::kFramesPerBuffer = 256;
const int STT::kNumChannels = 1;
const int STT::kRecordTimeSecs = 5;
const std::string apikey = "AIzaSyC7nOfOCovoNDIT8bzIdFh_X9n0lSWdnt4";

// Define the WAV file header structure
struct WAVHeader {
    char riffHeader[4] = {'R', 'I', 'F', 'F'};
    uint32_t riffChunkSize;
    char waveHeader[4] = {'W', 'A', 'V', 'E'};
    char fmtHeader[4] = {'f', 'm', 't', ' '};
    uint32_t fmtChunkSize = 16; // for PCM
    uint16_t audioFormat = 1;   // PCM = 1
    uint16_t numChannels;
    uint32_t sampleRate;
    uint32_t byteRate;
    uint16_t blockAlign;
    uint16_t bitsPerSample;
    char dataHeader[4] = {'d', 'a', 't', 'a'};
    uint32_t dataChunkSize;
};

// Write WAV header to a file
void WriteWAVHeader(std::ofstream &file, int numChannels, int sampleRate, int bitsPerSample, int dataSize) {
  WAVHeader wavHeader;
  wavHeader.numChannels = numChannels;
  wavHeader.sampleRate = sampleRate;
  wavHeader.bitsPerSample = bitsPerSample;
  wavHeader.byteRate = sampleRate * numChannels * bitsPerSample / 8;
  wavHeader.blockAlign = numChannels * bitsPerSample / 8;
  wavHeader.dataChunkSize = dataSize;
  wavHeader.riffChunkSize = 36 + dataSize;

  file.write(reinterpret_cast<const char *>(&wavHeader), sizeof(WAVHeader));
}

// Helper function to convert float to 16-bit PCM
int16_t FloatToPCM16(float sample) {
  return static_cast<int16_t>(std::max(-1.0f, std::min(1.0f, sample)) * std::numeric_limits<int16_t>::max());
}

void SaveAudioToFile(const std
::vector<int16_t> &buffer, const std::string &filename) {
  std::ofstream file(filename, std::ios::binary);

  int numChannels = 1;
  int sampleRate = 44100;
  int bitsPerSample = 16;
  int dataSize = buffer.size() * sizeof(int16_t);

  // Write the WAV file header
  WriteWAVHeader(file, numChannels, sampleRate, bitsPerSample, dataSize);
  file.close();
}


void STT::ListAudioDevices() {
  int numDevices = Pa_GetDeviceCount();
  if (numDevices < 1) {
    std::cerr << "No audio devices found!" << std::endl;
    return;
  }
  std::cout << "Available audio devices:" << std::endl;
  for (int i = 0; i < numDevices; i++) {
    const PaDeviceInfo *deviceInfo = Pa_GetDeviceInfo(i);
    std::cout << "Device " << i << ": " << deviceInfo->name << std::endl;
  }
}


// Function to record audio from default input device for a specified duration
std::vector<int16_t> STT::RecordAudio(int recordTimeSecs) {
  // Initialize PortAudio
  PaError err = Pa_Initialize();

  if (err != paNoError) throw std::runtime_error("PortAudio initialization failed");

  PaStreamParameters inputParameters;
  memset(&inputParameters, 0, sizeof(inputParameters)); // Use `memset` to clear the structure
  inputParameters.device = Pa_GetDefaultInputDevice();
  inputParameters.channelCount = STT::kNumChannels;
  inputParameters.sampleFormat = paFloat32;
  inputParameters.suggestedLatency = Pa_GetDeviceInfo(inputParameters.device)->defaultLowInputLatency;
  inputParameters.hostApiSpecificStreamInfo = nullptr;

  PaStream *stream;
  err = Pa_OpenStream(&stream, &inputParameters, nullptr, STT::kSampleRate, STT::kFramesPerBuffer, paClipOff, nullptr, nullptr);
  if (err != paNoError) throw std::runtime_error("Failed to open stream");

  err = Pa_StartStream(stream);
  if (err != paNoError) throw std::runtime_error("Failed to start stream");

  int totalFramesToRecord = STT::kSampleRate * recordTimeSecs;
  std::vector<float> floatBuffer(totalFramesToRecord);
  std::vector<int16_t> pcmBuffer(totalFramesToRecord);

  // Record loop
  int framesLeftToRecord = totalFramesToRecord;
  while (framesLeftToRecord > 0) {
    int framesToRead = std::min(STT::kFramesPerBuffer, framesLeftToRecord);
    err = Pa_ReadStream(stream, &floatBuffer[totalFramesToRecord - framesLeftToRecord], framesToRead);
    if (err != paNoError) throw std::runtime_error("Failed to read stream");

    framesLeftToRecord -= framesToRead;


  }

  err = Pa_CloseStream(stream);
  if (err != paNoError) throw std::runtime_error("Failed to close stream");

  Pa_Terminate();


  // Convert float buffer to 16-bit PCM
  std::transform(floatBuffer.begin(), floatBuffer.end(), pcmBuffer.begin(), FloatToPCM16);

  SaveAudioToFile(pcmBuffer, "recorded_audio.wav");
  return pcmBuffer;
}

std::string STT::run() {
    std::string transcribedText;

    PaError err = Pa_Initialize();
    curl_global_init(CURL_GLOBAL_ALL);
    if (err != paNoError) {
        std::cerr << "PortAudio initialization failed: " << Pa_GetErrorText(err) << std::endl;
        return "Error: PortAudio initialization failed";
    }

    try {
        SpeechRecognizer recognizer(apikey);
        ListAudioDevices();
        std::cout << "Press ENTER to start recording..." << std::endl;
        std::cin.get();

        auto audioBuffer = RecordAudio(kRecordTimeSecs);
        auto response = recognizer.performSpeechRecognition(audioBuffer);
        try {
            transcribedText = recognizer.getTranscribedText(response);
            std::cout << "Transcribed Text: " << transcribedText << std::endl;
        } catch (const std::exception &e) {
            std::cerr << "Error processing transcription: " << e.what() << std::endl;
            transcribedText = "Error processing transcription";
        }
        std::cout << "Full API Response: " << response << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
        transcribedText = "Error: An error occurred during speech recognition";
    }
    Pa_Terminate();
    curl_global_cleanup();

    return transcribedText;
}

