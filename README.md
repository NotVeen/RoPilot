# RoPilot

RoPilot is a lightning-fast, C++ based multi-instance account manager for Roblox. It allows you to store, organize, and launch multiple Roblox accounts simultaneously without running into the "same instance" restriction.

## Features
- **True Multi-Instancing:** Bypasses Roblox's single-instance restriction natively and dynamically, allowing you to run as many Roblox clients as your PC can handle.
- **Account Management:** Save multiple accounts using their `.ROBLOSECURITY` cookies.
- **Drag & Drop Reordering:** Organize your accounts effortlessly with full drag-and-drop support.
- **Group Sorting:** Group accounts together (e.g. "Mains", "Alts") for cleaner organization.
- **Instant Kill Switch:** Rapidly terminate all active Roblox processes instantly using native Windows APIs.
- **Rich UI:** Built using a lightweight WebView2 frontend featuring a dark-mode, animated interface with sleek toast notifications.

## How to Use
1. Download the repository by clicking the green **Code** button and selecting **Download ZIP**.
2. Extract the ZIP file to a folder on your computer.
3. Double-click on `RoPilot.exe` to launch the application.
4. **Important:** You must leave the `WebView2Loader.dll` file in the same folder as `RoPilot.exe`, as the interface relies on it!

## Adding Accounts
1. Click the **Add Account** button.
2. Paste your Roblox account's `.ROBLOSECURITY` cookie.
3. The app will automatically fetch your Username, Display Name, and Avatar thumbnail.

## Launching
Simply click the **Launch** button next to any account. RoPilot will securely handle the authentication ticket and bypass the instance limits to load the game. You can launch specific Job IDs by entering a Place ID into the search bar, or just click Launch to open the Roblox home page.

## Compiling from Source
If you wish to compile RoPilot yourself, you will need Visual Studio 2022/2026 Build Tools with C++ Desktop Development installed.
Run the build script via the Developer Command Prompt to compile the executable.