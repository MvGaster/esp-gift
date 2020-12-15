# esp-gift
Source code for a esp8266 project which shows a text and an animated gif on a TFT display

Bill of materials 

- 1x 18650 Batterijhouder
- Samsung 18650 Li-ion Batterij - 2600mAh
- JST-PH 2.0mm Connector met Kabel 
- Wemos Li-ion Batterij Shield voor D1 Mini
- Wemos D1 Mini V3 - ESP8266 - CH340
- Wemos 2.4 inch TFT Shield voor D1 Mini - Met Touchscreen


How to develop your own (variant) on a Macbook:

1. Start with installing Visual Studio Code
2. Next install the following extension: "PlatformIO IDE"
3. You might have to restart vscode after installing the extension
4. Next checkout this git repo
5. You should now be able to do a build of this code by going to PLATFORMIO Project Tasks and selecting "Build"
6. Of course you would like to deploy the build on the device as well. For that you must connect the device (including the TFT shield and preferrably without the charging shield) to you mac with a micro usb cable
7. After you connect the device to your mac over usb, the device turns on
8. Now you can press the "Upload and Monitor" button in vscode at the same spot as you did the Build thing before
9. You should see some progress in the terminal window of vscode. After less than a minute, your esp is updated with your changes

Tips:
- After you have made some changes on the code, you can just press upload. If there are code changes, the upload will do an (incremental) build first.
- You can find more info about PlatformIO and the vscode extension here: https://docs.platformio.org/en/latest/
- More info regarding the board used: https://docs.platformio.org/en/latest/boards/espressif8266/d1_mini.html
- And on the same spot more info over the ESP 8266: https://docs.platformio.org/en/latest/platforms/espressif8266.html#platform-espressif8266

