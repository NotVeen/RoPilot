# RoPilot v1.0.3 Changelog

## ✨ New Features & Improvements

- **Appearance & Customization Update**:
  - **Language Support**: Added full English and Indonesian translations.
  - **UI Scale**: Added an interactive slider to seamlessly scale the interface (80% to 120%).
  - **Font Changer**: Added font family settings with live preview.
  - **Window Transparency**: Added native Acrylic background blur and opacity settings.
- **Smart Cookie Validation**: Automatically checks account validity in the background and prompts "Re-Login" if a cookie expires.
- **Add Account Modal Revamp**: Redesigned the account addition interface with a centered modal, smooth transitions, and cookie security warnings.
- **Hide Identity**: Added a setting to hide Usernames and User IDs for privacy.
- **Avatar Delay Fix**: Added background polling to bypass Roblox's CDN delay, ensuring your outfit is always accurate.
- **Auto-Backup & Self-Recovery**: Automatically backups and restores `accounts.json` if data corruption occurs, preventing crashes and data loss.
- **Utility Dashboard Revamp**:
  - **Account Overview**: Displays detailed info like account age and Premium status.
  - **Display Name Editing**: Change your Display Name directly within the app.
  - **Outfit Changer**: Instantly load and switch between all saved outfits.
  - **Roblox Experience**: View recently played games, auto-kill old instances, and launch via Place ID or Private Server Links.
  - **Social**: View online friends and use the new **Bulk Unfriend** feature.
  - **Account Settings**: Added toggles to automatically join Low Servers and force Lowest Graphics upon launch.

## 🐛 Bug Fixes

- **Private Server Access**: Fixed "You do not have access" errors when joining via share links (`share?code=...`) or link codes. RoPilot now accurately pre-fetches the `AccessCode`.
- **Outfit Loading Pagination**: Fixed an issue where only the first 50 outfits were loaded. All pages are now fetched automatically.
- **Outfit Switching Speed**: Dramatically increased outfit changing speed to near-instant by using parallelized API requests.