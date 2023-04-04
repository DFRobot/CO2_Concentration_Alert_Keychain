# Making a CO2 Concentration Alert Keychain with Infrared SCD41 Photoacoustic Sensor

![image](https://user-images.githubusercontent.com/65546290/229737238-968b89ab-94ac-4090-9764-59a306f3c7f5.png)


## Background 
Carbon dioxide emissions seem to be a major culprit of environmental pollution. Not only do they cause a decrease in air quality, but they can also lead to symptoms such as headaches, drowsiness, and decreased attention. I don't want to become a lazy panda due to poor indoor air quality, do you?

If you're like me and worried about your indoor environment quality, you may need a portable carbon dioxide detection device. A detection card made using an infrared light sound carbon dioxide sensor should be able to help us achieve this. This type of detection card can help us understand the concentration of carbon dioxide in the surrounding environment. That way, we can determine when to ventilate or go outside for fresh air. No more complaining about dizziness and blurred vision due to insufficient oxygen indoors! With this small device, we can always be aware of the indoor air quality.

[The relevant information is uploaded on our YouTube channel](https://www.youtube.com/watch?v=Bqu9s5eTiL8)

# HARDWARE LIST

* Beetle ESP32-C3
* Infrared carbon dioxide sensor
* 1.47" 172×320 IPS LCD Display
* 3.7V battery
* RGB LED strip
* 3D printed shell
* Acrylic board


# Connection Diagram
![image](https://user-images.githubusercontent.com/65546290/229738027-6a79a1a2-5528-49db-a717-e585434d8d59.png)


## ESP32-C3 -- Infrared Light Sound Carbon Dioxide Sensor

* 3.3V   -   VCC
* GND  -   GND
* 9        -    SCL
* 8        -    SDA

 

## ESP32-C3 -- 1.4 Inch High-definition Display

* 3.3V   -   VCC
* GND  -   GND
* 4        -   SCLK
* 6        -   MOSI
* 7        -    CS
* 2        -   RES
* 1        -   DC
* 10      -   BLK

 

## ESP32-C3 -- RGB LED Strip

* 3.3V   -   VCC
* GND  -   GND
* 20      -   DI


# Shell Design
To create a 3D shell, first measure the spatial layout and size of the hardware.I used Solidworks to draw the model, which consists of a 3D printed shell and an acrylic board cover, with reserved positions for the type-C interface, switch, and probe hole.
![image](https://user-images.githubusercontent.com/65546290/229737869-88eb43b4-f873-48b7-86a6-79cf256f3b53.png)
![image](https://user-images.githubusercontent.com/65546290/229738398-d8829470-9c1c-41a0-90e9-75267fccf133.png)





# Upload Program
Next, connect the device to the computer using a USB cable and upload the program.
In the program, the data read by the infrared light sound carbon dioxide sensor is displayed on the screen.
The trend line of changes in carbon dioxide concentration is displayed on the screen.
The number of LED beads/light strip breathing frequency is lit by the RGB LED strip to reflect the size of the carbon dioxide concentration.
![image](https://user-images.githubusercontent.com/65546290/229738765-64e1ed19-f3a7-4737-8c3c-9427f1bdb086.png)


# Data Collection
When you see the keychain light up red, remember to open the window for ventilation or go outside for fresh air.
![image](https://user-images.githubusercontent.com/65546290/229739094-ea2094ac-2c1d-494c-b5c7-1843a5821a00.png)


# Thank
Well, that's all for this project.Thanks for reading, feel free to leave your comments if you have any other good ideas!






