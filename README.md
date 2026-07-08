# RoPilot

This is just a fun little side project I put together mostly through vibe coding. I built this as a modern, lightweight, and sleek alternative to the Roblox Account Manager (RAM).

This project is **100% open source**! Feel free to explore the codebase, fork it for your own use, or simply enjoy it as it is.

## What does it do?
RoPilot is a C++ based multi-instance account manager for Roblox. Basically, it lets you store, organize, and launch multiple Roblox accounts at the exact same time without the annoying "same instance" error stopping you. 

### Key Features:
- **True Multi-Instancing:** Bypasses Roblox's single-client limit natively so you can run as many accounts as your PC can physically handle.
- **Account Management:** Securely save your accounts using their `.ROBLOSECURITY` cookies.
- **Group Sorting:** Neatly group your accounts together (e.g., "Mains", "Alts") for a cleaner workspace.
- **Instant Kill Switch:** Rapidly terminate every active Roblox process instantly via native Windows APIs.
- **Modern UI:** Built on a super lightweight WebView2 frontend. It features dark/light modes, customizable fonts, UI scaling, smooth animations, and toast notifications.
- **Resource Optimizer:** Limits background CPU usage of unfocused Roblox clients to save your system resources.

## How to Use
1. Grab the latest version from the [Releases page](../../releases/latest).
2. Extract the ZIP file to a safe folder on your computer.
3. Double-click on `RoPilot.exe` to open the app.
4. **Important:** Make sure `WebView2Loader.dll` stays in the exact same folder as `RoPilot.exe`, otherwise the user interface won't load!

## Adding Accounts
1. Click the **Add Account** button.
2. Paste your account's `.ROBLOSECURITY` cookie, or login via browser.
3. Done! The app will automatically fetch your Username, Display Name, and Avatar thumbnail.

## Compiling from Source
If you're a developer and want to compile this yourself:
1. Make sure you have Visual Studio 2022/2026 Build Tools with "Desktop development with C++" installed.
2. Open the Developer Command Prompt.
3. Run the `build.bat` script to compile the executable.

## Contact & Support
If you run into any weird bugs, have suggestions, or just want to chat about the project, feel free to reach out to me!

**Discord:** `vxxn.`