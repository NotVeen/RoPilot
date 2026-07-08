# RoPilot Development Rules

## 📁 Key Files & Core Responsibilities

These files drive the core functionality. Any change must be tested extensively:

* **[main.cpp](file:///d:/Project/RoPilot/src/main.cpp)**: Application entry, window creation, WebView2 setup, message routing, setting application, tray icon management.
* **[UI_Frontend.h](file:///d:/Project/RoPilot/src/UI_Frontend.h)**: Entire frontend HTML, CSS, JavaScript. Controls UI appearance, cookies parsing/bulk action logic, message formatting.
* **[AccountManager.h](file:///d:/Project/RoPilot/src/AccountManager.h)** / **[AccountManager.cpp](file:///d:/Project/RoPilot/src/AccountManager.cpp)**: Accounts loading, DPAPI encryption, validation, database operations in `accounts.json`.
* **[Launcher.h](file:///d:/Project/RoPilot/src/Launcher.h)** / **[Launcher.cpp](file:///d:/Project/RoPilot/src/Launcher.cpp)**: Handles launching Roblox processes with parameters, bypass bootstrapping, authentication injection.
* **[HandleCloser.h](file:///d:/Project/RoPilot/src/HandleCloser.h)** / **[HandleCloser.cpp](file:///d:/Project/RoPilot/src/HandleCloser.cpp)**: Closes mutex/file handles on Roblox processes to enable multi-instance support.
* **[Optimizer.h](file:///d:/Project/RoPilot/src/Optimizer.h)** / **[Optimizer.cpp](file:///d:/Project/RoPilot/src/Optimizer.cpp)**: Natively handles memory compression and thread CPU limiting.
* **[SettingsManager.h](file:///d:/Project/RoPilot/src/SettingsManager.h)** / **[SettingsManager.cpp](file:///d:/Project/RoPilot/src/SettingsManager.cpp)**: Saves, loads configuration settings inside `settings.json`.

---

## ⚠️ Development Rules

1. **Strict Reversion Policy**: If a change causes compilation failures, execution crashes, or breaks existing critical flows, **revert the modifications immediately** (`git checkout` or `git reset`) before attempting a different solution. Do not leave broken/unused placeholder code in the workspace.
2. **UI Language Integrity**: All text, buttons, alerts, labels, and settings inside the User Interface (HTML/CSS/JS) must remain strictly in **English**.
3. **No Direct Code Execution without Consent**: Follow workspace rules and ensure the user approves edits via standard tool workflows.
4. **Safety with JSON Encryption**: Always ensure modifications to `AccountManager` do not corrupt DPAPI encryption/decryption routines.
