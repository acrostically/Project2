# Setup Guide
1. Make a copy of `include/credentials.h.template` and rename it to `credentials.h`.  
2. Change the credentials in the newly made `credentials.h` to be correct for the network you want to connect to.

# Running the Program
Firstly make sure you have PlatformIO installed (can be found [here](https://platformio.org/install/cli)). If you are on a Windows machine also make sure you have the correct drivers installed (most likely [these](https://www.silabs.com/developer-tools/usb-to-uart-bridge-vcp-drivers?tab=downloads)).  
Then connect your ESP to your machine and simply either choose `Upload and Monitor` from the PlatformIO functions or run `pio run -t upload -t monitor` from your terminal.  
Once it's done uploading simply look at the provided IP address in the terminal output and connect to it using your favorite browser on any device!