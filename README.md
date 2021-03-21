## Instructions to get software up and running

1. Go [here](https://krpc.github.io/krpc/getting-started.html) to get started with KRPC and install the mod. The easiest way is to install CPAK from [here](https://forum.kerbalspaceprogram.com/index.php?/topic/90246-the-comprehensive-kerbal-archive-network-ckan-package-manager-v1180-19-june-2016/) and search for KRPC. CPAK will handle the install.

2. After opening controller.ino in the Arduino IDE, go to *Tools* -> *Manage Libraries* -> Search for "krpc" and install.

3. After you upload the file to your Arduino, go to *Tools* -> *Serial Monitor* and wait until it prints out some junk and "Arduino Repeater." **Close the Serial Monitor**. **Note:** I have no idea why I need to do this, but I know the Arduino will refuse to talk to the server in Kerbal. Feel free to try this out for yourself and see if you can replicate the bug. Nothing will crash, it'll just not connect.

4. Go to Kerbal (it can be already be open or you can open it now, it doesn't matter) load up an aircraft and load onto the ramp or launchpad. If you don't already have the KRPC window open, you can click on the little three computer icon on the right to open it.

5. Click *Edit*, change the protocol to *Protobuf over SerialIO*, and select the port that your Arduino is connected to. Everything after that should remain unchanged.

6. Click *Save*.

7. Click *Show Advanced Settings* and enable *Auto-accept new clients.*

8. Click *Start*. A successful connection will be indicated by a green light on the Server panel and some activity on the amber status lights on your Arduino.

### Notes

- You don't have to restart the game every time you make an update to the Arduino. Simply stop the server, push the update, open the Serial Monitor, and start the server.

### Todo

- [ ] Fix the stupid Serial Monitor hack. It's probably some logging in the mod that's messing it up.
- [ ] Get the screen working
- [ ] Figure out how to work with multiple setting inputs
- [ ] Fix reaction wheels setting, KRPC docs say it's boolean, but in practice looks like a multiple choice.
