# Mirakuru
###### _Control HomeEasy RF sockets from Alexa_


Use a cheap ESP8266 board to control a multitude of RF sockets. Uses a 433MHz transmitter module. I will upload a schematic in the near future.

If you plan to use this yourself you will have to read your remotes transmissions.  I used a 433MHz receiver module hooked up to a cheap USB logic analyser. Decoding them by hand isn't too bad once you notice the patterns. I may look at creating a decoder script in the future but it's not a necessity.


## Dependencies
You will need your Arduino IDE setup for writing to ESP8266 boards. You will need to make sure you have:
* [fauxmoESP](https://github.com/simap/fauxmoesp)
* [ESPAsyncTCP](https://github.com/me-no-dev/ESPAsyncTCP)

If you want to try getting this to work on an ESP32 board you will need:
* [AsyncTCP](https://github.com/me-no-dev/AsyncTCP)
Let me know if you get it to communicate with Alexa!


## License
fauxmoESP is released by Xose Pérez under the MIT license as follows:

Copyright (C) 2016-2018 by Xose Pérez

The MIT License (MIT)

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.