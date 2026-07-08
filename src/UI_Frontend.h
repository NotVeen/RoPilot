#pragma once

const char* HTML_CONTENT = R"HTML(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>RoPilot</title>
    <link href="https://fonts.googleapis.com/css2?family=Inter:wght@400;500;600;700&family=Outfit:wght@400;500;600;700&family=Plus+Jakarta+Sans:wght@400;500;600;700&family=Poppins:wght@400;500;600;700&family=Roboto:wght@400;500;700&display=swap" rel="stylesheet">
    <style>
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
            box-shadow: 0 4px 12px rgba(0,0,0,0.4);
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
            box-shadow: 0 4px 12px rgba(0,0,0,0.1);
        }
        .styled-dropdown {
            appearance: none;
            -webkit-appearance: none;
            background: var(--btn-bg) url("data:image/svg+xml;utf8,<svg fill='%23888' height='24' viewBox='0 0 24 24' width='24' xmlns='http://www.w3.org/2000/svg'><path d='M7 10l5 5 5-5z'/><path d='M0 0h24v24H0z' fill='none'/></svg>") no-repeat right 8px center;
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
        * { 
            box-sizing: border-box; margin: 0; padding: 0;
        }
        :root {
            --bg-deep: #0a0a0a;
            --bg-sidebar: #111111;
            --bg-card: #1a1a1a;
            --bg-hover: #222222;
            --border-color: #2a2a2a;
            --separator-color: rgba(255, 255, 255, 0.04);
            --text-main: #ffffff;
            --text-muted: #888888;
            --text-green: #4ade80;
            --text-yellow: #facc15;
            --btn-bg: #222222;
            --btn-hover: #333333;
            --btn-danger: #3b1313;
            --btn-danger-hover: #5c1818;
            --text-danger: #f87171;
            --font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, Helvetica, Arial, sans-serif;
            --scrollbar-thumb: #1f1f1f;
            --scrollbar-thumb-hover: #333333;
            --ui-scale: 1.0;
        }
        html {
            --bg-opacity: 1.0;
        }
        html {
            background-color: transparent !important;
        }
        body {
            --bg-deep: rgba(10, 10, 10, var(--bg-opacity));
            --bg-sidebar: rgba(17, 17, 17, var(--bg-opacity));
            background-color: var(--bg-deep) !important;
        }
        body .card, body .btn-add-account, body #add-menu, body .group-header {
            background-color: rgba(26, 26, 26, var(--bg-opacity)) !important;
        }
        body .nav-item.active {
            background-color: rgba(34, 34, 34, var(--bg-opacity)) !important;
        }
        body .nav-item:hover, body .add-menu-item:hover {
            background-color: rgba(45, 45, 45, var(--bg-opacity)) !important;
        }
        [data-theme="light"] body {
            --bg-deep: rgba(243, 244, 246, var(--bg-opacity));
            --bg-sidebar: rgba(255, 255, 255, var(--bg-opacity));
        }
        [data-theme="light"] body .card, [data-theme="light"] body .btn-add-account, [data-theme="light"] body #add-menu, [data-theme="light"] body .group-header {
            background-color: rgba(255, 255, 255, var(--bg-opacity)) !important;
        }
        [data-theme="light"] body .nav-item.active {
            background-color: rgba(229, 231, 235, var(--bg-opacity)) !important;
        }
        [data-theme="light"] body .nav-item:hover, [data-theme="light"] body .add-menu-item:hover {
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
        [data-theme="light"] .setting-title { color: var(--text-main) !important; }
        [data-theme="light"] h1, [data-theme="light"] h2, [data-theme="light"] h3 { color: var(--text-main) !important; }
        [data-theme="light"] .hamburger-btn:hover { color: var(--text-main) !important; }
        [data-theme="light"] .nav-item.active { background-color: #e5e7eb !important; background-image: linear-gradient(var(--accent-tint-active, transparent), var(--accent-tint-active, transparent)) !important; color: var(--text-main) !important; }
        [data-theme="light"] .nav-item::before { background-color: var(--accent-color, var(--text-main)) !important; }
        [data-theme="light"] .nav-text { color: inherit !important; }
        [data-theme="light"] .btn-icon:hover { color: var(--text-main) !important; background-color: var(--btn-hover) !important; background-image: linear-gradient(var(--accent-tint-hover, transparent), var(--accent-tint-hover, transparent)) !important; }
        [data-theme="light"] .dropdown-menu { background: var(--bg-card) !important; color: var(--text-main) !important; }
        [data-theme="light"] .group-header { background: var(--bg-card) !important; }

        [data-theme="light"] .toast { background: #e5e7eb !important; color: #111827 !important; }
        [data-theme="light"] .toast.error { background: #fee2e2 !important; border-color: #fca5a5 !important; color: #7f1d1d !important; }
        [data-theme="light"] .toast.success { background: #dcfce7 !important; border-color: #86efac !important; color: #14532d !important; }

        

        
        
        [data-theme="light"] .btn-add-account { background-color: #e5e7eb !important; background-image: linear-gradient(var(--accent-tint-active, transparent), var(--accent-tint-active, transparent)) !important; color: #111827 !important; }
        [data-theme="light"] .btn-add-account:hover { filter: brightness(0.85); }

        
        [data-theme="light"] .btn-launch, [data-theme="light"] .btn-icon, [data-theme="light"] .btn-remove { background-color: #ffffff !important; border-color: #d1d5db !important; }
        
        [data-theme="light"] .btn-secondary { background-color: #f3f4f6 !important; border-color: #d1d5db !important; color: #111827 !important; }
        [data-theme="light"] .btn-primary:not(.danger) { background-color: var(--accent-color, #111827) !important; color: #ffffff !important; border: none !important; }
        [data-theme="light"] .btn-primary:not(.danger):hover { filter: brightness(0.85); }

        [data-theme="light"] .btn-launch:hover, [data-theme="light"] .btn-icon:hover, [data-theme="light"] .btn-secondary:hover { background-color: #e5e7eb !important; }

        [data-theme="light"] .btn-primary.danger:hover, [data-theme="light"] .btn-icon.danger:hover, [data-theme="light"] .btn-remove:hover {
            background-color: #fecaca !important; border-color: #fca5a5 !important;
            color: #991b1b !important;
        }

        [data-theme="light"] .group-container { background: #f9fafb !important; }

        

        [data-theme="light"] .control-btn.close:hover { background-color: #fecaca !important; color: #991b1b !important; }


        [data-theme="light"] .stat-box { background-color: #f3f4f6 !important; }

        [data-theme="light"] .stat-value.green { color: #16a34a !important; }
        [data-theme="light"] .status-dot.online { background-color: #16a34a !important; }

        [data-theme="light"] .avatar { background-color: #e5e7eb !important; }

        
        [data-theme="light"] .search-box input[type="text"] { border: none !important; box-shadow: none !important; outline: none !important; background: transparent !important; }

        [data-theme="light"] input[type="text"], [data-theme="light"] textarea { background: #ffffff !important; color: #111827 !important; border: 1px solid #d1d5db !important; }
        [data-theme="light"] .analytic-bar-bg { background: #d1d5db !important; }


        [data-theme="light"] .switch input:not(:checked) + .slider { background-color: #d1d5db !important; }
        [data-theme="light"] input[type=range] {
            -webkit-appearance: none;
            background: #d1d5db;
            height: 4px;
            border-radius: 2px;
        }
        [data-theme="light"] input[type=range]::-webkit-slider-thumb {
            -webkit-appearance: none;
            background: var(--text-green);
            width: 16px;
            height: 16px;
            border-radius: 50%;
            cursor: pointer;
        }

        


        html { background-color: var(--bg-deep); }
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
            transition: background 0.2s, color 0.2s;
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
            transition: color 0.2s, padding 0.3s cubic-bezier(0.4, 0, 0.2, 1);
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
            content: '';
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
            font-family: 'Consolas', 'Courier New', monospace;
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
            transition: background 0.2s, color 0.2s, padding 0.3s cubic-bezier(0.4, 0, 0.2, 1);
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
            transition: max-width 0.3s cubic-bezier(0.4, 0, 0.2, 1), opacity 0.3s cubic-bezier(0.4, 0, 0.2, 1), margin-left 0.3s cubic-bezier(0.4, 0, 0.2, 1);
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

        .btn-add-account { background-color: #1f1f1f;
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
            transition: max-width 0.3s cubic-bezier(0.4, 0, 0.2, 1), opacity 0.3s cubic-bezier(0.4, 0, 0.2, 1), margin-left 0.3s cubic-bezier(0.4, 0, 0.2, 1);
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
        .add-menu-item:hover { background-color: var(--accent-color, var(--bg-hover)); opacity: 0.9; }

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

        .status-dot.online { background-color: var(--text-green); }
        .status-dot.offline { background-color: var(--text-muted); }
        .status-dot.loading { background-color: var(--text-yellow); }
        .status-dot.danger { background-color: #ef4444 !important; box-shadow: 0 0 8px rgba(239, 68, 68, 0.4); }

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
            background-color: #111;
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

        .stat-value.green { color: var(--text-green); }
        .stat-value.yellow { color: var(--text-yellow); }

        .card-actions {
            display: flex;
            gap: 8px;
        }

        
        .btn-manage-account {
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
        .btn-manage-account:hover {
            background: var(--bg-hover);
            color: var(--text-main);
            transform: scale(1.05);
        }

        .btn-launch {
            flex-grow: 1;
            background-color: var(--btn-bg);
            color: var(--text-main);
            border: 1px solid var(--border-color);
            padding: 8px;
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
            background-color: var(--btn-hover);
        }

        [data-theme="light"] .btn-launch.btn-relogin, .btn-relogin {
            background-color: rgba(239, 68, 68, 0.1) !important;
            border-color: rgba(239, 68, 68, 0.3) !important;
            color: #ef4444 !important;
        }

        [data-theme="light"] .btn-launch.btn-relogin:hover, .btn-relogin:hover {
            background-color: rgba(239, 68, 68, 0.2) !important;
        }

        [data-theme="light"] .btn-launch.btn-relogin, .btn-relogin {
            background-color: rgba(239, 68, 68, 0.1) !important;
            border-color: rgba(239, 68, 68, 0.3) !important;
            color: #ef4444 !important;
        }

        [data-theme="light"] .btn-launch.btn-relogin:hover, .btn-relogin:hover {
            background-color: rgba(239, 68, 68, 0.2) !important;
        }

        .btn-icon {
            background-color: transparent;
            color: var(--text-muted);
            border: 1px solid var(--border-color);
            border-radius: 12px;
            cursor: pointer;
            transition: background 0.2s, color 0.2s, border-color 0.2s;
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
            transition: background 0.2s, color 0.2s, border-color 0.2s;
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
            background: rgba(0,0,0,0.2);
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
            border-radius:  12px;
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
            transition: .3s;
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
            transition: .3s;
            border-radius: 50%;
        }
        input:checked + .slider { background-color: var(--accent-color, var(--text-green)); }
        input:checked + .slider:before {
            transform: translateX(20px);
        }



        /* Animations */
        #add-menu {
            opacity: 0;
            transform: translateY(10px);
            transition: opacity 0.2s ease, transform 0.2s ease;
            pointer-events: none;
            display: flex;
        }
        #add-menu.show {
            opacity: 1;
            transform: translateY(0);
            pointer-events: auto;
        }

        #cookie-modal, #group-modal, #update-prompt-modal, #changelog-modal, #modal-add-account {
            opacity: 0;
            pointer-events: none;
            transition: opacity 0.2s ease;
            display: flex;
            backdrop-filter: blur(4px);
        }
        #cookie-modal.show, #group-modal.show, #update-prompt-modal.show, #changelog-modal.show, #modal-add-account.show {
            opacity: 1;
            pointer-events: auto;
        }
        #cookie-modal-content, #group-modal-content, #update-prompt-content, #changelog-modal-content, #modal-add-account-content {
            transform: scale(0.95);
            transition: transform 0.2s ease;
        }
        #cookie-modal.show #cookie-modal-content, #group-modal.show #group-modal-content, #update-prompt-modal.show #update-prompt-content, #changelog-modal.show #changelog-modal-content, #modal-add-account.show #modal-add-account-content {
            transform: scale(1);
        }

        .spinner {
            animation: spin 1s linear infinite;
        }
        @keyframes spin {
            0% { transform: rotate(0deg); }
            100% { transform: rotate(360deg); }
        }
        
        /* Resize Handles */
        .resize-edge { position: absolute; z-index: 9999; }
        .resize-edge.top { top: 0; left: 8px; right: 8px; height: 5px; cursor: n-resize; }
        .resize-edge.bottom { bottom: 0; left: 8px; right: 8px; height: 5px; cursor: s-resize; }
        .resize-edge.left { top: 8px; bottom: 8px; left: 0; width: 5px; cursor: w-resize; }
        .resize-edge.right { top: 8px; bottom: 8px; right: 0; width: 5px; cursor: e-resize; }
        .resize-edge.top-left { top: 0; left: 0; width: 8px; height: 8px; cursor: nw-resize; }
        .resize-edge.top-right { top: 0; right: 0; width: 8px; height: 8px; cursor: ne-resize; }
        .resize-edge.bottom-left { bottom: 0; left: 0; width: 8px; height: 8px; cursor: sw-resize; }
        .resize-edge.bottom-right { bottom: 0; right: 0; width: 8px; height: 8px; cursor: se-resize; }

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
            from { opacity: 0; transform: translateY(5px); }
            to { opacity: 1; transform: translateY(0); }
        }
        
        .page-container {
            display: none;
        }
        
        .page-container.active {
            display: block;
            animation: fadeSlide 0.25s cubic-bezier(0.4, 0, 0.2, 1) forwards;
        }
    
        input[type=range] {
            -webkit-appearance: none;
            width: 100%;
            background: transparent;
        }
        input[type=range]::-webkit-slider-thumb {
            -webkit-appearance: none;
            height: 16px;
            width: 16px;
            border-radius: 50%;
            background: #4ade80;
            cursor: pointer;
            margin-top: -6px;
            box-shadow: 0 0 10px rgba(74, 222, 128, 0.4);
        }
        input[type=range]::-webkit-slider-runnable-track {
            width: 100%;
            height: 4px;
            cursor: pointer;
            background: rgba(255, 255, 255, 0.1);
            border-radius: 2px;
        }
        input[type=range]:focus {
            outline: none;
        }
    
        .modal-header { border-bottom: 1px solid var(--border-color); }
        .modal-footer { border-top: 1px solid var(--border-color); }

        /* Manage Account Modal Styles */
        #manage-account-modal .modal-content {
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

        .manage-topbar {
            display: flex;
            align-items: center;
            justify-content: center;
            gap: 24px;
            padding: 8px 16px;
            border-bottom: 1px solid var(--border-color);
            background: var(--bg-elevated);
        }

        .manage-tab {
            color: var(--text-muted);
            cursor: pointer;
            padding: 8px 4px;
            position: relative;
            transition: color 0.2s ease;
            display: flex;
            align-items: center;
            justify-content: center;
        }

        .manage-tab:hover {
            color: var(--text-main);
        }

        .manage-tab.active {
            color: var(--text-main);
        }

        .manage-tab.active::after {
            content: '';
            position: absolute;
            bottom: -13px;
            left: 0;
            width: 100%;
            height: 2px;
            background: var(--text-main);
            border-radius: 2px;
        }

        .manage-content {
            flex-grow: 1;
            padding: 24px;
            overflow-y: auto;
        }

        .manage-overview-grid {
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
            font-size: 12px;
            color: var(--text-muted);
            text-transform: uppercase;
            letter-spacing: 0.5px;
            font-weight: 600;
        }

        .overview-value {
            font-size: 16px;
            color: var(--text-main);
            font-weight: 500;
            word-break: break-all;
        }
        
        .manage-header-close {
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
        .manage-header-close:hover {
            background: var(--bg-hover);
            color: var(--text-main);
        }

        @keyframes ellipsis {
            0% { content: ''; }
            25% { content: '.'; }
            50% { content: '..'; }
            75% { content: '...'; }
            100% { content: ''; }
        }
        .loading-text::after {
            content: '';
            animation: ellipsis 1.5s infinite;
            display: inline-block;
            width: 1em;
            text-align: left;
        }

        html[data-theme='dark'] #manage-account-modal .modal-content {
            background-color: #050505 !important;
            border-color: #222 !important;
        }
        html[data-theme='dark'] .manage-topbar {
            background-color: #050505 !important;
            border-bottom-color: #222 !important;
        }
        html[data-theme='dark'] .overview-card {
            background-color: #1a1a1a !important;
            border-color: #2a2a2a !important;
        }

        html[data-theme='dark'] .dynamic-icon {
            filter: grayscale(100%) brightness(0) invert(0.65) !important;
            -webkit-filter: grayscale(100%) brightness(0) invert(0.65) !important;
        }
        html[data-theme='light'] .dynamic-icon {
            filter: grayscale(100%) brightness(0) invert(0.4) !important;
            -webkit-filter: grayscale(100%) brightness(0) invert(0.4) !important;
        }

        .manage-page-enter {
            animation: fadeSlideIn 0.3s cubic-bezier(0.4, 0, 0.2, 1) forwards;
        }
</style>
</head>
<body>
    
    <!-- Resize Handles -->
    <div class="resize-edge top" data-edge="3"></div>
    <div class="resize-edge bottom" data-edge="6"></div>
    <div class="resize-edge left" data-edge="1"></div>
    <div class="resize-edge right" data-edge="2"></div>
    <div class="resize-edge top-left" data-edge="4"></div>
    <div class="resize-edge top-right" data-edge="5"></div>
    <div class="resize-edge bottom-left" data-edge="7"></div>
    <div class="resize-edge bottom-right" data-edge="8"></div>

    <!-- Titlebar -->
    <div class="titlebar">
        <div class="titlebar-left">
            <img src="data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAgAAAAIACAYAAAD0eNT6AAAACXBIWXMAAC4jAAAuIwF4pT92AAAGGmlUWHRYTUw6Y29tLmFkb2JlLnhtcAAAAAAAPD94cGFja2V0IGJlZ2luPSLvu78iIGlkPSJXNU0wTXBDZWhpSHpyZVN6TlRjemtjOWQiPz4gPHg6eG1wbWV0YSB4bWxuczp4PSJhZG9iZTpuczptZXRhLyIgeDp4bXB0az0iQWRvYmUgWE1QIENvcmUgOS4xLWMwMDEgNzkuMTQ2Mjg5OSwgMjAyMy8wNi8yNS0yMDowMTo1NSAgICAgICAgIj4gPHJkZjpSREYgeG1sbnM6cmRmPSJodHRwOi8vd3d3LnczLm9yZy8xOTk5LzAyLzIyLXJkZi1zeW50YXgtbnMjIj4gPHJkZjpEZXNjcmlwdGlvbiByZGY6YWJvdXQ9IiIgeG1sbnM6eG1wPSJodHRwOi8vbnMuYWRvYmUuY29tL3hhcC8xLjAvIiB4bWxuczp4bXBNTT0iaHR0cDovL25zLmFkb2JlLmNvbS94YXAvMS4wL21tLyIgeG1sbnM6c3RFdnQ9Imh0dHA6Ly9ucy5hZG9iZS5jb20veGFwLzEuMC9zVHlwZS9SZXNvdXJjZUV2ZW50IyIgeG1sbnM6ZGM9Imh0dHA6Ly9wdXJsLm9yZy9kYy9lbGVtZW50cy8xLjEvIiB4bWxuczpwaG90b3Nob3A9Imh0dHA6Ly9ucy5hZG9iZS5jb20vcGhvdG9zaG9wLzEuMC8iIHhtcDpDcmVhdG9yVG9vbD0iQWRvYmUgUGhvdG9zaG9wIDI0LjcgKDIwMjMwNjEyLm0uMjIwNSA5Y2ZhNzZjKSAgKFdpbmRvd3MpIiB4bXA6Q3JlYXRlRGF0ZT0iMjAyNi0wNy0wNVQyMDoyMzo1MSswNzowMCIgeG1wOk1ldGFkYXRhRGF0ZT0iMjAyNi0wNy0wNVQyMDoyMzo1MSswNzowMCIgeG1wOk1vZGlmeURhdGU9IjIwMjYtMDctMDVUMjA6MjM6NTErMDc6MDAiIHhtcE1NOkluc3RhbmNlSUQ9InhtcC5paWQ6YTFlOWMzNTQtNzRkZS1lZjQ3LWE2MTUtYTNkZDFlYjI1NjYwIiB4bXBNTTpEb2N1bWVudElEPSJhZG9iZTpkb2NpZDpwaG90b3Nob3A6NWU4MzRmMDgtNTdiYy04ZjQ2LTkzYjUtY2FhNzk0NWQ2OWVjIiB4bXBNTTpPcmlnaW5hbERvY3VtZW50SUQ9InhtcC5kaWQ6ZjgxZjVjNGItNWJjMS0yMjQyLThjOWMtNjBlOGFmYmJkNzA2IiBkYzpmb3JtYXQ9ImltYWdlL3BuZyIgcGhvdG9zaG9wOkNvbG9yTW9kZT0iMyI+IDx4bXBNTTpIaXN0b3J5PiA8cmRmOlNlcT4gPHJkZjpsaSBzdEV2dDphY3Rpb249ImNyZWF0ZWQiIHN0RXZ0Omluc3RhbmNlSUQ9InhtcC5paWQ6ZjgxZjVjNGItNWJjMS0yMjQyLThjOWMtNjBlOGFmYmJkNzA2IiBzdEV2dDp3aGVuPSIyMDI2LTA3LTA1VDIwOjIzOjUxKzA3OjAwIiBzdEV2dDpzb2Z0d2FyZUFnZW50PSJBZG9iZSBQaG90b3Nob3AgMjQuNyAoMjAyMzA2MTIubS4yMjA1IDljZmE3NmMpICAoV2luZG93cykiLz4gPHJkZjpsaSBzdEV2dDphY3Rpb249InNhdmVkIiBzdEV2dDppbnN0YW5jZUlEPSJ4bXAuaWlkOmExZTljMzU0LTc0ZGUtZWY0Ny1hNjE1LWEzZGQxZWIyNTY2MCIgc3RFdnQ6d2hlbj0iMjAyNi0wNy0wNVQyMDoyMzo1MSswNzowMCIgc3RFdnQ6c29mdHdhcmVBZ2VudD0iQWRvYmUgUGhvdG9zaG9wIDI0LjcgKDIwMjMwNjEyLm0uMjIwNSA5Y2ZhNzZjKSAgKFdpbmRvd3MpIiBzdEV2dDpjaGFuZ2VkPSIvIi8+IDwvcmRmOlNlcT4gPC94bXBNTTpIaXN0b3J5PiA8L3JkZjpEZXNjcmlwdGlvbj4gPC9yZGY6UkRGPiA8L3g6eG1wbWV0YT4gPD94cGFja2V0IGVuZD0iciI/Pnki1bsAAGBxSURBVHic7d13mB5V+cbx76aHJLSE3qU36SBIrwpKb4Ig2ABpKkVBmiKgiCKC2JCfNOlSpElv0gSk9957TSB9f3/cu2azbHvfnZnnzMz9ua690jbv3Nm8O/PMmXOe09La2oqZmZnVy4DoAGZmZlY8FwBmZmY15ALAzMyshlwAmJmZ1ZALADMzsxoaFB3AzAoxCzAXMBMwAzAbMLLtzyYCbwLjgbHAW8Cr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" width="28" height="28" style="margin-right: 2px;" id="titlebar-icon" />
            <span><strong style="font-weight: 700; font-size: 16px;">RoPilot</strong><span style="color: var(--text-muted); font-size: 14px; margin-left: 12px; font-weight: 500;">v1.0.3 by NotVeen</span></span>
        </div>
        <div class="drag-area" id="drag-area"></div>
        <div class="titlebar-controls">
            <button class="control-btn" id="btn-minimize">
                <svg width="14" height="2" viewBox="0 0 14 2" fill="none" xmlns="http://www.w3.org/2000/svg">
                    <path d="M0 0H14V2H0V0Z" fill="currentColor"/>
                </svg>
            </button>
            <button class="control-btn" id="btn-maximize">
                <svg width="12" height="12" viewBox="0 0 12 12" fill="none" xmlns="http://www.w3.org/2000/svg">
                    <path fill-rule="evenodd" clip-rule="evenodd" d="M2 0C0.895431 0 0 0.895431 0 2V10C0 11.1046 0.895431 12 2 12H10C11.1046 12 12 11.1046 12 10V2C12 0.895431 11.1046 0 10 0H2ZM2 2H10V10H2V2Z" fill="currentColor"/>
                </svg>
            </button>
            <button class="control-btn close" id="btn-close">
                <svg width="12" height="12" viewBox="0 0 12 12" fill="none" xmlns="http://www.w3.org/2000/svg">
                    <path fill-rule="evenodd" clip-rule="evenodd" d="M4.58579 6L0.292893 1.70711L1.70711 0.292893L6 4.58579L10.2929 0.292893L11.7071 1.70711L7.41421 6L11.7071 10.2929L10.2929 11.7071L6 7.41421L1.70711 11.7071L0.292893 10.2929L4.58579 6Z" fill="currentColor"/>
                </svg>
            </button>
        </div>
    </div>

    <div class="app-container">
        <!-- Sidebar -->
        <div class="sidebar" id="sidebar">
            <button class="hamburger-btn" id="btn-hamburger">
                <svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round" style="flex-shrink: 0;">
                    <line x1="3" y1="12" x2="21" y2="12"></line>
                    <line x1="3" y1="6" x2="21" y2="6"></line>
                    <line x1="3" y1="18" x2="21" y2="18"></line>
                </svg>
                <span class="nav-text">Menu</span>
            </button>
            <div class="nav-item active" id="nav-accounts">
                <svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round" style="flex-shrink: 0;">
                    <path d="M20 21v-2a4 4 0 0 0-4-4H8a4 4 0 0 0-4 4v2"></path>
                    <circle cx="12" cy="7" r="4"></circle>
                </svg>
                <span class="nav-text" data-i18n="nav_accounts">Accounts</span>
            </div>
            <div class="nav-item" id="nav-analytics">
                <svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round" style="flex-shrink: 0;">
                    <path d="M3 3v18h18"></path>
                    <path d="M18.7 8l-5.1 5.2-2.8-2.7L7 14.3"></path>
                </svg>
                <span class="nav-text" data-i18n="nav_analytics">Analytics</span>
            </div>
                <div class="nav-item" id="nav-settings">
                    <svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round" style="flex-shrink: 0;">
                        <circle cx="12" cy="12" r="3"></circle>
                        <path d="M19.4 15a1.65 1.65 0 0 0 .33 1.82l.06.06a2 2 0 0 1 0 2.83 2 2 0 0 1-2.83 0l-.06-.06a1.65 1.65 0 0 0-1.82-.33 1.65 1.65 0 0 0-1 1.51V21a2 2 0 0 1-2 2 2 2 0 0 1-2-2v-.09A1.65 1.65 0 0 0 9 19.4a1.65 1.65 0 0 0-1.82.33l-.06.06a2 2 0 0 1-2.83 0 2 2 0 0 1 0-2.83l.06-.06a1.65 1.65 0 0 0 .33-1.82 1.65 1.65 0 0 0-1.51-1H3a2 2 0 0 1-2-2 2 2 0 0 1 2-2h.09A1.65 1.65 0 0 0 4.6 9a1.65 1.65 0 0 0-.33-1.82l-.06-.06a2 2 0 0 1 0-2.83 2 2 0 0 1 2.83 0l.06.06a1.65 1.65 0 0 0 1.82.33H9a1.65 1.65 0 0 0 1-1.51V3a2 2 0 0 1 2-2 2 2 0 0 1 2 2v.09a1.65 1.65 0 0 0 1 1.51 1.65 1.65 0 0 0 1.82-.33l.06-.06a2 2 0 0 1 2.83 0 2 2 0 0 1 0 2.83l-.06.06a1.65 1.65 0 0 0-.33 1.82V9a1.65 1.65 0 0 0 1.51 1H21a2 2 0 0 1 2 2 2 2 0 0 1-2 2h-.09a1.65 1.65 0 0 0-1.51 1z"></path>
                    </svg>
                    <span class="nav-text" data-i18n="nav_settings">Settings</span>
                </div>
            
            <div class="sidebar-spacer"></div>
            
            <div style="position: relative; width: 100%;">

                <button class="btn-add-account" id="btn-add-account">
                    <svg width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round" style="flex-shrink: 0;">
                        <line x1="12" y1="5" x2="12" y2="19"></line>
                        <line x1="5" y1="12" x2="19" y2="12"></line>
                    </svg>
                    <span class="add-account-text" data-i18n="btn_add_account">Add Account</span>
                </button>
            </div>
        </div>

        <!-- Main Content -->
        <div class="main-content">
            <!-- Accounts Page -->
            <div id="page-accounts" class="page-container active">
                <div class="page-header" style="display: flex; justify-content: space-between; align-items: flex-end;">
                    <div>
                        <h1 style="margin: 0 0 8px 0; font-size: 24px; font-weight: 700;" data-i18n="lbl_manage_accounts">Manage Accounts</h1>
                        <div class="page-subtitle" data-i18n="desc_accounts_page">Manage and monitor your connected Roblox accounts.</div>
                    </div>
                    <div style="display: flex; gap: 16px; align-items: center;">
                        <div class="search-box">
                            <svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round" style="color: var(--text-muted);">
                                <circle cx="11" cy="11" r="8"></circle>
                                <line x1="21" y1="21" x2="16.65" y2="16.65"></line>
                            </svg>
                            <input type="text" id="search-input" placeholder="Search accounts..." data-i18n="placeholder_search_accounts" autocomplete="off" spellcheck="false" />
                        </div>

                        <div style="display: flex; gap: 8px; align-items: center;">
                            <button class="btn-icon" id="btn-create-group" title="Create Group" style="border-radius: 50%;">
                                <svg width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round"><line x1="12" y1="5" x2="12" y2="19"></line><line x1="5" y1="12" x2="19" y2="12"></line></svg>
                            </button>
                            <button class="btn-icon danger" onclick="window.showKillAllPrompt(true)" title="Kill All Instances" style="border-radius: 50%;">
                                <svg width="16" height="16" viewBox="0 0 24 24" fill="currentColor"><path d="M19 6.41L17.59 5 L12 10.59 6.41 5 5 6.41 10.59 12 5 17.59 6.41 19 12 13.41 17.59 19 19 17.59 13.41 12z"/></svg>
                            </button>
                        </div>
                    </div>
                </div>

                <div class="accounts-wrapper" id="accounts-grid">
                    <!-- Accounts will be injected here -->
                    <div class="empty-state">
                        <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><path d="M17 21v-2a4 4 0 0 0-4-4H5a4 4 0 0 0-4 4v2"></path><circle cx="9" cy="7" r="4"></circle><path d="M23 21v-2a4 4 0 0 0-3-3.87"></path><path d="M16 3.13a4 4 0 0 1 0 7.75"></path></svg>
                        <h3>Loading</h3>
                        <p data-i18n="lbl_fetching">Fetching your accounts...</p>
                    </div>
                </div>
            </div>

            <!-- Analytics Page -->
            <div id="page-analytics" class="page-container">
                <div class="page-header" style="display: flex; justify-content: space-between; align-items: flex-end;">
                    <div>
                        <h1 style="margin: 0 0 8px 0; font-size: 24px; font-weight: 700;"><span data-i18n="lbl_performance_analytics">Performance Analytics</span></h1>
                        <div class="page-subtitle" data-i18n="desc_analytics">Real-time statistics for your running Roblox instances.</div>
                    </div>
                    <div style="display: flex; gap: 16px; align-items: center;">
                        <div class="search-box">
                            <svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round" style="color: var(--text-muted);">
                                <circle cx="11" cy="11" r="8"></circle>
                                <line x1="21" y1="21" x2="16.65" y2="16.65"></line>
                            </svg>
                            <input type="text" id="search-input-analytics" placeholder="Search accounts..." data-i18n="placeholder_search_accounts" autocomplete="off" spellcheck="false" />
                        </div>
                        <button class="btn-icon danger" onclick="window.showKillAllPrompt(true)" title="Kill All Instances" style="border-radius: 50%;">
                            <svg width="16" height="16" viewBox="0 0 24 24" fill="currentColor"><path d="M19 6.41L17.59 5 L12 10.59 6.41 5 5 6.41 10.59 12 5 17.59 6.41 19 12 13.41 17.59 19 19 17.59 13.41 12z"/></svg>
                        </button>
                    </div>
                </div>

                <div class="accounts-wrapper" id="analytics-grid" style="display: grid; grid-template-columns: repeat(auto-fill, minmax(300px, 1fr)); gap: 16px;">
                    <div class="empty-state">
                        <svg width="32" height="32" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round" style="color: var(--text-muted); margin-bottom: 12px;"><path d="M3 3v18h18"></path><path d="M18.7 8l-5.1 5.2-2.8-2.7L7 14.3"></path></svg>
                        <h3 data-i18n="lbl_no_active_instances">No Active Instances</h3>
                        <p data-i18n="desc_accounts">Launch an account to see real-time analytics.</p>
                    </div>
                </div>
            </div>

            <!-- Settings Page -->
            <div id="page-settings" class="page-container">
                <div class="page-header" style="margin-bottom: 8px; display: flex; justify-content: space-between; align-items: flex-end;">
                    <div>
                        <h1 style="margin: 0 0 8px 0; font-size: 24px; font-weight: 700;" data-i18n="nav_settings">Settings</h1>
                        <div class="page-subtitle" data-i18n="desc_settings">Configure RoPilot preferences.</div>
                    </div>
                    <div class="search-box">
                        <svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round" style="color: var(--text-muted);">
                            <circle cx="11" cy="11" r="8"></circle>
                            <line x1="21" y1="21" x2="16.65" y2="16.65"></line>
                        </svg>
                        <input type="text" id="search-input-settings" placeholder="Search settings..." data-i18n="placeholder_search" autocomplete="off" spellcheck="false" />
                    </div>
                </div>
                
                
                <div id="settings-theme-container" class="settings-category" style="margin-bottom: 12px;">
                    <div class="setting-group" style="padding: 16px 0px 8px 0px; border-bottom: 1px solid var(--border-color); display: flex; align-items: center; gap: 8px;">
                        <svg width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round" style="color: var(--text-muted);">
                            <circle cx="12" cy="12" r="5"></circle>
                            <line x1="12" y1="1" x2="12" y2="3"></line>
                            <line x1="12" y1="21" x2="12" y2="23"></line>
                            <line x1="4.22" y1="4.22" x2="5.64" y2="5.64"></line>
                            <line x1="18.36" y1="18.36" x2="19.78" y2="19.78"></line>
                            <line x1="1" y1="12" x2="3" y2="12"></line>
                            <line x1="21" y1="12" x2="23" y2="12"></line>
                            <line x1="4.22" y1="19.78" x2="5.64" y2="18.36"></line>
                            <line x1="18.36" y1="5.64" x2="19.78" y2="4.22"></line>
                        </svg>
                        <h3 style="margin: 0; font-size: 14px; font-weight: 600; text-transform: uppercase; letter-spacing: 0.05em; color: var(--text-muted);" data-i18n="lbl_appearance_header">Theme</h3>
                    </div>
                    
                    <div class="setting-item" style="padding: 12px 0px; display: flex; justify-content: space-between; align-items: center;">
                        <div>
                            <div class="setting-title" style="font-size: 15px; font-weight: 500; margin-bottom: 4px; color: white;" data-i18n="lbl_light_mode">Light Mode</div>
                            <div class="setting-desc" style="font-size: 13px; color: var(--text-muted);" data-i18n="desc_light_mode">Use a lighter color scheme for the application.</div>
                        </div>
                        <label class="switch">
                            <input type="checkbox" id="setting-light-mode">
                            <span class="slider"></span>
                        </label>
                    </div>
                    <div class="setting-item" style="padding: 12px 0px; display: flex; justify-content: space-between; align-items: center; border-top: 1px solid var(--separator-color);">
                        <div>
                            <div class="setting-title" style="font-size: 15px; font-weight: 500; margin-bottom: 4px; color: white;" data-i18n="lbl_accent_color">Accent Color</div>
                            <div class="setting-desc" style="font-size: 13px; color: var(--text-muted);" data-i18n="desc_accent_color">Customize the primary highlight color of the UI.</div>
                        </div>
                        <div style="display: flex; align-items: center; gap: 8px;">
                            <button id="btn-reset-accent-color" style="background: none; border: none; color: var(--text-muted); cursor: pointer; padding: 4px; border-radius: 4px; display: flex; align-items: center; justify-content: center;" title="Reset Accent Color">
                                <svg width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><path d="M3 12a9 9 0 1 0 9-9 9.75 9.75 0 0 0-6.74 2.74L3 8"/><path d="M3 3v5h5"/></svg>
                            </button>
                            <input type="color" id="setting-accent-color" style="cursor: pointer; background: var(--bg-card); border: 1px solid var(--border-color); width: 36px; height: 36px; padding: 2px; border-radius: 8px;">
                        </div>
                    </div>
                    <div class="setting-item" style="padding: 12px 0px; display: flex; justify-content: space-between; align-items: center; border-top: 1px solid var(--separator-color);">
                        <div>
                            <div class="setting-title" style="font-size: 15px; font-weight: 500; margin-bottom: 4px; color: white;" data-i18n="lbl_font">Font</div>
                            <div class="setting-desc" style="font-size: 13px; color: var(--text-muted);" data-i18n="desc_font">Change the global application font</div>
                        </div>
                        <div class="custom-dropdown" id="font-dropdown-container">
                            <input type="hidden" id="setting-font-family" value="-apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, Helvetica, Arial, sans-serif">
                            <div class="dropdown-selected" id="font-dropdown-selected">
                                <span id="font-dropdown-text">Segoe UI (Default)</span>
                                <svg fill="#888" height="20" viewBox="0 0 24 24" width="20" xmlns="http://www.w3.org/2000/svg"><path d="M7 10l5 5 5-5z"/></svg>
                            </div>
                            <div class="dropdown-options" id="font-dropdown-options">
                                <div class="dropdown-option" data-value="-apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, Helvetica, Arial, sans-serif">Segoe UI (Default)</div>
                                <div class="dropdown-option" data-value="'Inter', sans-serif">Inter</div>
                                <div class="dropdown-option" data-value="'Outfit', sans-serif">Outfit</div>
                                <div class="dropdown-option" data-value="'Plus Jakarta Sans', sans-serif">Plus Jakarta Sans</div>
                                <div class="dropdown-option" data-value="'Poppins', sans-serif">Poppins</div>
                                <div class="dropdown-option" data-value="'Roboto', sans-serif">Roboto</div>
                                <div class="dropdown-option" data-value="Arial, Helvetica, sans-serif">Arial</div>
                                <div class="dropdown-option" data-value="'Consolas', 'Courier New', monospace">Consolas</div>
                                <div class="dropdown-option" data-value="'Comic Sans MS', 'Chalkboard SE', 'Comic Neue', sans-serif">Comic Sans</div>
                                <div class="dropdown-option" data-value="'Times New Roman', Times, serif">Times New Roman</div>
                            </div>
                        </div>
                    </div>
                    <div class="setting-item" style="padding: 12px 0px; display: flex; justify-content: space-between; align-items: center; border-top: 1px solid var(--separator-color);">
                        <div>
                            <div class="setting-title" style="font-size: 15px; font-weight: 500; margin-bottom: 4px; color: white;" data-i18n="lbl_language">Language</div>
                            <div class="setting-desc" style="font-size: 13px; color: var(--text-muted);" data-i18n="desc_language">Change the display language</div>
                        </div>
                        <div class="custom-dropdown" id="lang-dropdown-container">
                            <input type="hidden" id="setting-language" value="en">
                            <div class="dropdown-selected" id="lang-dropdown-selected">
                                <span id="lang-dropdown-text">English</span>
                                <svg fill="#888" height="20" viewBox="0 0 24 24" width="20" xmlns="http://www.w3.org/2000/svg"><path d="M7 10l5 5 5-5z"/></svg>
                            </div>
                            <div class="dropdown-options" id="lang-dropdown-options">
                                <div class="dropdown-option" data-value="en">English</div>
                                <div class="dropdown-option" data-value="id">Bahasa Indonesia</div>
                            </div>
                        </div>
                    </div>
                
                    <div class="setting-item" style="padding: 12px 0px; display: flex; justify-content: space-between; align-items: center; border-top: 1px solid var(--separator-color);">
                        <div>
                            <div class="setting-title" style="font-size: 15px; font-weight: 500; margin-bottom: 4px; color: white;" data-i18n="lbl_ui_scale">UI Scale</div>
                            <div class="setting-desc" style="font-size: 13px; color: var(--text-muted);" data-i18n="desc_ui_scale">Adjust the overall size of the user interface</div>
                        </div>
                        <div style="display: flex; align-items: center; gap: 12px; width: 180px;">
                            <input type="range" id="setting-ui-scale" min="0.8" max="1.2" step="0.2" value="1.0" style="flex-grow: 1; accent-color: var(--accent-color);">
                            <span id="ui-scale-val" style="font-size: 13px; font-weight: 600; width: 45px; text-align: right; color: var(--text-main);">100%</span>
                        </div>
                    </div>
                    <div class="setting-item" style="padding: 12px 0px; display: flex; justify-content: space-between; align-items: center; border-top: 1px solid var(--separator-color);">
                        <div>
                            <div class="setting-title" style="font-size: 15px; font-weight: 500; margin-bottom: 4px; color: white;" data-i18n="lbl_window_opacity">Window Opacity</div>
                            <div class="setting-desc" style="font-size: 13px; color: var(--text-muted);" data-i18n="desc_window_opacity">Adjust the transparency of the entire application window</div>
                        </div>
                        <div style="display: flex; align-items: center; gap: 12px; width: 180px;">
                            <input type="range" id="setting-window-opacity" min="0.2" max="1.0" step="0.05" value="1.0" style="flex-grow: 1; accent-color: var(--accent-color);">
                            <span id="window-opacity-val" style="font-size: 13px; font-weight: 600; width: 45px; text-align: right; color: var(--text-main);">100%</span>
                        </div>
                    </div>
                    <div class="setting-item" style="padding: 12px 0px; display: flex; justify-content: space-between; align-items: center; border-top: 1px solid var(--separator-color);">
                        <div>
                            <div class="setting-title" style="font-size: 15px; font-weight: 500; margin-bottom: 4px; color: white;" data-i18n="lbl_window_blur">Background Blur</div>
                            <div class="setting-desc" style="font-size: 13px; color: var(--text-muted);" data-i18n="desc_window_blur">Apply Acrylic blur effect to the background (Windows 10/11)</div>
                        </div>
                        <label class="switch">
                            <input type="checkbox" id="setting-window-blur">
                            <span class="slider"></span>
                        </label>
                    </div>

                </div>

                <div id="settings-container" class="settings-category" style="margin-bottom: 12px;">
                    <div class="setting-group" style="padding: 16px 0px 8px 0px; border-bottom: 1px solid var(--border-color); display: flex; align-items: center; gap: 8px;">
                        <svg width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round" style="color: var(--text-muted);">
                            <circle cx="12" cy="12" r="3"></circle>
                            <path d="M19.4 15a1.65 1.65 0 0 0 .33 1.82l.06.06a2 2 0 0 1 0 2.83 2 2 0 0 1-2.83 0l-.06-.06a1.65 1.65 0 0 0-1.82-.33 1.65 1.65 0 0 0-1 1.51V21a2 2 0 0 1-2 2 2 2 0 0 1-2-2v-.09A1.65 1.65 0 0 0 9 19.4a1.65 1.65 0 0 0-1.82.33l-.06.06a2 2 0 0 1-2.83 0 2 2 0 0 1 0-2.83l.06-.06a1.65 1.65 0 0 0 .33-1.82 1.65 1.65 0 0 0-1.51-1H3a2 2 0 0 1-2-2 2 2 0 0 1 2-2h.09A1.65 1.65 0 0 0 4.6 9a1.65 1.65 0 0 0-.33-1.82l-.06-.06a2 2 0 0 1 0-2.83 2 2 0 0 1 2.83 0l.06.06a1.65 1.65 0 0 0 1.82.33H9a1.65 1.65 0 0 0 1-1.51V3a2 2 0 0 1 2-2 2 2 0 0 1 2 2v.09a1.65 1.65 0 0 0 1 1.51 1.65 1.65 0 0 0 1.82-.33l.06-.06a2 2 0 0 1 2.83 0 2 2 0 0 1 0 2.83l-.06.06a1.65 1.65 0 0 0-.33 1.82V9a1.65 1.65 0 0 0 1.51 1H21a2 2 0 0 1 2 2 2 2 0 0 1-2 2h-.09a1.65 1.65 0 0 0-1.51 1z"></path>
                        </svg>
                        <h3 style="margin: 0; font-size: 14px; font-weight: 600; text-transform: uppercase; letter-spacing: 0.05em; color: var(--text-muted);" data-i18n="lbl_general">General</h3>
                    </div>
                    
                    <div class="setting-item" style="padding: 12px 0px; border-bottom: 1px solid var(--separator-color); display: flex; justify-content: space-between; align-items: center;">
                        <div>
                            <div class="setting-title" style="font-size: 15px; font-weight: 500; margin-bottom: 4px; color: white;" data-i18n="lbl_run_on_startup">Run on Startup</div>
                            <div class="setting-desc" style="font-size: 13px; color: var(--text-muted);" data-i18n="desc_startup">Automatically launch RoPilot minimized in the system tray when Windows starts.</div>
                        </div>
                        <label class="switch">
                            <input type="checkbox" id="setting-run-startup">
                            <span class="slider"></span>
                        </label>
                    </div>

                    <div class="setting-item" style="padding: 12px 0px; border-bottom: 1px solid var(--separator-color); display: flex; justify-content: space-between; align-items: center;">
                        <div>
                            <div class="setting-title" style="font-size: 15px; font-weight: 500; margin-bottom: 4px; color: white;"><span data-i18n="lbl_minimize_tray">Minimize to Tray on Close</span></div>
                            <div class="setting-desc" style="font-size: 13px; color: var(--text-muted);" data-i18n="desc_background">Keep RoPilot running in the background when you close the window.</div>
                        </div>
                        <label class="switch">
                            <input type="checkbox" id="setting-minimize-tray">
                            <span class="slider"></span>
                        </label>
                    </div>

                    <div class="setting-item" style="padding: 12px 0px; border-bottom: 1px solid var(--separator-color); display: flex; justify-content: space-between; align-items: center;">
                        <div>
                            <div class="setting-title" style="font-size: 15px; font-weight: 500; margin-bottom: 4px; color: white;" data-i18n="lbl_hide_identity">Hide Username & User ID</div>
                            <div class="setting-desc" style="font-size: 13px; color: var(--text-muted);" data-i18n="desc_hide_identity">Censor your account's username and user ID.</div>
                        </div>
                        <label class="switch">
                            <input type="checkbox" id="setting-hide-identity">
                            <span class="slider"></span>
                        </label>
                    </div>

                    <div class="setting-item" style="padding: 12px 0px; border-bottom: 1px solid var(--separator-color); display: flex; justify-content: space-between; align-items: center;">
                        <div>
                            <div class="setting-title" style="font-size: 15px; font-weight: 500; margin-bottom: 4px; color: white;" data-i18n="lbl_auto_updates">Automatic Updates</div>
                            <div class="setting-desc" style="font-size: 13px; color: var(--text-muted);" data-i18n="desc_auto_updates">Automatically download and install new versions when you launch RoPilot.</div>
                        </div>
                        <label class="switch">
                            <input type="checkbox" id="setting-auto-update">
                            <span class="slider"></span>
                        </label>
                    </div>

                    <div class="setting-item" style="padding: 12px 0px; border-bottom: 1px solid var(--separator-color); display: flex; justify-content: space-between; align-items: center;">
                        <div>
                            <div class="setting-title" style="font-size: 15px; font-weight: 500; margin-bottom: 4px; color: white;" data-i18n="lbl_always_on_top">Always on Top</div>
                            <div class="setting-desc" style="font-size: 13px; color: var(--text-muted);" data-i18n="desc_always_on_top">Keep the RoPilot window pinned on top of all other windows.</div>
                        </div>
                        <label class="switch">
                            <input type="checkbox" id="setting-always-on-top">
                            <span class="slider"></span>
                        </label>
                    </div>

                    <div class="setting-item" style="padding: 12px 0px; display: flex; justify-content: space-between; align-items: center;">
                        <div>
                            <div class="setting-title" style="font-size: 15px; font-weight: 500; margin-bottom: 4px; color: white;"><span data-i18n="lbl_auto_kill">Auto-kill Instances on Exit</span></div>
                            <div class="setting-desc" style="font-size: 13px; color: var(--text-muted);" data-i18n="desc_auto_kill">Automatically terminate all Roblox instances when RoPilot is fully closed.</div>
                        </div>
                        <label class="switch">
                            <input type="checkbox" id="setting-auto-kill-exit">
                            <span class="slider"></span>
                        </label>
                    </div>
                </div>

                <div id="settings-performance-container" class="settings-category" style="margin-bottom: 12px;">
                    <div class="setting-group" style="padding: 16px 0px 8px 0px; border-bottom: 1px solid var(--border-color); display: flex; align-items: center; gap: 8px;">
                        <svg width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round" style="color: var(--text-muted);">
                            <polygon points="13 2 3 14 12 14 11 22 21 10 12 10 13 2"></polygon>
                        </svg>
                        <h3 style="margin: 0; font-size: 14px; font-weight: 600; text-transform: uppercase; letter-spacing: 0.05em; color: var(--text-muted);" data-i18n="lbl_performance">Performance</h3>
                    </div>
                    
                    <div class="setting-item" style="padding: 12px 0px; display: flex; justify-content: space-between; align-items: center;">
                        <div>
                            <div class="setting-title" style="font-size: 15px; font-weight: 500; margin-bottom: 4px; color: white;"><span data-i18n="lbl_hardware_accel">Hardware Acceleration (UI)</span></div>
                            <div class="setting-desc" style="font-size: 13px; color: var(--text-muted);"><span data-i18n="desc_hardware_accel">Use GPU to render the UI smoothly. Disable this to save GPU memory for Roblox.</span> <span style="color: #ff9800; font-weight: 500;" data-i18n="lbl_requires_restart">(Requires App Restart)</span></div>
                        </div>
                        <label class="switch">
                            <input type="checkbox" id="setting-hardware-accel">
                            <span class="slider"></span>
                        </label>
                    </div>
                    
                    <div class="setting-item" style="padding: 12px 0px; display: flex; justify-content: space-between; align-items: center; border-top: 1px solid var(--separator-color); margin-top: 4px;">
                        <div>
                            <div class="setting-title" style="font-size: 15px; font-weight: 500; margin-bottom: 4px; color: white;"><span data-i18n="lbl_resource_opt">Resource Optimizer</span></div>
                            <div class="setting-desc" style="font-size: 13px; color: var(--text-muted);" data-i18n="desc_resource_opt">Prioritize focused window and limit CPU/RAM usage of background instances.</div>
                        </div>
                        <label class="switch">
                            <input type="checkbox" id="setting-resource-opt">
                            <span class="slider"></span>
                        </label>
                    </div>
                    
                    <div class="setting-item" style="padding: 12px 0px; display: flex; justify-content: space-between; align-items: center; border-top: 1px solid var(--separator-color); margin-top: 4px;">
                        <div>
                            <div class="setting-title" style="font-size: 15px; font-weight: 500; margin-bottom: 4px; color: white;"><span data-i18n="lbl_cpu_limiter">Background CPU Limiter</span></div>
                            <div class="setting-desc" style="font-size: 13px; color: var(--text-muted);" data-i18n="desc_cpu_limiter_desc">Throttles background instances to save CPU cycles.</div>
                        </div>
                        <label class="switch">
                            <input type="checkbox" id="setting-cpu-limiter-toggle">
                            <span class="slider"></span>
                        </label>
                    </div>
                    <div class="setting-item" id="cpu-limit-container" style="display: none; border-bottom: none; padding-top: 5px;">
                        <div style="width: 100%;">
                            <div class="setting-title" style="font-size: 14px; font-weight: 500; margin-bottom: 8px; color: white; display: flex; justify-content: space-between;">
                                <span data-i18n="lbl_cpu_limit">Background CPU Limit</span>
                                <span id="cpu-limit-value" style="color: var(--accent);">2%</span>
                            </div>
                            <input type="range" id="setting-cpu-limit" min="1" max="100" value="2" style="width: 100%; cursor: pointer;">
                        </div>
                    </div>

                </div>
            </div>
            
            <!-- Update Overlay -->
            <div id="update-overlay" style="position: fixed; inset: 0; background: rgba(10, 10, 14, 0.95); z-index: 9999; display: flex; flex-direction: column; align-items: center; justify-content: center; backdrop-filter: blur(10px); opacity: 0; pointer-events: none; transition: opacity 0.3s ease;">
                <div style="width: 64px; height: 64px; border-radius: 50%; background: rgba(74, 222, 128, 0.1); display: flex; align-items: center; justify-content: center; margin-bottom: 24px;">
                    <svg width="32" height="32" viewBox="0 0 24 24" fill="none" stroke="#4ade80" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
                        <path d="M21 15v4a2 2 0 0 1-2 2H5a2 2 0 0 1-2-2v-4"></path>
                        <polyline points="7 10 12 15 17 10"></polyline>
                        <line x1="12" y1="15" x2="12" y2="3"></line>
                    </svg>
                </div>
                <h2 id="update-overlay-title" style="margin: 0 0 12px 0; font-size: 24px; font-weight: 600; color: white;" data-i18n="lbl_downloading_update">Downloading Update</h2>
                <div id="update-overlay-desc" style="color: var(--text-muted); font-size: 14px; margin-bottom: 32px;">Fetching the latest features and fixes.</div>
                <div style="width: 300px; height: 6px; background: rgba(255,255,255,0.1); border-radius: 3px; overflow: hidden; position: relative;">
                    <div id="update-progress-bar" style="position: absolute; left: 0; top: 0; bottom: 0; width: 0%; background: #4ade80; border-radius: 3px; transition: width 0.1s linear;"></div>
                </div>
                <div id="update-progress-text" style="margin-top: 12px; font-size: 13px; color: var(--text-muted); font-weight: 500;">0%</div>
            </div>

    <!-- Toast Notification -->
    <div id="toast" class="toast"></div>

    <!-- Create Group Modal -->
    <div id="group-modal" class="modal" style="position: fixed; inset: 0; background: rgba(0,0,0,0.5); z-index: 1000; align-items: center; justify-content: center;">
        <div id="group-modal-content" style="background: var(--bg-card); border: 1px solid var(--border-color); border-radius: 12px; width: 400px; max-width: 90%; overflow: hidden; display: flex; flex-direction: column;">
            <div class="modal-header" style="padding: 20px; display: flex; justify-content: space-between; align-items: center;">
                <h2 style="margin: 0; font-size: 18px; font-weight: 600; display: flex; align-items: center; gap: 8px;"><svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round"><path d="M22 19a2 2 0 0 1-2 2H4a2 2 0 0 1-2-2V5a2 2 0 0 1 2-2h5l2 3h9a2 2 0 0 1 2 2z"></path><line x1="12" y1="11" x2="12" y2="17"></line><line x1="9" y1="14" x2="15" y2="14"></line></svg><span data-i18n="lbl_create_group_modal">Create Group</span></h2>
                <button class="btn-icon" id="btn-close-group-modal" style="margin: -8px;" onclick="document.getElementById('group-modal').classList.remove('show');"><svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round"><line x1="18" y1="6" x2="6" y2="18"></line><line x1="6" y1="6" x2="18" y2="18"></line></svg></button>
            </div>
            <div style="padding: 20px;">
                <p style="color: var(--text-muted); margin-bottom: 16px; font-size: 14px;" data-i18n="desc_create_group">Enter a name for the new group. You can drag and drop accounts into this group later.</p>
                <input type="text" id="group-name-input" placeholder="e.g. Main Accounts" style="width: 100%; padding: 10px 14px; background: var(--bg-hover); border: 1px solid var(--border-color); border-radius: 12px; color: var(--text-main); outline: none; transition: border-color 0.2s;" />
            </div>
            <div class="modal-footer" style="padding: 16px 20px; display: flex; justify-content: flex-end; gap: 12px;">
                <button class="btn-secondary" id="btn-cancel-group" style="padding: 8px 16px; background: rgba(255,255,255,0.1); border: none; border-radius: 12px; color: var(--text-main); cursor: pointer;" onclick="document.getElementById('group-modal').classList.remove('show');" data-i18n="btn_cancel">Cancel</button>
                <button class="btn-primary" id="btn-confirm-group" style="padding: 8px 16px; background: white; color: black; border: none; border-radius: 12px; font-weight: 600; cursor: pointer;" data-i18n="btn_create_group">Create</button>
            </div>
        </div>
    </div>

    <!-- Update Prompt Modal -->
    <div id="update-prompt-modal" class="modal" style="position: fixed; inset: 0; background: rgba(0,0,0,0.5); z-index: 1000; align-items: center; justify-content: center;">
        <div id="update-prompt-content" style="background: var(--bg-card); border: 1px solid var(--border-color); border-radius: 12px; width: 450px; max-width: 90%; overflow: hidden; display: flex; flex-direction: column;">
            <div class="modal-header" style="padding: 20px; display: flex; justify-content: space-between; align-items: center;">
                <div style="display: flex; align-items: center; gap: 8px;">
                    <svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="#4ade80" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
                        <circle cx="12" cy="12" r="10"></circle>
                        <polyline points="12 16 16 12 12 8"></polyline>
                        <line x1="8" y1="12" x2="16" y2="12"></line>
                    </svg>
                    <h2 style="margin: 0; font-size: 18px; font-weight: 600;" data-i18n="lbl_update">Update Available</h2>
                </div>
                <button class="btn-icon" onclick="document.getElementById('update-prompt-modal').classList.remove('show');" style="margin: -8px;"><svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round"><line x1="18" y1="6" x2="6" y2="18"></line><line x1="6" y1="6" x2="18" y2="18"></line></svg></button>
            </div>
            <div style="padding: 20px;">
                <p style="color: var(--text-main); margin-bottom: 8px; font-size: 15px; font-weight: 500;"><span data-i18n="lbl_ropilot">RoPilot</span> <span id="update-version-text" style="color: #4ade80;"></span> <span data-i18n="lbl_is_now_available">is now available!</span></p>
                <p style="color: var(--text-muted); font-size: 14px; line-height: 1.5;" data-i18n="desc_update">A new version of RoPilot is detected! Would you like to download and install it now?</p>
            </div>
            <div class="modal-footer" style="padding: 16px 20px; display: flex; justify-content: flex-end; gap: 12px;">
                <button class="btn-secondary" onclick="document.getElementById('update-prompt-modal').classList.remove('show');" style="padding: 8px 16px; background: rgba(255,255,255,0.1); border: none; border-radius: 12px; color: var(--text-main); cursor: pointer;" data-i18n="btn_skip_now">Skip for Now</button>
                <button class="btn-primary" id="btn-start-update" style="padding: 8px 16px; background: #4ade80; color: black; border: none; border-radius: 12px; font-weight: 600; cursor: pointer;" data-i18n="btn_install_now">Install Now</button>
            </div>
        </div>
    </div>

    <!-- Changelog Modal -->
    <div id="changelog-modal" class="modal" style="position: fixed; inset: 0; background: rgba(0,0,0,0.5); z-index: 1000; align-items: center; justify-content: center;" onclick="if(event.target === this) this.classList.remove('show');">
        <div id="changelog-modal-content" style="background: var(--bg-card); border: 1px solid var(--border-color); border-radius: 12px; width: 500px; max-width: 90%; max-height: 80vh; overflow: hidden; display: flex; flex-direction: column;">
            <div class="modal-header" style="padding: 20px; display: flex; justify-content: space-between; align-items: center;">
                <div style="display: flex; align-items: center; gap: 8px;">
                    <svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="#60a5fa" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
                        <path d="M12 22c5.523 0 10-4.477 10-10S17.523 2 12 2 2 6.477 2 12s4.477 10 10 10z"></path>
                        <path d="M12 8v4l3 3"></path>
                    </svg>
                    <h2 id="changelog-title" style="margin: 0; font-size: 18px; font-weight: 600;" data-i18n="lbl_whats_new">What's New in RoPilot</h2>
                </div>
                <button class="btn-icon" onclick="document.getElementById('changelog-modal').classList.remove('show');" style="margin: -8px;"><svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round"><line x1="18" y1="6" x2="6" y2="18"></line><line x1="6" y1="6" x2="18" y2="18"></line></svg></button>
            </div>
            <div style="padding: 20px; overflow-y: auto; flex-grow: 1;">
                <div id="changelog-content" style="color: var(--text-muted); font-size: 14px; line-height: 1.6; font-family: inherit, 'Segoe UI Emoji', sans-serif; margin: 0;"></div>
            </div>
        </div>
    </div>

    
    <!-- Manage Account Modal -->
    <div id="manage-account-modal" class="modal" style="position: fixed; inset: 0; background: rgba(0,0,0,0.6); z-index: 1000; align-items: center; justify-content: center; opacity: 0; pointer-events: none; transition: opacity 0.3s ease; display: flex; backdrop-filter: blur(4px);">
        <div class="modal-content manage-modal-content" style="position: relative; transform: scale(0.95); transition: transform 0.3s cubic-bezier(0.175, 0.885, 0.32, 1.275); display: flex; flex-direction: column; width: 800px; max-width: 90vw; height: 600px; max-height: 85vh; padding: 0; overflow: hidden; border-radius: 12px; border: 1px solid var(--border-color);">
            <div class="manage-topbar" style="position: relative; display: flex; align-items: center; justify-content: center; gap: 32px; padding: 8px 16px; border-bottom: 1px solid var(--border-color);">
                <div class="manage-tab active" data-tab="manage-home" title="Account Overview">
                    <svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><path d="M3 9l9-7 9 7v11a2 2 0 0 1-2 2H5a2 2 0 0 1-2-2z"></path><polyline points="9 22 9 12 15 12 15 22"></polyline></svg>
                </div>
                <div class="manage-tab" data-tab="manage-code" title="Developer/API">
                    <svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><polyline points="16 18 22 12 16 6"></polyline><polyline points="8 6 2 12 8 18"></polyline></svg>
                </div>
                <div class="manage-tab" data-tab="manage-globe" title="Network/Sessions">
                    <svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><circle cx="12" cy="12" r="10"></circle><line x1="2" y1="12" x2="22" y2="12"></line><path d="M12 2a15.3 15.3 0 0 1 4 10 15.3 15.3 0 0 1-4 10 15.3 15.3 0 0 1-4-10 15.3 15.3 0 0 1 4-10z"></path></svg>
                </div>
                <div class="manage-tab" data-tab="manage-gear" title="Account Settings">
                    <svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><circle cx="12" cy="12" r="3"></circle><path d="M19.4 15a1.65 1.65 0 0 0 .33 1.82l.06.06a2 2 0 0 1 0 2.83 2 2 0 0 1-2.83 0l-.06-.06a1.65 1.65 0 0 0-1.82-.33 1.65 1.65 0 0 0-1 1.51V21a2 2 0 0 1-2 2 2 2 0 0 1-2-2v-.09A1.65 1.65 0 0 0 9 19.4a1.65 1.65 0 0 0-1.82.33l-.06.06a2 2 0 0 1-2.83 0 2 2 0 0 1 0-2.83l.06-.06a1.65 1.65 0 0 0 .33-1.82 1.65 1.65 0 0 0-1.51-1H3a2 2 0 0 1-2-2 2 2 0 0 1 2-2h.09A1.65 1.65 0 0 0 4.6 9a1.65 1.65 0 0 0-.33-1.82l-.06-.06a2 2 0 0 1 0-2.83 2 2 0 0 1 2.83 0l.06.06a1.65 1.65 0 0 0 1.82.33H9a1.65 1.65 0 0 0 1-1.51V3a2 2 0 0 1 2-2 2 2 0 0 1 2 2v.09a1.65 1.65 0 0 0 1 1.51 1.65 1.65 0 0 0 1.82-.33l.06-.06a2 2 0 0 1 2.83 0 2 2 0 0 1 0 2.83l-.06.06a1.65 1.65 0 0 0-.33 1.82V9a1.65 1.65 0 0 0 1.51 1H21a2 2 0 0 1 2 2 2 2 0 0 1-2 2h-.09a1.65 1.65 0 0 0-1.51 1z"></path></svg>
                </div>
                
                <button class="manage-header-close" onclick="closeManageAccountModal()" style="position: absolute; right: 16px; top: 50%; transform: translateY(-50%); background: none; border: none; color: var(--text-muted); cursor: pointer; padding: 4px; border-radius: 4px; display: flex; align-items: center; justify-content: center; transition: all 0.2s;">
                    <svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><line x1="18" y1="6" x2="6" y2="18"></line><line x1="6" y1="6" x2="18" y2="18"></line></svg>
                </button>
            </div>
            
            <div class="manage-content" style="flex-grow: 1; padding: 32px 48px; overflow-y: auto; display: flex; flex-direction: column; position: relative;">
                <div id="manage-loading" style="display: flex; flex-direction: column; align-items: center; justify-content: center; position: absolute; inset: 0; background: var(--bg-card); z-index: 10;">
                    <svg class="spinner" width="40" height="40" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round" style="margin-bottom: 16px; color: var(--text-main);"><path d="M21 12a9 9 0 1 1-6.219-8.56"></path></svg>
                    <span data-i18n="lbl_fetching_details" style="color: var(--text-muted); font-weight: 500;">Fetching account details...</span>
                </div>
                
                <div id="manage-home" class="manage-page" style="display: none; flex-direction: column; gap: 32px;">
                    <div style="display: flex; align-items: center; gap: 20px;">
                        <img id="manage-avatar" src="" style="width: 90px; height: 90px; border-radius: 50%; border: 2px solid var(--border-color); object-fit: cover;">
                        <div style="display: flex; flex-direction: column; gap: 4px;">
                            <h2 id="manage-title-username" style="margin: 0; font-size: 28px; color: var(--text-main); font-weight: 700; letter-spacing: -0.5px;">Username</h2>
                            <div style="color: var(--text-muted); font-size: 15px; font-weight: 500;" data-i18n="lbl_account_overview">Account Overview</div>
                        </div>
                    </div>
                    
                    <div class="manage-overview-list" style="display: flex; flex-direction: column; gap: 0;">
                        <div class="overview-item" style="display: flex; align-items: center; justify-content: space-between; padding: 16px 12px; border-bottom: 1px solid var(--border-color);">
                            <div style="display: flex; align-items: center;">
                                <div class="overview-icon" style="color: var(--text-muted); margin-right: 16px; width: 24px; display: flex; justify-content: center;">
                                    <svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><path d="M20 21v-2a4 4 0 0 0-4-4H8a4 4 0 0 0-4 4v2"></path><circle cx="12" cy="7" r="4"></circle></svg>
                                </div>
                                <span class="overview-label" style="font-size: 14px; color: var(--text-muted); font-weight: 500;">Username</span>
                            </div>
                            <span class="overview-value" id="mo-username" style="font-size: 16px; color: var(--text-main); font-weight: 500;">-</span>
                        </div>
                        <div class="overview-item" style="display: flex; align-items: center; justify-content: space-between; padding: 16px 12px; border-bottom: 1px solid var(--border-color);">
                            <div style="display: flex; align-items: center;">
                                <div class="overview-icon" style="color: var(--text-muted); margin-right: 16px; width: 24px; display: flex; justify-content: center;">
                                    <svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><rect x="3" y="4" width="18" height="18" rx="2" ry="2"></rect><line x1="16" y1="2" x2="16" y2="6"></line><line x1="8" y1="2" x2="8" y2="6"></line><line x1="3" y1="10" x2="21" y2="10"></line></svg>
                                </div>
                                <span class="overview-label" data-i18n="lbl_display_name" style="font-size: 14px; color: var(--text-muted); font-weight: 500;">Display Name</span>
                            </div>
                            <span class="overview-value" id="mo-displayname" style="font-size: 16px; color: var(--text-main); font-weight: 500;">-</span>
                        </div>
                        <div class="overview-item" style="display: flex; align-items: center; justify-content: space-between; padding: 16px 12px; border-bottom: 1px solid var(--border-color);">
                            <div style="display: flex; align-items: center;">
                                <div class="overview-icon" style="color: var(--text-muted); margin-right: 16px; width: 24px; display: flex; justify-content: center;">
                                    <svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><circle cx="12" cy="12" r="10"></circle><line x1="12" y1="16" x2="12" y2="12"></line><line x1="12" y1="8" x2="12.01" y2="8"></line></svg>
                                </div>
                                <span class="overview-label" data-i18n="lbl_user_id" style="font-size: 14px; color: var(--text-muted); font-weight: 500;">User ID</span>
                            </div>
                            <span class="overview-value" id="mo-userid" style="font-size: 16px; color: var(--text-main); font-weight: 500;">-</span>
                        </div>
                        <div class="overview-item" style="display: flex; align-items: center; justify-content: space-between; padding: 16px 12px; border-bottom: 1px solid var(--border-color);">
                            <div style="display: flex; align-items: center;">
                                <div class="overview-icon" style="margin-right: 16px; width: 24px; display: flex; justify-content: center;">
                                    <img src="https://img.icons8.com/color/48/robux--v2.png" style="width: 26px; height: 26px; filter: grayscale(100%) brightness(0); -webkit-filter: grayscale(100%) brightness(0);" class="dynamic-icon"/>
                                </div>
                                <span class="overview-label" style="font-size: 14px; color: var(--text-muted); font-weight: 500;">Robux</span>
                            </div>
                            <span class="overview-value" id="mo-robux" style="font-size: 16px; color: var(--text-main); font-weight: 700;">-</span>
                        </div>
                        <div class="overview-item" style="display: flex; align-items: center; justify-content: space-between; padding: 16px 12px; border-bottom: 1px solid var(--border-color);">
                            <div style="display: flex; align-items: center;">
                                <div class="overview-icon" style="color: var(--text-muted); margin-right: 16px; width: 24px; display: flex; justify-content: center;">
                                    <svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><circle cx="12" cy="12" r="10"></circle><polyline points="12 6 12 12 16 14"></polyline></svg>
                                </div>
                                <span class="overview-label" data-i18n="lbl_account_age" style="font-size: 14px; color: var(--text-muted); font-weight: 500;">Account Age</span>
                            </div>
                            <span class="overview-value" id="mo-age" style="font-size: 16px; color: var(--text-main); font-weight: 500;">-</span>
                        </div>
                        <div class="overview-item" style="display: flex; align-items: center; justify-content: space-between; padding: 16px 12px;">
                            <div style="display: flex; align-items: center;">
                                <div class="overview-icon" style="margin-right: 16px; width: 24px; display: flex; justify-content: center;">
                                    <img src="data:image/png;base64,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" style="width: 22px; height: 22px; filter: grayscale(100%) brightness(0); -webkit-filter: grayscale(100%) brightness(0);" class="dynamic-icon" />
                                </div>
                                <span class="overview-label" style="font-size: 14px; color: var(--text-muted); font-weight: 500;">Roblox Plus</span>
                            </div>
                            <span class="overview-value" id="mo-premium" style="font-size: 16px; font-weight: 600;">-</span>
                        </div>
                    </div>
                </div>
                
                <div id="manage-placeholder" class="manage-page" style="display: none; align-items: center; justify-content: center; height: 100%; color: var(--text-muted);">
                    <div style="text-align: center;">
                        <svg width="48" height="48" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="1.5" stroke-linecap="round" stroke-linejoin="round" style="margin-bottom: 16px; opacity: 0.5;"><path d="M12 20h9"></path><path d="M16.5 3.5a2.121 2.121 0 0 1 3 3L7 19l-4 1 1-4L16.5 3.5z"></path></svg>
                        <h3 style="margin: 0 0 8px 0; color: var(--text-main);">Work in Progress</h3>
                        <p style="margin: 0; font-size: 14px;">This section is currently under development.</p>
                    </div>
                </div>
            </div>
        </div>
    </div>
<!-- Add Account Modal -->
    <div id="modal-add-account" class="modal" style="position: fixed; inset: 0; background: rgba(0,0,0,0.5); z-index: 1000; align-items: center; justify-content: center;">
        <div id="modal-add-account-content" style="background: var(--bg-card); border: 1px solid var(--border-color); border-radius: 12px; width: 400px; max-width: 90%; overflow: hidden; display: flex; flex-direction: column;">
            <div class="modal-header" style="padding: 20px; display: flex; justify-content: space-between; align-items: center;">
                <h2 style="margin: 0; font-size: 18px; font-weight: 600; display: flex; align-items: center; gap: 8px;">
                    <svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
                        <line x1="12" y1="5" x2="12" y2="19"></line>
                        <line x1="5" y1="12" x2="19" y2="12"></line>
                    </svg>
                    <span data-i18n="btn_add_account">Add Account</span>
                </h2>
                <button class="btn-icon" style="margin: -8px;" onclick="document.getElementById('modal-add-account').classList.remove('show');"><svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round"><line x1="18" y1="6" x2="6" y2="18"></line><line x1="6" y1="6" x2="18" y2="18"></line></svg></button>
            </div>
            <div style="padding: 20px; display: flex; gap: 12px;">
                <button class="btn-launch" id="btn-add-browser" style="flex: 1; padding: 20px; flex-direction: column; height: auto; border: 1px solid var(--border-color);" onclick="document.getElementById('modal-add-account').classList.remove('show');">
                    <svg width="32" height="32" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round" style="margin-bottom: 8px;"><circle cx="12" cy="12" r="10"></circle><line x1="2" y1="12" x2="22" y2="12"></line><path d="M12 2a15.3 15.3 0 0 1 4 10 15.3 15.3 0 0 1-4 10 15.3 15.3 0 0 1-4-10 15.3 15.3 0 0 1 4-10z"></path></svg>
                    <span data-i18n="btn_add_browser">Add via Browser</span>
                </button>
                <button class="btn-launch" id="btn-add-cookie" style="flex: 1; padding: 20px; flex-direction: column; height: auto; border: 1px solid var(--border-color);" onclick="document.getElementById('modal-add-account').classList.remove('show');">
                    <svg width="32" height="32" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round" style="margin-bottom: 8px;"><path d="M12 2a10 10 0 1 0 10 10 4 4 0 0 1-5-5 4 4 0 0 1-5-5"></path><path d="M8.5 8.5v.01"></path><path d="M16 12.5v.01"></path><path d="M12 16v.01"></path><path d="M11 12.5v.01"></path></svg>
                    <span data-i18n="btn_add_cookie">Add via Cookie</span>
                </button>
            </div>
        </div>
    </div>

    <!-- Cookie Login Modal -->
    <div id="cookie-modal" class="modal" style="position: fixed; inset: 0; background: rgba(0,0,0,0.5); z-index: 1000; align-items: center; justify-content: center;">
        <div id="cookie-modal-content" style="background: var(--bg-card); border: 1px solid var(--border-color); border-radius: 12px; width: 500px; max-width: 90%; overflow: hidden; display: flex; flex-direction: column;">
            <div class="modal-header" style="padding: 20px; display: flex; justify-content: space-between; align-items: center;">
                <h2 style="margin: 0; font-size: 18px; font-weight: 600; display: flex; align-items: center; gap: 8px;"><svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round"><path d="M21 2l-2 2m-7.61 7.61a5.5 5.5 0 1 1-7.778 7.778 5.5 5.5 0 0 1 7.777-7.777zm0 0L15.5 7.5m0 0l3 3L22 7l-3-3m-3.5 3.5L19 4"></path></svg><span data-i18n="lbl_add_via_cookie">Add Account via Cookie</span></h2>
                <button class="btn-icon" id="btn-close-cookie-modal" style="margin: -8px;" onclick="document.getElementById('cookie-modal').classList.remove('show'); document.getElementById('modal-add-account').classList.add('show');"><svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round"><line x1="18" y1="6" x2="6" y2="18"></line><line x1="6" y1="6" x2="18" y2="18"></line></svg></button>
            </div>
            <div style="padding: 20px;">
                <p style="color: var(--text-muted); margin-bottom: 16px; font-size: 14px;" data-i18n="desc_add_cookie_desc">Enter your Roblox .ROBLOSECURITY cookies below (one per line).</p>
                <textarea id="cookie-input" placeholder="_|WARNING:-DO-NOT-SHARE-THIS..." style="width: 100%; height: 100px; padding: 10px 14px; background: var(--bg-hover); border: 1px solid var(--border-color); border-radius: 12px; color: var(--text-main); outline: none; resize: none; font-family: monospace; font-size: 12px;"></textarea>
                <div style="margin-top: 12px; display: flex; align-items: flex-start; gap: 8px; color: var(--text-muted); font-size: 12px; background: rgba(255, 255, 255, 0.05); padding: 10px 12px; border-radius: 8px; border-left: 3px solid #60a5fa;">
                    <svg width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round" style="flex-shrink: 0; margin-top: 2px;"><circle cx="12" cy="12" r="10"></circle><line x1="12" y1="16" x2="12" y2="12"></line><line x1="12" y1="8" x2="12.01" y2="8"></line></svg>
                    <span data-i18n="desc_cookie_local_warning">Cookies are stored locally and encrypted on your device. For your security, do not share your cookies with anyone.</span>
                </div>
            </div>
            <div class="modal-footer" style="padding: 16px 20px; display: flex; justify-content: flex-end; gap: 12px;">
                <button class="btn-secondary" id="btn-cancel-cookie" style="padding: 8px 16px; background: rgba(255,255,255,0.1); border: none; border-radius: 12px; color: var(--text-main); cursor: pointer;" onclick="document.getElementById('cookie-modal').classList.remove('show'); document.getElementById('modal-add-account').classList.add('show');" data-i18n="btn_cancel">Cancel</button>
                <button class="btn-primary" id="btn-confirm-cookie" style="padding: 8px 16px; background: white; color: black; border: none; border-radius: 12px; font-weight: 600; cursor: pointer;">Login</button>
            </div>
        </div>
    </div>

    <!-- Rename Group Modal -->
    <div id="rename-group-modal" class="modal" style="position: fixed; inset: 0; background: rgba(0,0,0,0.5); z-index: 1000; align-items: center; justify-content: center; opacity: 0; pointer-events: none; transition: opacity 0.2s ease; display: flex; backdrop-filter: blur(4px);">
        <div id="rename-group-modal-content" style="background: var(--bg-card); border: 1px solid var(--border-color); border-radius: 12px; width: 400px; max-width: 90%; overflow: hidden; display: flex; flex-direction: column; transform: scale(0.95); transition: transform 0.2s ease;">
            <div class="modal-header" style="padding: 20px; display: flex; justify-content: space-between; align-items: center;">
                <h2 style="margin: 0; font-size: 18px; font-weight: 600; display: flex; align-items: center; gap: 8px;"><svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round"><path d="M11 4H4a2 2 0 0 0-2 2v14a2 2 0 0 0 2 2h14a2 2 0 0 0 2-2v-7"></path><path d="M18.5 2.5a2.121 2.121 0 0 1 3 3L12 15l-4 1 1-4 9.5-9.5z"></path></svg><span data-i18n="lbl_rename_group_modal">Rename Group</span></h2>
                <button class="btn-icon" onclick="document.getElementById('rename-group-modal').classList.remove('show');" style="margin: -8px;"><svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round"><line x1="18" y1="6" x2="6" y2="18"></line><line x1="6" y1="6" x2="18" y2="18"></line></svg></button>
            </div>
            <div style="padding: 20px;">
                <p style="color: var(--text-muted); margin-bottom: 16px; font-size: 14px;" data-i18n="desc_rename_group">Enter a new name for this group.</p>
                <input type="hidden" id="rename-group-old-name" />
                <input type="text" id="rename-group-name-input" placeholder="New Group Name" style="width: 100%; padding: 10px 14px; background: var(--bg-hover); border: 1px solid var(--border-color); border-radius: 12px; color: var(--text-main); outline: none; transition: border-color 0.2s;" />
            </div>
            <div class="modal-footer" style="padding: 16px 20px; display: flex; justify-content: flex-end; gap: 12px;">
                <button class="btn-secondary" onclick="document.getElementById('rename-group-modal').classList.remove('show');" style="padding: 8px 16px; background: rgba(255,255,255,0.1); border: none; border-radius: 12px; color: var(--text-main); cursor: pointer;" data-i18n="btn_cancel">Cancel</button>
                <button class="btn-primary" id="btn-confirm-rename-group" style="padding: 8px 16px; background: white; color: black; border: none; border-radius: 12px; font-weight: 600; cursor: pointer;">Save</button>
            </div>
        </div>
    </div>

    <!-- Delete Group Modal -->
    <div id="delete-group-modal" class="modal" style="position: fixed; inset: 0; background: rgba(0,0,0,0.5); z-index: 1000; align-items: center; justify-content: center; opacity: 0; pointer-events: none; transition: opacity 0.2s ease; display: flex; backdrop-filter: blur(4px);">
        <div id="delete-group-modal-content" style="background: var(--bg-card); border: 1px solid var(--border-color); border-radius: 12px; width: 400px; max-width: 90%; overflow: hidden; display: flex; flex-direction: column; transform: scale(0.95); transition: transform 0.2s ease;">
            <div class="modal-header" style="padding: 20px; display: flex; justify-content: space-between; align-items: center;">
                <h2 style="margin: 0; font-size: 18px; font-weight: 600; color: #ff5252; display: flex; align-items: center; gap: 8px;"><svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round"><polyline points="3 6 5 6 21 6"></polyline><path d="M19 6v14a2 2 0 0 1-2 2H7a2 2 0 0 1-2-2V6m3 0V4a2 2 0 0 1 2-2h4a2 2 0 0 1 2 2v2"></path></svg><span data-i18n="lbl_delete_group_modal">Delete Group</span></h2>
                <button class="btn-icon" onclick="document.getElementById('delete-group-modal').classList.remove('show');" style="margin: -8px;"><svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round"><line x1="18" y1="6" x2="6" y2="18"></line><line x1="6" y1="6" x2="18" y2="18"></line></svg></button>
            </div>
            <div style="padding: 20px;">
                <p style="color: var(--text-muted); margin-bottom: 0; font-size: 14px;"><span data-i18n="desc_delete_group_1">Are you sure you want to delete </span><strong id="delete-group-name-display" style="color: var(--text-main); font-weight: 600;"></strong><span data-i18n="desc_delete_group_2">? The accounts inside will become "Ungrouped". This action cannot be undone.</span></p>
                <input type="hidden" id="delete-group-name" />
            </div>
            <div class="modal-footer" style="padding: 16px 20px; display: flex; justify-content: flex-end; gap: 12px;">
                <button class="btn-secondary" onclick="document.getElementById('delete-group-modal').classList.remove('show');" style="padding: 8px 16px; background: rgba(255,255,255,0.1); border: none; border-radius: 12px; color: var(--text-main); cursor: pointer;" data-i18n="btn_cancel">Cancel</button>
                <button class="btn-primary danger" id="btn-confirm-delete-group" style="padding: 8px 16px; background: #ff5252; color: white; border: none; border-radius: 12px; font-weight: 600; cursor: pointer;">Delete</button>
            </div>
        </div>
    </div>

    <!-- Kill Instance Modal -->
    <div id="kill-modal" class="modal" style="position: fixed; inset: 0; background: rgba(0,0,0,0.5); z-index: 1000; align-items: center; justify-content: center; opacity: 0; pointer-events: none; transition: opacity 0.2s ease; display: flex; backdrop-filter: blur(4px);">
        <div id="kill-modal-content" style="background: var(--bg-card); border: 1px solid var(--border-color); border-radius: 12px; width: 400px; max-width: 90%; overflow: hidden; display: flex; flex-direction: column; transform: scale(0.95); transition: transform 0.2s ease;">
            <div class="modal-header" style="padding: 20px; display: flex; justify-content: space-between; align-items: center;">
                <h2 style="margin: 0; font-size: 18px; font-weight: 600; color: #ff5252; display: flex; align-items: center; gap: 8px;"><svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round"><path d="M18.36 6.64a9 9 0 1 1-12.73 0"></path><line x1="12" y1="2" x2="12" y2="12"></line></svg> Kill Instance</h2>
                <button class="btn-icon" onclick="document.getElementById('kill-modal').classList.remove('show');" style="margin: -8px;"><svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round"><line x1="18" y1="6" x2="6" y2="18"></line><line x1="6" y1="6" x2="18" y2="18"></line></svg></button>
            </div>
            <div style="padding: 20px;">
                <p style="color: var(--text-muted); margin-bottom: 0; font-size: 14px;">Are you sure you want to terminate the Roblox instance for <strong id="kill-account-name-display" style="color: var(--text-main); font-weight: 600;"></strong>? The game will close immediately.</p>
                <input type="hidden" id="kill-account-cookie" />
            </div>
            <div class="modal-footer" style="padding: 16px 20px; display: flex; justify-content: flex-end; gap: 12px;">
                <button class="btn-secondary" onclick="document.getElementById('kill-modal').classList.remove('show');" style="padding: 8px 16px; background: rgba(255,255,255,0.1); border: none; border-radius: 12px; color: var(--text-main); cursor: pointer;" data-i18n="btn_cancel">Cancel</button>
                <button class="btn-primary danger" id="btn-confirm-kill" style="padding: 8px 16px; background: #ff5252; color: white; border: none; border-radius: 12px; font-weight: 600; cursor: pointer;">Terminate</button>
            </div>
        </div>
    </div>

    <!-- Remove Account Modal -->
    <div id="remove-modal" class="modal" style="position: fixed; inset: 0; background: rgba(0,0,0,0.5); z-index: 1000; align-items: center; justify-content: center; opacity: 0; pointer-events: none; transition: opacity 0.2s ease; display: flex; backdrop-filter: blur(4px);">
        <div id="remove-modal-content" style="background: var(--bg-card); border: 1px solid var(--border-color); border-radius: 12px; width: 400px; max-width: 90%; overflow: hidden; display: flex; flex-direction: column; transform: scale(0.95); transition: transform 0.2s ease;">
            <div class="modal-header" style="padding: 20px; display: flex; justify-content: space-between; align-items: center;">
                <h2 style="margin: 0; font-size: 18px; font-weight: 600; color: #ff5252; display: flex; align-items: center; gap: 8px;"><svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round"><path d="M16 21v-2a4 4 0 0 0-4-4H5a4 4 0 0 0-4 4v2"></path><circle cx="8.5" cy="7" r="4"></circle><line x1="18" y1="8" x2="23" y2="13"></line><line x1="23" y1="8" x2="18" y2="13"></line></svg> Remove Account</h2>
                <button class="btn-icon" onclick="document.getElementById('remove-modal').classList.remove('show');" style="margin: -8px;"><svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round"><line x1="18" y1="6" x2="6" y2="18"></line><line x1="6" y1="6" x2="18" y2="18"></line></svg></button>
            </div>
            <div style="padding: 20px;">
                <p style="color: var(--text-muted); margin-bottom: 0; font-size: 14px;"><span data-i18n="desc_remove_account_1">Are you sure you want to remove </span><strong id="remove-account-name-display" style="color: var(--text-main); font-weight: 600;"></strong><span data-i18n="desc_remove_account_2"> from RoPilot? You will need to log back in to add it again.</span></p>
                <input type="hidden" id="remove-account-cookie" />
            </div>
            <div class="modal-footer" style="padding: 16px 20px; display: flex; justify-content: flex-end; gap: 12px;">
                <button class="btn-secondary" onclick="document.getElementById('remove-modal').classList.remove('show');" style="padding: 8px 16px; background: rgba(255,255,255,0.1); border: none; border-radius: 12px; color: var(--text-main); cursor: pointer;" data-i18n="btn_cancel">Cancel</button>
                <button class="btn-primary danger" id="btn-confirm-remove" style="padding: 8px 16px; background: #ff5252; color: white; border: none; border-radius: 12px; font-weight: 600; cursor: pointer;">Remove</button>
            </div>
        </div>
    </div>
    <!-- Kill All Modal -->
    <div id="kill-all-modal" class="modal" style="position: fixed; inset: 0; background: rgba(0,0,0,0.5); z-index: 1000; align-items: center; justify-content: center; opacity: 0; pointer-events: none; transition: opacity 0.2s ease; display: flex; backdrop-filter: blur(4px);">
        <div id="kill-all-modal-content" style="background: var(--bg-card); border: 1px solid var(--border-color); border-radius: 12px; width: 400px; max-width: 90%; overflow: hidden; display: flex; flex-direction: column; transform: scale(0.95); transition: transform 0.2s ease;">
            <div class="modal-header" style="padding: 20px; display: flex; justify-content: space-between; align-items: center;">
                <h2 id="kill-all-title" style="margin: 0; font-size: 18px; font-weight: 600; color: #ff5252; display: flex; align-items: center; gap: 8px;"><svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round"><path d="M10.29 3.86L1.82 18a2 2 0 0 0 1.71 3h16.94a2 2 0 0 0 1.71-3L13.71 3.86a2 2 0 0 0-3.42 0z"></path><line x1="12" y1="9" x2="12" y2="13"></line><line x1="12" y1="17" x2="12.01" y2="17"></line></svg> Running Instances Detected</h2>
                <button class="btn-icon" onclick="document.getElementById('kill-all-modal').classList.remove('show');" style="margin: -8px;"><svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round"><line x1="18" y1="6" x2="6" y2="18"></line><line x1="6" y1="6" x2="18" y2="18"></line></svg></button>
            </div>
            <div style="padding: 20px;">
                <p id="kill-all-desc" style="color: var(--text-muted); margin-bottom: 0; font-size: 14px;">To make multi-instance work properly, you must terminate all currently running Roblox instances before launching new ones. Do you want to terminate them now?</p>
            </div>
            <div class="modal-footer" style="padding: 16px 20px; display: flex; justify-content: flex-end; gap: 12px;">
                <button class="btn-secondary" onclick="document.getElementById('kill-all-modal').classList.remove('show');" style="padding: 8px 16px; background: rgba(255,255,255,0.1); border: none; border-radius: 12px; color: var(--text-main); cursor: pointer;" data-i18n="btn_no">No</button>
                <button class="btn-primary danger" id="btn-confirm-kill-all" style="padding: 8px 16px; background: #ff5252; color: white; border: none; border-radius: 12px; font-weight: 600; cursor: pointer;" data-i18n="btn_yes">Yes</button>
            </div>
        </div>
    </div>
    

    <!-- Toast Container -->
    <div id="toast-container" class="toast-container"></div>

<style>
    #rename-group-modal.show, #delete-group-modal.show, #kill-modal.show, #remove-modal.show, #kill-all-modal.show { opacity: 1 !important; pointer-events: auto !important; }
    #rename-group-modal.show #rename-group-modal-content, #delete-group-modal.show #delete-group-modal-content, #kill-modal.show #kill-modal-content, #remove-modal.show #remove-modal-content, #kill-all-modal.show #kill-all-modal-content { transform: scale(1) !important; }

        input[type=range] {
            -webkit-appearance: none;
            width: 100%;
            background: transparent;
        }
        input[type=range]::-webkit-slider-thumb {
            -webkit-appearance: none;
            height: 16px;
            width: 16px;
            border-radius: 50%;
            background: #4ade80;
            cursor: pointer;
            margin-top: -6px;
            box-shadow: 0 0 10px rgba(74, 222, 128, 0.4);
        }
        input[type=range]::-webkit-slider-runnable-track {
            width: 100%;
            height: 4px;
            cursor: pointer;
            background: rgba(255, 255, 255, 0.1);
            border-radius: 2px;
        }
        input[type=range]:focus {
            outline: none;
        }
    
        .modal-header { border-bottom: 1px solid var(--border-color); }
        .modal-footer { border-top: 1px solid var(--border-color); }
</style>
<script src="https://cdn.jsdelivr.net/npm/sortablejs@1.15.7/Sortable.min.js"></script>
<script>
            let accountsGrid = document.getElementById('accounts-grid');
            let currentAccounts = [];
            let currentGroups = [];
            let collapsedGroups = new Set();
            let currentSearchTerm = '';
            let currentAnalyticsSearchTerm = '';
            
            function escapeHtml(unsafe) {
                if (!unsafe) return "";
                if (typeof unsafe !== 'string') return String(unsafe);
                return unsafe
                     .replace(/&/g, "&amp;")
                     .replace(/</g, "&lt;")
                     .replace(/>/g, "&gt;")
                     .replace(/"/g, "&quot;")
                     .replace(/'/g, "&#039;");
            }

            function hashJobId(jobId) {
                if (!jobId || typeof jobId !== 'string') return translations[document.getElementById('setting-language')?.value || 'en']?.lbl_none || 'None';
                let hash = 0;
                for (let i = 0; i < jobId.length; i++) {
                    hash = ((hash << 5) - hash) + jobId.charCodeAt(i);
                    hash |= 0;
                }
                return '#' + Math.abs(hash).toString(16).substring(0, 6).toUpperCase();
            }
            
            window.renderAccounts = function(accounts) {
                try {
                    if (accounts.length === 0 && currentGroups.length === 0) {
                        accountsGrid.innerHTML = `
                            <div class="empty-state">
                                <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="1.5" stroke-linecap="round" stroke-linejoin="round"><path d="M17 21v-2a4 4 0 0 0-4-4H5a4 4 0 0 0-4 4v2"></path><circle cx="9" cy="7" r="4"></circle><path d="M23 21v-2a4 4 0 0 0-3-3.87"></path><path d="M16 3.13a4 4 0 0 1 0 7.75"></path></svg>
                                <h3 data-i18n="lbl_no_accounts" data-i18n="lbl_no_accounts">No Accounts Found</h3>
                                <p data-i18n="desc_empty_accounts">Click the "Add Account" button to add your Roblox account.</p>
                            </div>
                        `;
                        return;
                    }

                    let accountsStr = JSON.stringify(accounts.map(a => ({id: a.Id, uid: a.UserId, stat: a.Status, grp: a.Group, pid: a.ProcessId})));
                    let groupsStr = JSON.stringify(currentGroups);
                    let collapsedStr = JSON.stringify(Array.from(collapsedGroups));
                    
                    let canQuickUpdate = false;
                    if (window.lastRenderedAccountsString === accountsStr && 
                        window.lastRenderedGroupsString === groupsStr &&
                        window.lastRenderedCollapsedString === collapsedStr &&
                        window.lastRenderedSearchTerm === currentSearchTerm && 
                        window.lastRenderedAnalyticsSearchTerm === currentAnalyticsSearchTerm) {
                        canQuickUpdate = true;
                    }
                    
                    window.lastRenderedAccountsString = accountsStr;
                    window.lastRenderedGroupsString = groupsStr;
                    window.lastRenderedCollapsedString = collapsedStr;
                    window.lastRenderedSearchTerm = currentSearchTerm;
                    window.lastRenderedAnalyticsSearchTerm = currentAnalyticsSearchTerm;

                    if (canQuickUpdate) {
                        accounts.forEach(acc => {
                            let userId = acc.Id || acc.UserId || '0';
                            let instanceVal = document.getElementById(`instance-val-${userId}`);
                            if (instanceVal) {
                                instanceVal.innerText = (acc.ProcessId && acc.Status !== 0) ? acc.ProcessId.toString() : (translations[document.getElementById('setting-language')?.value || 'en']?.lbl_none || 'None');
                            }
                            let cpuBar = document.getElementById(`analytics-cpu-bar-${userId}`);
                            if (cpuBar) {
                                cpuBar.style.width = Math.min(acc.CpuUsage, 100) + '%';
                                cpuBar.style.background = acc.CpuUsage > 80 ? '#ff5252' : 'var(--accent-color)';
                            }
                            let cpuVal = document.getElementById(`analytics-cpu-val-${userId}`);
                            if (cpuVal) cpuVal.innerText = acc.CpuUsage.toFixed(1) + '%';
                            let ramBar = document.getElementById(`analytics-ram-bar-${userId}`);
                            if (ramBar) ramBar.style.width = Math.min((acc.RamUsage / 2048) * 100, 100) + '%';
                            let ramVal = document.getElementById(`analytics-ram-val-${userId}`);
                            if (ramVal) ramVal.innerText = Math.round(acc.RamUsage) + ' MB';
                            let uptimeVal = document.getElementById(`analytics-uptime-val-${userId}`);
                            if (uptimeVal) {
                                const hrs = Math.floor(acc.RuntimeSeconds / 3600).toString().padStart(2, '0');
                                const mins = Math.floor((acc.RuntimeSeconds % 3600) / 60).toString().padStart(2, '0');
                                const secs = (acc.RuntimeSeconds % 60).toString().padStart(2, '0');
                                uptimeVal.innerText = `${hrs}:${mins}:${secs}`;
                            }
                        });
                        return;
                    }

                    let filtered = accounts.filter(acc => {
                        if (!currentSearchTerm) return true;
                        let usernameMatch = acc.Username && acc.Username.toLowerCase().includes(currentSearchTerm);
                        let idMatch = acc.Id && String(acc.Id).includes(currentSearchTerm);
                        let userIdMatch = acc.UserId && String(acc.UserId).includes(currentSearchTerm);
                        let jobIdMatch = acc.JobId && acc.JobId.toLowerCase().includes(currentSearchTerm);
                        let groupMatch = acc.Group && acc.Group.toLowerCase().includes(currentSearchTerm);
                        return usernameMatch || idMatch || userIdMatch || jobIdMatch || groupMatch;
                    });

                    let isSearching = currentSearchTerm !== '';
                    let html = '';
                    if (isSearching && filtered.length === 0) {
                        html = `
                            <div class="empty-state">
                                <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="1.5" stroke-linecap="round" stroke-linejoin="round"><circle cx="11" cy="11" r="8"></circle><line x1="21" y1="21" x2="16.65" y2="16.65"></line></svg>
                                <h3>No Results</h3>
                                <p data-i18n="desc_no_accounts">No accounts match your current search.</p>
                            </div>
                        `;
                    } else {
                    
                    let groups = {};
                    currentGroups.forEach(g => { groups[g] = []; });
                    filtered.forEach(acc => {
                        let groupName = acc.Group || 'Ungrouped';
                        if (!groups[groupName]) groups[groupName] = [];
                        groups[groupName].push(acc);
                    });
                    

                    
                    html = '';
                    for (let groupName in groups) {
                        let groupAccounts = groups[groupName];
                        if (isSearching && groupAccounts.length === 0) continue;
                        
                        let isCollapsed = collapsedGroups.has(groupName);
                        if (isSearching && groupAccounts.length > 0) {
                            isCollapsed = false;
                        }
                        
                        html += `
                        <div class="group-container ${isCollapsed ? 'collapsed' : ''}" data-group="${escapeHtml(groupName)}">
                            <div class="group-header" onclick="window.toggleGroup(this.parentElement, '${escapeHtml(groupName)}')">
                                <div class="group-header-left">
                                    <svg class="chevron" width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round"><polyline points="6 9 12 15 18 9"></polyline></svg>
                                    <h3>${escapeHtml(groupName)}</h3>
                                    <span class="group-badge">${groupAccounts.length}</span>
                                </div>
                                <div class="group-actions">
                                    ${groupName !== 'Ungrouped' ? `
                                        <button class="btn-icon" onclick="event.stopPropagation(); window.renameGroup('${escapeHtml(groupName)}')"><svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><polygon points="16 3 21 8 8 21 3 21 3 16 16 3"></polygon></svg></button>
                                        <button class="btn-icon danger" onclick="event.stopPropagation(); window.deleteGroup('${escapeHtml(groupName)}')"><svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><polyline points="3 6 5 6 21 6"></polyline><path d="M19 6v14a2 2 0 0 1-2 2H7a2 2 0 0 1-2-2V6m3 0V4a2 2 0 0 1 2-2h4a2 2 0 0 1 2 2v2"></path></svg></button>
                                    ` : ''}
                                </div>
                            </div>
                            <div class="group-content-wrapper">
                                <div class="group-content">
                                    <div class="group-content-inner">
                        `;
                        
                        groupAccounts.forEach(acc => {
                            let statusColorClass = 'offline';
                            let statusText = 'Offline';
                            let statusValueClass = '';
                            
                            if (acc.Status === 2) {
                                statusColorClass = 'online';
                                statusText = 'Online';
                                statusValueClass = 'green';
                            } else if (acc.Status === 1) {
                                statusColorClass = 'online';
                                statusText = translations[document.getElementById('setting-language')?.value || 'en']?.lbl_launching || 'Launching';
                                statusValueClass = 'green';
                            } else if (acc.Status === 3) {
                                statusColorClass = 'danger';
                                statusText = translations[document.getElementById('setting-language')?.value || 'en']?.lbl_invalid || 'Invalid Cookie';
                                statusValueClass = 'red';
                            }
                                            
                            let instanceText = (acc.ProcessId && acc.Status !== 0) ? acc.ProcessId.toString() : (translations[document.getElementById('setting-language')?.value || 'en']?.lbl_none || 'None');
                            let avatarSrc = acc.ThumbnailUrl ? acc.ThumbnailUrl : "https://tr.rbxcdn.com/30DAY-AvatarHeadshot-15E6D8A279B926E6C5779D6BA1D97ACD-Bc/150/150/AvatarHeadshot/Png/noFilter";
                            let username = escapeHtml(acc.Username);
                            let userId = escapeHtml(acc.Id || acc.UserId || '0');
                            let cookie = escapeHtml(acc.Cookie);

                            html += `
                                                        <div class="card" data-cookie="${cookie}">
                                <button class="btn-manage-account" onclick="event.stopPropagation(); openManageAccountModal('${cookie}', '${userId}', '${avatarSrc}', '${escapeHtml(acc.Username)}')" title="Manage Account">
                                    <svg width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><circle cx="12" cy="12" r="3"></circle><path d="M19.4 15a1.65 1.65 0 0 0 .33 1.82l.06.06a2 2 0 0 1 0 2.83 2 2 0 0 1-2.83 0l-.06-.06a1.65 1.65 0 0 0-1.82-.33 1.65 1.65 0 0 0-1 1.51V21a2 2 0 0 1-2 2 2 2 0 0 1-2-2v-.09A1.65 1.65 0 0 0 9 19.4a1.65 1.65 0 0 0-1.82.33l-.06.06a2 2 0 0 1-2.83 0 2 2 0 0 1 0-2.83l.06-.06a1.65 1.65 0 0 0 .33-1.82 1.65 1.65 0 0 0-1.51-1H3a2 2 0 0 1-2-2 2 2 0 0 1 2-2h.09A1.65 1.65 0 0 0 4.6 9a1.65 1.65 0 0 0-.33-1.82l-.06-.06a2 2 0 0 1 0-2.83 2 2 0 0 1 2.83 0l.06.06a1.65 1.65 0 0 0 1.82.33H9a1.65 1.65 0 0 0 1-1.51V3a2 2 0 0 1 2-2 2 2 0 0 1 2 2v.09a1.65 1.65 0 0 0 1 1.51 1.65 1.65 0 0 0 1.82-.33l.06-.06a2 2 0 0 1 2.83 0 2 2 0 0 1 0 2.83l-.06.06a1.65 1.65 0 0 0-.33 1.82V9a1.65 1.65 0 0 0 1.51 1H21a2 2 0 0 1 2 2 2 2 0 0 1-2 2h-.09a1.65 1.65 0 0 0-1.51 1z"></path></svg>
                                </button>
                                <div class="card-header">
                                    <div class="avatar">
                                        <img src="${avatarSrc}" onerror="this.style.opacity=0.5"/>
                                        <div class="status-dot ${statusColorClass}"></div>
                                    </div>
                                    <div class="user-info">
                                        <span class="username">${document.getElementById('setting-hide-identity') && document.getElementById('setting-hide-identity').checked ? '******' : username}</span>
                                        <span class="userid">${document.getElementById('setting-hide-identity') && document.getElementById('setting-hide-identity').checked ? '******' : userId}</span>
                                    </div>
                                </div>
                                
                                <div class="card-stats">
                                    <div class="stat-box">
                                        <span class="stat-label">Status</span>
                                        <span class="stat-value ${statusValueClass}">${statusText}</span>
                                    </div>
                                    <div class="stat-box">
                                        <span class="stat-label" data-i18n="lbl_instance">Instance</span>
                                        <span class="stat-value" id="instance-val-${userId}">${escapeHtml(instanceText)}</span>
                                    </div>
                                </div>
                                
                                <div class="card-actions">

                                    ${acc.Status === 3 ?
                                        `<button class="btn-launch btn-relogin" onclick="event.stopPropagation(); document.getElementById('btn-add-account').click()">
                                            <svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><path d="M15 3h4a2 2 0 0 1 2 2v14a2 2 0 0 1-2 2h-4"/><polyline points="10 17 15 12 10 7"/><line x1="15" y1="12" x2="3" y2="12"/></svg>
                                            <span data-i18n="btn_relogin">Re-Login</span>
                                        </button>` :
                                        (acc.Status === 1 ? 
                                            `<button class="btn-launch" id="launch-${acc.Id || acc.UserId || cookie}" onclick="window.launchAccount('${cookie}', '${username}', this)">
                                                <svg class="spinner" width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round" style="flex-shrink: 0;"><path d="M21 12a9 9 0 1 1-6.219-8.56"></path></svg>
                                            </button>` : 
                                            `<button class="btn-launch" id="launch-${acc.Id || acc.UserId || cookie}" onclick="window.launchAccount('${cookie}', '${username}', this)"><svg width="14" height="14" viewBox="0 0 24 24" fill="currentColor"><path d="M5 3l14 9-14 9V3z"/></svg> <span data-i18n="btn_launch">Launch</span></button>`
                                        )
                                    }
                                    <button class="btn-icon danger" onclick="window.removeAccount('${cookie}', '${username}')">
                                        <svg width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><polyline points="3 6 5 6 21 6"></polyline><path d="M19 6v14a2 2 0 0 1-2 2H7a2 2 0 0 1-2-2V6m3 0V4a2 2 0 0 1 2-2h4a2 2 0 0 1 2 2v2"></path></svg>
                                    </button>
                                    ${acc.Status === 1 || acc.Status === 2 ? `<button class="btn-icon danger" onclick="window.killAccount('${cookie}', '${username}')"><svg width="16" height="16" viewBox="0 0 24 24" fill="currentColor"><path d="M19 6.41L17.59 5 L12 10.59 6.41 5 5 6.41 10.59 12 5 17.59 6.41 19 12 13.41 17.59 19 19 17.59 13.41 12z"/></svg></button>` : ''}
                                </div>
                            </div>`;
                        });
                        html += `
                                    </div>
                                </div>
                            </div>
                        </div>`;
                    }
                    } // close else block
                    
                    accountsGrid.innerHTML = html;
                    
                    if (window.groupSortable) window.groupSortable.destroy();
                    window.groupSortable = new Sortable(accountsGrid, {
                        animation: 150,
                        handle: '.group-header',
                        onEnd: function() { syncLayoutFromDOM(); }
                    });
                    
                    document.querySelectorAll('.group-content-inner').forEach(el => {
                        new Sortable(el, {
                            group: 'shared',
                            animation: 150,
                            delay: 150,
                            delayOnTouchOnly: true,
                            onEnd: function() { syncLayoutFromDOM(); }
                        });
                    });
                    
                    // Render Analytics Page
                    let analyticsGrid = document.getElementById('analytics-grid');
                    if (analyticsGrid) {
                        let activeAccs = accounts.filter(a => a.Status === 1 || a.Status === 2);
                        if (currentAnalyticsSearchTerm) {
                            activeAccs = activeAccs.filter(acc => {
                                let usernameMatch = acc.Username && acc.Username.toLowerCase().includes(currentAnalyticsSearchTerm);
                                let idMatch = acc.Id && String(acc.Id).includes(currentAnalyticsSearchTerm);
                                let userIdMatch = acc.UserId && String(acc.UserId).includes(currentAnalyticsSearchTerm);
                                let jobIdMatch = acc.JobId && acc.JobId.toLowerCase().includes(currentAnalyticsSearchTerm);
                                return usernameMatch || idMatch || userIdMatch || jobIdMatch;
                            });
                        }

                        if (activeAccs.length === 0) {
                            if (currentAnalyticsSearchTerm) {
                                analyticsGrid.innerHTML = `
                                <div class="empty-state">
                                    <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="1.5" stroke-linecap="round" stroke-linejoin="round" style="color: var(--text-muted); margin-bottom: 12px; width: 32px; height: 32px;"><circle cx="11" cy="11" r="8"></circle><line x1="21" y1="21" x2="16.65" y2="16.65"></line></svg>
                                    <h3>No Results</h3>
                                    <p>No active instances match your current search.</p>
                                </div>`;
                            } else {
                                analyticsGrid.innerHTML = `
                                <div class="empty-state">
                                    <svg width="32" height="32" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round" style="color: var(--text-muted); margin-bottom: 12px;"><path d="M3 3v18h18"></path><path d="M18.7 8l-5.1 5.2-2.8-2.7L7 14.3"></path></svg>
                                    <h3 data-i18n="lbl_no_active_instances">No Active Instances</h3>
                                    <p data-i18n="desc_accounts">Launch an account to see real-time analytics.</p>
                                </div>`;
                            }
                        } else {
                            let html = '';
                            activeAccs.forEach(acc => {
                                const hrs = Math.floor(acc.RuntimeSeconds / 3600).toString().padStart(2, '0');
                                const mins = Math.floor((acc.RuntimeSeconds % 3600) / 60).toString().padStart(2, '0');
                                const secs = (acc.RuntimeSeconds % 60).toString().padStart(2, '0');
                                
                                html += `
                                <div class="card">
                                    <div class="card-header" style="padding-bottom: 12px; border-bottom: 1px solid var(--border-color); margin-bottom: 12px;">
                                        <div class="avatar">
                                            <img src="${acc.ThumbnailUrl}" onerror="this.src='https://tr.rbxcdn.com/30DAY-AvatarHeadshot-15E6D8A279B926E6C5779D6BA1D97ACD-Bc/150/150/AvatarHeadshot/Png/noFilter'">
                                            <div class="status-dot green"></div>
                                        </div>
                                        <div class="user-info">
                                            <span class="username">${document.getElementById('setting-hide-identity') && document.getElementById('setting-hide-identity').checked ? '******' : escapeHtml(acc.Username)}</span>
                                            <span class="userid">PID: ${document.getElementById('setting-hide-identity') && document.getElementById('setting-hide-identity').checked ? '******' : (acc.ProcessId || 'Loading...')}</span>
                                        </div>
                                    </div>
                                    <div style="display: flex; flex-direction: column; gap: 12px;">
                                        <div style="display: flex; align-items: center; gap: 12px;">
                                            <div style="display: flex; align-items: center; gap: 8px; width: 60px;">
                                                <svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="var(--text-muted)" stroke-width="2"><rect x="4" y="4" width="16" height="16" rx="2" ry="2"></rect><rect x="9" y="9" width="6" height="6"></rect><line x1="9" y1="1" x2="9" y2="4"></line><line x1="15" y1="1" x2="15" y2="4"></line><line x1="9" y1="20" x2="9" y2="23"></line><line x1="15" y1="20" x2="15" y2="23"></line><line x1="20" y1="9" x2="23" y2="9"></line><line x1="20" y1="14" x2="23" y2="14"></line><line x1="1" y1="9" x2="4" y2="9"></line><line x1="1" y1="14" x2="4" y2="14"></line></svg>
                                                <span style="font-size: 12px; color: var(--text-muted); font-weight: 600;">CPU</span>
                                            </div>
                                            <div class="analytic-bar-bg" style="flex-grow: 1; height: 4px; background: rgba(255,255,255,0.1); border-radius: 2px; overflow: hidden;"><div id="analytics-cpu-bar-${acc.Id || acc.UserId}" style="width: ${Math.min(acc.CpuUsage, 100)}%; height: 100%; background: ${acc.CpuUsage > 80 ? '#ff5252' : 'var(--accent-color)'};"></div></div>
                                            <span id="analytics-cpu-val-${acc.Id || acc.UserId}" style="font-size: 13px; font-weight: 700; width: 45px; text-align: right;">${acc.CpuUsage.toFixed(1)}%</span>
                                        </div>
                                        <div style="display: flex; align-items: center; gap: 12px;">
                                            <div style="display: flex; align-items: center; gap: 8px; width: 60px;">
                                                <svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="var(--text-muted)" stroke-width="2"><rect x="2" y="3" width="20" height="18" rx="2" ry="2"></rect><line x1="2" y1="9" x2="22" y2="9"></line><line x1="2" y1="15" x2="22" y2="15"></line><line x1="6" y1="3" x2="6" y2="21"></line><line x1="10" y1="3" x2="10" y2="21"></line><line x1="14" y1="3" x2="14" y2="21"></line><line x1="18" y1="3" x2="18" y2="21"></line></svg>
                                                <span style="font-size: 12px; color: var(--text-muted); font-weight: 600;">RAM</span>
                                            </div>
                                            <div class="analytic-bar-bg" style="flex-grow: 1; height: 4px; background: rgba(255,255,255,0.1); border-radius: 2px; overflow: hidden;"><div id="analytics-ram-bar-${acc.Id || acc.UserId}" style="width: ${Math.min((acc.RamUsage / 2048) * 100, 100)}%; height: 100%; background: #a270ff;"></div></div>
                                            <span id="analytics-ram-val-${acc.Id || acc.UserId}" style="font-size: 13px; font-weight: 700; width: 55px; text-align: right;">${Math.round(acc.RamUsage)} MB</span>
                                        </div>
                                        <div style="display: flex; justify-content: space-between; align-items: center;">
                                            <div style="display: flex; align-items: center; gap: 8px;">
                                                <svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="var(--text-muted)" stroke-width="2"><circle cx="12" cy="12" r="10"></circle><polyline points="12 6 12 12 16 14"></polyline></svg>
                                                <span style="font-size: 12px; color: var(--text-muted); font-weight: 600;">Uptime</span>
                                            </div>
                                            <div style="display: flex; align-items: center; gap: 6px;">
                                                <div style="width: 6px; height: 6px; border-radius: 50%; background: #4caf50; box-shadow: 0 0 6px #4caf50; animation: pulse 2s infinite;"></div>
                                                <span id="analytics-uptime-val-${acc.Id || acc.UserId}" style="font-size: 13px; font-weight: 700; letter-spacing: 1px;">${hrs}:${mins}:${secs}</span>
                                            </div>
                                        </div>
                                    </div>
                                </div>`;
                            });
                            analyticsGrid.innerHTML = html;
                        }
                    }
                    let langInput = document.getElementById('setting-language');
                    if (langInput && langInput.value) applyLanguage(langInput.value);

                } catch (e) {
                    console.error("Error rendering accounts: ", e);
                    let errStr = escapeHtml(e.toString() + (e.stack ? '\\n' + e.stack : ''));
                    accountsGrid.innerHTML = `
                        <div class="empty-state">
                            <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="1.5" stroke-linecap="round" stroke-linejoin="round"><circle cx="12" cy="12" r="10"></circle><line x1="12" y1="8" x2="12" y2="12"></line><line x1="12" y1="16" x2="12.01" y2="16"></line></svg>
                            <h3>Error</h3>
                            <p>Failed to render accounts.</p>
                            <div style="position: relative; margin-top: 10px; width: 100%; max-width: 400px; margin-left: auto; margin-right: auto;">
                                <button onclick="navigator.clipboard.writeText(this.nextElementSibling.innerText); this.innerText='Copied!'; setTimeout(()=>this.innerText='Copy', 2000);" style="position: absolute; top: 8px; right: 8px; background: var(--border-color); color: var(--text-color); border: none; border-radius: 4px; padding: 4px 8px; font-size: 10px; cursor: pointer; font-weight: 600;">Copy</button>
                                <pre style="text-align: left; background: rgba(0,0,0,0.5); padding: 28px 12px 12px 12px; border-radius: 12px; font-size: 10px; max-width: 100%; overflow-x: auto; margin: 0; color: #ff5252;">${errStr}</pre>
                            </div>
                        </div>
                    `;
                }
            };
            
            window.toggleGroup = function(element, groupName) {
                if (element.classList.contains('collapsed')) {
                    element.classList.remove('collapsed');
                    collapsedGroups.delete(groupName);
                } else {
                    element.classList.add('collapsed');
                    collapsedGroups.add(groupName);
                }
            };
            
            window.removeAccount = function(cookie, username) {
                let modal = document.getElementById('remove-modal');
                if (modal) {
                    modal.classList.add('show');
                    document.getElementById('remove-account-cookie').value = cookie;
                    document.getElementById('remove-account-name-display').textContent = username || 'this account';
                }
            };
            
            window.killAccount = function(cookie, username) {
                let modal = document.getElementById('kill-modal');
                if (modal) {
                    modal.classList.add('show');
                    document.getElementById('kill-account-cookie').value = cookie;
                    document.getElementById('kill-account-name-display').textContent = username || 'this account';
                }
            };
            
            window.createGroup = function() {
                let modal = document.getElementById('group-modal');
                if (modal) {
                    modal.classList.add('show');
                    let input = document.getElementById('group-name-input');
                    if (input) {
                        input.value = '';
                        setTimeout(() => input.focus(), 50);
                    }
                }
            };
            
            window.renameGroup = function(oldName) {
                // unescape html just in case
                let div = document.createElement('div');
                div.innerHTML = oldName;
                oldName = div.textContent || div.innerText || '';

                let modal = document.getElementById('rename-group-modal');
                if (modal) {
                    modal.classList.add('show');
                    document.getElementById('rename-group-old-name').value = oldName;
                    let input = document.getElementById('rename-group-name-input');
                    if (input) {
                        input.value = oldName;
                        setTimeout(() => input.focus(), 50);
                    }
                }
            };
            
            window.deleteGroup = function(groupName) {
                let modal = document.getElementById('delete-group-modal');
                if (modal) {
                    modal.classList.add('show');
                    document.getElementById('delete-group-name').value = groupName;
                    document.getElementById('delete-group-name-display').textContent = groupName;
                }
            };
            
            function syncLayoutFromDOM() {
                let newGroups = [];
                let newAccounts = [];
                
                document.querySelectorAll('.group-container').forEach(groupEl => {
                    let groupName = groupEl.getAttribute('data-group');
                    if (groupName !== 'Ungrouped') newGroups.push(groupName);
                    
                    groupEl.querySelectorAll('.card').forEach(cardEl => {
                        let cookie = cardEl.getAttribute('data-cookie');
                        let acc = currentAccounts.find(a => a.Cookie === cookie);
                        if (acc) {
                            acc.Group = groupName;
                            newAccounts.push(acc);
                        }
                    });
                });
                
                // Ensure Ungrouped exists at the start
                if (!newGroups.includes('Ungrouped')) {
                    newGroups.unshift('Ungrouped');
                }
                
                currentGroups = newGroups;
                currentAccounts = newAccounts;
                syncLayout();
            }

            function syncLayout() {
                if(window.chrome && window.chrome.webview) {
                    window.chrome.webview.postMessage(JSON.stringify({
                        action: 'update_layout', 
                        accounts: currentAccounts.map(a => ({cookie: a.Cookie, group: a.Group})),
                        groups: currentGroups
                    }));
                }
                window.renderAccounts(currentAccounts);
            }


            window.updateAccounts = function(jsonStr) {
                try {
                    let payload = JSON.parse(jsonStr);
                    currentAccounts = payload.accounts || [];
                    let loadedGroups = payload.groups || [];
                    
                    loadedGroups.forEach(g => {
                        if (!currentGroups.includes(g)) {
                            collapsedGroups.add(g);
                        }
                    });
                    currentGroups = loadedGroups;
                    
                    if (window.renderAccounts) window.renderAccounts(currentAccounts);
                } catch (e) {
                    console.error(e);
                }
            };
            
            window.launchAccount = function(cookie, username, btnElement) {
                if (btnElement) {
                    btnElement.innerHTML = '<svg class="spinner" width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round" style="flex-shrink: 0;"><path d="M21 12a9 9 0 1 1-6.219-8.56"></path></svg>';
                }
                window.chrome.webview.postMessage(JSON.stringify({
                    action: 'launch',
                    cookie: cookie
                }));
            };
            

            document.getElementById('search-input').addEventListener('input', (e) => {
                currentSearchTerm = e.target.value.toLowerCase();
                if (window.renderAccounts) window.renderAccounts(currentAccounts);
            });
            
            document.getElementById('search-input-analytics').addEventListener('input', (e) => {
                currentAnalyticsSearchTerm = e.target.value.toLowerCase();
                if (window.renderAccounts) window.renderAccounts(currentAccounts);
            });
            
            document.getElementById('search-input-settings').addEventListener('input', (e) => {
                let term = e.target.value.toLowerCase();
                let categories = document.querySelectorAll('.settings-category');
                
                categories.forEach(category => {
                    let items = category.querySelectorAll('.setting-item');
                    let hasVisible = false;
                    items.forEach(item => {
                        let text = item.querySelector('.setting-title').innerText.toLowerCase();
                        let desc = item.querySelector('.setting-desc').innerText.toLowerCase();
                        if (text.includes(term) || desc.includes(term)) {
                            item.style.display = 'flex';
                            hasVisible = true;
                        } else {
                            item.style.display = 'none';
                        }
                    });
                    category.style.display = hasVisible ? 'block' : 'none';
                });
            });
            
            
            // UI Button Listeners
            let btnAddAccount = document.getElementById('btn-add-account');
            if (btnAddAccount) {
                btnAddAccount.addEventListener('click', (e) => {
                    e.stopPropagation();
                    let modal = document.getElementById('modal-add-account');
                    if (modal) modal.classList.add('show');
                });
            }

            let btnClose = document.getElementById('btn-close');
            if (btnClose) btnClose.addEventListener('click', () => window.chrome.webview.postMessage(JSON.stringify({action: 'close'})));
            
            let btnMinimize = document.getElementById('btn-minimize');
            if (btnMinimize) btnMinimize.addEventListener('click', () => window.chrome.webview.postMessage(JSON.stringify({action: 'minimize'})));
            
            let btnMaximize = document.getElementById('btn-maximize');
            if (btnMaximize) btnMaximize.addEventListener('click', () => window.chrome.webview.postMessage(JSON.stringify({action: 'maximize'})));

            let btnAddBrowser = document.getElementById('btn-add-browser');
            if (btnAddBrowser) {
                btnAddBrowser.addEventListener('click', () => {
                    window.chrome.webview.postMessage(JSON.stringify({ action: 'add_browser' }));
                });
            }



            let btnHamburger = document.getElementById('btn-hamburger');
            if (btnHamburger) {
                btnHamburger.addEventListener('click', () => {
                    let sb = document.getElementById('sidebar');
                    if (sb) {
                        sb.classList.toggle('collapsed');
                        if (typeof saveSettings === 'function') saveSettings(true);
                    }
                });
            }

            let navAccounts = document.getElementById('nav-accounts');
            let navAnalytics = document.getElementById('nav-analytics');
            let navSettings = document.getElementById('nav-settings');
            let pageAccounts = document.getElementById('page-accounts');
            let pageAnalytics = document.getElementById('page-analytics');
            let pageSettings = document.getElementById('page-settings');
            let pages = [pageAccounts, pageAnalytics, pageSettings];

            
            const translations = {
                "en": {
                    "nav_accounts": "Accounts",
                    "lbl_fetching_details": "Fetching account details",
                    "lbl_account_overview": "Account Overview",
                    "lbl_display_name": "Display Name",
                    "lbl_user_id": "User ID",
                    "lbl_account_age": "Account Age",
                    "lbl_hide_identity": "Hide Username & User ID",
                    "desc_hide_identity": "Censor your account's username and user ID",
                    "nav_settings": "Settings",
                    "btn_add_account": "Add Account",
                    "lbl_manage_accounts": "Manage Accounts",
                    "desc_accounts": "Launch an account to see real-time analytics",
                    "lbl_general": "General",
                    "lbl_always_on_top": "Always on Top",
                    "desc_always_on_top": "Keep the RoPilot window pinned on top of all other windows",
                    "lbl_run_on_startup": "Run on Startup",
                    "lbl_run_background": "Run in Background",
                    "desc_background": "Keep RoPilot running in the background when you close the window",
                    "lbl_auto_updates": "Automatic Updates",
                    "desc_auto_updates": "Automatically download and install new versions when you launch RoPilot",
                    "lbl_appearance": "Appearance",
                    "lbl_light_mode": "Light Mode",
                    "lbl_accent_color": "Accent Color",
                    "desc_accent_color": "Customize the primary highlight color of the UI",
                    "lbl_font": "Font",
                    "desc_font": "Change the global application font",
                    "lbl_language": "Language",
                    "desc_language": "Change the display language",
                                        "placeholder_search": "Search settings..",
                    "nav_analytics": "Analytics",
                    "lbl_no_accounts": "No Accounts Found",
                    "desc_no_accounts": "No accounts match your current search",
                    "lbl_fetching": "Fetching your accounts..",
                    "lbl_update": "Update Available",
                    "desc_update": "A new version of RoPilot is detected! Would you like to download and install it now?",
                    "btn_cancel": "Cancel",
                    "btn_update": "Update Now",
                    "lbl_kill_all": "Kill All Roblox Clients",
                    "desc_kill_all": "Are you sure you want to kill all active Roblox clients? The game will close immediately.",
                                        "btn_kill_all": "Kill All",
                    "lbl_minimize_tray": "Minimize to Tray on Close",
                    "lbl_hardware_accel": "Hardware Acceleration (UI)",
                    "lbl_resource_opt": "Resource Optimizer",
                    "lbl_cpu_limiter": "Background CPU Limiter",
                    "lbl_cpu_limit": "Background CPU Limit",
                                                            "lbl_no_active_instances": "No Active Instances",
                    "desc_accounts_page": "Manage and monitor your connected Roblox accounts",
                    "desc_settings": "Configure RoPilot preferences",
                    "desc_hardware_accel": "Use GPU to render the UI smoothly. Disable this to save GPU memory for Roblox.",
                    "desc_resource_opt": "Prioritize focused window and limit CPU/RAM usage of background instances",
                    "desc_add_cookie_desc": "Enter your Roblox .ROBLOSECURITY cookies below (one per line)",
                "desc_cookie_local_warning": "Cookies are stored locally and encrypted on your device. For your security, do not share your cookies with anyone.",
                    "btn_no": "No",
                    "btn_yes": "Yes",
                    "desc_empty_accounts": "Click the \"Add Account\" button to add your Roblox account",
                    "lbl_kill_all_instances": "Kill All Instances",
                    "desc_kill_all_instances": "Are you sure you want to terminate all currently running Roblox instances? All active games will be closed immediately.",
                    "lbl_running_instances": "Running Instances Detected",
                    "desc_running_instances": "To make multi-instance work properly, you must terminate all currently running Roblox instances before launching new ones. Do you want to terminate them now?",
                    "lbl_requires_restart": "(Requires App Restart)",
                    "lbl_appearance_header": "Appearance",
                    "placeholder_search_accounts": "Search accounts or groups..",
                    "desc_light_mode": "Use a lighter color scheme for the application",
                    "desc_startup": "Automatically launch RoPilot minimized in the system tray when Windows starts",
                    "lbl_performance": "Performance",
                    "lbl_add_via_cookie": "Add Account via Cookie",
                    "lbl_rename_group_modal": "Rename Group",
                    "lbl_delete_group_modal": "Delete Group",
                    "lbl_performance_analytics": "Performance Analytics",
                    "lbl_create_group_modal": "Create Group",
                    "desc_create_group": "Enter a name for the new group. You can drag and drop accounts into this group later.",
                    "desc_rename_group": "Enter a new name for this group",
                    "desc_delete_group_1": "Are you sure you want to delete ",
                    "desc_delete_group_2": "? The accounts inside will become \"Ungrouped\". This action cannot be undone.",
                    "desc_remove_account_1": "Are you sure you want to remove ",
                    "desc_remove_account_2": " from RoPilot? You will need to log back in to add it again.",
                    "btn_add_browser": "Add via Browser",
                    "btn_create_group": "Create",
                    "lbl_ui_scale": "UI Scale",
                    "desc_ui_scale": "Adjust the overall size of the user interface",
                    "lbl_window_opacity": "Window Opacity",
                    "desc_window_opacity": "Adjust the transparency of the entire application window",
                    "lbl_window_blur": "Background Blur",
                    "desc_window_blur": "Apply Acrylic blur effect to the background (Windows 10/11)",
                    "opt_scale_small": "Small",
                    "opt_scale_medium": "Medium",
                    "opt_scale_large": "Large",
                    "toast_fail_add_cookie": "Failed to add account. Invalid cookie?",
                    "toast_browser_cancel": "Browser login cancelled or failed.",
                    "toast_fail_add_accounts": "Failed to add accounts. Invalid or expired cookies?",
                    "toast_all_term": "All Roblox instances terminated.",
                    "toast_no_inst": "No instance is running.",
                    "toast_acc_removed": "Account removed.",
                    "toast_hw_accel": "Hardware Acceleration changed. Restart RoPilot to apply.",
                    "toast_settings_saved": "Settings saved successfully.",
                    "toast_inst_killed": "Instance killed.",
                    "toast_fail_kill": "Failed to kill instance (process not found?).",
                    "toast_no_active": "No active instance found.",
                    "toast_added_succ": " added successfully!",
                    "toast_already_run": " is already running!",
                    "toast_multi_added": " account(s) added successfully!",
                    "toast_multi_failed": " failed",
                    "lbl_downloading_update": "Downloading Update",
                    "desc_preparing_download": "Preparing download...",
                    "btn_install_now": "Install Now",
                    "btn_skip_now": "Skip for Now",
                    "lbl_whats_new": "What's New in RoPilot",
                    "lbl_installing_update": "Installing Update...",
                    "desc_restarting": "Please wait while RoPilot restarts.",
                    "lbl_ropilot": "RoPilot",
                    "lbl_is_now_available": "is now available!",
                    "lbl_instance": "Instance",
                    "btn_launch": "Launch",
                    "lbl_launching": "Launching",
                    "lbl_none": "None",
                    "desc_cpu_limiter_desc": "Throttles background instances to save CPU cycles",
                    "btn_add_cookie": "Add via Cookie",
                    "lbl_auto_kill": "Auto Kill on Exit",
                    "desc_auto_kill": "Automatically terminate all Roblox instances when RoPilot is fully closed",
                    "desc_analytics": "Real-time statistics for your running Roblox instances"
                },
                "id": {
                    "nav_accounts": "Akun",
                    "nav_settings": "Pengaturan",
                    "btn_add_account": "Tambah Akun",
                    "lbl_manage_accounts": "Kelola Akun",
                    "desc_accounts": "Luncurkan akun untuk melihat analitik langsung",
                    "lbl_general": "Umum",
                    "lbl_always_on_top": "Selalu di Atas",
                    "desc_always_on_top": "Pertahankan jendela RoPilot tetap di atas jendela lainnya",
                    "lbl_run_on_startup": "Jalankan saat Startup",
                    "lbl_run_background": "Jalankan di Latar Belakang",
                    "desc_background": "Biarkan RoPilot berjalan di latar belakang saat jendela ditutup",
                    "lbl_auto_updates": "Pembaruan Otomatis",
                    "desc_auto_updates": "Unduh dan instal versi baru secara otomatis saat meluncurkan RoPilot",
                    "lbl_fetching_details": "Mengambil rincian akun",
                    "lbl_account_overview": "Ringkasan Akun",
                    "lbl_display_name": "Nama Tampilan",
                    "lbl_user_id": "ID User",
                    "lbl_account_age": "Umur Akun",
                    "lbl_hide_identity": "Sembunyikan Username & ID",
                    "desc_hide_identity": "Sensor username dan user ID akun Anda",
                    "lbl_appearance": "Tampilan",
                    "lbl_light_mode": "Mode Terang",
                    "lbl_accent_color": "Warna Aksen",
                    "desc_accent_color": "Sesuaikan warna sorotan utama antarmuka pengguna",
                    "lbl_font": "Font",
                    "desc_font": "Ubah font utama aplikasi",
                    "lbl_language": "Bahasa",
                    "desc_language": "Ubah bahasa tampilan",
                                        "placeholder_search": "Cari pengaturan..",
                    "nav_analytics": "Analitik",
                    "lbl_no_accounts": "Tidak Ada Akun Ditemukan",
                    "desc_no_accounts": "Tidak ada akun yang cocok dengan pencarian Anda",
                    "lbl_fetching": "Mengambil akun Anda..",
                    "lbl_update": "Pembaruan Tersedia",
                    "desc_update": "Versi baru RoPilot terdeteksi! Apakah Anda ingin mengunduh dan menginstalnya sekarang?",
                    "btn_cancel": "Batal",
                    "btn_update": "Perbarui Sekarang",
                    "lbl_kill_all": "Tutup Semua Klien Roblox",
                    "desc_kill_all": "Apakah Anda yakin ingin menutup semua klien Roblox yang aktif? Permainan akan langsung ditutup.",
                                        "btn_kill_all": "Tutup Semua",
                    "lbl_minimize_tray": "Minimalkan ke Tray saat Ditutup",
                    "lbl_hardware_accel": "Akselerasi Perangkat Keras (UI)",
                    "lbl_resource_opt": "Pengoptimal Sumber Daya",
                    "lbl_cpu_limiter": "Pembatas CPU Latar Belakang",
                    "lbl_cpu_limit": "Batas CPU Latar Belakang",
                                                            "lbl_no_active_instances": "Tidak Ada Klien Aktif",
                    "desc_accounts_page": "Kelola dan pantau akun Roblox Anda yang terhubung",
                    "desc_settings": "Konfigurasi preferensi RoPilot",
                    "desc_hardware_accel": "Gunakan GPU untuk merender UI dengan mulus. Nonaktifkan ini untuk menghemat memori GPU bagi Roblox.",
                    "desc_resource_opt": "Prioritaskan jendela yang difokuskan dan batasi penggunaan CPU/RAM klien di latar belakang",
                    "desc_add_cookie_desc": "Masukkan cookies .ROBLOSECURITY Roblox Anda di bawah ini (satu per baris)",
                "desc_cookie_local_warning": "Cookies disimpan secara lokal dan dienkripsi di perangkat Anda. Demi keamanan, jangan pernah membagikan cookie Anda kepada siapa pun.",
                    "btn_no": "Tidak",
                    "btn_yes": "Ya",
                    "desc_empty_accounts": "Klik tombol \"Tambah Akun\" untuk menambahkan akun Roblox Anda",
                    "lbl_kill_all_instances": "Tutup Semua Klien",
                    "desc_kill_all_instances": "Apakah Anda yakin ingin menutup semua klien Roblox yang sedang berjalan? Semua permainan aktif akan langsung ditutup.",
                    "lbl_running_instances": "Klien Berjalan Terdeteksi",
                    "desc_running_instances": "Agar multi-instance berfungsi dengan baik, Anda harus menutup semua klien Roblox yang sedang berjalan sebelum meluncurkan yang baru. Apakah Anda ingin menutupnya sekarang?",
                    "lbl_requires_restart": "(Membutuhkan Mulai Ulang Aplikasi)",
                    "lbl_appearance_header": "Tampilan",
                    "placeholder_search_accounts": "Cari akun atau grup..",
                    "desc_light_mode": "Gunakan skema warna yang lebih terang untuk aplikasi",
                    "desc_startup": "Secara otomatis meluncurkan RoPilot dalam keadaan diminimalkan saat Windows dimulai",
                    "lbl_performance": "Performa",
                    "lbl_add_via_cookie": "Tambahkan via Cookie",
                    "lbl_rename_group_modal": "Ganti Nama Grup",
                    "lbl_delete_group_modal": "Hapus Grup",
                    "lbl_performance_analytics": "Analitik Performa",
                    "lbl_create_group_modal": "Buat Grup",
                    "desc_create_group": "Masukkan nama untuk grup baru. Anda dapat menarik dan menaruh akun ke grup ini nanti.",
                    "desc_rename_group": "Masukkan nama baru untuk grup ini",
                    "desc_delete_group_1": "Apakah Anda yakin ingin menghapus ",
                    "desc_delete_group_2": "? Akun-akun di dalamnya akan menjadi \"Tidak Digrupkan\". Tindakan ini tidak dapat dibatalkan.",
                    "desc_remove_account_1": "Apakah Anda yakin ingin menghapus ",
                    "desc_remove_account_2": " dari RoPilot? Anda perlu masuk kembali untuk menambahkannya lagi.",
                    "btn_add_browser": "Tambahkan via Browser",
                    "btn_create_group": "Buat",
                    "lbl_ui_scale": "Ukuran UI",
                    "desc_ui_scale": "Sesuaikan ukuran keseluruhan antarmuka pengguna",
                    "lbl_window_opacity": "Transparansi Jendela",
                    "desc_window_opacity": "Atur transparansi keseluruhan jendela aplikasi",
                    "lbl_window_blur": "Latar Belakang Blur",
                    "desc_window_blur": "Terapkan efek blur pada latar belakang (Windows 10/11)",
                    "opt_scale_small": "Kecil",
                    "opt_scale_medium": "Sedang",
                    "opt_scale_large": "Besar",
                    "toast_fail_add_cookie": "Gagal menambahkan akun. Cookie tidak valid?",
                    "toast_browser_cancel": "Login via browser dibatalkan atau gagal.",
                    "toast_fail_add_accounts": "Gagal menambahkan akun. Cookie tidak valid atau kedaluwarsa?",
                    "toast_all_term": "Semua klien Roblox telah ditutup.",
                    "toast_no_inst": "Tidak ada klien yang berjalan.",
                    "toast_acc_removed": "Akun dihapus.",
                    "toast_hw_accel": "Akselerasi Perangkat Keras diubah. Mulai ulang RoPilot untuk menerapkannya.",
                    "toast_settings_saved": "Pengaturan berhasil disimpan.",
                    "toast_inst_killed": "Klien dihentikan.",
                    "toast_fail_kill": "Gagal menghentikan klien (proses tidak ditemukan?).",
                    "toast_no_active": "Tidak ada klien aktif yang ditemukan.",
                    "toast_added_succ": " berhasil ditambahkan!",
                    "toast_already_run": " sudah berjalan!",
                    "toast_multi_added": " akun berhasil ditambahkan!",
                    "toast_multi_failed": " gagal",
                    "lbl_downloading_update": "Mengunduh Pembaruan",
                    "desc_preparing_download": "Menyiapkan unduhan...",
                    "btn_install_now": "Instal Sekarang",
                    "btn_skip_now": "Lewati Dulu",
                    "lbl_whats_new": "Apa yang Baru di RoPilot",
                    "lbl_installing_update": "Menginstal Pembaruan...",
                    "desc_restarting": "Harap tunggu sementara RoPilot dimulai ulang.",
                    "lbl_ropilot": "RoPilot",
                    "lbl_is_now_available": "kini telah tersedia!",
                    "lbl_instance": "Instance",
                    "btn_launch": "Luncurkan",
                    "lbl_launching": "Meluncurkan",
                    "lbl_none": "Tidak Ada",
                    "desc_cpu_limiter_desc": "Membatasi kecepatan klien di latar belakang untuk menghemat siklus CPU",
                    "btn_add_cookie": "Tambahkan via Cookie",
                    "lbl_auto_kill": "Tutup Otomatis saat Keluar",
                    "desc_auto_kill": "Secara otomatis menutup semua klien Roblox saat RoPilot ditutup sepenuhnya",
                    "desc_analytics": "Statistik waktu nyata untuk klien Roblox Anda yang sedang berjalan"
                }
            };
            
            function applyLanguage(lang) {
                if (!translations[lang]) lang = "en";
                const dict = translations[lang];
                document.querySelectorAll('[data-i18n]').forEach(el => {
                    const key = el.getAttribute('data-i18n');
                    if (dict[key]) {
                        if (el.tagName === 'INPUT' && el.type === 'text') {
                            el.placeholder = dict[key];
                        } else {
                            el.textContent = dict[key];
                        }
                    }
                });
            }

            function switchPage(activeNav, activePage) {
                [navAccounts, navAnalytics, navSettings].forEach(nav => {
                    if (nav) nav.classList.remove('active');
                });
                if (activeNav) activeNav.classList.add('active');

                pages.forEach(p => {
                    if (p) p.classList.remove('active');
                });
                if (activePage) {
                    activePage.classList.add('active');
                }
            }

            if (navAccounts) {
                navAccounts.addEventListener('click', () => {
                    switchPage(navAccounts, pageAccounts);
                });
            }
            if (navAnalytics) {
                navAnalytics.addEventListener('click', () => {
                    switchPage(navAnalytics, pageAnalytics);
                });
            }
            if (navSettings) {
                navSettings.addEventListener('click', () => {
                    switchPage(navSettings, pageSettings);
                });
            }

            let btnAddCookie = document.getElementById('btn-add-cookie');
            if (btnAddCookie) {
                btnAddCookie.addEventListener('click', () => {
                    let modal = document.getElementById('cookie-modal');
                    if (modal) {
                        modal.classList.add('show');
                        let input = document.getElementById('cookie-input');
                        if (input) {
                            input.value = '';
                            input.focus();
                        }
                    }
                });
            }

            let btnConfirmCookie = document.getElementById('btn-confirm-cookie');
            if (btnConfirmCookie) {
                btnConfirmCookie.addEventListener('click', () => {
                    let input = document.getElementById('cookie-input');
                    let val = input ? input.value : '';
                    let cookies = [];
                    if (val.includes('_|WARNING:-DO-NOT-SHARE-THIS')) {
                        let parts = val.split('_|WARNING:-DO-NOT-SHARE-THIS');
                        for (let i = 1; i < parts.length; i++) {
                            let c = parts[i].split(/[\r\n,;\t ]/)[0];
                            if (c) {
                                cookies.push('_|WARNING:-DO-NOT-SHARE-THIS' + c);
                            }
                        }
                    } else {
                        let lines = val.split(/[\r\n,;\t ]+/);
                        for (let line of lines) {
                            let c = line.trim();
                            if (c) cookies.push(c);
                        }
                    }
                    if (cookies.length > 0) {
                        window.chrome.webview.postMessage(JSON.stringify({ action: 'add_cookie_bulk', cookies: cookies }));
                    }
                    document.getElementById('cookie-modal').classList.remove('show');
                });
            }

            let btnCreateGroup = document.getElementById('btn-create-group');
            if (btnCreateGroup) {
                btnCreateGroup.addEventListener('click', window.createGroup);
            }

            let btnConfirmGroup = document.getElementById('btn-confirm-group');
            if (btnConfirmGroup) {
                btnConfirmGroup.addEventListener('click', () => {
                    let input = document.getElementById('group-name-input');
                    let name = input ? input.value.trim() : '';
                    if (name !== '') {
                        if (!currentGroups.includes(name)) {
                            currentGroups.push(name);
                            syncLayout();
                        }
                    }
                    let modal = document.getElementById('group-modal');
                    if (modal) modal.classList.remove('show');
                });
            }

            let btnConfirmRenameGroup = document.getElementById('btn-confirm-rename-group');
            if (btnConfirmRenameGroup) {
                btnConfirmRenameGroup.addEventListener('click', () => {
                    let oldName = document.getElementById('rename-group-old-name').value;
                    let newName = document.getElementById('rename-group-name-input').value.trim();
                    
                    if (newName !== '' && newName !== oldName) {
                        currentAccounts.forEach(acc => {
                            if (acc.Group === oldName) acc.Group = newName;
                        });
                        
                        let idx = currentGroups.indexOf(oldName);
                        if (idx > -1) {
                            currentGroups[idx] = newName;
                        }
                        
                        currentGroups = currentGroups.filter(g => g !== oldName);
                        if (!currentGroups.includes(newName)) {
                            currentGroups.push(newName);
                        }
                        currentGroups = [...new Set(currentGroups)];
                        
                        syncLayout();
                    }
                    
                    document.getElementById('rename-group-modal').classList.remove('show');
                });
            }

            let btnConfirmKill = document.getElementById('btn-confirm-kill');
            if (btnConfirmKill) {
                btnConfirmKill.addEventListener('click', () => {
                    let cookie = document.getElementById('kill-account-cookie').value;
                    window.chrome.webview.postMessage(JSON.stringify({
                        action: 'kill',
                        cookie: cookie
                    }));
                    document.getElementById('kill-modal').classList.remove('show');
                });
            }
            
            let btnConfirmDeleteGroup = document.getElementById('btn-confirm-delete-group');
            if (btnConfirmDeleteGroup) {
                btnConfirmDeleteGroup.addEventListener('click', () => {
                    let groupName = document.getElementById('delete-group-name').value;
                    
                    currentAccounts.forEach(acc => {
                        if (acc.Group === groupName) acc.Group = 'Ungrouped';
                    });
                    
                    let idx = currentGroups.indexOf(groupName);
                    if (idx > -1) {
                        currentGroups.splice(idx, 1);
                    }
                    
                    syncLayout();
                    
                    document.getElementById('delete-group-modal').classList.remove('show');
                });
            }
            let btnConfirmRemove = document.getElementById('btn-confirm-remove');
            if (btnConfirmRemove) {
                btnConfirmRemove.addEventListener('click', () => {
                    let cookie = document.getElementById('remove-account-cookie').value;
                    window.chrome.webview.postMessage(JSON.stringify({
                        action: 'remove',
                        cookie: cookie
                    }));
                    document.getElementById('remove-modal').classList.remove('show');
                });
            }

            let btnConfirmKillAll = document.getElementById('btn-confirm-kill-all');
            if (btnConfirmKillAll) {
                btnConfirmKillAll.addEventListener('click', () => {
                    window.chrome.webview.postMessage(JSON.stringify({ action: 'kill_all' }));
                    document.getElementById('kill-all-modal').classList.remove('show');
                });
            }

            window.showStatus = function(msg, isError) {
                let container = document.getElementById('toast-container');
                if (!container) return;
                
                let translatedMsg = msg;
                let lang = document.getElementById('setting-language')?.value || 'en';
                let dict = translations[lang] || translations['en'];
                if (dict) {
                    if (msg === "Failed to add account. Invalid cookie?") translatedMsg = dict.toast_fail_add_cookie || msg;
                    else if (msg === "Browser login cancelled or failed.") translatedMsg = dict.toast_browser_cancel || msg;
                    else if (msg === "Failed to add accounts. Invalid or expired cookies?") translatedMsg = dict.toast_fail_add_accounts || msg;
                    else if (msg === "All Roblox instances terminated.") translatedMsg = dict.toast_all_term || msg;
                    else if (msg === "No instance is running.") translatedMsg = dict.toast_no_inst || msg;
                    else if (msg === "Account removed.") translatedMsg = dict.toast_acc_removed || msg;
                    else if (msg === "Hardware Acceleration changed. Restart RoPilot to apply.") translatedMsg = dict.toast_hw_accel || msg;
                    else if (msg === "Settings saved successfully.") translatedMsg = dict.toast_settings_saved || msg;
                    else if (msg === "Instance killed.") translatedMsg = dict.toast_inst_killed || msg;
                    else if (msg === "Failed to kill instance (process not found?).") translatedMsg = dict.toast_fail_kill || msg;
                    else if (msg === "No active instance found.") translatedMsg = dict.toast_no_active || msg;
                    else {
                        let multiAddedMatch = msg.match(/^(\d+) account\(s\) added successfully!(?: \((\d+) failed\))?$/);
                        if (multiAddedMatch) {
                            translatedMsg = multiAddedMatch[1] + (dict.toast_multi_added || " account(s) added successfully!");
                            if (multiAddedMatch[2]) {
                                translatedMsg += " (" + multiAddedMatch[2] + (dict.toast_multi_failed || " failed") + ")";
                            }
                        } else {
                            let addedSuccMatch = msg.match(/^(.*) added successfully!$/);
                            if (addedSuccMatch) {
                                translatedMsg = addedSuccMatch[1] + (dict.toast_added_succ || " added successfully!");
                            } else {
                                let alreadyRunMatch = msg.match(/^(.*) is already running!$/);
                                if (alreadyRunMatch) {
                                    translatedMsg = alreadyRunMatch[1] + (dict.toast_already_run || " is already running!");
                                }
                            }
                        }
                    }
                }
                
                let toast = document.createElement('div');
                toast.className = 'toast ' + (isError ? 'error' : 'success');
                toast.innerHTML = `
                    <svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="${isError ? '#f87171' : '#4ade80'}" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
                        ${isError ? '<circle cx="12" cy="12" r="10"></circle><line x1="12" y1="8" x2="12" y2="12"></line><line x1="12" y1="16" x2="12.01" y2="16"></line>' : '<path d="M22 11.08V12a10 10 0 1 1-5.93-9.14"></path><polyline points="22 4 12 14.01 9 11.01"></polyline>'}
                    </svg>
                    <span>${escapeHtml(translatedMsg)}</span>
                `;
                
                container.appendChild(toast);
                
                // Trigger animation
                requestAnimationFrame(() => {
                    toast.classList.add('show');
                });
                
                setTimeout(() => {
                    toast.classList.remove('show');
                    setTimeout(() => {
                        if (toast.parentElement) {
                            toast.parentElement.removeChild(toast);
                        }
                    }, 300);
                }, 3000);
            };

            window.showKillAllPrompt = function(isManual = false) {
                let modal = document.getElementById('kill-all-modal');
                let title = document.getElementById('kill-all-title');
                let desc = document.getElementById('kill-all-desc');
                
                if (title && desc) {
                    if (isManual) {
                        title.innerHTML = '<svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round"><path d="M10.29 3.86L1.82 18a2 2 0 0 0 1.71 3h16.94a2 2 0 0 0 1.71-3L13.71 3.86a2 2 0 0 0-3.42 0z"></path><line x1="12" y1="9" x2="12" y2="13"></line><line x1="12" y1="17" x2="12.01" y2="17"></line></svg> <span data-i18n="lbl_kill_all_instances">Kill All Instances</span>';
                        desc.setAttribute('data-i18n', 'desc_kill_all_instances'); desc.textContent = 'Are you sure you want to terminate all currently running Roblox instances? All active games will be closed immediately.';
                    } else {
                        title.innerHTML = '<svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round"><path d="M10.29 3.86L1.82 18a2 2 0 0 0 1.71 3h16.94a2 2 0 0 0 1.71-3L13.71 3.86a2 2 0 0 0-3.42 0z"></path><line x1="12" y1="9" x2="12" y2="13"></line><line x1="12" y1="17" x2="12.01" y2="17"></line></svg> <span data-i18n="lbl_running_instances">Running Instances Detected</span>';
                        desc.setAttribute('data-i18n', 'desc_running_instances'); desc.textContent = 'To make multi-instance work properly, you must terminate all currently running Roblox instances before launching new ones. Do you want to terminate them now?';
                    }
                }
                
                if (modal) modal.classList.add('show');
                let _lang = document.getElementById('setting-language'); if (_lang && _lang.value) applyLanguage(_lang.value);
            };

            // Drag window
            let dragArea = document.getElementById('drag-area');
            if (dragArea) {
                dragArea.addEventListener('mousedown', (e) => {
                    if (e.target.tagName !== 'BUTTON') {
                        window.chrome.webview.postMessage(JSON.stringify({ action: 'drag' }));
                    }
                });
            }

            
            let uiScaleSlider = document.getElementById('setting-ui-scale');
            let uiScaleVal = document.getElementById('ui-scale-val');
            if (uiScaleSlider && uiScaleVal) {
                uiScaleSlider.addEventListener('input', (e) => {
                    let val = parseFloat(e.target.value);
                    uiScaleVal.textContent = Math.round(val * 100) + '%';
                    document.documentElement.style.setProperty('--ui-scale', val);
                });
                uiScaleSlider.addEventListener('change', (e) => {
                    saveSettings();
                });
            }
            let lightModeToggle = document.getElementById('setting-light-mode');
            let accentColorInput = document.getElementById('setting-accent-color');
            let fontFamilyInput = document.getElementById('setting-font-family');
            
            if (accentColorInput) {
                accentColorInput.addEventListener('input', (e) => {
                    document.documentElement.style.setProperty('--accent-color', e.target.value);
                    document.documentElement.style.setProperty('--accent-tint-hover', e.target.value + '26');
                    document.documentElement.style.setProperty('--accent-tint-active', e.target.value + '40');
                });
                accentColorInput.addEventListener('change', (e) => {
                    accentColorInput.dataset.reset = 'false';
                    saveSettings();
                });
            }
            if (fontFamilyInput) {
                fontFamilyInput.addEventListener('change', (e) => {
                    document.documentElement.style.setProperty('--font-family', e.target.value);
                    saveSettings();
                });
            }
            
            let fDropdownSel = document.getElementById('font-dropdown-selected');
            let fDropdownOpts = document.getElementById('font-dropdown-options');
            let fDropdownText = document.getElementById('font-dropdown-text');
            
            let lDropdownSel = document.getElementById('lang-dropdown-selected');
            let lDropdownOpts = document.getElementById('lang-dropdown-options');
            let lDropdownText = document.getElementById('lang-dropdown-text');
            let langInput = document.getElementById('setting-language');
            
            if (lDropdownSel && lDropdownOpts && langInput) {
                lDropdownSel.addEventListener('click', (e) => {
                    lDropdownOpts.classList.toggle('show');
                    if(fDropdownOpts) fDropdownOpts.classList.remove('show');
                    e.stopPropagation();
                });
                document.addEventListener('click', () => {
                    lDropdownOpts.classList.remove('show');
                });
                lDropdownOpts.querySelectorAll('.dropdown-option').forEach(opt => {
                    opt.addEventListener('click', (e) => {
                        lDropdownText.textContent = opt.textContent;
                        langInput.value = opt.getAttribute('data-value');
                        applyLanguage(langInput.value);
                        saveSettings();
                        lDropdownOpts.classList.remove('show');
                        e.stopPropagation();
                    });
                });
            }
            if (fDropdownSel && fDropdownOpts && fontFamilyInput) {
                fDropdownSel.addEventListener('click', (e) => {
                    fDropdownOpts.classList.toggle('show');
                    if(lDropdownOpts) lDropdownOpts.classList.remove('show');
                    e.stopPropagation();
                });
                document.addEventListener('click', () => {
                    fDropdownOpts.classList.remove('show');
                });
                fDropdownOpts.querySelectorAll('.dropdown-option').forEach(opt => {
                    opt.addEventListener('click', (e) => {
                        fDropdownText.textContent = opt.textContent;
                        fontFamilyInput.value = opt.getAttribute('data-value');
                        fontFamilyInput.dispatchEvent(new Event('change'));
                        fDropdownOpts.classList.remove('show');
                        e.stopPropagation();
                    });
                });
            }
            const icon_black_b64 = "data:image/png;base64," + "iVBORw0KGgoAAAANSUhEUgAAAgAAAAIACAYAAAD0eNT6AAAABGdBTUEAALGPC/xhBQAACktpQ0NQc1JHQiBJRUM2MTk2Ni0yLjEAAEiJnVNnVFPpFj333vRCS4iAlEtvUhUIIFJCi4BUaaISkgChhBgSQOyIqMCIoiKCFRkUccDREZCxIoqFQbH3AXkIKOPgKDZU3g/eGn2z5r03b/avvfY5Z53vnH0+AEZgsESahaoBZEoV8ogAHzw2Lh4ndwMKVCCBA4BAmC0LifSPAgDg+/Hw7IgAH/gCBODNbUAAAG7YBIbhOPx/UBfK5AoAJAwApovE2UIApBAAMnIVMgUAMgoA7KR0mQIAJQAAWx4bFw+AagEAO2WSTwMAdtIk9wIAtihTKgJAowBAJsoUiQDQDgBYl6MUiwCwYAAoypGIcwGwmwBgkqHMlABg7wCAnSkWZAMQGABgohALUwEI9gDAkEdF8AAIMwEojJSveNJXXCHOUwAA8LJki+WSlFQFbiG0xB1cXbl4oDg3Q6xQ2IQJhOkCuQjnZWXKBNLFAJMzAwCARnZEgA/O9+M5O7g6O9s42jp8taj/GvyLiI2L/5c/r8IBAQCE0/VF+7O8rBoA7hgAtvGLlrQdoGUNgNb9L5rJHgDVQoDmq1/Nw+H78fBUhULmZmeXm5trKxELbYWpX/X5nwl/AV/1s+X78fDf14P7ipMFygwFHhHggwuzMrKUcjxbJhCKcZs/HvHfLvzzd0yLECeL5WKpUIxHS8S5EmkKzsuSiiQKSZYUl0j/k4l/s+wPmLxrAGDVfgb2QltQu8oG7JcuILDogCXsAgDkd9+CqdEQBgAxBoOTdw8AMPmb/x1oGQCg2ZIUHACAFxGFC5XynMkYAQCACDRQBTZogz4YgwXYgCO4gDt4gR/MhlCIgjhYAEJIhUyQQy4shVVQBCWwEbZCFeyGWqiHRjgCLXACzsIFuALX4BY8gF4YgOcwCm9gHEEQMsJEWIg2YoCYItaII8JFZiF+SDASgcQhiUgKIkWUyFJkNVKClCNVyF6kHvkeOY6cRS4hPcg9pA8ZRn5DPqAYykDZqB5qhtqhXNQbDUKj0PloCroIzUcL0Q1oJVqDHkKb0bPoFfQW2os+R8cwwOgYBzPEbDAuxsNCsXgsGZNjy7FirAKrwRqxNqwTu4H1YiPYewKJwCLgBBuCOyGQMJcgJCwiLCeUEqoIBwjNhA7CDUIfYZTwmcgk6hKtiW5EPjGWmELMJRYRK4h1xGPE88RbxAHiGxKJxCGZk1xIgaQ4UhppCamUtJPURDpD6iH1k8bIZLI22ZrsQQ4lC8gKchF5O/kQ+TT5OnmA/I5CpxhQHCn+lHiKlFJAqaAcpJyiXKcMUsapalRTqhs1lCqiLqaWUWupbdSr1AHqOE2dZk7zoEXR0miraJW0Rtp52kPaKzqdbkR3pYfTJfSV9Er6YfpFeh/9PUODYcXgMRIYSsYGxn7GGcY9xismk2nG9GLGMxXMDcx65jnmY+Y7FZaKrQpfRaSyQqVapVnlusoLVaqqqaq36gLVfNUK1aOqV1VH1KhqZmo8NYHacrVqteNqd9TG1FnqDuqh6pnqpeoH1S+pD2mQNcw0/DREGoUa+zTOafSzMJYxi8cSslazalnnWQNsEtuczWensUvY37G72aOaGpozNKM18zSrNU9q9nIwjhmHz8nglHGOcG5zPkzRm+I9RTxl/ZTGKdenvNWaquWlJdYq1mrSuqX1QRvX9tNO196k3aL9SIegY6UTrpOrs0vnvM7IVPZU96nCqcVTj0y9r4vqWulG6C7R3afbpTump68XoCfT2653Tm9En6PvpZ+mv0X/lP6wActgloHEYIvBaYNnuCbujWfglXgHPmqoaxhoqDTca9htOG5kbjTXqMCoyeiRMc2Ya5xsvMW43XjUxMAkxGSpSYPJfVOqKdc01XSbaafpWzNzsxiztWYtZkPmWuZ883zzBvOHFkwLT4tFFjUWNy1JllzLdMudltesUCsnq1Sraqur1qi1s7XEeqd1zzTiNNdp0mk10+7YMGy8bXJsGmz6bDm2wbYFti22L+xM7OLtNtl12n22d7LPsK+1f+Cg4TDbocChzeE3RytHoWO1483pzOn+01dMb53+cob1DPGMXTPuOrGcQpzWOrU7fXJ2cZY7NzoPu5i4JLrscLnDZXPDuKXci65EVx/XFa4nXN+7Obsp3I64/epu457uftB9aKb5TPHM2pn9HkYeAo+9Hr2z8FmJs/bM6vU09BR41ng+8TL2EnnVeQ16W3qneR/yfuFj7yP3OebzlufGW8Y744v5BvgW+3b7afjN9avye+xv5J/i3+A/GuAUsCTgTCAxMChwU+Advh5fyK/nj852mb1sdkcQIygyqCroSbBVsDy4LQQNmR2yOeThHNM50jktoRDKD90c+ijMPGxR2I/hpPCw8OrwpxEOEUsjOiNZkQsjD0a+ifKJKot6MNdirnJue7RqdEJ0ffTbGN+Y8pjeWLvYZbFX4nTiJHGt8eT46Pi6+LF5fvO2zhtIcEooSrg933x+3vxLC3QWZCw4uVB1oWDh0URiYkziwcSPglBBjWAsiZ+0I2lUyBNuEz4XeYm2iIbFHuJy8WCyR3J58lCKR8rmlOFUz9SK1BEJT1IleZkWmLY77W16aPr+9ImMmIymTEpmYuZxqYY0XdqRpZ+Vl9Ujs5YVyXoXuS3aumhUHiSvy0ay52e3KtgKmaJLaaFco+zLmZVTnfMuNzr3aJ56njSva7HV4vWLB/P9879dQlgiXNK+1HDpqqV9y7yX7V2OLE9a3r7CeEXhioGVASsPrKKtSl/1U4F9QXnB69Uxq9sK9QpXFvavCVjTUKRSJC+6s9Z97e51hHWSdd3rp6/fvv5zsaj4col9SUXJx1Jh6eVvHL6p/GZiQ/KG7jLnsl0bSRulG29v8tx0oFy9PL+8f3PI5uYt+JbiLa+3Ltx6qWJGxe5ttG3Kbb2VwZWt2022b9z+sSq16la1T3XTDt0d63e83SnaeX2X167G3Xq7S3Z/2CPZc3dvwN7mGrOain2kfTn7ntZG13Z+y/22vk6nrqTu037p/t4DEQc66l3q6w/qHixrQBuUDcOHEg5d+873u9ZGm8a9TZymksNwWHn42feJ398+EnSk/Sj3aOMPpj/sOMY6VtyMNC9uHm1JbeltjWvtOT77eHube9uxH21/3H/C8ET1Sc2TZadopwpPTZzOPz12RnZm5GzK2f72he0PzsWeu9kR3tF9Puj8xQv+F851eneevuhx8cQlt0vHL3Mvt1xxvtLc5dR17Cenn451O3c3X3W52nrN9Vpbz8yeU9c9r5+94Xvjwk3+zSu35tzquT339t07CXd674ruDt3LuPfyfs798QcrHxIfFj9Se1TxWPdxzc+WPzf1Ovee7PPt63oS+eRBv7D/+T+y//FxoPAp82nFoMFg/ZDj0Ilh/+Frz+Y9G3guez4+UvSL+i87Xli8+OFXr1+7RmNHB17KX078VvpK+9X+1zNet4+FjT1+k/lm/G3xO+13B95z33d+iPkwOJ77kfyx8pPlp7bPQZ8fTmROTPwTA5jz/IzFdaUAAAAgY0hSTQAAeiYAAICEAAD6AAAAgOgAAHUwAADqYAAAOpgAABdwnLpRPAAAAAlwSFlzAAAuIwAALiMBeKU/dgAABwJpVFh0WE1MOmNvbS5hZG9iZS54bXAAAAAAADw/eHBhY2tldCBiZWdpbj0i77u/IiBpZD0iVzVNME1wQ2VoaUh6cmVTek5UY3prYzlkIj8+IDx4OnhtcG1ldGEgeG1sbnM6eD0iYWRvYmU6bnM6bWV0YS8iIHg6eG1wdGs9IkFkb2JlIFhNUCBDb3JlIDkuMS1jMDAxIDc5LjE0NjI4OTksIDIwMjMvMDYvMjUtMjA6MDE6NTUgICAgICAgICI+IDxyZGY6UkRGIHhtbG5zOnJkZj0iaHR0cDovL3d3dy53My5vcmcvMTk5OS8wMi8yMi1yZGYtc3ludGF4LW5zIyI+IDxyZGY6RGVzY3JpcHRpb24gcmRmOmFib3V0PSIiIHhtbG5zOnhtcD0iaHR0cDovL25zLmFkb2JlLmNvbS94YXAvMS4wLyIgeG1sbnM6eG1wTU09Imh0dHA6Ly9ucy5hZG9iZS5jb20veGFwLzEuMC9tbS8iIHhtbG5zOnN0RXZ0PSJodHRwOi8vbnMuYWRvYmUuY29tL3hhcC8xLjAvc1R5cGUvUmVzb3VyY2VFdmVudCMiIHhtbG5zOmRjPSJodHRwOi8vcHVybC5vcmcvZGMvZWxlbWVudHMvMS4xLyIgeG1sbnM6cGhvdG9zaG9wPSJodHRwOi8vbnMuYWRvYmUuY29tL3Bob3Rvc2hvcC8xLjAvIiB4bXA6Q3JlYXRvclRvb2w9IkFkb2JlIFBob3Rvc2hvcCAyNC43ICgyMDIzMDYxMi5tLjIyMDUgOWNmYTc2YykgIChXaW5kb3dzKSIgeG1wOkNyZWF0ZURhdGU9IjIwMjYtMDctMDVUMjA6MjM6MzErMDc6MDAiIHhtcDpNZXRhZGF0YURhdGU9IjIwMjYtMDctMDdUMjE6MTE6MTArMDc6MDAiIHhtcDpNb2RpZnlEYXRlPSIyMDI2LTA3LTA3VDIxOjExOjEwKzA3OjAwIiB4bXBNTTpJbnN0YW5jZUlEPSJ4bXAuaWlkOmJjYzgxYzVjLTY5ZjYtOGI0Yy1iODRkLWUwNmI5Y2I4ZjA2ZiIgeG1wTU06RG9jdW1lbnRJRD0iYWRvYmU6ZG9jaWQ6cGhvdG9zaG9wOmU2M2JjY2EyLTgxNTAtNmI0NS1iZGUwLTI5OWJmZGQ0MmQyNyIgeG1wTU06T3JpZ2luYWxEb2N1bWVudElEPSJ4bXAuZGlkOmQwNjg5MjhlLTVmNTYtNzE0My1hYmUyLWM0NGQ2MDJmNDEzYyIgZGM6Zm9ybWF0PSJpbWFnZS9wbmciIHBob3Rvc2hvcDpDb2xvck1vZGU9IjMiPiA8eG1wTU06SGlzdG9yeT4gPHJkZjpTZXE+IDxyZGY6bGkgc3RFdnQ6YWN0aW9uPSJjcmVhdGVkIiBzdEV2dDppbnN0YW5jZUlEPSJ4bXAuaWlkOmQwNjg5MjhlLTVmNTYtNzE0My1hYmUyLWM0NGQ2MDJmNDEzYyIgc3RFdnQ6d2hlbj0iMjAyNi0wNy0wNVQyMDoyMzozMSswNzowMCIgc3RFdnQ6c29mdHdhcmVBZ2VudD0iQWRvYmUgUGhvdG9zaG9wIDI0LjcgKDIwMjMwNjEyLm0uMjIwNSA5Y2ZhNzZjKSAgKFdpbmRvd3MpIi8+IDxyZGY6bGkgc3RFdnQ6YWN0aW9uPSJzYXZlZCIgc3RFdnQ6aW5zdGFuY2VJRD0ieG1wLmlpZDo5ZWI1MDkzOS0yMGUxLWVkNDItOWUzNy0xODdlMDhiMjk3NWMiIHN0RXZ0OndoZW49IjIwMjYtMDctMDVUMjA6MjM6MzErMDc6MDAiIHN0RXZ0OnNvZnR3YXJlQWdlbnQ9IkFkb2JlIFBob3Rvc2hvcCAyNC43ICgyMDIzMDYxMi5tLjIyMDUgOWNmYTc2YykgIChXaW5kb3dzKSIgc3RFdnQ6Y2hhbmdlZD0iLyIvPiA8cmRmOmxpIHN0RXZ0OmFjdGlvbj0ic2F2ZWQiIHN0RXZ0Omluc3RhbmNlSUQ9InhtcC5paWQ6YmNjODFjNWMtNjlmNi04YjRjLWI4NGQtZTA2YjljYjhmMDZmIiBzdEV2dDp3aGVuPSIyMDI2LTA3LTA3VDIxOjExOjEwKzA3OjAwIiBzdEV2dDpzb2Z0d2FyZUFnZW50PSJBZG9iZSBQaG90b3Nob3AgMjQuNyAoMjAyMzA2MTIubS4yMjA1IDljZmE3NmMpICAoV2luZG93cykiIHN0RXZ0OmNoYW5nZWQ9Ii8iLz4gPC9yZGY6U2VxPiA8L3htcE1NOkhpc3Rvcnk+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";
            const icon_white_b64 = "data:image/png;base64," + "iVBORw0KGgoAAAANSUhEUgAAAgAAAAIACAYAAAD0eNT6AAAACXBIWXMAAC4jAAAuIwF4pT92AAAGGmlUWHRYTUw6Y29tLmFkb2JlLnhtcAAAAAAAPD94cGFja2V0IGJlZ2luPSLvu78iIGlkPSJXNU0wTXBDZWhpSHpyZVN6TlRjemtjOWQiPz4gPHg6eG1wbWV0YSB4bWxuczp4PSJhZG9iZTpuczptZXRhLyIgeDp4bXB0az0iQWRvYmUgWE1QIENvcmUgOS4xLWMwMDEgNzkuMTQ2Mjg5OSwgMjAyMy8wNi8yNS0yMDowMTo1NSAgICAgICAgIj4gPHJkZjpSREYgeG1sbnM6cmRmPSJodHRwOi8vd3d3LnczLm9yZy8xOTk5LzAyLzIyLXJkZi1zeW50YXgtbnMjIj4gPHJkZjpEZXNjcmlwdGlvbiByZGY6YWJvdXQ9IiIgeG1sbnM6eG1wPSJodHRwOi8vbnMuYWRvYmUuY29tL3hhcC8xLjAvIiB4bWxuczp4bXBNTT0iaHR0cDovL25zLmFkb2JlLmNvbS94YXAvMS4wL21tLyIgeG1sbnM6c3RFdnQ9Imh0dHA6Ly9ucy5hZG9iZS5jb20veGFwLzEuMC9zVHlwZS9SZXNvdXJjZUV2ZW50IyIgeG1sbnM6ZGM9Imh0dHA6Ly9wdXJsLm9yZy9kYy9lbGVtZW50cy8xLjEvIiB4bWxuczpwaG90b3Nob3A9Imh0dHA6Ly9ucy5hZG9iZS5jb20vcGhvdG9zaG9wLzEuMC8iIHhtcDpDcmVhdG9yVG9vbD0iQWRvYmUgUGhvdG9zaG9wIDI0LjcgKDIwMjMwNjEyLm0uMjIwNSA5Y2ZhNzZjKSAgKFdpbmRvd3MpIiB4bXA6Q3JlYXRlRGF0ZT0iMjAyNi0wNy0wNVQyMDoyMzo1MSswNzowMCIgeG1wOk1ldGFkYXRhRGF0ZT0iMjAyNi0wNy0wNVQyMDoyMzo1MSswNzowMCIgeG1wOk1vZGlmeURhdGU9IjIwMjYtMDctMDVUMjA6MjM6NTErMDc6MDAiIHhtcE1NOkluc3RhbmNlSUQ9InhtcC5paWQ6YTFlOWMzNTQtNzRkZS1lZjQ3LWE2MTUtYTNkZDFlYjI1NjYwIiB4bXBNTTpEb2N1bWVudElEPSJhZG9iZTpkb2NpZDpwaG90b3Nob3A6NWU4MzRmMDgtNTdiYy04ZjQ2LTkzYjUtY2FhNzk0NWQ2OWVjIiB4bXBNTTpPcmlnaW5hbERvY3VtZW50SUQ9InhtcC5kaWQ6ZjgxZjVjNGItNWJjMS0yMjQyLThjOWMtNjBlOGFmYmJkNzA2IiBkYzpmb3JtYXQ9ImltYWdlL3BuZyIgcGhvdG9zaG9wOkNvbG9yTW9kZT0iMyI+IDx4bXBNTTpIaXN0b3J5PiA8cmRmOlNlcT4gPHJkZjpsaSBzdEV2dDphY3Rpb249ImNyZWF0ZWQiIHN0RXZ0Omluc3RhbmNlSUQ9InhtcC5paWQ6ZjgxZjVjNGItNWJjMS0yMjQyLThjOWMtNjBlOGFmYmJkNzA2IiBzdEV2dDp3aGVuPSIyMDI2LTA3LTA1VDIwOjIzOjUxKzA3OjAwIiBzdEV2dDpzb2Z0d2FyZUFnZW50PSJBZG9iZSBQaG90b3Nob3AgMjQuNyAoMjAyMzA2MTIubS4yMjA1IDljZmE3NmMpICAoV2luZG93cykiLz4gPHJkZjpsaSBzdEV2dDphY3Rpb249InNhdmVkIiBzdEV2dDppbnN0YW5jZUlEPSJ4bXAuaWlkOmExZTljMzU0LTc0ZGUtZWY0Ny1hNjE1LWEzZGQxZWIyNTY2MCIgc3RFdnQ6d2hlbj0iMjAyNi0wNy0wNVQyMDoyMzo1MSswNzowMCIgc3RFdnQ6c29mdHdhcmVBZ2VudD0iQWRvYmUgUGhvdG9zaG9wIDI0LjcgKDIwMjMwNjEyLm0uMjIwNSA5Y2ZhNzZjKSAgKFdpbmRvd3MpIiBzdEV2dDpjaGFuZ2VkPSIvIi8+IDwvcmRmOlNlcT4gPC94bXBNTTpIaXN0b3J5PiA8L3JkZjpEZXNjcmlwdGlvbj4gPC9yZGY6UkRGPiA8L3g6eG1wbWV0YT4gPD94cGFja2V0IGVuZD0iciI/Pnki1bsAAGBxSURBVHic7d13mB5V+cbx76aHJLSE3qU36SBIrwpKb4Ig2ABpKkVBmiKgiCKC2JCfNOlSpElv0gSk9957TSB9f3/cu2azbHvfnZnnzMz9ua690jbv3Nm8O/PMmXOe09La2oqZmZnVy4DoAGZmZlY8FwBmZmY15ALAzMyshlwAmJmZ1ZALADMzsxoaFB3AzAoxCzAXMBMwAzAbMLLtzyYCbwLjgbHAW8Cr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";
            
            function saveSettings(silent = false) {
                window.chrome.webview.postMessage(JSON.stringify({ 
                    action: 'save_settings',
                    silent: silent,
                    autoUpdate: autoUpdateToggle ? autoUpdateToggle.checked : false,
                    runOnStartup: startupToggle ? startupToggle.checked : false,
                    minimizeToTrayOnClose: minimizeTrayToggle ? minimizeTrayToggle.checked : true,
                    hideIdentity: hideIdentityToggle ? hideIdentityToggle.checked : false,
                    alwaysOnTop: alwaysOnTopToggle ? alwaysOnTopToggle.checked : false,
                    autoKillOnExit: autoKillExitToggle ? autoKillExitToggle.checked : false,
                    hardwareAcceleration: hardwareAccelToggle ? hardwareAccelToggle.checked : true,
                    resourceOptimizer: resourceOptToggle ? resourceOptToggle.checked : false,
                    cpuLimiter: cpuLimiterToggle ? cpuLimiterToggle.checked : false,
                    backgroundCpuLimit: cpuLimitSlider ? parseInt(cpuLimitSlider.value) : 2,
                    lightMode: lightModeToggle ? lightModeToggle.checked : false,
                    accentColor: (accentColorInput && accentColorInput.dataset.reset !== "true") ? accentColorInput.value : "",
                    fontFamily: fontFamilyInput ? fontFamilyInput.value : "-apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, Helvetica, Arial, sans-serif",
                    language: document.getElementById('setting-language') ? document.getElementById('setting-language').value : "en",
                    uiScale: document.getElementById('setting-ui-scale') ? parseFloat(document.getElementById('setting-ui-scale').value) : 1.0,
                    sidebarCollapsed: document.getElementById('sidebar') ? document.getElementById('sidebar').classList.contains('collapsed') : false,
                    windowOpacity: document.getElementById('setting-window-opacity') ? parseFloat(document.getElementById('setting-window-opacity').value) : 1.0,
                    enableWindowBlur: document.getElementById('setting-window-blur') ? document.getElementById('setting-window-blur').checked : false
                }));
            }

let autoUpdateToggle = document.getElementById('setting-auto-update');
            let startupToggle = document.getElementById('setting-run-startup');
            let minimizeTrayToggle = document.getElementById('setting-minimize-tray');
            let hideIdentityToggle = document.getElementById('setting-hide-identity');
            let alwaysOnTopToggle = document.getElementById('setting-always-on-top');
            let autoKillExitToggle = document.getElementById('setting-auto-kill-exit');
            let hardwareAccelToggle = document.getElementById('setting-hardware-accel');
                        let resourceOptToggle = document.getElementById('setting-resource-opt');
            let cpuLimiterToggle = document.getElementById('setting-cpu-limiter-toggle');
            let cpuLimitContainer = document.getElementById('cpu-limit-container');
            let cpuLimitSlider = document.getElementById('setting-cpu-limit');
            let cpuLimitValue = document.getElementById('cpu-limit-value');

            if (autoUpdateToggle) {
                autoUpdateToggle.addEventListener('change', (e) => {
                    saveSettings();
                });
            }
            if (startupToggle) {
                startupToggle.addEventListener('change', (e) => {
                    saveSettings();
                });
            }
            if (minimizeTrayToggle) {
                minimizeTrayToggle.addEventListener('change', (e) => {
                    saveSettings();
                });
            }
            if (hideIdentityToggle) {
                hideIdentityToggle.addEventListener('change', (e) => {
                    saveSettings();
                    if (window.renderAccounts) window.renderAccounts(currentAccounts);
                });
            }
            if (alwaysOnTopToggle) {
                alwaysOnTopToggle.addEventListener('change', (e) => {
                    saveSettings();
                });
            }
            if (autoKillExitToggle) {
                autoKillExitToggle.addEventListener('change', (e) => {
                    saveSettings();
                });
            }
                        
            if (cpuLimitSlider) {
                cpuLimitSlider.addEventListener('input', (e) => {
                    if (cpuLimitValue) cpuLimitValue.textContent = e.target.value + "%";
                });
                cpuLimitSlider.addEventListener('change', (e) => {
                    saveSettings();
                });
            }

            let opacitySlider = document.getElementById('setting-window-opacity');
            if (opacitySlider) {
                opacitySlider.addEventListener('input', (e) => {
                    let opacityVal = document.getElementById('window-opacity-val');
                    let val = parseFloat(e.target.value);
                    if (opacityVal) opacityVal.textContent = Math.round(val * 100) + '%';
                    
                    document.documentElement.style.setProperty('--bg-opacity', val);
                    
                    let blurInput = document.getElementById('setting-window-blur');
                    if (!blurInput || !blurInput.checked) {
                        window.chrome.webview.postMessage(JSON.stringify({ action: 'preview_opacity', value: val }));
                    }
                });
                opacitySlider.addEventListener('change', (e) => {
                    saveSettings(true);
                });
            }
            
            let blurToggle = document.getElementById('setting-window-blur');
            if (blurToggle) {
                blurToggle.addEventListener('change', (e) => {
                    if (e.target.checked) document.body.classList.add('blur-enabled');
                    else document.body.classList.remove('blur-enabled');
                    saveSettings(true);
                });
            }

            
            if (cpuLimiterToggle) {
                cpuLimiterToggle.addEventListener('change', (e) => {
                    if (cpuLimitContainer) {
                        cpuLimitContainer.style.display = e.target.checked ? 'flex' : 'none';
                    }
                    if (resourceOptToggle) {
                        resourceOptToggle.dispatchEvent(new Event('change'));
                    }
                });
            }
            if (resourceOptToggle) {
                
                resourceOptToggle.addEventListener('change', (e) => {

                    saveSettings();
                });
            }
            if (hardwareAccelToggle) {
                hardwareAccelToggle.addEventListener('change', (e) => {
                    saveSettings();
                });
            }
            
            
            if (lightModeToggle) {
                lightModeToggle.addEventListener('change', (e) => {
                    document.documentElement.setAttribute('data-theme', e.target.checked ? 'light' : '');
                    let icon = document.getElementById('titlebar-icon');
                    if (icon) {
                        icon.src = e.target.checked ? icon_black_b64 : icon_white_b64;
                    }
                    saveSettings();
                });
            }
            let btnResetAccent = document.getElementById('btn-reset-accent-color');
            if (btnResetAccent && accentColorInput) {
                btnResetAccent.addEventListener('click', () => {
                    accentColorInput.dataset.reset = 'true';
                    accentColorInput.value = '#ffffff';
                    document.documentElement.style.removeProperty('--accent-color');
                    document.documentElement.style.removeProperty('--accent-tint-hover');
                    document.documentElement.style.removeProperty('--accent-tint-active');
                    saveSettings();
                });
            }
            let btnStartUpdate = document.getElementById('btn-start-update');
            if (btnStartUpdate) {
                btnStartUpdate.addEventListener('click', () => {
                    document.getElementById('update-prompt-modal').classList.remove('show');
                    window.chrome.webview.postMessage(JSON.stringify({ action: 'start_update' }));
                });
            }

            if (window.chrome && window.chrome.webview) {
                window.addEventListener('message', (e) => {
                    try {
                        let msg = typeof e.data === 'string' ? JSON.parse(e.data) : e.data;
                        if (msg.action === 'settings_data') {
                            if (autoUpdateToggle) autoUpdateToggle.checked = msg.autoUpdate;
                            if (startupToggle) startupToggle.checked = msg.runOnStartup;
                            if (minimizeTrayToggle) minimizeTrayToggle.checked = msg.minimizeToTrayOnClose;
                            if (hideIdentityToggle) hideIdentityToggle.checked = msg.hideIdentity;
                            if (alwaysOnTopToggle) alwaysOnTopToggle.checked = msg.alwaysOnTop;
                            if (autoKillExitToggle) autoKillExitToggle.checked = msg.autoKillOnExit;
                            if (hardwareAccelToggle) hardwareAccelToggle.checked = msg.hardwareAcceleration;
                            if (typeof msg.sidebarCollapsed !== 'undefined') {
                                let sb = document.getElementById('sidebar');
                                if (sb) {
                                    if (msg.sidebarCollapsed) sb.classList.add('collapsed');
                                    else sb.classList.remove('collapsed');
                                }
                            }
                            if (lightModeToggle) {
                                lightModeToggle.checked = msg.lightMode;
                                document.documentElement.setAttribute('data-theme', msg.lightMode ? 'light' : '');
                                let icon = document.getElementById('titlebar-icon');
                                if (icon) {
                                    icon.src = msg.lightMode ? icon_black_b64 : icon_white_b64;
                                }
                            }
                            if (accentColorInput) {
                                if (typeof msg.accentColor === 'string' && msg.accentColor.trim() !== '') {
                                    accentColorInput.value = msg.accentColor;
                                    accentColorInput.dataset.reset = 'false';
                                    document.documentElement.style.setProperty('--accent-color', msg.accentColor);
                                    document.documentElement.style.setProperty('--accent-tint-hover', msg.accentColor + '26');
                                    document.documentElement.style.setProperty('--accent-tint-active', msg.accentColor + '40');
                                } else {
                                    accentColorInput.value = '#ffffff';
                                    accentColorInput.dataset.reset = 'true';
                                    document.documentElement.style.removeProperty('--accent-color');
                                    document.documentElement.style.removeProperty('--accent-tint-hover');
                                    document.documentElement.style.removeProperty('--accent-tint-active');
                                }
                            }
                            let fFamInput = document.getElementById('setting-font-family');
                            
                            let langInput = document.getElementById('setting-language');
                            if (langInput && msg.language) {
                                langInput.value = msg.language;
                                applyLanguage(msg.language);
                                let lOpt = document.querySelector('#lang-dropdown-options .dropdown-option[data-value="' + msg.language + '"]');
                                if (lOpt) {
                                    let lText = document.getElementById('lang-dropdown-text');
                                    if (lText) lText.textContent = lOpt.textContent;
                                }
                            } else {
                                applyLanguage('en');
                            }
                            if (fFamInput && msg.fontFamily) {
                                fFamInput.value = msg.fontFamily;
                                document.documentElement.style.setProperty('--font-family', msg.fontFamily);
                                let fOptsList = document.querySelectorAll('#font-dropdown-options .dropdown-option');
                                for(let i=0; i<fOptsList.length; i++) {
                                    // Trim to avoid whitespace issues just in case
                                    if (fOptsList[i].getAttribute('data-value').trim() === msg.fontFamily.trim()) {
                                        let fText = document.getElementById('font-dropdown-text');
                                        if (fText) fText.textContent = fOptsList[i].textContent;
                                        break;
                                    }
                                }
                            }

            if (resourceOptToggle) {
                                resourceOptToggle.checked = msg.resourceOptimizer;
                                if (cpuLimiterToggle) cpuLimiterToggle.checked = msg.cpuLimiter;
                                if (cpuLimitContainer) cpuLimitContainer.style.display = msg.cpuLimiter ? 'flex' : 'none';
                            }
                            if (cpuLimitSlider) {
                                cpuLimitSlider.value = msg.backgroundCpuLimit || 2;
                                if (cpuLimitValue) cpuLimitValue.textContent = (msg.backgroundCpuLimit || 2) + "%";
                            }
                            
                            let uiScaleInput = document.getElementById('setting-ui-scale');
                            let uiScaleVal = document.getElementById('ui-scale-val');
                            if (uiScaleInput && typeof msg.uiScale !== 'undefined') {
                                uiScaleInput.value = msg.uiScale;
                                if (uiScaleVal) uiScaleVal.textContent = Math.round(msg.uiScale * 100) + '%';
                                document.documentElement.style.setProperty('--ui-scale', msg.uiScale);
                            }
                            
                            let opacityInput = document.getElementById('setting-window-opacity');
                            let opacityVal = document.getElementById('window-opacity-val');
                            if (opacityInput && typeof msg.windowOpacity !== 'undefined') {
                                let roundedOpacity = Math.round(msg.windowOpacity * 100) / 100;
                                opacityInput.value = roundedOpacity;
                                if (opacityVal) opacityVal.textContent = Math.round(roundedOpacity * 100) + '%';
                                document.documentElement.style.setProperty('--bg-opacity', roundedOpacity);
                            }
                            
                            let blurInput = document.getElementById('setting-window-blur');
                            if (blurInput && typeof msg.enableWindowBlur !== 'undefined') {
                                blurInput.checked = msg.enableWindowBlur;
                                if (msg.enableWindowBlur) {
                                    document.body.classList.add('blur-enabled');
                                    document.documentElement.classList.add('blur-enabled');
                                } else {
                                    document.body.classList.remove('blur-enabled');
                                    document.documentElement.classList.remove('blur-enabled');
                                }
                            }

                        }
                        else if (msg.action === 'update_available') {
                            let verText = document.getElementById('update-version-text');
                            if (verText) verText.textContent = msg.version;
                            let promptModal = document.getElementById('update-prompt-modal');
                            if (promptModal) promptModal.classList.add('show');
                        }
                        else if (msg.action === 'show_changelog') {
                            let titleEl = document.getElementById('changelog-title');
                            if (titleEl && msg.version) titleEl.textContent = (translations[document.getElementById('setting-language')?.value || 'en']?.lbl_whats_new || "What's New in RoPilot") + " " + msg.version;
                            
                            let contentEl = document.getElementById('changelog-content');
                            if (contentEl) {
                                let content = msg.content || '';
                                content = content.replace(/\r\n/g, '\n');
                                content = content.replace(/^#+.*?\n+/gm, '');
                                content = content.trim();

                                let htmlContent = content
                                    .replace(/^-\s+(.*)$/gm, '<li>$1</li>')
                                    .replace(/\*\*(.*?)\*\*/g, '<strong>$1</strong>')
                                    .replace(/\*(.*?)\*/g, '<em>$1</em>');
                                
                                htmlContent = htmlContent.replace(/(?:<li>.*?<\/li>\s*)+/g, match => {
                                    let items = match.replace(/<\/li>\s*<li>/g, '</li><li>').trim();
                                    return '<ul style="margin: 12px 0; padding-left: 20px; display: flex; flex-direction: column; gap: 6px;">' + items + '</ul>';
                                });
                                
                                htmlContent = htmlContent.replace(/\n/g, '<br>');
                                contentEl.innerHTML = htmlContent;
                            }
                            let clModal = document.getElementById('changelog-modal');
                            if (clModal) clModal.classList.add('show');
                        }
                        else if (msg.action === 'start_update') {
                            window.chrome.webview.postMessage(JSON.stringify({ action: 'start_update' }));
                        }
                        else if (msg.action === 'account_overview_data') {
                            document.getElementById('manage-loading').style.display = 'none';
                            document.getElementById('manage-home').style.display = 'flex';
                            
                            document.getElementById('mo-username').innerText = msg.username || "-";
                            document.getElementById('mo-displayname').innerText = msg.displayName || "-";
                            document.getElementById('mo-userid').innerText = msg.userId || "-";
                            document.getElementById('mo-robux').innerText = (msg.robux != null) ? msg.robux.toLocaleString() : "-";
                            document.getElementById('mo-age').innerText = calculateAccountAge(msg.createdDate);
                            
                            let premiumEl = document.getElementById('mo-premium');
                            if (msg.isPremium) {
                                premiumEl.innerText = (document.getElementById('setting-language') && document.getElementById('setting-language').value === 'id') ? "Aktif" : "Active";
                                premiumEl.style.color = "white";
                            } else {
                                premiumEl.innerText = (document.getElementById('setting-language') && document.getElementById('setting-language').value === 'id') ? "Tidak Aktif" : "Inactive";
                                premiumEl.style.color = "var(--text-muted)";
                            }
                            
                            updateTranslations();
                        }
                        else if (msg.action === 'update_progress') {
                            let overlay = document.getElementById('update-overlay');
                            if (overlay) overlay.style.opacity = '1';
                            
                            let bar = document.getElementById('update-progress-bar');
                            let text = document.getElementById('update-progress-text');
                            let title = document.getElementById('update-overlay-title');
                            let desc = document.getElementById('update-overlay-desc');
                            
                            if (bar) bar.style.width = msg.percentage + '%';
                            if (text) text.textContent = msg.percentage + '%';
                            
                            if (msg.status) {
                                if (desc) desc.textContent = msg.status;
                            }
                            if (msg.percentage >= 100) {
                                if (title) title.textContent = translations[document.getElementById('setting-language')?.value || 'en']?.lbl_installing_update || 'Installing Update...';
                                if (desc) desc.textContent = translations[document.getElementById('setting-language')?.value || 'en']?.desc_restarting || 'Please wait while RoPilot restarts.';
                            }
                        }
                    } catch(err) {
                        console.error("Message error:", err);
                    }
                });
                
                // Request initial settings
                window.chrome.webview.postMessage(JSON.stringify({ action: 'get_settings' }));
                
                // Check for updates
                window.chrome.webview.postMessage(JSON.stringify({ action: 'check_update' }));
            }

            // Close modals on outside click
            window.addEventListener('click', (e) => {
                if (e.target.classList.contains('modal')) {
                    e.target.classList.remove('show');
                }
            });

            // Resize Handles
            document.querySelectorAll('.resize-edge').forEach(edge => {
                edge.addEventListener('mousedown', (e) => {
                    let edgeCode = parseInt(e.target.getAttribute('data-edge'));
                    window.chrome.webview.postMessage(JSON.stringify({ action: 'resize', edge: edgeCode }));
                });
            });

        // Manage Account Modal Logic
        let currentManageCookie = '';
        let manageAccountModal = document.getElementById('manage-account-modal');
        let manageTabs = document.querySelectorAll('.manage-tab');
        
        function calculateAccountAge(createdIsoStr) {
            if (!createdIsoStr) return "-";
            const createdDate = new Date(createdIsoStr);
            const now = new Date();
            const diffTime = Math.abs(now - createdDate);
            const diffDays = Math.ceil(diffTime / (1000 * 60 * 60 * 24));
            
            const years = Math.floor(diffDays / 365);
            const months = Math.floor((diffDays % 365) / 30);
            
            if (years > 0) {
                return `${years} Year${years > 1 ? 's' : ''}, ${months} Month${months !== 1 ? 's' : ''}`;
            } else if (months > 0) {
                return `${months} Month${months > 1 ? 's' : ''}`;
            } else {
                return `${diffDays} Day${diffDays !== 1 ? 's' : ''}`;
            }
        }
        
        window.openManageAccountModal = function(cookie, userId, avatarSrc, username) {
            currentManageCookie = cookie;
            
            // Set initial state
            document.getElementById('manage-avatar').src = avatarSrc;
            document.getElementById('manage-title-username').innerText = username;
            
            document.getElementById('manage-loading').style.display = 'flex';
            document.getElementById('manage-home').style.display = 'none';
            document.getElementById('manage-placeholder').style.display = 'none';
            
            // Set home tab active
            manageTabs.forEach(t => t.classList.remove('active'));
            if(manageTabs.length > 0) manageTabs[0].classList.add('active');
            
            manageAccountModal.style.pointerEvents = 'auto';
            manageAccountModal.style.opacity = '1';
            manageAccountModal.querySelector('.modal-content').style.transform = 'scale(1)';
            
            window.chrome.webview.postMessage(JSON.stringify({
                action: 'get_account_overview',
                cookie: cookie,
                userId: parseInt(userId)
            }));
        };
        
        window.closeManageAccountModal = function() {
            manageAccountModal.style.pointerEvents = 'none';
            manageAccountModal.style.opacity = '0';
            manageAccountModal.querySelector('.modal-content').style.transform = 'scale(0.95)';
        };
        
        manageTabs.forEach(tab => {
            tab.addEventListener('click', () => {
                manageTabs.forEach(t => t.classList.remove('active'));
                tab.classList.add('active');
                
                const target = tab.getAttribute('data-tab');
                document.getElementById('manage-loading').style.display = 'none';
                
                if (target === 'manage-home') {
                    document.getElementById('manage-placeholder').style.display = 'none';
                    let el = document.getElementById('manage-home');
                    el.style.display = 'flex';
                    el.classList.remove('manage-page-enter');
                    void el.offsetWidth;
                    el.classList.add('manage-page-enter');
                } else {
                    document.getElementById('manage-home').style.display = 'none';
                    let el = document.getElementById('manage-placeholder');
                    el.style.display = 'flex';
                    el.classList.remove('manage-page-enter');
                    void el.offsetWidth;
                    el.classList.add('manage-page-enter');
                }
            });
        });

</script>
)HTML";
