# RoPilot v1.0.5 Changelog

## ✨ New Features
- **Hard Reset**: Added a "Forgot Password" option on the unlock screen to easily factory reset the app if you forget your master password.
- **Private Server Auto-Fill**: Automatically extracts Place IDs from Private Server links and warns if there is a mismatch.
- **UI Redesign**: Sleek obsidian look with refined Dark and Light modes, compact titlebar, and cleaner navigation.
- **Group Launch Setup**: Configure launch settings per account group with flexible priority overrides (Override -> Account -> Group -> Global).
- **Instant Account Validation**: Automatically checks saved account cookies in the background right after unlocking.
- **Enhanced Anti-AFK**: Improved idle prevention that simulates benign keypresses without disrupting character positioning or Shift Lock.
- **Window Grid Organizer**: Automatically tile Roblox windows with Auto Grid, Compact Mini Grid (200x200), or quick layout presets.
- **Performance Analytics**: View aggregated stats across all active Roblox clients (total CPU, total RAM, instance count, and peak uptime).
- **Crash Watchdog & Smart Auto-Rejoin**: Automatically detects unexpected Roblox crashes, error dialogs, or server disconnects and re-launches only the affected account back into the game after a configurable cooldown.
- **Live In-Game Tracker**: Real-time detection and display of the active game name on Account Cards and Performance Analytics cards using official public Roblox APIs.

## 🐛 Bug Fixes
- Fixed duplicate eye icon appearing in master password fields.
- Fixed launch failure when only a Private Server link was provided without a Place ID.
- Fixed UI scaling affecting full-screen overlays like the unlock screen and modals.
- Refined UI titlebar and sidebar background to seamlessly match #0d0e12 without titlebar separator line.
- Removed up/down spinner arrows on numeric input fields (Rejoin Cooldown and Max Retries) for a cleaner UI.