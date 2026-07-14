<p align="center">
  <img src="res/Header%20RoPilot.png" alt="RoPilot Header" width="100%">
</p>

RoPilot is a fun little side project I put together mostly through vibe coding! It's designed to be a modern, lightweight, and sleek alternative to the Roblox Account Manager (RAM).

And yes, this project is **100% open-source**! Feel free to dig into the code, fork it, modify it, or just use it as is.

## Screenshots
<p align="center">
  <img src="res/Preview%201.png" width="49%">
  <img src="res/Preview%202.png" width="49%">
</p>

## What is it?
At its core, RoPilot is a multi-instance account manager for Roblox, built entirely in C++. Basically, it lets you safely store your accounts, organize them, and launch as many of them as you want at the exact same time as your PC can handle!

### Why use RoPilot?
- **True Multi-Instancing:** Play on multiple accounts simultaneously! It bypasses Roblox's single-client limit natively, meaning you can launch as many accounts as your PC can physically handle.
- **Smart Account Management:** Your accounts are safely encrypted and saved locally. It features auto-backups to prevent data loss and even checks if your cookies are expired in the background.
- **Utility Dashboard:** A super handy hub where you can check account details (like age and Premium status), change your Display Name, swap outfits in seconds, and many more!
- **Advanced Launching:** Launch directly into specific games using a Place ID or a Private Server link. You can also force Roblox to join low-player servers or boot up with the lowest graphics settings to save your computer's resources.
- **Social Manager:** See your entire friend list and clean it up quickly with the Bulk Unfriend tool.
- **Instant Kill Switch:** Need to close all your accounts immediately? Press one button, and it forcefully terminates every active Roblox process in a split second.
- **Group Sorting:** Keep your workspace clean by organizing your accounts into custom folders (like "Mains", "Alts", "Bots").
- **Privacy Mode:** Streaming or recording? Toggle this on to censor your Username and User ID from the interface.
- **Modern & Snappy UI:** Built on WebView2, the interface is super smooth. It features smooth animations, Dark & Light modes, English & Indonesian language support, custom UI scaling, and custom fonts.
- **Resource Optimizer:** Automatically limits the CPU usage of Roblox clients running in the background so your PC doesn't catch on fire.

## How to Get Started
1. Grab the latest version from the [Releases page](../../releases/latest).
2. Extract the ZIP file into a folder on your PC.
3. Run `RoPilot.exe` to open the app.
4. **Important:** Make sure `WebView2Loader.dll` stays in the exact same folder as `RoPilot.exe`, or the app simply won't open!

## Adding Your First Account
1. Click the **Add Account** button.
2. Paste your `.ROBLOSECURITY` cookie, or just log in directly through the built-in browser.
3. You're done! RoPilot will instantly fetch your username, display name, and avatar.

## Compiling it Yourself
If you're a developer and want to build it from scratch:
1. Make sure you have Visual Studio 2022 (or 2026) Build Tools installed, specifically the "Desktop development with C++" workload.
2. Open the Developer Command Prompt.
3. Run `build.bat` and let it do its thing.

## VirusTotal & False Positives
You can check out the latest VirusTotal scan for the executable here: **[VirusTotal Scan Results](https://www.virustotal.com/gui/file/832cd335e6caa462b8e88ba942974cbb94ba35d716b4127380b74f44b846526f)**

Some antiviruses (especially those relying on AI or machine learning) might flag RoPilot as suspicious. Please don't panic, these are just **False Positives**! This happens because:
1. RoPilot is a brand new, unsigned executable (Code Signing Certificates are expensive!).
2. It interacts with other running processes on your computer to bypass Roblox's limits and optimize your CPU usage. 
3. It uses local encryption (like DPAPI and AES-GCM) to keep your account cookies safe, which is a technique sometimes used by actual malware, confusing the AI.

Since the whole project is open-source, you can always read the code yourself or compile it on your own machine for total peace of mind.

## Credits
This project wouldn't exist without the original [Roblox Account Manager (RAM)](https://github.com/ic3w0lf22/Roblox-Account-Manager/). Huge thanks to ic3w0lf22 and all their contributors for the massive inspiration!

## Contact & Support
If you run into any weird bugs, have feature ideas, or just want to chat, feel free to reach out!

**Discord:** `vxxn.`