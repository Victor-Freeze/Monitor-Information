# Monitor Information
This application retrieves the current monitor resolution and refresh rate using two different methods for diagnostic purposes.

I have found that some applications read incorrect display resolution and refresh rate in Windows 11 (Version	10.0.26100 Build 26100).
This results, for example, in the Netflix application for Windows not letting you watch videos in 4K on a 4K Monitor.

This application retrieves the current monitor state information using the **DEVMODE** and **DirectX** APIs and can show which one reports incorrect data (the latter).
