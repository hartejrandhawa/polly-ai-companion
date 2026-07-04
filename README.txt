HHP%%#%%%%%%%%%%%%%%%%#%%%%%%%#%%VHH
dHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHb
██████╗  ██████╗ ██╗     ██╗  ██╗   ██╗
██╔══██╗██╔═══██╗██║     ██║  ╚██╗ ██╔╝
██████╔╝██║   ██║██║     ██║   ╚████╔╝
██╔═══╝ ██║   ██║██║     ██║    ╚██╔╝
██║     ╚██████╔╝███████╗███████╗██║
╚═╝      ╚═════╝ ╚══════╝╚══════╝╚═╝
dHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHb
HHP%%#%%%%%%%%%%%%%%%%#%%%%%%%#%%VHH
HH%%%%%%%%%%#%v~~~~~~\%%%#%%%%%%%%HH
HH%%%%%#%%%%v'        ~~~~\%%%%%#%HH
HH%%#%%%%%%v'dHHb      a%%%#%%%%%%HH
HH%%%%%#%%v'dHHHA     :%%%%%%#%%%%HH
HH%%%#%%%v' VHHHHaadHHb:%#%%%%%%%%HH
HH%%%%%#v'   `VHHHHHHHHb:%%%%%#%%%HH
HH%#%%%v'      `VHHHHHHH:%%%#%%#%%HH
HH%%%%%'        dHHHHHHH:%%#%%%%%%HH
HH%%#%%        dHHHHHHHH:%%%%%%#%%HH
HH%%%%%       dHHHHHHHHH:%%#%%%%%%HH
HH#%%%%       VHHHHHHHHH:%%%%%#%%%HH
HH%%%%#   b    HHHHHHHHV:%%%#%%%%#HH
HH%%%%%   Hb   HHHHHHHV'%%%%%%%%%%HH
HH%%#%%   HH  dHHHHHHV'%%%#%%%%%%%HH
HH%#%%%   VHbdHHHHHHV'#%%%%%%%%#%%HH
HHb%%#%    VHHHHHHHV'%%%%%#%%#%%%%HH
HHHHHHHb    VHHHHHHH:%odHHHHHHbo%dHH
HHHHHHHHboodboooooodHHHHHHHHHHHHHHHH
HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH
VHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHV


            _____
______________  /____  _________
__  ___/  _ \  __/  / / /__  __ \
_(__  )/  __/ /_ / /_/ /__  /_/ /
/____/ \___/\__/ \__,_/ _  .___/
                        /_/
First make sure you are on 64bit Fedora or Debian/Ubuntu based Linux distro
    (works best on CentOS 7 with the following versions)

Then install the following:

* python ( version 3.6.8 )
* g++ ( version 4.8.5 )
* flask ( version 2.0.3 )
* pytorch ( version 1.10.1+cu102 )
* transformers ( version 4.18.0 )
* curl ( version 7.29.0 )
* boost ( version 1.53 )
* espeak ( version 1.47.11 )
* openssl-devel ( version 1:1.0.2k-26.el7_9 )
* portaudio-devel ( version 19-16.el7 )
* lcurl ( version 7.29.0-59.el7_9.1 )
* crow ( version v0.1-29-g2b43d3c )
* node ( version v16.20.2 )
* npm ( version 8.19.4 )
* npx ( version 8.19.4 )

Please ensure all the above packages are added to PATH before proceeding.

       _____              _____
_________  /______ _________  /_
__  ___/  __/  __ `/_  ___/  __/
_(__  )/ /_ / /_/ /_  /   / /_
/____/ \__/ \__,_/ /_/    \__/
Open two terminals, turn your volume on, then enter these commands:

1. python3 llm_service.py (opens server..MUST KEEP IT RUNNING)
2. (in new terminal) g++ -o Polly Polly.cpp Display.cpp LLMProcessing.cpp TextToSpeech.cpp VoiceCustomization.cpp STT.cpp SpeechRecognizer.cpp main.cpp -std=c++11 -lcurl -lportaudio -lcrypto -lssl -lpthread -lboost_system -I. -I ./crow/include
3. (in the terminal you ran the g++ command) ./Polly (opens crow server .. ALSO MUST KEEP IT RUNNING)
4. change directory into webui folder and run the command: npx vite (keep this one running too)


to run use the following command from the root directory:

./polly.sh

_____            __________
__  /______________  /___(_)_____________ _
_  __/  _ \_  ___/  __/_  /__  __ \_  __ `/
/ /_ /  __/(__  )/ /_ _  / _  / / /  /_/ /
\__/ \___//____/ \__/ /_/  /_/ /_/_\__, /
                                  /____/
you type your message into the box and press "SEND" ... only press once or it will loop



CURRENT ISSUES:

-STT is not properly integrated..ideally there should be a button in the webUI that you will click and then it will start listening
-no animation for "love", maybe add heart emojis or <3 in the text section (or add another text section) and implement how the "THINKING" message is implemented
-LLM finetuning (play around with the paramaters in llm_service.py)



