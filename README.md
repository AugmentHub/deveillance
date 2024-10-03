# Deveillance Devices
We started this project as we have seen an increase in potentially privacy invading devices, such as wearable AI necklace, AR glasses that can take pictures of you, and more. Throughout the 2024 Augmentation Residency, we - Aida Baradari, Treyden Chiaravalloti, and Nasibe Nur Dundar - were thinking about ways of building what we coin "deveillance" technologies. 

**Deveillance** means off-watching, and we define deveillance technologies as technologies that give the power to the individual as to what data is collected and what is not collected. This can take the form of digital data and data potentially collected in the real world. This is, fundamentally, to build the use of these technologies for good and protective measures rather than for malicious intentions. 

There is still a lot of work to be done and we need more engineers tackling the problems of data collection in all realms. We started with Audio but this is a hub for everyone to contribute to a better design of deveillance devices. 

## Audio
Audio Jammers have existed for a while now but has been increasingly difficult to find affordable ones or even good, structured tutorials of how to make your own. This is how we built an audio jammer and how this works.

We are using a motor driver - more specifically the H-Bridge - L298N in order to generate a square wave signal. We formally tried using a signal generator with an amplifier but found it more difficult to get a non-noisy signal. 
The generated square-wave signal is based on the input voltage, for which you can use a power supply for now but we will eventually add batteries that are high enough (to get a good power output, we are currently using the H-Bridge that go up to 30V and 2A) but when we make it more wearable and you want to use it on your shelf, you may want to use a DC-DC Converter, upping the voltage of a generic LiPo battery. 
