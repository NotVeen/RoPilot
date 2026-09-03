# RoPilot v1.0.5 Changelog

## ✨ New Features
- **Hard Reset**: Added a "Forgot Password" button on the unlock screen to securely factory reset the application without manually deleting files.
- **Private Server Auto-Fill & Mismatch Detection**: Automatically resolves Place ID from Private Server links (including share links), auto-fills empty Place ID forms, and provides bilingual notifications if Place ID and Private Server link mismatch.
- **Modern Obsidian UI Redesign**: Redesigned UI with sleek obsidian dark palette (`#0d0e12` background, `#17181e` cards, `#232530` borders), compact 38px frameless titlebar with 32x28px controls, modern sidebar navigation with brand header and dedicated collapse toggle button, 6px sleek scrollbars, and refined account cards and group containers.

## 🐛 Bug Fixes
- Removed the duplicate eye icon that appeared when typing the master password.
- Fixed an issue where the auto-update progress was hidden behind the master key screen.
- Fixed an issue where accounts could not be launched if only Private Server link was provided without Place ID.
- Fixed UI scale affecting full-screen overlays (unlock screen, update progress, and modals) by scoping UI scaling strictly to the main application container.