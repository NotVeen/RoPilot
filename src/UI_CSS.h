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
@keyframes mpContainerFadeUp {
    from {
        opacity: 0;
        transform: translateY(16px);
    }
    to {
        opacity: 1;
        transform: translateY(0);
    }
}
@keyframes spin {
    to { transform: rotate(360deg); }
}
.spinner {
    animation: spin 1s linear infinite;
    display: inline-block;
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
    --bg-deep: #0d0e12;
    --bg-sidebar: #14151a;
    --bg-card: #17181e;
    --bg-hover: #1b1c24;
    --bg-active: #22242e;
    --border-color: #232530;
    --border-subtle: #1f2128;
    --border-medium: #2a2f3e;
    --border-faint: rgba(255, 255, 255, 0.05);
    --separator-color: #20222a;
    --stat-bg: #111216;
    --text-main: #ffffff;
    --text-secondary: #9ca3af;
    --text-muted: #717582;
    --text-tertiary: #5c5e66;
    --text-green: #10b981;
    --text-yellow: #f59e0b;
    --text-blue: #6366f1;
    --text-indigo: #818cf8;
    --accent-color: #6366f1;
    --accent-hover: #818cf8;
    --toggle-color: #6366f1;
    --accent-text: #ffffff;
    --accent-tint-hover: rgba(99, 102, 241, 0.15);
    --accent-tint-active: rgba(99, 102, 241, 0.25);
    --btn-bg: #111216;
    --btn-hover: #1b1c24;
    --btn-danger: #ef4444;
    --btn-danger-hover: #dc2626;
    --text-danger: #ef4444;
    --font-family: 'Geist', 'Inter', -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, sans-serif;
    --scrollbar-thumb: #232530;
    --scrollbar-thumb-hover: #363948;
    --ui-scale: 1;
}
html {
    --bg-opacity: 1;
}
html {
    background-color: transparent !important;
}
body {
    --bg-deep: rgba(13, 14, 18, var(--bg-opacity));
    --bg-sidebar: rgba(20, 21, 26, var(--bg-opacity));
    background-color: var(--bg-deep) !important;
}
body .card,
body #add-menu,
body .group-header {
    background-color: rgba(23, 24, 30, var(--bg-opacity)) !important;
}
body .nav-item.active,
body .btn-add-account {
    background-color: rgba(34, 36, 46, var(--bg-opacity)) !important;
}
body .nav-item:hover,
body .add-menu-item:hover,
body .btn-add-account:hover {
    background-color: rgba(27, 28, 36, var(--bg-opacity)) !important;
}
[data-theme="light"] body {
    --bg-deep: rgba(248, 250, 252, var(--bg-opacity));
    --bg-sidebar: rgba(255, 255, 255, var(--bg-opacity));
}
[data-theme="light"] body .card,
[data-theme="light"] body #add-menu,
[data-theme="light"] body .group-header {
    background-color: rgba(255, 255, 255, var(--bg-opacity)) !important;
}
[data-theme="light"] body .nav-item.active,
[data-theme="light"] body .btn-add-account {
    background-color: rgba(224, 231, 255, var(--bg-opacity)) !important;
}
[data-theme="light"] body .nav-item:hover,
[data-theme="light"] body .add-menu-item:hover,
[data-theme="light"] body .btn-add-account:hover {
    background-color: rgba(241, 245, 249, var(--bg-opacity)) !important;
}

[data-theme="light"] {
    --bg-deep: #f8fafc;
    --bg-sidebar: #ffffff;
    --bg-card: #ffffff;
    --bg-hover: #f1f5f9;
    --bg-active: #e0e7ff;
    --border-color: #e2e8f0;
    --border-subtle: #e2e8f0;
    --border-medium: #cbd5e1;
    --border-faint: rgba(0, 0, 0, 0.05);
    --separator-color: #f1f5f9;
    --stat-bg: #f8fafc;
    --text-main: #0f172a;
    --text-secondary: #475569;
    --text-muted: #64748b;
    --text-tertiary: #94a3b8;
    --btn-bg: #f1f5f9;
    --btn-hover: #e2e8f0;
    --btn-danger: #fee2e2;
    --btn-danger-hover: #fecaca;
    --text-danger: #ef4444;
    --scrollbar-thumb: #cbd5e1;
    --scrollbar-thumb-hover: #94a3b8;
    --text-green: #059669;
    --text-yellow: #d97706;
    --text-blue: #4f46e5;
    --text-indigo: #4f46e5;
    --accent-color: #4f46e5;
    --accent-hover: #6366f1;
    --toggle-color: #4f46e5;
    --accent-text: #ffffff;
    --accent-tint-hover: rgba(79, 70, 229, 0.10);
    --accent-tint-active: rgba(79, 70, 229, 0.18);
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
[data-theme="light"] .nav-item {
    color: #475569;
}
[data-theme="light"] .nav-item svg {
    color: #64748b;
}
[data-theme="light"] .nav-item:hover {
    color: #0f172a !important;
    background-color: #f1f5f9 !important;
}
[data-theme="light"] .nav-item:hover svg {
    color: #334155 !important;
}
[data-theme="light"] .nav-item.active {
    background-color: #e0e7ff !important;
    background-image: none !important;
    color: #4338ca !important;
    font-weight: 600;
}
[data-theme="light"] .nav-item.active svg {
    color: #4f46e5 !important;
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
    background-color: var(--accent-color, #4f46e5) !important;
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
    background-color: var(--accent-color, #6366f1);
    color: #ffffff;
    border: none;
    border-radius: 8px;
    font-weight: 600;
    cursor: pointer;
    padding: 8px 16px;
    transition: all 0.2s ease;
}
.btn-primary:hover {
    background-color: var(--accent-hover, #818cf8);
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
    background: var(--accent-color, #4f46e5);
    width: 16px;
    height: 16px;
    border-radius: 50%;
    cursor: pointer;
    box-shadow: 0 0 8px var(--accent-tint-active, rgba(79, 70, 229, 0.35));
    transition: background-color 0.15s ease, box-shadow 0.15s ease;
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
    height: 100vh;
    width: 100vw;
    margin: 0;
    padding: 0;
    user-select: none; /* Disable text selection */
}

/* Custom Scrollbar */

input {
    user-select: auto;
}
input::-ms-reveal,
input::-ms-clear {
    display: none;
}

/* Titlebar */
.titlebar {
    position: relative;
    height: 38px;
    background-color: var(--bg-deep);
    border-bottom: 1px solid var(--border-subtle);
    display: flex;
    align-items: center;
    justify-content: space-between;
    padding: 0 8px 0 12px;
    user-select: none;
    flex-shrink: 0;
    z-index: 100000;
}

.titlebar-left {
    display: flex;
    align-items: center;
    gap: 8px;
    font-size: 13px;
    font-weight: 600;
    color: var(--text-main);
    letter-spacing: -0.2px;
}

.title-badge {
    background-color: var(--bg-card);
    color: var(--text-muted);
    padding: 2px 6px;
    border-radius: 4px;
    font-size: 11px;
    font-weight: 500;
    border: 1px solid var(--border-subtle);
}

.drag-area {
    flex-grow: 1;
    height: 100%;
    cursor: default;
}

.titlebar-controls {
    display: flex;
    align-items: center;
    gap: 4px;
    margin-right: 4px;
}

.control-btn {
    background: transparent;
    border: none !important;
    outline: none !important;
    box-shadow: none !important;
    color: var(--text-muted);
    width: 32px;
    height: 28px;
    border-radius: 6px !important;
    display: flex;
    align-items: center;
    justify-content: center;
    cursor: pointer;
    transition: background-color 120ms ease, color 120ms ease;
    margin: 0;
    padding: 0;
}

.control-btn:hover {
    background-color: rgba(255, 255, 255, 0.08);
    color: #ffffff;
}

.control-btn.close:hover {
    background-color: var(--text-danger) !important;
    color: #ffffff !important;
}

[data-theme="light"] .control-btn:hover {
    background-color: #f1f5f9 !important;
    color: #0f172a !important;
}
[data-theme="light"] .control-btn.close:hover {
    background-color: var(--text-danger) !important;
    color: #ffffff !important;
}

/* Layout */
body.app-locked .titlebar {
    background: transparent !important;
    border-bottom: 1px solid transparent !important;
}

body.app-locked .app-container {
    opacity: 0 !important;
    pointer-events: none !important;
}

.app-container {
    display: flex;
    position: absolute;
    top: 38px;
    left: 0;
    width: calc(100vw / var(--ui-scale));
    height: calc((100vh - 38px) / var(--ui-scale));
    transform: scale(var(--ui-scale));
    transform-origin: top left;
    overflow: hidden;
    transition: opacity 0.4s ease;
    z-index: 1;
}

/* Sidebar */
.sidebar {
    width: 220px;
    background-color: var(--bg-sidebar);
    border-right: 1px solid var(--border-color);
    display: flex;
    flex-direction: column;
    padding: 12px 10px;
    gap: 6px;
    flex-shrink: 0;
    transition: width 200ms cubic-bezier(0.16, 1, 0.3, 1);
}

.sidebar.collapsed {
    width: 56px;
}

.sidebar-header {
    display: flex;
    align-items: center;
    justify-content: space-between;
    height: 36px;
    padding: 0 4px;
    margin-bottom: 8px;
    cursor: default;
}

.sidebar-brand {
    display: flex;
    align-items: center;
    gap: 10px;
    font-size: 14px;
    font-weight: 700;
    color: var(--text-main);
    white-space: nowrap;
    overflow: hidden;
    user-select: none;
}

.sidebar-brand-icon {
    width: 22px;
    height: 22px;
    object-fit: contain;
    flex-shrink: 0;
}

.sidebar-toggle-btn {
    width: 28px;
    height: 28px;
    display: flex;
    align-items: center;
    justify-content: center;
    background: transparent;
    border: none;
    color: var(--text-muted);
    border-radius: 6px;
    cursor: pointer;
    transition: color 150ms ease, background-color 150ms ease;
    flex-shrink: 0;
    padding: 0;
}

.sidebar-toggle-btn:hover {
    color: var(--text-main);
    background-color: var(--bg-hover);
}

.sidebar.collapsed .sidebar-header {
    justify-content: center;
    padding: 0;
    cursor: pointer;
}

.sidebar.collapsed .sidebar-brand {
    display: none;
}

.sidebar.collapsed .sidebar-toggle-btn {
    margin: 0 auto;
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
    gap: 12px;
    height: 38px;
    padding: 0 10px;
    border-radius: 8px;
    color: var(--text-secondary);
    font-size: 13px;
    font-weight: 600;
    cursor: pointer;
    background: transparent;
    border: none;
    width: 100%;
    text-align: left;
    white-space: nowrap;
    transition: color 150ms ease, background-color 150ms ease;
    position: relative;
    user-select: none;
    -webkit-user-select: none;
}

.nav-item svg {
    flex-shrink: 0;
    transition: color 150ms ease;
}

.nav-item:hover {
    color: var(--text-main);
    background-color: var(--bg-hover);
}

.nav-item.active {
    color: #ffffff;
    background-color: var(--bg-active);
    background-image: linear-gradient(
        var(--accent-tint-active, transparent),
        var(--accent-tint-active, transparent)
    );
    font-weight: 600;
}

.nav-item.active svg {
    color: var(--accent-hover, #818cf8);
}

.sidebar.collapsed .nav-item {
    justify-content: center !important;
    padding: 0 !important;
}

.nav-text {
    white-space: nowrap;
    overflow: hidden;
    text-overflow: ellipsis;
    transition: opacity 0.2s ease;
}

.sidebar.collapsed .nav-text {
    display: none !important;
}

.sidebar-spacer {
    flex-grow: 1;
}

.btn-add-account {
    background-color: var(--bg-card);
    border: 1px solid var(--border-color);
    color: var(--text-main);
    height: 38px;
    padding: 0 10px;
    border-radius: 8px;
    font-weight: 600;
    font-size: 13px;
    cursor: pointer;
    transition: background-color 150ms ease, border-color 150ms ease, color 150ms ease;
    width: 100%;
    display: flex;
    align-items: center;
    justify-content: center;
    gap: 8px;
    overflow: hidden;
}

.btn-add-account:hover {
    background-color: var(--bg-hover);
    border-color: var(--border-medium);
    color: var(--text-main);
}

.sidebar.collapsed .btn-add-account {
    padding: 0 !important;
    justify-content: center !important;
}

.sidebar.collapsed .add-account-text {
    display: none !important;
}

.add-menu-item {
    padding: 10px 14px;
    font-size: 13px;
    cursor: pointer;
    transition: background 0.15s ease;
    text-align: left;
}
.add-menu-item:hover {
    background-color: var(--bg-hover);
    color: var(--text-main);
}

/* Main Content */
.main-content {
    flex-grow: 1;
    padding: 20px 24px;
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
    padding: 14px 16px;
    display: flex;
    flex-direction: column;
    gap: 12px;
    transition: border-color 0.15s ease, box-shadow 0.15s ease;
    box-shadow: 0 2px 8px rgba(0, 0, 0, 0.2);
}

.card:hover {
    border-color: var(--border-medium);
    box-shadow: 0 4px 16px rgba(0, 0, 0, 0.3);
}

[data-theme="light"] .card {
    box-shadow: 0 2px 8px rgba(0, 0, 0, 0.04);
}
[data-theme="light"] .card:hover {
    box-shadow: 0 4px 16px rgba(0, 0, 0, 0.08);
}

.card-header {
    display: flex;
    align-items: center;
    gap: 12px;
}

.avatar {
    width: 42px;
    height: 42px;
    border-radius: 50%;
    background-color: var(--stat-bg);
    border: 2px solid var(--border-color);
    position: relative;
    flex-shrink: 0;
}

.avatar img {
    width: 100%;
    height: 100%;
    border-radius: 50%;
    object-fit: cover;
}

.status-dot {
    width: 10px;
    height: 10px;
    border-radius: 50%;
    position: absolute;
    bottom: -1px;
    right: -1px;
    border: 2px solid var(--bg-card);
}

.status-dot.online {
    background-color: var(--text-green);
}
.status-dot.ingame {
    background-color: var(--text-blue);
}
.status-dot.offline {
    background-color: var(--text-tertiary);
}
.status-dot.loading {
    background-color: var(--text-yellow);
}
.status-dot.danger {
    background-color: var(--text-danger) !important;
    box-shadow: 0 0 6px var(--text-danger);
}

.user-info {
    display: flex;
    flex-direction: column;
    min-width: 0;
    flex: 1;
}

.username {
    font-weight: 700;
    font-size: 14px;
    color: var(--text-main);
    white-space: nowrap;
    overflow: hidden;
    text-overflow: ellipsis;
}

.userid {
    font-size: 12px;
    color: var(--text-secondary);
    margin-top: 1px;
}

.card-stats {
    display: grid;
    grid-template-columns: 1fr 1fr;
    gap: 8px;
}

.stat-box {
    background-color: var(--stat-bg);
    border: 1px solid var(--border-subtle);
    padding: 6px 10px;
    border-radius: 8px;
    display: flex;
    flex-direction: column;
}

.stat-label {
    font-size: 10px;
    text-transform: uppercase;
    color: var(--text-muted);
    font-weight: 700;
    letter-spacing: 0.5px;
}

.stat-value {
    font-size: 12px;
    font-weight: 600;
    margin-top: 2px;
    color: var(--text-main);
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
.stat-value.red {
    color: var(--text-danger);
}

.card-actions {
    display: flex;
    gap: 8px;
    align-items: center;
    border-top: 1px solid var(--border-subtle);
    padding-top: 10px;
    margin-top: 2px;
}

.btn-utility {
    position: absolute;
    top: 12px;
    right: 12px;
    width: 28px;
    height: 28px;
    border-radius: 6px;
    background: rgba(99, 102, 241, 0.08);
    border: 1px solid rgba(99, 102, 241, 0.2);
    color: #818cf8;
    cursor: pointer;
    display: flex;
    align-items: center;
    justify-content: center;
    padding: 0;
    transition: all 0.15s ease;
    z-index: 5;
}

.btn-utility:hover {
    background: rgba(99, 102, 241, 0.18);
    border-color: rgba(99, 102, 241, 0.4);
    color: #a5b4fc;
}

.btn-launch {
    flex-grow: 1;
    background: rgba(16, 185, 129, 0.1);
    border: 1px solid rgba(16, 185, 129, 0.25);
    color: #10b981;
    border-radius: 8px;
    padding: 0 14px;
    height: 32px;
    display: inline-flex;
    align-items: center;
    justify-content: center;
    gap: 6px;
    font-size: 12px;
    font-weight: 700;
    cursor: pointer;
    transition: all 0.15s ease;
}

.btn-launch:hover {
    background: rgba(16, 185, 129, 0.18);
    border-color: rgba(16, 185, 129, 0.4);
    color: #10b981;
}

.btn-launch.btn-relogin,
.btn-relogin {
    background-color: rgba(239, 68, 68, 0.1) !important;
    border: 1px solid rgba(239, 68, 68, 0.25) !important;
    color: #ef4444 !important;
}

.btn-launch.btn-relogin:hover,
.btn-relogin:hover {
    background-color: rgba(239, 68, 68, 0.18) !important;
}

.modal-login-btn {
    flex: 1;
    padding: 22px 20px;
    display: flex;
    flex-direction: column;
    align-items: center;
    justify-content: center;
    height: auto;
    background: var(--stat-bg) !important;
    border: 1px solid var(--border-color) !important;
    border-radius: 12px !important;
    color: var(--text-main) !important;
    font-size: 13px;
    font-weight: 600;
    cursor: pointer;
    transition: all 0.15s ease !important;
}

.modal-login-btn:hover {
    background: var(--bg-hover) !important;
    border-color: var(--border-medium) !important;
    color: var(--text-main) !important;
}

[data-theme="light"] .modal-login-btn {
    background: #f3f4f6 !important;
    border-color: #d1d5db !important;
    color: #111827 !important;
}

[data-theme="light"] .modal-login-btn:hover {
    background: #e5e7eb !important;
    border-color: #9ca3af !important;
    color: #111827 !important;
}

.btn-icon {
    background-color: var(--stat-bg);
    color: var(--text-muted);
    border: 1px solid var(--border-color);
    border-radius: 8px;
    cursor: pointer;
    transition: all 0.15s ease;
    display: inline-flex;
    align-items: center;
    justify-content: center;
    width: 32px;
    height: 32px;
    flex-shrink: 0;
}

.btn-icon:hover {
    background-color: var(--bg-hover);
    color: var(--text-main);
    border-color: var(--border-medium);
}

.btn-icon.danger {
    background: rgba(239, 68, 68, 0.08);
    border-color: rgba(239, 68, 68, 0.2);
    color: #ef4444;
}

.btn-icon.danger:hover {
    background: rgba(239, 68, 68, 0.18);
    border-color: rgba(239, 68, 68, 0.4);
    color: #ef4444;
}

.btn-remove {
    background-color: var(--stat-bg);
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
    margin-bottom: 14px;
    background: transparent;
    border-radius: 12px;
    border: 1px solid var(--border-color);
    overflow: hidden;
    transition: border-color 0.2s ease;
}

.group-container:hover {
    border-color: var(--border-medium);
}

.group-header {
    padding: 10px 14px;
    background: var(--bg-card);
    border-bottom: 1px solid var(--border-color);
    display: flex;
    align-items: center;
    justify-content: space-between;
    cursor: pointer;
    user-select: none;
    transition: background-color 0.15s ease;
}

.group-header:hover {
    background-color: var(--bg-hover);
}

.group-header-left {
    display: flex;
    align-items: center;
    gap: 10px;
}

.group-header-left h3 {
    margin: 0;
    font-size: 14px;
    font-weight: 700;
    color: var(--text-main);
}

.group-badge {
    background: var(--stat-bg);
    border: 1px solid var(--border-subtle);
    padding: 2px 8px;
    border-radius: 12px;
    font-size: 11px;
    font-weight: 700;
    color: var(--text-secondary);
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

body.is-dragging-card .card {
    transition: none !important;
}
.sortable-ghost {
    opacity: 0.2 !important;
    background-color: var(--bg-hover) !important;
    border: 2px dashed var(--border-color) !important;
    border-radius: 12px !important;
    transition: none !important;
}
.sortable-drag {
    cursor: grabbing !important;
    opacity: 1 !important;
    box-shadow: 0 10px 25px rgba(0,0,0,0.5) !important;
    background-color: var(--bg-card) !important;
    transition: none !important;
    will-change: transform;
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
    align-items: center;
    gap: 6px;
}

.group-actions .btn-icon {
    width: 28px;
    height: 28px;
    border-radius: 6px;
    background: transparent;
    border: 1px solid var(--border-color);
    color: var(--text-secondary);
    display: inline-flex;
    align-items: center;
    justify-content: center;
    cursor: pointer;
    transition: all 0.15s ease;
}

.group-actions .btn-icon:hover {
    background: var(--bg-hover);
    color: var(--text-main);
    border-color: var(--border-medium);
}

.group-actions .btn-icon.danger {
    background: transparent;
    border-color: var(--border-color);
    color: var(--text-secondary);
}

.group-actions .btn-icon.danger:hover {
    background: rgba(239, 68, 68, 0.15);
    border-color: rgba(239, 68, 68, 0.35);
    color: #ef4444;
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
    z-index: 999999;
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
    background-color: var(--border-color);
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
    background-color: var(--accent-color, #6366f1);
}
[data-theme="light"] input:checked + .slider {
    background-color: var(--accent-color, #4f46e5);
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
    z-index: 100001;
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
    width: 6px;
    height: 6px;
}
::-webkit-scrollbar-track {
    background: transparent;
}
::-webkit-scrollbar-thumb {
    background: var(--scrollbar-thumb);
    border-radius: 4px;
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
    background-color: var(--stat-bg);
    border: 1px solid var(--border-color);
    border-radius: 8px;
    padding: 0 12px;
    height: 36px;
    width: 260px;
    transition: all 0.15s ease;
    flex-shrink: 0;
}
.search-box:focus-within {
    border-color: var(--border-medium);
    background-color: var(--bg-card);
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
    background: var(--accent-color, #6366f1);
    cursor: pointer;
    margin-top: -6px;
    box-shadow: 0 0 10px var(--accent-tint-active, rgba(99, 102, 241, 0.45));
    transition: background-color 0.15s ease, box-shadow 0.15s ease;
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
    background-color: var(--accent-color, #6366f1);
    border-color: var(--accent-color, #6366f1);
}
[data-theme="light"] .social-checkbox:checked {
    background-color: var(--accent-color, #4f46e5);
    border-color: var(--accent-color, #4f46e5);
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
.modal,
#cookie-modal,
#group-modal,
#update-prompt-modal,
#changelog-modal,
#modal-add-account,
#wear-outfit-modal,
#display-name-modal,
#rename-group-modal,
#delete-group-modal,
#group-launch-modal,
#kill-modal,
#remove-modal,
#kill-all-modal,
#unfriend-modal,
#kill-group-modal,
#change-password-modal,
#hard-reset-modal {
    top: 38px !important;
    height: calc(100vh - 38px) !important;
}
#wear-outfit-modal.show,
#display-name-modal.show,
#rename-group-modal.show,
#delete-group-modal.show,
#group-launch-modal.show,
#kill-modal.show,
#remove-modal.show,
#kill-all-modal.show,
#unfriend-modal.show,
#kill-group-modal.show,
#change-password-modal.show,
#hard-reset-modal.show {
    opacity: 1 !important;
    pointer-events: auto !important;
}
#wear-outfit-modal.show #wear-outfit-modal-content,
#display-name-modal.show #display-name-modal-content,
#rename-group-modal.show #rename-group-modal-content,
#delete-group-modal.show #delete-group-modal-content,
#group-launch-modal.show #group-launch-modal-content,
#kill-modal.show #kill-modal-content,
#remove-modal.show #remove-modal-content,
#kill-all-modal.show #kill-all-modal-content,
#unfriend-modal.show #unfriend-modal-content,
#kill-group-modal.show .modal-content,
#change-password-modal.show #change-password-modal-content,
#hard-reset-modal.show #hard-reset-modal-content {
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
    background: var(--accent-color, #6366f1);
    cursor: pointer;
    margin-top: -6px;
    box-shadow: 0 0 10px var(--accent-tint-active, rgba(99, 102, 241, 0.45));
    transition: background-color 0.15s ease, box-shadow 0.15s ease;
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

/* OPTION 1: Smooth Linear Gradient */
#master-password-overlay {
    position: fixed;
    top: 0;
    left: 0;
    width: 100vw;
    height: 100vh;
    background-color: rgba(5, 5, 5, var(--bg-opacity));
    background-image: linear-gradient(-45deg, rgba(5, 5, 5, var(--bg-opacity)), rgba(28, 28, 28, var(--bg-opacity)), rgba(10, 10, 10, var(--bg-opacity)), rgba(42, 42, 42, var(--bg-opacity)));
    background-size: 400% 400%;
    animation: mpGradientBG 12s ease infinite;
    z-index: 99999;
    display: flex;
    align-items: center;
    justify-content: center;
}

[data-theme="light"] #master-password-overlay {
    background-color: rgba(248, 249, 250, var(--bg-opacity));
    background-image: linear-gradient(-45deg, rgba(226, 232, 240, var(--bg-opacity)), rgba(255, 255, 255, var(--bg-opacity)), rgba(203, 213, 225, var(--bg-opacity)), rgba(241, 245, 249, var(--bg-opacity)));
    background-size: 400% 400%;
}

@keyframes mpGradientBG {
    0% { background-position: 0% 50%; }
    50% { background-position: 100% 50%; }
    100% { background-position: 0% 50%; }
}

.mp-container {
    width: 400px;
    text-align: left;
    transform: translateY(0);
    animation: mpContainerFadeUp 0.5s cubic-bezier(0.16, 1, 0.3, 1) forwards;
}

.mp-brand-icon {
    width: 48px;
    height: 48px;
    object-fit: contain;
    user-select: none;
    pointer-events: none;
}

.mp-container h2 {
    margin: 0 0 10px 0;
    font-size: 24px;
    font-weight: 600;
    text-align: center;
}

.mp-container p {
    color: var(--text-muted);
    font-size: 14px;
    margin-bottom: 24px;
    text-align: center;
}

#btn-mp-submit {
    font-weight: 700;
    background-color: #ffffff !important;
    color: #111111 !important;
    border: none !important;
    border-radius: 8px !important;
    cursor: pointer;
    transition: background-color 0.2s ease, filter 0.2s ease;
}

#btn-mp-submit:hover {
    background-color: #f3f4f6 !important;
}

[data-theme="light"] #btn-mp-submit {
    background-color: #111827 !important;
    color: #ffffff !important;
    border: none !important;
    border-radius: 8px !important;
}

[data-theme="light"] #btn-mp-submit:hover {
    background-color: #1f2937 !important;
}

.mp-container .input-group {
    margin-bottom: 15px;
    position: relative;
    display: flex;
    align-items: center;
    width: 100%;
}

.mp-container .input-icon {
    position: absolute;
    left: 14px;
    top: 50%;
    transform: translateY(-50%);
    color: var(--text-muted);
    pointer-events: none;
}

.mp-container .input-toggle {
    position: absolute;
    right: 8px;
    top: 50%;
    transform: translateY(-50%);
    color: var(--text-muted);
    cursor: pointer;
    transition: color 0.2s;
    background: transparent;
    border: none;
    padding: 0;
    width: 32px;
    height: 32px;
    display: flex;
    align-items: center;
    justify-content: center;
    border-radius: 6px;
}

.mp-container .input-toggle:hover {
    color: var(--text-main);
}

.mp-container input, .ropilot-swal .swal2-input {
    width: 100%;
    text-align: left;
    background: rgba(0, 0, 0, 0.2);
    border: 1px solid var(--border-color);
    color: var(--text-main);
    font-size: 14px;
    padding: 12px 44px;
    border-radius: 8px;
    outline: none;
    transition: border-color 0.2s;
    box-sizing: border-box;
}

[data-theme="light"] .mp-container input, [data-theme="light"] .ropilot-swal .swal2-input {
    background: #ffffff;
    border-color: #d1d5db;
}

.mp-container input:focus, .ropilot-swal .swal2-input:focus {
    border-color: #717582 !important;
    box-shadow: none !important;
}

[data-theme="light"] .mp-container input:focus, [data-theme="light"] .ropilot-swal .swal2-input:focus {
    border-color: #475569 !important;
    box-shadow: none !important;
}

.ropilot-swal .swal2-input {
    margin: 10px 0;
    max-width: 100%;
    display: flex;
}

.mp-container button {
    width: 100%;
}

/* Window Grid Organizer Dropdown */
.tile-dropdown-wrapper {
    position: relative;
    display: inline-block;
}

.tile-dropdown-menu {
    position: absolute;
    top: calc(100% + 6px);
    right: 0;
    width: 215px;
    background: var(--bg-card, #17181e);
    border: 1px solid var(--border-color, #232530);
    border-radius: 10px;
    box-shadow: 0 10px 30px rgba(0, 0, 0, 0.55);
    padding: 6px;
    display: none;
    flex-direction: column;
    gap: 2px;
    z-index: 1000;
    backdrop-filter: blur(12px);
}

.tile-dropdown-menu.show {
    display: flex;
    animation: fadeInTile 0.15s ease-out;
}

@keyframes fadeInTile {
    from { opacity: 0; transform: translateY(-4px); }
    to { opacity: 1; transform: translateY(0); }
}

.tile-dropdown-header {
    padding: 6px 10px 4px 10px;
    font-size: 11px;
    font-weight: 700;
    text-transform: uppercase;
    letter-spacing: 0.5px;
    color: var(--text-muted, #8b949e);
}

.tile-dropdown-item {
    display: flex;
    align-items: center;
    gap: 10px;
    padding: 8px 10px;
    background: transparent;
    border: none;
    border-radius: 6px;
    color: var(--text-main, #f0f6fc);
    font-size: 13px;
    font-weight: 500;
    cursor: pointer;
    text-align: left;
    width: 100%;
    box-sizing: border-box;
    transition: background 0.15s, color 0.15s;
}

.tile-dropdown-item:hover {
    background: var(--btn-hover, #21262d);
    color: var(--accent-color, #10b981);
}

.tile-dropdown-item svg {
    flex-shrink: 0;
    color: var(--text-muted, #8b949e);
    transition: color 0.15s;
}

.tile-dropdown-item:hover svg {
    color: var(--accent-color, #10b981);
}

.tile-dropdown-divider {
    height: 1px;
    background: var(--border-color, #232530);
    margin: 4px 2px;
}

.tile-dropdown-toggle {
    display: flex;
    align-items: center;
    justify-content: space-between;
    padding: 7px 10px;
    font-size: 12px;
    font-weight: 500;
    color: var(--text-main, #f0f6fc);
    cursor: pointer;
    user-select: none;
    border-radius: 6px;
    transition: all 0.15s ease;
}

.tile-dropdown-toggle:hover {
    background: var(--btn-hover, #21262d);
}

.tile-dropdown-toggle input[type="checkbox"] {
    appearance: none;
    -webkit-appearance: none;
    width: 32px;
    height: 18px;
    background: #232530;
    border: 1.5px solid #333846;
    border-radius: 20px;
    position: relative;
    cursor: pointer;
    outline: none;
    margin: 0;
    flex-shrink: 0;
    transition: background-color 0.25s cubic-bezier(0.4, 0, 0.2, 1),
                border-color 0.25s cubic-bezier(0.4, 0, 0.2, 1),
                box-shadow 0.25s cubic-bezier(0.4, 0, 0.2, 1);
}

.tile-dropdown-toggle input[type="checkbox"]::before {
    content: "";
    position: absolute;
    width: 12px;
    height: 12px;
    border-radius: 50%;
    top: 1.5px;
    left: 2px;
    background: #6b7280;
    box-shadow: 0 1px 3px rgba(0, 0, 0, 0.35);
    transition: transform 0.25s cubic-bezier(0.4, 0, 0.2, 1),
                background-color 0.25s cubic-bezier(0.4, 0, 0.2, 1);
}

.tile-dropdown-toggle:hover input[type="checkbox"]:not(:checked) {
    border-color: #4a5063;
    background: #282c3a;
}

.tile-dropdown-toggle:hover input[type="checkbox"]:not(:checked)::before {
    background: #9ca3af;
}

.tile-dropdown-toggle input[type="checkbox"]:checked {
    background: var(--accent-color, #6366f1);
    border-color: var(--accent-color, #6366f1);
    box-shadow: 0 0 10px var(--accent-tint-active, rgba(99, 102, 241, 0.45));
}

.tile-dropdown-toggle input[type="checkbox"]:checked::before {
    transform: translateX(14px);
    background: #ffffff;
    box-shadow: 0 1px 3px rgba(0, 0, 0, 0.25);
}

[data-theme="light"] .tile-dropdown-menu {
    background: #ffffff !important;
    border-color: #e5e7eb !important;
    box-shadow: 0 10px 25px rgba(0, 0, 0, 0.12) !important;
}

[data-theme="light"] .tile-dropdown-item {
    color: #111827 !important;
}

[data-theme="light"] .tile-dropdown-item:hover {
    background: #f3f4f6 !important;
}

[data-theme="light"] .tile-dropdown-divider {
    background: #e5e7eb !important;
}

[data-theme="light"] .tile-dropdown-toggle {
    color: #111827 !important;
}

[data-theme="light"] .tile-dropdown-toggle:hover {
    background: #f3f4f6 !important;
}

[data-theme="light"] .tile-dropdown-toggle input[type="checkbox"] {
    background: #e5e7eb;
    border-color: #d1d5db;
}

[data-theme="light"] .tile-dropdown-toggle input[type="checkbox"]::before {
    background: #9ca3af;
    box-shadow: 0 1px 2px rgba(0, 0, 0, 0.15);
}

[data-theme="light"] .tile-dropdown-toggle:hover input[type="checkbox"]:not(:checked) {
    border-color: #9ca3af;
    background: #cbd5e1;
}

[data-theme="light"] .tile-dropdown-toggle input[type="checkbox"]:checked {
    background: var(--accent-color, #4f46e5);
    border-color: var(--accent-color, #4f46e5);
    box-shadow: 0 0 8px var(--accent-tint-active, rgba(79, 70, 229, 0.35));
}

[data-theme="light"] .tile-dropdown-toggle input[type="checkbox"]:checked::before {
    transform: translateX(14px);
    background: #ffffff;
}

/* Overall Total Stats Summary Cards (Compact Style) */
.analytics-summary-grid {
    display: grid;
    grid-template-columns: repeat(4, 1fr);
    gap: 12px;
    margin-bottom: 20px;
}

@media (max-width: 1024px) {
    .analytics-summary-grid {
        grid-template-columns: repeat(2, 1fr);
    }
}

@media (max-width: 640px) {
    .analytics-summary-grid {
        grid-template-columns: 1fr;
    }
}

.analytics-summary-card {
    position: relative;
    background-color: var(--bg-card, #13151b);
    border: 1px solid var(--border-color, #232733);
    border-radius: 8px;
    padding: 12px 16px;
    display: flex;
    align-items: center;
    justify-content: space-between;
    overflow: hidden;
    min-height: 56px;
    box-sizing: border-box;
    transition: all 0.15s ease;
}

.analytics-summary-card:hover {
    border-color: var(--border-medium, #373e4f);
    background-color: var(--bg-card-hover, #171a22);
}

.analytics-summary-content {
    position: relative;
    z-index: 2;
    display: flex;
    flex-direction: column;
    gap: 4px;
}

.analytics-summary-title {
    font-size: 10px;
    font-weight: 700;
    color: var(--text-muted, #787f95);
    text-transform: uppercase;
    letter-spacing: 0.8px;
    line-height: 1;
}

.analytics-summary-value {
    font-size: 16px;
    font-weight: 700;
    color: var(--text-main, #ffffff);
    letter-spacing: -0.2px;
    line-height: 1.2;
}

.analytics-summary-value.text-green,
.analytics-summary-value.text-indigo {
    color: var(--accent-hover, #818cf8);
}

.analytics-summary-watermark {
    position: absolute;
    right: 12px;
    top: 50%;
    transform: translateY(-50%);
    pointer-events: none;
    z-index: 1;
    opacity: 0.13;
    color: var(--text-muted, #ffffff);
    display: flex;
    align-items: center;
    justify-content: center;
}

.analytics-summary-watermark svg {
    width: 42px;
    height: 42px;
}

[data-theme="light"] .analytics-summary-card {
    background-color: #ffffff;
    border-color: #e5e7eb;
    box-shadow: 0 1px 3px rgba(0, 0, 0, 0.04);
}

[data-theme="light"] .analytics-summary-card:hover {
    border-color: #cbd5e1;
    background-color: #f8fafc;
    box-shadow: 0 2px 6px rgba(0, 0, 0, 0.06);
}

[data-theme="light"] .analytics-summary-title {
    color: #64748b;
}

[data-theme="light"] .analytics-summary-value {
    color: #0f172a;
}

[data-theme="light"] .analytics-summary-value.text-green,
[data-theme="light"] .analytics-summary-value.text-indigo {
    color: var(--accent-color, #4f46e5);
}

[data-theme="light"] .analytics-summary-watermark {
    opacity: 0.10;
    color: #0f172a;
}
)CSS";