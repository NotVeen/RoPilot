# RoPilot v1.0.5 Changelog

## ✨ New Features
- **Hard Reset**: Added a "Forgot Password" button on the unlock screen to securely factory reset the application without manually deleting files.
- **Private Server Auto-Fill & Mismatch Detection**: Automatically resolves Place ID from Private Server links (including share links), auto-fills empty Place ID forms, and provides bilingual notifications if Place ID and Private Server link mismatch.
- **Modern Obsidian UI Redesign**: Redesigned UI with sleek obsidian dark palette (`#0d0e12` background, `#17181e` cards, `#232530` borders), compact 38px frameless titlebar with 32x28px controls, modern sidebar navigation with brand header and dedicated collapse toggle button, adjusted default window dimensions to 1120x620 (min 800x480), 6px sleek scrollbars, and refined account cards and group containers.
- **Group Launch Setup & Cascading Priority**: Added dedicated Group Launch Setup for account groups with Place ID, Private Server link auto-fill, Join Low Server, Lowest Graphics, Anti-AFK, and Fast Flags optimization. Features a Force Override switch and hierarchical cascading launch priority (Group Override -> Individual -> Group -> Global).
- **Instant Post-Unlock Account Validation**: Automatically validates all saved account cookies asynchronously right after entering master password (or on startup).
- **Enhanced Anti-AFK**: Upgraded Anti-AFK mechanism to use benign Insert key taps instead of jumping or shift to preserve character farming positions and prevent triggering Shift Lock, bypassed Windows foreground lock timeout for reliable multi-instance switching, and added randomized human-like timing jitter.
- **Window Grid Organizer**: Added automated multi-instance window layout organizer with Auto Grid (proportional 1x2, 2x2, 2x3, 3x3 based on active instances), Compact Mini Grid (200x200 flow layout for extreme GPU and space savings), manual presets, and an optional Auto-Tile on Launch toggle.
- **Aggregate Performance Analytics**: Added an overall instances summary dashboard to the Performance Analytics page with a sleek compact card layout displaying active Roblox client count, total aggregated CPU usage with dynamic threshold coloring, total aggregated RAM usage (MB/GB), and peak active session uptime with watermark vector icons.

## 🐛 Bug Fixes
- Removed the duplicate eye icon that appeared when typing the master password.
- Fixed an issue where the auto-update progress was hidden behind the master key screen.
- Fixed an issue where accounts could not be launched if only Private Server link was provided without Place ID.
- Fixed UI scale affecting full-screen overlays (unlock screen, update progress, and modals) by scoping UI scaling strictly to the main application container.
- Fixed Roblox window tiling overlap and invisible border gaps in grid presets (Auto, 2x2, 2x3, 3x3) by removing WS_THICKFRAME to bypass WM_GETMINMAXINFO minimum height clamping (800x600) and dynamically compensating for DWM extended frame bounds.
- Fixed RoPilot brand icon in sidebar disappearing in Light Mode by dynamically syncing between dark and light base64 assets identical to the titlebar icon.