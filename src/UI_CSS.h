#pragma once

constexpr const char* UI_CSS = R"CSS(
@keyframes loadingSkeleton {
    0% {
        background-position: 200% 0;
    }
    100% {
        background-position: -200% 0;
    }
}
/* CSS Reset & Variables */
.custom-dropdown {
    position: relative;
    width: 180px;
}
.dropdown-selected {
    background: var(--btn-bg);
    color: var(--text-main);
    border: 1px solid var(--border-color);
    padding: 8px 12px;
    border-radius: 8px;
    cursor: pointer;
    display: flex;
    justify-content: space-between;
    align-items: center;
    font-size: 13px;
    transition: all 0.2s ease;
}
.dropdown-selected:hover {
    border-color: var(--text-muted);
    background-color: var(--btn-hover);
}
.dropdown-options {
    position: absolute;
    top: 100%;
    left: 0;
    right: 0;
    background: var(--bg-card);
    border: 1px solid var(--border-color);
    border-radius: 8px;
    margin-top: 4px;
    max-height: 200px;
    overflow-y: auto;
    z-index: 100;
    display: none;
    box-shadow: 0 4px 12px rgba(0, 0, 0, 0.4);
}
.dropdown-options.show {
    display: block;
}
.dropdown-option {
    padding: 8px 12px;
    font-size: 13px;
    cursor: pointer;
    color: var(--text-main);
}
.dropdown-option:hover {
    background: var(--btn-hover);
}
[data-theme="light"] .dropdown-options {
    box-shadow: 0 4px 12px rgba(0, 0, 0, 0.1);
}
.styled-dropdown {
    appearance: none;
    -webkit-appearance: none;
    background: var(--btn-bg)
        url("data:image/svg+xml;utf8,<svg fill='%23888' height='24' viewBox='0 0 24 24' width='24' xmlns='http://www.w3.org/2000/svg'><path d='M7 10l5 5 5-5z'/><path d='M0 0h24v24H0z' fill='none'/></svg>")
        no-repeat right 8px center;
    color: var(--text-main);
    border: 1px solid var(--border-color);
    padding: 8px 32px 8px 12px;
    border-radius: 8px;
    cursor: pointer;
    outline: none;
    font-size: 13px;
    font-family: inherit;
    transition: all 0.2s ease;
}
.styled-dropdown:hover {
    border-color: var(--text-muted);
    background-color: var(--btn-hover);
}
[data-theme="light"] .styled-dropdown {
    background-image: url("data:image/svg+xml;utf8,<svg fill='%23666' height='24' viewBox='0 0 24 24' width='24' xmlns='http://www.w3.org/2000/svg'><path d='M7 10l5 5 5-5z'/><path d='M0 0h24v24H0z' fill='none'/></svg>");
}
.settings-input {
    background: var(--btn-bg);
    color: var(--text-main);
    border: 1px solid var(--border-color);
    padding: 10px 12px;
    border-radius: 8px;
    outline: none;
    font-family: inherit;
    font-size: 13px;
    transition: all 0.2s ease;
}
.settings-input:focus {
    border-color: var(--text-muted);
    background: var(--bg-hover);
}
.settings-input::placeholder {
    color: var(--text-muted);
    opacity: 0.7;
}
* {
    box-sizing: border-box;
    margin: 0;
    padding: 0;
}
:root {
    --bg-deep: #0a0a0a;
    --bg-sidebar: #111111;
    --bg-card: #111111;
    --bg-hover: #222222;
    --border-color: #2a2a2a;
    --separator-color: rgba(255, 255, 255, 0.04);
    --text-main: #ffffff;
    --text-muted: #888888;
    --text-green: #4ade80;
    --text-yellow: #facc15;
    --text-blue: #60a5fa;
    --btn-bg: #222222;
    --btn-hover: #333333;
    --btn-danger: #3b1313;
    --btn-danger-hover: #5c1818;
    --text-danger: #f87171;
    --font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, Helvetica, Arial, sans-serif;
    --scrollbar-thumb: #1f1f1f;
    --scrollbar-thumb-hover: #333333;
    --ui-scale: 1;
}
html {
    --bg-opacity: 1;
}
html {
    background-color: transparent !important;
}
body {
    --bg-deep: rgba(10, 10, 10, var(--bg-opacity));
    --bg-sidebar: rgba(17, 17, 17, var(--bg-opacity));
    background-color: var(--bg-deep) !important;
}
body .card,
body #add-menu,
body .group-header {
    background-color: rgba(17, 17, 17, var(--bg-opacity)) !important;
}
body .nav-item.active {
    background-color: rgba(34, 34, 34, var(--bg-opacity)) !important;
}
body .nav-item:hover,
body .add-menu-item:hover {
    background-color: rgba(45, 45, 45, var(--bg-opacity)) !important;
}
[data-theme="light"] body {
    --bg-deep: rgba(243, 244, 246, var(--bg-opacity));
    --bg-sidebar: rgba(255, 255, 255, var(--bg-opacity));
}
[data-theme="light"] body .card,
[data-theme="light"] body #add-menu,
[data-theme="light"] body .group-header {
    background-color: rgba(255, 255, 255, var(--bg-opacity)) !important;
}
[data-theme="light"] body .nav-item.active {
    background-color: rgba(229, 231, 235, var(--bg-opacity)) !important;
}
[data-theme="light"] body .nav-item:hover,
[data-theme="light"] body .add-menu-item:hover {
    background-color: rgba(209, 213, 219, var(--bg-opacity)) !important;
}

[data-theme="light"] {
    --bg-deep: #f3f4f6;
    --bg-sidebar: #ffffff;
    --bg-card: #ffffff;
    --bg-hover: #e5e7eb;
    --border-color: #d1d5db;
    --separator-color: rgba(0, 0, 0, 0.05);
    --text-main: #111827;
    --text-muted: #6b7280;
    --btn-bg: #f3f4f6;
    --btn-hover: #e5e7eb;
    --btn-danger: #fef2f2;
    --btn-danger-hover: #fee2e2;
    --scrollbar-thumb: #d1d5db;
    --scrollbar-thumb-hover: #9ca3af;
}
[data-theme="light"] .setting-title {
    color: var(--text-main) !important;
}
[data-theme="light"] h1,
[data-theme="light"] h2,
[data-theme="light"] h3 {
    color: var(--text-main) !important;
}
[data-theme="light"] .hamburger-btn:hover {
    color: var(--text-main) !important;
}
[data-theme="light"] .nav-item.active {
    background-color: #e5e7eb !important;
    background-image: linear-gradient(
        var(--accent-tint-active, transparent),
        var(--accent-tint-active, transparent)
    ) !important;
    color: var(--text-main) !important;
}
[data-theme="light"] .nav-item::before {
    background-color: var(--accent-color, var(--text-main)) !important;
}
[data-theme="light"] .nav-text {
    color: inherit !important;
}
[data-theme="light"] .btn-icon:hover {
    color: var(--text-main) !important;
    background-color: var(--btn-hover) !important;
    background-image: linear-gradient(
        var(--accent-tint-hover, transparent),
        var(--accent-tint-hover, transparent)
    ) !important;
}
[data-theme="light"] .dropdown-menu {
    background: var(--bg-card) !important;
    color: var(--text-main) !important;
}
[data-theme="light"] .group-header {
    background: var(--bg-card) !important;
}

[data-theme="light"] .toast {
    background: #e5e7eb !important;
    color: #111827 !important;
}
[data-theme="light"] .toast.error {
    background: #fee2e2 !important;
    border-color: #fca5a5 !important;
    color: #7f1d1d !important;
}
[data-theme="light"] .toast.success {
    background: #dcfce7 !important;
    border-color: #86efac !important;
    color: #14532d !important;
}

[data-theme="light"] .btn-add-account {
    background-color: #e5e7eb;
    background-image: linear-gradient(
        var(--accent-tint-active, transparent),
        var(--accent-tint-active, transparent)
    ) !important;
    color: #111827 !important;
}
[data-theme="light"] .btn-add-account:hover {
    filter: brightness(0.85);
}

[data-theme="light"] .btn-launch,
[data-theme="light"] .btn-icon,
[data-theme="light"] .btn-remove {
    background-color: #ffffff !important;
    border-color: #d1d5db !important;
}

[data-theme="light"] .btn-secondary {
    background-color: #f3f4f6 !important;
    border-color: #d1d5db !important;
    color: #111827 !important;
}
[data-theme="light"] .btn-primary:not(.danger) {
    background-color: var(--accent-color, #111827) !important;
    color: #ffffff !important;
    border: none !important;
}
[data-theme="light"] .btn-primary:not(.danger):hover {
    filter: brightness(0.85);
}

[data-theme="light"] .btn-launch:hover,
[data-theme="light"] .btn-icon:hover,
[data-theme="light"] .btn-secondary:hover {
    background-color: #e5e7eb !important;
}

[data-theme="light"] .btn-primary.danger:hover,
[data-theme="light"] .btn-icon.danger:hover,
[data-theme="light"] .btn-remove:hover {
    background-color: #fecaca !important;
    border-color: #fca5a5 !important;
    color: #991b1b !important;
}

[data-theme="light"] .group-container {
    background: #f9fafb !important;
}

[data-theme="light"] .control-btn.close:hover {
    background-color: #fecaca !important;
    color: #991b1b !important;
}

.btn-secondary {
    background-color: transparent;
    color: var(--text-muted);
    border: 1px solid var(--border-color);
    border-radius: 12px;
    font-weight: 500;
    cursor: pointer;
    padding: 8px 16px;
    transition:
        background 0.2s,
        color 0.2s,
        border-color 0.2s;
}
.btn-secondary:hover {
    background-color: var(--bg-hover);
    color: var(--text-main);
}
.btn-primary {
    background-color: var(--accent-color, transparent);
    color: var(--text-main);
    border: 1px solid var(--border-color);
    border-radius: 12px;
    font-weight: 500;
    cursor: pointer;
    padding: 8px 16px;
    transition: background 0.2s;
}
.btn-primary:hover {
    background-color: var(--accent-color, var(--bg-hover));
}
.btn-primary.danger {
    background-color: transparent;
    color: var(--text-muted);
}
.btn-primary.danger:hover {
    background-color: var(--btn-danger);
    color: var(--text-danger);
    border-color: var(--btn-danger-hover);
}

[data-theme="light"] .stat-box {
    background-color: #f3f4f6 !important;
}

[data-theme="light"] .stat-value.green {
    color: #16a34a !important;
}
[data-theme="light"] .stat-value.blue {
    color: #2563eb !important;
}
[data-theme="light"] .status-dot.online {
    background-color: #16a34a !important;
}
[data-theme="light"] .status-dot.ingame {
    background-color: #2563eb !important;
}

[data-theme="light"] .avatar {
    background-color: #e5e7eb !important;
}

[data-theme="light"] .search-box input[type="text"] {
    border: none !important;
    box-shadow: none !important;
    outline: none !important;
    background: transparent !important;
}

[data-theme="light"] input[type="text"],
[data-theme="light"] textarea {
    background: #ffffff !important;
    color: #111827 !important;
    border: 1px solid #d1d5db !important;
}
[data-theme="light"] .analytic-bar-bg {
    background: #d1d5db !important;
}

[data-theme="light"] .switch input:not(:checked) + .slider {
    background-color: #d1d5db !important;
}
[data-theme="light"] input[type="range"] {
    -webkit-appearance: none;
    background: #d1d5db;
    height: 4px;
    border-radius: 2px;
}
[data-theme="light"] input[type="range"]::-webkit-slider-thumb {
    -webkit-appearance: none;
    background: var(--text-green);
    width: 16px;
    height: 16px;
    border-radius: 50%;
    cursor: pointer;
}

html {
    background-color: var(--bg-deep);
}
body {
    font-family: var(--font-family);
    background-color: var(--bg-deep);
    color: var(--text-main);
    overflow: hidden;
    display: flex;
    flex-direction: column;
    height: calc(100vh / var(--ui-scale));
    width: calc(100vw / var(--ui-scale));
    transform: scale(var(--ui-scale));
    transform-origin: top left;
    margin: 0;
    padding: 0;
    user-select: none; /* Disable text selection */
}

/* Custom Scrollbar */

input {
    user-select: auto;
}

/* Titlebar */
.titlebar {
    height: 48px;
    background-color: var(--bg-deep);
    border-bottom: 1px solid var(--border-color);
    display: flex;
    align-items: center;
    justify-content: space-between;
    padding: 0 16px;
    user-select: none;
    flex-shrink: 0;
    z-index: 50;
}

.titlebar-left {
    display: flex;
    align-items: center;
    gap: 12px;
    font-size: 16px;
    letter-spacing: 0.3px;
}

.title-badge {
    background-color: #222;
    color: var(--text-muted);
    padding: 2px 6px;
    border-radius: 4px;
    font-size: 10px;
    font-weight: 700;
    text-transform: uppercase;
}

.drag-area {
    flex-grow: 1;
    height: 100%;
    cursor: default;
}

.titlebar-controls {
    display: flex;
    align-items: center;
    gap: 8px;
}

.control-btn {
    background: none;
    border: none;
    color: var(--text-muted);
    width: 32px;
    height: 32px;
    border-radius: 50%;
    display: flex;
    align-items: center;
    justify-content: center;
    cursor: pointer;
    transition:
        background 0.2s,
        color 0.2s;
}

.control-btn:hover {
    background-color: var(--bg-hover);
    color: var(--text-main);
}

.control-btn.close:hover {
    background-color: var(--btn-danger);
    color: var(--text-danger);
}

/* Layout */
.app-container {
    display: flex;
    flex-grow: 1;
    overflow: hidden;
}

/* Sidebar */
.sidebar {
    width: 240px;
    background-color: var(--bg-sidebar);
    border-right: 1px solid var(--border-color);
    display: flex;
    flex-direction: column;
    padding: 16px;
    gap: 8px;
    flex-shrink: 0;
    transition: width 0.3s cubic-bezier(0.4, 0, 0.2, 1);
}

.sidebar.collapsed {
    width: 72px;
}

.hamburger-btn {
    background: transparent;
    border: none;
    color: var(--text-muted);
    cursor: pointer;
    padding: 12px 16px;
    display: flex;
    align-items: center;
    justify-content: flex-start;
    margin-bottom: 8px;
    transition:
        color 0.2s,
        padding 0.3s cubic-bezier(0.4, 0, 0.2, 1);
    overflow: hidden;
    font-size: 13px;
    font-weight: 700;
    text-transform: uppercase;
    letter-spacing: 1px;
    border-bottom: 1px solid #1a1a1a;
    border-radius: 0;
    gap: 12px;
}

.sidebar.collapsed .hamburger-btn {
    padding: 12px 10px;
}

.hamburger-btn:hover {
    color: #fff;
}

.hamburger-btn.danger {
    color: #ff5252;
}

/* Context Menu */
.context-menu {
    position: fixed;
    background: var(--bg-card);
    border: 1px solid var(--border-color);
    border-radius: 8px;
    box-shadow: 0 4px 12px rgba(0,0,0,0.4);
    padding: 6px;
    min-width: 160px;
    z-index: 1000;
    flex-direction: column;
    visibility: hidden;
    opacity: 0;
    transform: translateY(-8px);
    transition: opacity 0.15s ease, transform 0.15s ease, visibility 0.15s;
    pointer-events: none;
    display: flex;
}
.context-menu.show {
    visibility: visible;
    opacity: 1;
    transform: translateY(0);
    pointer-events: auto;
}
.context-menu-item {
    padding: 8px 12px;
    display: flex;
    align-items: center;
    gap: 12px;
    cursor: pointer;
    color: var(--text-main);
    font-size: 13px;
    position: relative;
    justify-content: space-between;
    border-radius: 6px;
    transition: background 0.1s;
}
.context-menu-item:hover { background: var(--btn-hover); }
.context-sub-menu {
    position: absolute;
    left: calc(100% + 8px);
    top: 0;
    background: var(--bg-card);
    border: 1px solid var(--border-color);
    border-radius: 8px;
    box-shadow: 0 4px 12px rgba(0,0,0,0.4);
    padding: 6px;
    min-width: 160px;
    max-height: 250px;
    overflow-y: auto;
    overflow-x: hidden;
    flex-direction: column;
    visibility: hidden;
    opacity: 0;
    transform: translateY(-8px);
    transition: opacity 0.15s ease, transform 0.15s ease, visibility 0.15s;
    pointer-events: none;
    display: flex;
}
.context-menu-item.has-sub:hover .context-sub-menu {
    visibility: visible;
    opacity: 1;
    transform: translateY(0);
    pointer-events: auto;
}
.context-menu-item.has-sub::before, .context-menu-item.has-sub::after {
    content: '';
    position: absolute;
    top: -4px;
    bottom: -4px;
    width: 16px;
}
.context-menu-item.has-sub::before {
    left: -12px;
}
.context-menu-item.has-sub::after {
    right: -12px;
}
.context-menu.align-left .context-sub-menu {
    left: auto;
    right: calc(100% + 8px);
}
.context-menu.align-bottom .context-sub-menu {
    top: auto;
    bottom: -6px;
}
[data-theme="light"] .context-menu, [data-theme="light"] .context-sub-menu {
    box-shadow: 0 4px 12px rgba(0,0,0,0.1);
}

.nav-item {
    display: flex;
    align-items: center;
    padding: 12px 16px;
    border-radius: 12px;
    cursor: pointer;
    outline: none;
    user-select: none;
    -webkit-user-select: none;
    position: relative;
    transition: padding 0.3s cubic-bezier(0.4, 0, 0.2, 1);
}
.nav-item::before {
    content: "";
    position: absolute;
    left: 0;
    top: 50%;
    transform: translateY(-50%);
    width: 2px;
    height: 16px;
    border-radius: 2px;
    background-color: var(--text-main);
    opacity: 0;
    transition: opacity 0.2s;
}
.nav-item.active::before {
    opacity: 1;
}
.nav-item svg {
    transition: margin 0.3s cubic-bezier(0.4, 0, 0.2, 1);
}

.analytics-bar {
    display: flex;
    justify-content: space-between;
    align-items: center;
    background: rgba(0, 0, 0, 0.2);
    border-radius: 4px;
    padding: 4px 8px;
    margin-top: 10px;
    font-size: 11px;
    color: var(--text-muted);
    font-family: "Consolas", "Courier New", monospace;
    border: 1px solid var(--separator-color);
}

.analytics-stat {
    display: flex;
    align-items: center;
    gap: 4px;
}

.dropdown-menu {
    font-weight: 600;
    font-size: 13px;
    transition:
        background 0.2s,
        color 0.2s,
        padding 0.3s cubic-bezier(0.4, 0, 0.2, 1);
    overflow: hidden;
}

.sidebar.collapsed .nav-item {
    padding: 12px 10px;
}
.sidebar.collapsed .nav-item svg {
    margin: 0;
}

.nav-text {
    white-space: nowrap;
    margin-left: 12px;
    transition:
        max-width 0.3s cubic-bezier(0.4, 0, 0.2, 1),
        opacity 0.3s cubic-bezier(0.4, 0, 0.2, 1),
        margin-left 0.3s cubic-bezier(0.4, 0, 0.2, 1);
    opacity: 1;
    max-width: 150px;
    overflow: hidden;
}

.sidebar.collapsed .nav-text {
    opacity: 0;
    max-width: 0;
    margin-left: 0;
}

.nav-item:hover {
    background-color: var(--bg-hover);
    background-image: linear-gradient(var(--accent-tint-hover, transparent), var(--accent-tint-hover, transparent));
    color: var(--text-main);
}

.nav-item.active {
    background-color: #1f1f1f;
    background-image: linear-gradient(var(--accent-tint-active, transparent), var(--accent-tint-active, transparent));
    color: #ffffff;
}

.sidebar-spacer {
    flex-grow: 1;
}

.btn-add-account {
    background-color: #1f1f1f;
    background-image: linear-gradient(var(--accent-tint-active, transparent), var(--accent-tint-active, transparent));
    color: #ffffff;
    border: none;
    padding: 12px;
    border-radius: 12px;
    font-weight: 600;
    font-size: 14px;
    cursor: pointer;
    transition: background 0.2s;
    width: 100%;
    display: flex;
    align-items: center;
    justify-content: center;
    overflow: hidden;
}

.add-account-text {
    white-space: nowrap;
    margin-left: 8px;
    transition:
        max-width 0.3s cubic-bezier(0.4, 0, 0.2, 1),
        opacity 0.3s cubic-bezier(0.4, 0, 0.2, 1),
        margin-left 0.3s cubic-bezier(0.4, 0, 0.2, 1);
    overflow: hidden;
    max-width: 150px;
}

.sidebar.collapsed .add-account-text {
    opacity: 0;
    max-width: 0;
    margin-left: 0;
}

.btn-add-account:hover {
    filter: brightness(0.85);
}

.add-menu-item {
    padding: 12px;
    font-size: 13px;
    cursor: pointer;
    transition: background 0.2s;
    text-align: center;
}
.add-menu-item:hover {
    background-color: var(--accent-color, var(--bg-hover));
    opacity: 0.9;
}

/* Main Content */
.main-content {
    flex-grow: 1;
    padding: 24px;
    overflow-y: auto;
    background-color: var(--bg-deep);
    scrollbar-gutter: stable;
}

.page-header {
    margin-bottom: 24px;
}

.page-title {
    font-size: 24px;
    font-weight: 700;
    margin-bottom: 4px;
}

.page-subtitle {
    color: var(--text-muted);
    font-size: 14px;
}

/* Cards Grid / Wrappers */
.accounts-wrapper {
    display: flex;
    flex-direction: column;
    position: relative;
}

.card {
    position: relative;
    background-color: var(--bg-card);
    border: 1px solid var(--border-color);
    border-radius: 12px;
    padding: 16px;
    display: flex;
    flex-direction: column;
    gap: 16px;
}

.card-header {
    display: flex;
    align-items: center;
    gap: 12px;
}

.avatar {
    width: 48px;
    height: 48px;
    border-radius: 50%;
    background-color: #222;
    position: relative;
}

.avatar img {
    width: 100%;
    height: 100%;
    border-radius: 50%;
    object-fit: cover;
}

.status-dot {
    width: 12px;
    height: 12px;
    border-radius: 50%;
    position: absolute;
    bottom: 0;
    right: 0;
    border: 2px solid var(--bg-card);
}

.status-dot.online {
    background-color: var(--text-green);
}
.status-dot.ingame {
    background-color: var(--text-blue);
}
.status-dot.offline {
    background-color: var(--text-muted);
}
.status-dot.loading {
    background-color: var(--text-yellow);
}
.status-dot.danger {
    background-color: #ef4444 !important;
    box-shadow: 0 0 8px rgba(239, 68, 68, 0.4);
}

.user-info {
    display: flex;
    flex-direction: column;
}

.username {
    font-weight: 600;
    font-size: 16px;
}

.userid {
    font-size: 12px;
    color: var(--text-muted);
}

.card-stats {
    display: grid;
    grid-template-columns: 1fr 1fr;
    gap: 8px;
}

.stat-box {
    background-color: var(--bg-deep);
    padding: 8px;
    border-radius: 12px;
    display: flex;
    flex-direction: column;
}

.stat-label {
    font-size: 10px;
    text-transform: uppercase;
    color: var(--text-muted);
    font-weight: 700;
}

.stat-value {
    font-size: 13px;
    font-weight: 500;
    margin-top: 2px;
}

.stat-value.green {
    color: var(--text-green);
}
.stat-value.blue {
    color: var(--text-blue);
}
.stat-value.yellow {
    color: var(--text-yellow);
}

.card-actions {
    display: flex;
    gap: 8px;
}

.btn-utility {
    position: absolute;
    top: 12px;
    right: 12px;
    width: 32px;
    height: 32px;
    border-radius: 50%;
    background: transparent;
    border: 1px solid var(--border-color);
    color: var(--text-muted);
    cursor: pointer;
    display: flex;
    align-items: center;
    justify-content: center;
    padding: 0;
    transition: all 0.2s ease;
    z-index: 5;
}
.btn-utility:hover {
    background: var(--bg-hover);
    color: var(--text-main);
    transform: scale(1.05);
}

.btn-launch {
    flex-grow: 1;
    background-color: var(--accent-color, transparent);
    color: var(--text-main);
    border: 1px solid var(--border-color);
    padding: 8px 16px;
    border-radius: 12px;
    font-weight: 500;
    font-size: 13px;
    cursor: pointer;
    transition: background 0.2s;
    display: flex;
    align-items: center;
    justify-content: center;
    gap: 6px;
}

.btn-launch:hover {
    background-color: var(--accent-color, var(--bg-hover));
}

[data-theme="light"] .btn-launch.btn-relogin,
.btn-relogin {
    background-color: rgba(239, 68, 68, 0.1) !important;
    border-color: rgba(239, 68, 68, 0.3) !important;
    color: #ef4444 !important;
}

[data-theme="light"] .btn-launch.btn-relogin:hover,
.btn-relogin:hover {
    background-color: rgba(239, 68, 68, 0.2) !important;
}

[data-theme="light"] .btn-launch.btn-relogin,
.btn-relogin {
    background-color: rgba(239, 68, 68, 0.1) !important;
    border-color: rgba(239, 68, 68, 0.3) !important;
    color: #ef4444 !important;
}

[data-theme="light"] .btn-launch.btn-relogin:hover,
.btn-relogin:hover {
    background-color: rgba(239, 68, 68, 0.2) !important;
}

.btn-icon {
    background-color: transparent;
    color: var(--text-muted);
    border: 1px solid var(--border-color);
    border-radius: 12px;
    cursor: pointer;
    transition:
        background 0.2s,
        color 0.2s,
        border-color 0.2s;
    display: flex;
    align-items: center;
    justify-content: center;
    width: 35px;
    height: 35px;
    flex-shrink: 0;
}

.btn-icon:hover {
    background-color: var(--bg-hover);
    background-image: linear-gradient(var(--accent-tint-hover, transparent), var(--accent-tint-hover, transparent));
    color: var(--text-main);
}

.btn-icon.danger:hover {
    background-color: var(--btn-danger);
    color: var(--text-danger);
    border-color: var(--btn-danger-hover);
}

.btn-remove {
    background-color: transparent;
    color: var(--text-muted);
    border: 1px solid var(--border-color);
    padding: 8px 16px;
    border-radius: 12px;
    font-weight: 500;
    font-size: 13px;
    cursor: pointer;
    transition:
        background 0.2s,
        color 0.2s,
        border-color 0.2s;
}

.btn-remove:hover {
    background-color: var(--btn-danger);
    color: var(--text-danger);
    border-color: var(--btn-danger-hover);
}

/* Toast */
.toast {
    position: fixed;
    bottom: 24px;
    right: 24px;
    background-color: #333;
    color: #fff;
    padding: 12px 20px;
    border-radius: 12px;
    font-weight: 500;
    font-size: 14px;
    transform: translateY(100px);
    opacity: 0;
    transition: all 0.3s ease;
    z-index: 1000;
    border: 1px solid #444;
}

.toast.show {
    transform: translateY(0);
    opacity: 1;
}

/* Empty State */
.empty-state {
    grid-column: 1 / -1;
    display: flex;
    flex-direction: column;
    align-items: center;
    justify-content: center;
    height: 100%;
    min-height: 400px;
    color: var(--text-muted);
    text-align: center;
    animation: fadeIn 0.4s ease;
}

.empty-state svg {
    width: 72px;
    height: 72px;
    margin-bottom: 20px;
    color: var(--text-muted);
    opacity: 0.3;
}

.empty-state h3 {
    color: var(--text-main);
    font-size: 20px;
    margin: 0 0 8px 0;
    font-weight: 600;
}

.empty-state p {
    font-size: 14px;
    margin: 0;
    max-width: 300px;
    line-height: 1.5;
}

/* Groups */
.group-container {
    margin-bottom: 12px;
    background: rgba(0, 0, 0, 0.2);
    border-radius: 12px;
    border: 1px solid var(--border-color);
    overflow: hidden;
    transition: all 0.3s ease;
}

.group-header {
    padding: 12px 16px;
    background: var(--bg-card);
    border-bottom: 1px solid var(--border-color);
    display: flex;
    align-items: center;
    justify-content: space-between;
    cursor: pointer;
    user-select: none;
}

.group-header-left {
    display: flex;
    align-items: center;
    gap: 12px;
}

.group-header-left h3 {
    margin: 0;
    font-size: 15px;
    font-weight: 600;
    color: var(--text-main);
}

.group-badge {
    background: var(--bg-hover);
    padding: 2px 8px;
    border-radius: 12px;
    font-size: 12px;
    color: var(--text-muted);
}

.group-content-wrapper {
    display: grid;
    grid-template-rows: 1fr;
    transition: grid-template-rows 0.3s cubic-bezier(0.4, 0, 0.2, 1);
}

.group-container.collapsed .group-content-wrapper {
    grid-template-rows: 0fr;
}

.group-content {
    overflow: hidden;
}

.group-content-inner {
    padding: 16px;
    display: grid;
    grid-template-columns: repeat(auto-fill, minmax(280px, 1fr));
    gap: 16px;
    min-height: 20px;
    transition: background 0.2s;
}

body.is-dragging-card #ungrouped-grid.is-empty {
    height: 60px !important;
    min-height: 60px !important;
    border: 2px dashed var(--border-color);
    border-radius: 12px;
    margin-bottom: 16px;
    background: var(--bg-main);
    display: block !important;
    overflow: hidden;
    position: relative;
}
body.is-dragging-card #ungrouped-grid.is-empty::after {
    content: attr(data-drop-text);
    color: var(--text-secondary);
    font-weight: 500;
    position: absolute;
    top: 50%;
    left: 50%;
    transform: translate(-50%, -50%);
    pointer-events: none;
    z-index: 2;
}
body.is-dragging-card #ungrouped-grid.is-empty .card {
    position: absolute !important;
    top: 0 !important;
    left: 0 !important;
    right: 0 !important;
    bottom: 0 !important;
    opacity: 0 !important;
    margin: 0 !important;
    width: 100% !important;
    height: 100% !important;
    z-index: 1;
}

.sortable-ghost {
    opacity: 0.2 !important;
    background-color: var(--bg-hover) !important;
    border: 2px dashed var(--border-color) !important;
    border-radius: 12px !important;
}
.sortable-drag {
    cursor: grabbing !important;
    opacity: 1 !important;
    box-shadow: 0 10px 25px rgba(0,0,0,0.5) !important;
    background-color: var(--bg-card) !important;
}

.group-container.collapsed .group-header {
    border-bottom: none;
}

.group-container.collapsed .chevron {
    transform: rotate(-90deg);
}

.chevron {
    transition: transform 0.2s ease;
    color: var(--text-muted);
}

.group-actions {
    display: flex;
    gap: 8px;
}

.card.dragging {
    opacity: 0.5;
}

.group-content.drag-over {
    background: rgba(255, 255, 255, 0.02);
    border-radius: 12px;
}

/* Toast Notifications */
.toast-container {
    position: fixed;
    bottom: 24px;
    right: 24px;
    display: flex;
    flex-direction: column;
    gap: 12px;
    z-index: 9999;
    pointer-events: none;
}

.toast {
    background: #111111;
    border: 1px solid var(--border-color);
    color: var(--text-main);
    padding: 12px 16px;
    border-radius: 12px;
    display: flex;
    align-items: center;
    gap: 12px;
    transform: translateX(120%);
    transition: transform 0.3s ease;
    pointer-events: auto;
}

.toast.show {
    transform: translateX(0);
}

.toast.error {
    background: #2a1111;
    border: 1px solid #4a1a1a;
}

.toast.success {
    background: #112a1a;
    border: 1px solid #1a4a2a;
}

/* Toggle Switch CSS */
.switch {
    flex-shrink: 0;
    position: relative;
    display: inline-block;
    width: 44px;
    height: 24px;
}
.switch input {
    opacity: 0;
    width: 0;
    height: 0;
}
.slider {
    position: absolute;
    cursor: pointer;
    top: 0;
    left: 0;
    right: 0;
    bottom: 0;
    background-color: #2d2d3a;
    transition: 0.3s;
    border-radius: 24px;
}
.slider:before {
    position: absolute;
    content: "";
    height: 18px;
    width: 18px;
    left: 3px;
    bottom: 3px;
    background-color: white;
    transition: 0.3s;
    border-radius: 50%;
}
input:checked + .slider {
    background-color: var(--accent-color, var(--text-green));
}
input:checked + .slider:before {
    transform: translateX(20px);
}

/* Animations */
#add-menu {
    opacity: 0;
    transform: translateY(10px);
    transition:
        opacity 0.2s ease,
        transform 0.2s ease;
    pointer-events: none;
    display: flex;
}
#add-menu.show {
    opacity: 1;
    transform: translateY(0);
    pointer-events: auto;
}

#cookie-modal,
#group-modal,
#update-prompt-modal,
#changelog-modal,
#modal-add-account {
    opacity: 0;
    pointer-events: none;
    transition: opacity 0.2s ease;
    display: flex;
    backdrop-filter: blur(4px);
    background-color: rgba(0, 0, 0, 0.75);
}
#cookie-modal.show,
#group-modal.show,
#update-prompt-modal.show,
#changelog-modal.show,
#modal-add-account.show {
    opacity: 1;
    pointer-events: auto;
}
#cookie-modal-content,
#group-modal-content,
#update-prompt-content,
#changelog-modal-content,
#modal-add-account-content {
    transform: scale(0.95);
    transition: transform 0.2s ease;
}
#cookie-modal.show #cookie-modal-content,
#group-modal.show #group-modal-content,
#update-prompt-modal.show #update-prompt-content,
#changelog-modal.show #changelog-modal-content,
#modal-add-account.show #modal-add-account-content {
    transform: scale(1);
}

.spinner {
    animation: spin 1s linear infinite;
}
@keyframes spin {
    0% {
        transform: rotate(0deg);
    }
    100% {
        transform: rotate(360deg);
    }
}

/* Resize Handles */
.resize-edge {
    position: absolute;
    z-index: 9999;
}
.resize-edge.top {
    top: 0;
    left: 8px;
    right: 8px;
    height: 5px;
    cursor: n-resize;
}
.resize-edge.bottom {
    bottom: 0;
    left: 8px;
    right: 8px;
    height: 5px;
    cursor: s-resize;
}
.resize-edge.left {
    top: 8px;
    bottom: 8px;
    left: 0;
    width: 5px;
    cursor: w-resize;
}
.resize-edge.right {
    top: 8px;
    bottom: 8px;
    right: 0;
    width: 5px;
    cursor: e-resize;
}
.resize-edge.top-left {
    top: 0;
    left: 0;
    width: 8px;
    height: 8px;
    cursor: nw-resize;
}
.resize-edge.top-right {
    top: 0;
    right: 0;
    width: 8px;
    height: 8px;
    cursor: ne-resize;
}
.resize-edge.bottom-left {
    bottom: 0;
    left: 0;
    width: 8px;
    height: 8px;
    cursor: sw-resize;
}
.resize-edge.bottom-right {
    bottom: 0;
    right: 0;
    width: 8px;
    height: 8px;
    cursor: se-resize;
}

/* Scrollbar Redesign */
::-webkit-scrollbar {
    width: 14px;
    height: 14px;
}
::-webkit-scrollbar-track {
    background: transparent;
}
::-webkit-scrollbar-thumb {
    background: var(--scrollbar-thumb);
    border-radius: 12px;
    border: 4px solid transparent;
    background-clip: padding-box;
}
::-webkit-scrollbar-thumb:hover {
    background-color: var(--scrollbar-thumb-hover);
}

.main-content {
    overflow-y: auto;
}

/* Search Box */
.search-box {
    display: flex;
    align-items: center;
    background-color: var(--bg-card);
    border: 1px solid var(--border-color);
    border-radius: 12px;
    padding: 6px 12px;
    width: 250px;
    transition: border-color 0.2s;
    flex-shrink: 0;
}
.search-box:focus-within {
    border-color: #555;
}
.search-box input {
    background: transparent;
    border: none;
    color: var(--text-main);
    font-size: 13px;
    outline: none;
    width: 100%;
    margin-left: 8px;
}
.search-box input::placeholder {
    color: var(--text-muted);
}

@keyframes fadeSlide {
    from {
        opacity: 0;
        transform: translateY(5px);
    }
    to {
        opacity: 1;
        transform: translateY(0);
    }
}

.page-container {
    display: none;
}

.page-container.active {
    display: block;
    animation: fadeSlide 0.25s cubic-bezier(0.4, 0, 0.2, 1) forwards;
}

input[type="range"] {
    -webkit-appearance: none;
    width: 100%;
    background: transparent;
}
input[type="range"]::-webkit-slider-thumb {
    -webkit-appearance: none;
    height: 16px;
    width: 16px;
    border-radius: 50%;
    background: #4ade80;
    cursor: pointer;
    margin-top: -6px;
    box-shadow: 0 0 10px rgba(74, 222, 128, 0.4);
}
input[type="range"]::-webkit-slider-runnable-track {
    width: 100%;
    height: 4px;
    cursor: pointer;
    background: rgba(255, 255, 255, 0.1);
    border-radius: 2px;
}
input[type="range"]:focus {
    outline: none;
}

.modal-header {
    border-bottom: 1px solid var(--border-color);
}
.modal-footer {
    border-top: 1px solid var(--border-color);
}

/* Utility Modal Styles */
#utility-modal .modal-content {
    width: 600px;
    max-width: 95vw;
    height: 420px;
    max-height: 90vh;
    display: flex;
    flex-direction: column;
    padding: 0;
    overflow: hidden;
    background: var(--bg-card);
    border: 1px solid var(--border-color);
    border-radius: 12px;
}

.utility-topbar {
    display: flex;
    align-items: center;
    justify-content: center;
    gap: 24px;
    padding: 8px 16px;
    border-bottom: 1px solid var(--border-color);
    background: var(--bg-elevated);
}

.utility-tab {
    color: var(--text-muted);
    cursor: pointer;
    padding: 8px 4px;
    position: relative;
    transition: color 0.2s ease;
    display: flex;
    align-items: center;
    justify-content: center;
}

.utility-tab:hover {
    color: var(--text-main);
}

.utility-tab.active {
    color: var(--text-main);
}

.utility-content {
    flex-grow: 1;
    padding: 24px;
    overflow-y: auto;
}

.utility-overview-grid {
    display: grid;
    grid-template-columns: repeat(2, 1fr);
    gap: 16px;
}

.overview-card {
    background: var(--bg-elevated);
    border: 1px solid var(--border-color);
    border-radius: 8px;
    padding: 16px;
    display: flex;
    flex-direction: column;
    gap: 4px;
}

.overview-label {
    font-size: 14px;
    color: var(--text-muted);
    letter-spacing: 0.5px;
    font-weight: 500;
}

.overview-value {
    font-size: 16px;
    color: var(--text-main);
    font-weight: 500;
    word-break: break-all;
}

.utility-header-close {
    position: absolute;
    top: 16px;
    right: 16px;
    background: none;
    border: none;
    color: var(--text-muted);
    cursor: pointer;
    padding: 4px;
    border-radius: 4px;
    z-index: 10;
}
.utility-header-close:hover {
    background: var(--bg-hover);
    color: var(--text-main);
}

@keyframes ellipsis {
    0% {
        content: "";
    }
    25% {
        content: ".";
    }
    50% {
        content: "..";
    }
    75% {
        content: "...";
    }
    100% {
        content: "";
    }
}
.loading-text::after {
    content: "";
    animation: ellipsis 1.5s infinite;
    display: inline-block;
    width: 1em;
    text-align: left;
}

html:not([data-theme="light"]) #utility-modal .modal-content {
    background-color: #0e0e0e !important;
    border-color: #222 !important;
}
html:not([data-theme="light"]) .utility-topbar {
    background-color: #080808 !important;
    border-bottom-color: #222 !important;
}
html:not([data-theme="light"]) .overview-card {
    background-color: #1a1a1a !important;
    border-color: #2a2a2a !important;
}

html:not([data-theme="light"]) .dynamic-icon {
    filter: grayscale(100%) brightness(0) invert(0.65) !important;
    -webkit-filter: grayscale(100%) brightness(0) invert(0.65) !important;
}
html[data-theme="light"] .dynamic-icon {
    filter: grayscale(100%) brightness(0) invert(0.4) !important;
    -webkit-filter: grayscale(100%) brightness(0) invert(0.4) !important;
}

.mask-icon {
    background-color: var(--text-muted);
    -webkit-mask-size: contain;
    -webkit-mask-repeat: no-repeat;
    -webkit-mask-position: center;
}

@keyframes swipeInLeft {
    from {
        opacity: 0;
        transform: translateX(30px);
    }
    to {
        opacity: 1;
        transform: translateX(0);
    }
}
@keyframes swipeInRight {
    from {
        opacity: 0;
        transform: translateX(-30px);
    }
    to {
        opacity: 1;
        transform: translateX(0);
    }
}
.utility-page-left {
    animation: swipeInLeft 0.3s cubic-bezier(0.4, 0, 0.2, 1) forwards;
}
.utility-page-right {
    animation: swipeInRight 0.3s cubic-bezier(0.4, 0, 0.2, 1) forwards;
}

.social-item {
    display: flex;
    align-items: center;
    justify-content: space-between;
    padding: 12px;
    background: var(--bg-card);
    border: 1px solid var(--border-color);
    border-radius: 12px;
    transition: all 0.2s ease;
}
.social-item:hover {
    border-color: var(--accent-color, var(--text-muted));
}
html:not([data-theme="light"]) .social-item {
    background: #111;
}
html:not([data-theme="light"]) .social-item:hover {
    background: #1a1a1a;
}
.social-checkbox {
    display: none;
    appearance: none;
    -webkit-appearance: none;
    width: 22px;
    height: 22px;
    margin-right: 16px;
    border: 2px solid var(--border-color);
    border-radius: 6px;
    background-color: transparent;
    cursor: pointer;
    position: relative;
    transition: all 0.2s;
    flex-shrink: 0;
}
.social-checkbox:checked {
    background-color: #335fff;
    border-color: #335fff;
}
.social-checkbox:checked::after {
    content: "";
    position: absolute;
    left: 6px;
    top: 2px;
    width: 5px;
    height: 10px;
    border: solid white;
    border-width: 0 2px 2px 0;
    transform: rotate(45deg);
}
.bulk-mode .social-checkbox {
    display: block;
}
.bulk-mode .social-item {
    cursor: pointer;
}
.bulk-mode .btn-unfriend-single {
    display: none;
}

@media (min-width: 1200px) and (min-height: 800px) {
    .utility-modal-content {
        width: 1200px !important;
        height: 800px !important;
    }
}
#wear-outfit-modal.show,
#display-name-modal.show,
#rename-group-modal.show,
#delete-group-modal.show,
#kill-modal.show,
#remove-modal.show,
#kill-all-modal.show,
#unfriend-modal.show,
#kill-group-modal.show {
    opacity: 1 !important;
    pointer-events: auto !important;
}
#wear-outfit-modal.show #wear-outfit-modal-content,
#display-name-modal.show #display-name-modal-content,
#rename-group-modal.show #rename-group-modal-content,
#delete-group-modal.show #delete-group-modal-content,
#kill-modal.show #kill-modal-content,
#remove-modal.show #remove-modal-content,
#kill-all-modal.show #kill-all-modal-content,
#unfriend-modal.show #unfriend-modal-content,
#kill-group-modal.show .modal-content {
    transform: scale(1) !important;
}

input[type="range"] {
    -webkit-appearance: none;
    width: 100%;
    background: transparent;
}
input[type="range"]::-webkit-slider-thumb {
    -webkit-appearance: none;
    height: 16px;
    width: 16px;
    border-radius: 50%;
    background: #4ade80;
    cursor: pointer;
    margin-top: -6px;
    box-shadow: 0 0 10px rgba(74, 222, 128, 0.4);
}
input[type="range"]::-webkit-slider-runnable-track {
    width: 100%;
    height: 4px;
    cursor: pointer;
    background: rgba(255, 255, 255, 0.1);
    border-radius: 2px;
}
input[type="range"]:focus {
    outline: none;
}

.modal-header {
    border-bottom: 1px solid var(--border-color);
}
.modal-footer {
    border-top: 1px solid var(--border-color);
}
)CSS";