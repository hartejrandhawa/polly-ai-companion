# SpeechtoText Program README

## Introduction

SpeechtoText is a versatile audio transcription program that records audio from the microphone and converts it to text using Google's Speech-to-Text API. Initially developed and tested on a MacBook with an M1 Pro chip and macOS Sonoma, the application is designed to be cross-platform, compatible with various operating systems.

## Prerequisites

Before building and running SpeechtoText, ensure you have the following prerequisites installed:

- **CMake**: A cross-platform build system.
- **Make**: A build automation tool.

### Installing CMake and Make on macOS

To install CMake and Make on macOS, use Homebrew, a package manager for macOS. If you don’t have Homebrew installed, visit [Homebrew's website](https://brew.sh/) for installation instructions.

Install CMake:

```bash
brew install cmake
```

Install Make:

```bash
brew install make
```

## Building and Running the Program

Navigate to the project's root directory in your terminal, and use the following commands to build and run the SpeechtoText program:

1. Generate build files with CMake:

   ```bash

   cmake .

   ```

2. Compile the program using Make:

   ```bash

   make

   ```

3. Run the executable:

   ```bash

   ./SpeechtoText

   ```

## Usage Instructions

Upon running SpeechtoText, the program will prompt you to hit ENTER to start recording. The application records audio for 5 seconds and then automatically saves the recording as a `.wav` file in the same directory as the program.

You can listen to the recorded audio using any standard audio player to verify the recording quality. The program then processes the recorded audio, converting it to text using Google's Speech-to-Text API. The transcribed text is displayed in the console.

### Console Outputs

- The program outputs the API response to the console, allowing you to see the interaction with Google's API.
- Error messages and status updates are also printed to the console for debugging and information purposes.

## Features

- **Cross-Platform Compatibility**: Works on macOS, Windows, and Linux.
- **Audio Recording**: Records audio from the default input device.
- **Audio File Saving**: Saves recorded audio in WAV format.
- **Speech-to-Text Conversion**: Uses Google Cloud's Speech-to-Text API for accurate transcription.
- **Console Outputs**: Displays API calls and program status.

## Future Enhancements

- **Configurable Recording Duration**: Allow users to set the recording duration.
- **GUI Implementation**: Develop a user-friendly graphical interface.
- **Enhanced Error Handling**: Improve error messages for better user guidance.
- **Support for Additional Audio Formats**: Enable processing of different audio file formats.

## Acknowledgments

A special thanks to the open-source community and the developers of the tools and libraries used in this project. Your contributions make projects like this possible.

---

End of README.
