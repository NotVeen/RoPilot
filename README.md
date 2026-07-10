<p align="center">
  <img src="res/Header%20RoPilot.png" alt="RoPilot Header" width="100%">
</p>

RoPilot is a fun little side project I put together mostly through vibe coding. I built this as a modern, lightweight, and sleek alternative to the Roblox Account Manager (RAM).

This project is **100% open source**! Feel free to explore the codebase, fork it for your own use, or simply enjoy it as it is.

## Screenshots
<p align="center">
  <img src="res/preview%201.png" width="49%">
  <img src="res/preview%202.png" width="49%">
</p>

## What does it do?
RoPilot is a C++ based multi-instance account manager for Roblox. Basically, it lets you store, organize, and launch multiple Roblox accounts at the exact same time without the annoying "same instance" error stopping you. 

### Key Features
- **True Multi-Instancing:** Bypasses Roblox's single-client limit natively so you can run as many accounts as your PC can physically handle.
- **Smart Account Management:** Securely save your accounts locally with Auto-Backup & Self-Recovery to prevent data loss. The system automatically validates expired cookies in the background.
- **Utility Dashboard:** A powerful central hub to view account details (age, Premium status), edit your Display Name, and manage/switch outfits instantly.
- **Advanced Launching:** Launch games directly via Place ID or Private Server share links. Automatically force Roblox to join Low-Player Servers or use the Lowest Graphics settings to save resources.
- **Social Manager:** View your online friends and easily clean up your friend list using the Bulk Unfriend tool.
- **Instant Kill Switch:** Rapidly terminate every active Roblox process instantly via native Windows APIs.
- **Group Sorting:** Neatly categorize your accounts (e.g., "Mains", "Alts") for a much cleaner workspace.
- **Privacy Mode:** Hide your Username and User ID from the interface—perfect for streaming or recording.
- **Modern & Lightweight UI:** Built on a native WebView2 frontend. Enjoy dark/light modes, bilingual support (English/Indonesian), UI scaling, customizable fonts, and smooth animations.
- **Resource Optimizer:** Limits background CPU usage of unfocused Roblox clients to save your system's processing power.

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