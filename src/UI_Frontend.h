#pragma once

const char* HTML_CONTENT = R"HTML(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>RoPilot</title>
    <style>
        /* CSS Reset & Variables */
        * { box-sizing: border-box; margin: 0; padding: 0; }
        :root {
            --bg-deep: #0a0a0a;
            --bg-sidebar: #111111;
            --bg-card: #1a1a1a;
            --bg-hover: #222222;
            --border-color: #2a2a2a;
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
        }

        body {
            font-family: var(--font-family);
            background-color: var(--bg-deep);
            color: var(--text-main);
            overflow: hidden;
            display: flex;
            flex-direction: column;
            height: 100vh;
            user-select: none; /* Disable text selection */
        }
        
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
            border-radius: 8px;
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
            border-radius: 6px;
            padding: 4px 8px;
            margin-top: 10px;
            font-size: 11px;
            color: var(--text-muted);
            font-family: 'Consolas', 'Courier New', monospace;
            border: 1px solid rgba(255, 255, 255, 0.05);
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
            color: var(--text-main);
        }

        .nav-item.active {
            background-color: #1f1f1f;
            color: #ffffff;
        }

        .sidebar-spacer {
            flex-grow: 1;
        }

        .btn-add-account {
            background-color: #ffffff;
            color: #000000;
            border: none;
            padding: 12px;
            border-radius: 8px;
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
            background-color: #dddddd;
        }

        .add-menu-item {
            padding: 12px;
            font-size: 13px;
            cursor: pointer;
            transition: background 0.2s;
            text-align: center;
        }
        .add-menu-item:hover {
            background-color: var(--bg-hover);
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
        }

        .card {
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
            border-radius: 8px;
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

        .btn-launch {
            flex-grow: 1;
            background-color: var(--btn-bg);
            color: var(--text-main);
            border: 1px solid var(--border-color);
            padding: 8px;
            border-radius: 8px;
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

        .btn-icon {
            background-color: transparent;
            color: var(--text-muted);
            border: 1px solid var(--border-color);
            border-radius: 8px;
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
            border-radius: 8px;
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
            border-radius: 8px;
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
            border-radius: 8px;
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
            border-radius: 4px;
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
        input:checked + .slider {
            background-color: var(--text-green);
        }
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

        #cookie-modal, #group-modal {
            opacity: 0;
            pointer-events: none;
            transition: opacity 0.2s ease;
            display: flex;
            backdrop-filter: blur(4px);
        }
        #cookie-modal.show, #group-modal.show {
            opacity: 1;
            pointer-events: auto;
        }
        #cookie-modal-content, #group-modal-content {
            transform: scale(0.95);
            transition: transform 0.2s ease;
        }
        #cookie-modal.show #cookie-modal-content, #group-modal.show #group-modal-content {
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
            background: #3a3a3a;
            border-radius: 10px;
            border: 4px solid var(--bg-deep); /* Creates the gap effect */
        }
        ::-webkit-scrollbar-thumb:hover {
            background: #555;
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
            border-radius: 8px;
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
            <img src="data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAYAAABzenr0AAAE0klEQVR4nK1XW4iWVRRd++8fTR3tYoMZo1OSlEVWvlRP9dCVoaIUy66opRgVYfRQCfZgRYgNXbGLRBkVoRFF6CAkCJEhZkXZxQgjuptW3phxnBXrd33jnq//H51yw8e57bP2Pvvsy/kCgySSEREkOQ7APQAqAJ6IiK3F2mAx/0UC0tdonuQokl/wIH1F8phD7cPhEMlK7ueNJI9ye4kFd5Pscv+KzJME98Mry6uUNY2IXpLDSZ6kfsmktIDvAOwGUAXQBGCv5rzWx6+9xhtLstn9+pagtSN5GUmB7SL5BslhxabUDiX5LMlef8+Jr8RTMd8rJHeS/J7klXUtwYN3O8yMoh63c80zpI6C+/3t8r72Eu8sY4hH9KOt2+cTlWwuACMAjLEZe7x0ipmlkNZe1xyA4d5fcX88gNd0deYN88FYwmwB0Fw3UkhW3S6yxgJRaLUknhk8NN2S+E8wRmGph7OsPgvY+XpswlYA3V77C8BtSQmddA2AWwGcDWCsv8kAJHg1gKGFcO01RhizleRVllW7gpDnRoS0mw3gxboeCnwNYEpE7DH4OQDOt7KiHwCsj4hPCycFsAHAWQ3w5kXE0lrUJAf80h5dxLX6PR7Pldncbh7A/Fq7w7xywL3GEBaNpcN+0y85kWwm+XsSXAgXXW2eVSVhm0i+SXJFKSuK1jgM2xNWgSv6g+Sx2QGnlUKvaB/w+oYELk8/3Q7WRnK0eSaTfCvxbfT8fQ2wb8gKLCe5zybrcn+L155KoHeTbCK5hORPntOe90mea/75Ke6f8dxmY3aZX32FM7Q4guSOOvc5h2RrMttj5t9Yh7c42cXmWew57R1H8uY6/H+THFmNiN3OdvLcIkEoMl4mucAh9BuA+0k+qGhw7l8I4D0AbQCWAJgE4GlbQmvTnZxmR8RDrhO9KRK6I2KnvPU6F5ZaiBX5geRNAKZ53OlCMsMgSyNisddk3h0A1gE4DcAFEaErUb5QaF9L8lsA25MCtYOSvF4ZaRaAS9GYlEZrYQNglEE+9ImqXv8YwDYnpfEuNhrDuWB5A+y1VZvqcSeWSJ+0PNVC2vwKOs5j5XM5WtXtNRYu5TbZWlOMoUz4q/cVNUZ8HwG4N0eCrmOIv6PdzndoLbT3P+/xhbKA506Ut5N8VZXSWJenhHZ7+dj9yjEPZKS+V8z/JRUjktst/HOX+DHOovOUO9KBo5YObV6F2QQA+1OZrqTrkNkKRdXPJbXiKDrDGKKf5VsRISVWAJgKoMsl+hcx1MpyKsMLeGSo2+l5onHvSmtzPNdn8apPLOoAICervWbSSTNVShYoHiS9Pnm7xysjYgvJiwA86T2dAF4oqi+ONPGAL33gk+5xbdD9r7UvNHy218iL1Tpfk9uRJN+2kJkkjzf4OpIdxpjgSif6zJHUkgrW4f0bZCq8leTUdJfnee3RNHej55T5ClqWcAYvvPRcX+2Cs8rjwnllBdE2kid7riMpMTPz/1fhk1JimV56ek/0u1/UmRR7x9VQxe7MjDcYBapuH3GN35rf9WldCaag9SRf8gtqn/d9Il8y72H/I4bb0ST/NPiirJj7xf+iSvFAtNJO2S/rDnQvRQbUj8a7LiLLrFjODzJzJSLudAlWZdX1yNyFMMW9ft1U1JQfxF/D+AeQdxFJK/yGAQAAAABJRU5ErkJggg==" width="28" height="28" style="margin-right: 2px;" />
            <span><strong style="font-weight: 700; font-size: 16px;">RoPilot</strong><span style="color: var(--text-muted); font-size: 14px; margin-left: 12px; font-weight: 500;">v1.0.1 by NotVeen</span></span>
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
                <span class="nav-text">Accounts</span>
            </div>
            <div class="nav-item" id="nav-analytics">
                <svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round" style="flex-shrink: 0;">
                    <path d="M3 3v18h18"></path>
                    <path d="M18.7 8l-5.1 5.2-2.8-2.7L7 14.3"></path>
                </svg>
                <span class="nav-text">Analytics</span>
            </div>
                <div class="nav-item" id="nav-settings">
                    <svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round" style="flex-shrink: 0;">
                        <circle cx="12" cy="12" r="3"></circle>
                        <path d="M19.4 15a1.65 1.65 0 0 0 .33 1.82l.06.06a2 2 0 0 1 0 2.83 2 2 0 0 1-2.83 0l-.06-.06a1.65 1.65 0 0 0-1.82-.33 1.65 1.65 0 0 0-1 1.51V21a2 2 0 0 1-2 2 2 2 0 0 1-2-2v-.09A1.65 1.65 0 0 0 9 19.4a1.65 1.65 0 0 0-1.82.33l-.06.06a2 2 0 0 1-2.83 0 2 2 0 0 1 0-2.83l.06-.06a1.65 1.65 0 0 0 .33-1.82 1.65 1.65 0 0 0-1.51-1H3a2 2 0 0 1-2-2 2 2 0 0 1 2-2h.09A1.65 1.65 0 0 0 4.6 9a1.65 1.65 0 0 0-.33-1.82l-.06-.06a2 2 0 0 1 0-2.83 2 2 0 0 1 2.83 0l.06.06a1.65 1.65 0 0 0 1.82.33H9a1.65 1.65 0 0 0 1-1.51V3a2 2 0 0 1 2-2 2 2 0 0 1 2 2v.09a1.65 1.65 0 0 0 1 1.51 1.65 1.65 0 0 0 1.82-.33l.06-.06a2 2 0 0 1 2.83 0 2 2 0 0 1 0 2.83l-.06.06a1.65 1.65 0 0 0-.33 1.82V9a1.65 1.65 0 0 0 1.51 1H21a2 2 0 0 1 2 2 2 2 0 0 1-2 2h-.09a1.65 1.65 0 0 0-1.51 1z"></path>
                    </svg>
                    <span class="nav-text">Settings</span>
                </div>
            
            <div class="sidebar-spacer"></div>
            
            <div style="position: relative; width: 100%;">
                <div id="add-menu" style="position: absolute; bottom: 100%; left: 0; width: 208px; background: var(--bg-card); border: 1px solid var(--border-color); border-radius: 8px; margin-bottom: 8px; flex-direction: column; overflow: hidden; z-index: 100;">
                    <div class="add-menu-item" id="btn-add-browser">Add via Browser</div>
                    <div class="add-menu-item" id="btn-add-cookie">Add via Cookie</div>
                </div>
                <button class="btn-add-account" id="btn-add-account">
                    <svg width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round" style="flex-shrink: 0;">
                        <line x1="12" y1="5" x2="12" y2="19"></line>
                        <line x1="5" y1="12" x2="19" y2="12"></line>
                    </svg>
                    <span class="add-account-text">Add Account</span>
                </button>
            </div>
        </div>

        <!-- Main Content -->
        <div class="main-content">
            <!-- Accounts Page -->
            <div id="page-accounts" class="page-container active">
                <div class="page-header" style="display: flex; justify-content: space-between; align-items: flex-end;">
                    <div>
                        <h1 style="margin: 0 0 8px 0; font-size: 24px; font-weight: 700;">Manage Accounts</h1>
                        <div class="page-subtitle">Manage and monitor your connected Roblox instances.</div>
                    </div>
                    <div style="display: flex; gap: 16px; align-items: center;">
                        <div class="search-box">
                            <svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round" style="color: var(--text-muted);">
                                <circle cx="11" cy="11" r="8"></circle>
                                <line x1="21" y1="21" x2="16.65" y2="16.65"></line>
                            </svg>
                            <input type="text" id="search-input" placeholder="Search accounts..." autocomplete="off" spellcheck="false" />
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
                        <p>Fetching your accounts...</p>
                    </div>
                </div>
            </div>

            <!-- Analytics Page -->
            <div id="page-analytics" class="page-container">
                <div class="page-header" style="display: flex; justify-content: space-between; align-items: flex-end;">
                    <div>
                        <h1 style="margin: 0 0 8px 0; font-size: 24px; font-weight: 700;">Performance Analytics</h1>
                        <div class="page-subtitle">Real-time statistics for your running Roblox instances.</div>
                    </div>
                    <div style="display: flex; gap: 16px; align-items: center;">
                        <div class="search-box">
                            <svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round" style="color: var(--text-muted);">
                                <circle cx="11" cy="11" r="8"></circle>
                                <line x1="21" y1="21" x2="16.65" y2="16.65"></line>
                            </svg>
                            <input type="text" id="search-input-analytics" placeholder="Search accounts..." autocomplete="off" spellcheck="false" />
                        </div>
                        <button class="btn-icon danger" onclick="window.showKillAllPrompt(true)" title="Kill All Instances" style="border-radius: 50%;">
                            <svg width="16" height="16" viewBox="0 0 24 24" fill="currentColor"><path d="M19 6.41L17.59 5 L12 10.59 6.41 5 5 6.41 10.59 12 5 17.59 6.41 19 12 13.41 17.59 19 19 17.59 13.41 12z"/></svg>
                        </button>
                    </div>
                </div>

                <div class="accounts-wrapper" id="analytics-grid" style="display: grid; grid-template-columns: repeat(auto-fill, minmax(300px, 1fr)); gap: 16px;">
                    <div class="empty-state">
                        <svg width="32" height="32" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round" style="color: var(--text-muted); margin-bottom: 12px;"><path d="M3 3v18h18"></path><path d="M18.7 8l-5.1 5.2-2.8-2.7L7 14.3"></path></svg>
                        <h3>No Active Instances</h3>
                        <p>Launch an account to see real-time analytics.</p>
                    </div>
                </div>
            </div>

            <!-- Settings Page -->
            <div id="page-settings" class="page-container">
                <div class="page-header">
                    <div>
                        <h1 style="margin: 0 0 8px 0; font-size: 24px; font-weight: 700;">Settings</h1>
                        <div class="page-subtitle">Configure RoPilot preferences.</div>
                    </div>
                </div>
                
                <div style="margin-bottom: 24px;">
                    <div style="padding: 16px 0px; border-bottom: 1px solid var(--border-color);">
                        <h3 style="margin: 0; font-size: 14px; font-weight: 600; text-transform: uppercase; letter-spacing: 0.05em; color: var(--text-muted);">General</h3>
                    </div>
                    <div style="padding: 20px 0px; display: flex; justify-content: space-between; align-items: center;">
                        <div>
                            <div style="font-size: 15px; font-weight: 500; margin-bottom: 4px; color: white;">Automatic Updates</div>
                            <div style="font-size: 13px; color: var(--text-muted);">Automatically download and install new versions when you launch RoPilot.</div>
                        </div>
                        <label class="switch">
                            <input type="checkbox" id="setting-auto-update">
                            <span class="slider"></span>
                        </label>
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
                <h2 id="update-overlay-title" style="margin: 0 0 12px 0; font-size: 24px; font-weight: 600; color: white;">Downloading Update...</h2>
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
            <div style="padding: 20px; border-bottom: 1px solid var(--border-color); display: flex; justify-content: space-between; align-items: center;">
                <h2 style="margin: 0; font-size: 18px; font-weight: 600;">Create Group</h2>
                <button class="btn-icon" id="btn-close-group-modal" style="margin: -8px;" onclick="document.getElementById('group-modal').classList.remove('show');"><svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round"><line x1="18" y1="6" x2="6" y2="18"></line><line x1="6" y1="6" x2="18" y2="18"></line></svg></button>
            </div>
            <div style="padding: 20px;">
                <p style="color: var(--text-muted); margin-bottom: 16px; font-size: 14px;">Enter a name for the new group. You can drag and drop accounts into this group later.</p>
                <input type="text" id="group-name-input" placeholder="e.g. Main Accounts" style="width: 100%; padding: 10px 14px; background: rgba(255,255,255,0.03); border: 1px solid var(--border-color); border-radius: 8px; color: white; outline: none; transition: border-color 0.2s;" />
            </div>
            <div style="padding: 16px 20px; background: rgba(0,0,0,0.2); border-top: 1px solid var(--border-color); display: flex; justify-content: flex-end; gap: 12px;">
                <button class="btn-secondary" id="btn-cancel-group" style="padding: 8px 16px; background: rgba(255,255,255,0.1); border: none; border-radius: 8px; color: white; cursor: pointer;" onclick="document.getElementById('group-modal').classList.remove('show');">Cancel</button>
                <button class="btn-primary" id="btn-confirm-group" style="padding: 8px 16px; background: white; color: black; border: none; border-radius: 8px; font-weight: 600; cursor: pointer;">Create</button>
            </div>
        </div>
    </div>

    <!-- Cookie Login Modal -->
    <div id="cookie-modal" class="modal" style="position: fixed; inset: 0; background: rgba(0,0,0,0.5); z-index: 1000; align-items: center; justify-content: center;">
        <div id="cookie-modal-content" style="background: var(--bg-card); border: 1px solid var(--border-color); border-radius: 12px; width: 500px; max-width: 90%; overflow: hidden; display: flex; flex-direction: column;">
            <div style="padding: 20px; border-bottom: 1px solid var(--border-color); display: flex; justify-content: space-between; align-items: center;">
                <h2 style="margin: 0; font-size: 18px; font-weight: 600;">Add Account via Cookie</h2>
                <button class="btn-icon" id="btn-close-cookie-modal" style="margin: -8px;" onclick="document.getElementById('cookie-modal').classList.remove('show');"><svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round"><line x1="18" y1="6" x2="6" y2="18"></line><line x1="6" y1="6" x2="18" y2="18"></line></svg></button>
            </div>
            <div style="padding: 20px;">
                <p style="color: var(--text-muted); margin-bottom: 16px; font-size: 14px;">Enter your Roblox .ROBLOSECURITY cookie below.</p>
                <textarea id="cookie-input" placeholder="_|WARNING:-DO-NOT-SHARE-THIS..." style="width: 100%; height: 100px; padding: 10px 14px; background: rgba(255,255,255,0.03); border: 1px solid var(--border-color); border-radius: 8px; color: white; outline: none; resize: none; font-family: monospace; font-size: 12px;"></textarea>
            </div>
            <div style="padding: 16px 20px; background: rgba(0,0,0,0.2); border-top: 1px solid var(--border-color); display: flex; justify-content: flex-end; gap: 12px;">
                <button class="btn-secondary" id="btn-cancel-cookie" style="padding: 8px 16px; background: rgba(255,255,255,0.1); border: none; border-radius: 8px; color: white; cursor: pointer;" onclick="document.getElementById('cookie-modal').classList.remove('show');">Cancel</button>
                <button class="btn-primary" id="btn-confirm-cookie" style="padding: 8px 16px; background: white; color: black; border: none; border-radius: 8px; font-weight: 600; cursor: pointer;">Login</button>
            </div>
        </div>
    </div>

    <!-- Rename Group Modal -->
    <div id="rename-group-modal" class="modal" style="position: fixed; inset: 0; background: rgba(0,0,0,0.5); z-index: 1000; align-items: center; justify-content: center; opacity: 0; pointer-events: none; transition: opacity 0.2s ease; display: flex; backdrop-filter: blur(4px);">
        <div id="rename-group-modal-content" style="background: var(--bg-card); border: 1px solid var(--border-color); border-radius: 12px; width: 400px; max-width: 90%; overflow: hidden; display: flex; flex-direction: column; transform: scale(0.95); transition: transform 0.2s ease;">
            <div style="padding: 20px; border-bottom: 1px solid var(--border-color); display: flex; justify-content: space-between; align-items: center;">
                <h2 style="margin: 0; font-size: 18px; font-weight: 600;">Rename Group</h2>
                <button class="btn-icon" onclick="document.getElementById('rename-group-modal').classList.remove('show');" style="margin: -8px;"><svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round"><line x1="18" y1="6" x2="6" y2="18"></line><line x1="6" y1="6" x2="18" y2="18"></line></svg></button>
            </div>
            <div style="padding: 20px;">
                <p style="color: var(--text-muted); margin-bottom: 16px; font-size: 14px;">Enter a new name for this group.</p>
                <input type="hidden" id="rename-group-old-name" />
                <input type="text" id="rename-group-name-input" placeholder="New Group Name" style="width: 100%; padding: 10px 14px; background: rgba(255,255,255,0.03); border: 1px solid var(--border-color); border-radius: 8px; color: white; outline: none; transition: border-color 0.2s;" />
            </div>
            <div style="padding: 16px 20px; background: rgba(0,0,0,0.2); border-top: 1px solid var(--border-color); display: flex; justify-content: flex-end; gap: 12px;">
                <button class="btn-secondary" onclick="document.getElementById('rename-group-modal').classList.remove('show');" style="padding: 8px 16px; background: rgba(255,255,255,0.1); border: none; border-radius: 8px; color: white; cursor: pointer;">Cancel</button>
                <button class="btn-primary" id="btn-confirm-rename-group" style="padding: 8px 16px; background: white; color: black; border: none; border-radius: 8px; font-weight: 600; cursor: pointer;">Save</button>
            </div>
        </div>
    </div>

    <!-- Delete Group Modal -->
    <div id="delete-group-modal" class="modal" style="position: fixed; inset: 0; background: rgba(0,0,0,0.5); z-index: 1000; align-items: center; justify-content: center; opacity: 0; pointer-events: none; transition: opacity 0.2s ease; display: flex; backdrop-filter: blur(4px);">
        <div id="delete-group-modal-content" style="background: var(--bg-card); border: 1px solid var(--border-color); border-radius: 12px; width: 400px; max-width: 90%; overflow: hidden; display: flex; flex-direction: column; transform: scale(0.95); transition: transform 0.2s ease;">
            <div style="padding: 20px; border-bottom: 1px solid var(--border-color); display: flex; justify-content: space-between; align-items: center;">
                <h2 style="margin: 0; font-size: 18px; font-weight: 600; color: #ff5252;">Delete Group</h2>
                <button class="btn-icon" onclick="document.getElementById('delete-group-modal').classList.remove('show');" style="margin: -8px;"><svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round"><line x1="18" y1="6" x2="6" y2="18"></line><line x1="6" y1="6" x2="18" y2="18"></line></svg></button>
            </div>
            <div style="padding: 20px;">
                <p style="color: var(--text-muted); margin-bottom: 0; font-size: 14px;">Are you sure you want to delete <strong id="delete-group-name-display" style="color: white;"></strong>? The accounts inside will become "Ungrouped". This action cannot be undone.</p>
                <input type="hidden" id="delete-group-name" />
            </div>
            <div style="padding: 16px 20px; background: rgba(0,0,0,0.2); border-top: 1px solid var(--border-color); display: flex; justify-content: flex-end; gap: 12px;">
                <button class="btn-secondary" onclick="document.getElementById('delete-group-modal').classList.remove('show');" style="padding: 8px 16px; background: rgba(255,255,255,0.1); border: none; border-radius: 8px; color: white; cursor: pointer;">Cancel</button>
                <button class="btn-primary danger" id="btn-confirm-delete-group" style="padding: 8px 16px; background: #ff5252; color: white; border: none; border-radius: 8px; font-weight: 600; cursor: pointer;">Delete</button>
            </div>
        </div>
    </div>

    <!-- Kill Instance Modal -->
    <div id="kill-modal" class="modal" style="position: fixed; inset: 0; background: rgba(0,0,0,0.5); z-index: 1000; align-items: center; justify-content: center; opacity: 0; pointer-events: none; transition: opacity 0.2s ease; display: flex; backdrop-filter: blur(4px);">
        <div id="kill-modal-content" style="background: var(--bg-card); border: 1px solid var(--border-color); border-radius: 12px; width: 400px; max-width: 90%; overflow: hidden; display: flex; flex-direction: column; transform: scale(0.95); transition: transform 0.2s ease;">
            <div style="padding: 20px; border-bottom: 1px solid var(--border-color); display: flex; justify-content: space-between; align-items: center;">
                <h2 style="margin: 0; font-size: 18px; font-weight: 600; color: #ff5252;">Kill Instance</h2>
                <button class="btn-icon" onclick="document.getElementById('kill-modal').classList.remove('show');" style="margin: -8px;"><svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round"><line x1="18" y1="6" x2="6" y2="18"></line><line x1="6" y1="6" x2="18" y2="18"></line></svg></button>
            </div>
            <div style="padding: 20px;">
                <p style="color: var(--text-muted); margin-bottom: 0; font-size: 14px;">Are you sure you want to terminate the Roblox instance for <strong id="kill-account-name-display" style="color: white;"></strong>? The game will close immediately.</p>
                <input type="hidden" id="kill-account-cookie" />
            </div>
            <div style="padding: 16px 20px; background: rgba(0,0,0,0.2); border-top: 1px solid var(--border-color); display: flex; justify-content: flex-end; gap: 12px;">
                <button class="btn-secondary" onclick="document.getElementById('kill-modal').classList.remove('show');" style="padding: 8px 16px; background: rgba(255,255,255,0.1); border: none; border-radius: 8px; color: white; cursor: pointer;">Cancel</button>
                <button class="btn-primary danger" id="btn-confirm-kill" style="padding: 8px 16px; background: #ff5252; color: white; border: none; border-radius: 8px; font-weight: 600; cursor: pointer;">Terminate</button>
            </div>
        </div>
    </div>

    <!-- Remove Account Modal -->
    <div id="remove-modal" class="modal" style="position: fixed; inset: 0; background: rgba(0,0,0,0.5); z-index: 1000; align-items: center; justify-content: center; opacity: 0; pointer-events: none; transition: opacity 0.2s ease; display: flex; backdrop-filter: blur(4px);">
        <div id="remove-modal-content" style="background: var(--bg-card); border: 1px solid var(--border-color); border-radius: 12px; width: 400px; max-width: 90%; overflow: hidden; display: flex; flex-direction: column; transform: scale(0.95); transition: transform 0.2s ease;">
            <div style="padding: 20px; border-bottom: 1px solid var(--border-color); display: flex; justify-content: space-between; align-items: center;">
                <h2 style="margin: 0; font-size: 18px; font-weight: 600; color: #ff5252;">Remove Account</h2>
                <button class="btn-icon" onclick="document.getElementById('remove-modal').classList.remove('show');" style="margin: -8px;"><svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round"><line x1="18" y1="6" x2="6" y2="18"></line><line x1="6" y1="6" x2="18" y2="18"></line></svg></button>
            </div>
            <div style="padding: 20px;">
                <p style="color: var(--text-muted); margin-bottom: 0; font-size: 14px;">Are you sure you want to remove <strong id="remove-account-name-display" style="color: white;"></strong> from RoPilot? You will need to log back in to add it again.</p>
                <input type="hidden" id="remove-account-cookie" />
            </div>
            <div style="padding: 16px 20px; background: rgba(0,0,0,0.2); border-top: 1px solid var(--border-color); display: flex; justify-content: flex-end; gap: 12px;">
                <button class="btn-secondary" onclick="document.getElementById('remove-modal').classList.remove('show');" style="padding: 8px 16px; background: rgba(255,255,255,0.1); border: none; border-radius: 8px; color: white; cursor: pointer;">Cancel</button>
                <button class="btn-primary danger" id="btn-confirm-remove" style="padding: 8px 16px; background: #ff5252; color: white; border: none; border-radius: 8px; font-weight: 600; cursor: pointer;">Remove</button>
            </div>
        </div>
    </div>
    <!-- Kill All Modal -->
    <div id="kill-all-modal" class="modal" style="position: fixed; inset: 0; background: rgba(0,0,0,0.5); z-index: 1000; align-items: center; justify-content: center; opacity: 0; pointer-events: none; transition: opacity 0.2s ease; display: flex; backdrop-filter: blur(4px);">
        <div id="kill-all-modal-content" style="background: var(--bg-card); border: 1px solid var(--border-color); border-radius: 12px; width: 400px; max-width: 90%; overflow: hidden; display: flex; flex-direction: column; transform: scale(0.95); transition: transform 0.2s ease;">
            <div style="padding: 20px; border-bottom: 1px solid var(--border-color); display: flex; justify-content: space-between; align-items: center;">
                <h2 id="kill-all-title" style="margin: 0; font-size: 18px; font-weight: 600; color: #ff5252;">Running Instances Detected</h2>
                <button class="btn-icon" onclick="document.getElementById('kill-all-modal').classList.remove('show');" style="margin: -8px;"><svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round"><line x1="18" y1="6" x2="6" y2="18"></line><line x1="6" y1="6" x2="18" y2="18"></line></svg></button>
            </div>
            <div style="padding: 20px;">
                <p id="kill-all-desc" style="color: var(--text-muted); margin-bottom: 0; font-size: 14px;">To make multi-instance work properly, you must terminate all currently running Roblox instances before launching new ones. Do you want to terminate them now?</p>
            </div>
            <div style="padding: 16px 20px; background: rgba(0,0,0,0.2); border-top: 1px solid var(--border-color); display: flex; justify-content: flex-end; gap: 12px;">
                <button class="btn-secondary" onclick="document.getElementById('kill-all-modal').classList.remove('show');" style="padding: 8px 16px; background: rgba(255,255,255,0.1); border: none; border-radius: 8px; color: white; cursor: pointer;">No</button>
                <button class="btn-primary danger" id="btn-confirm-kill-all" style="padding: 8px 16px; background: #ff5252; color: white; border: none; border-radius: 8px; font-weight: 600; cursor: pointer;">Yes</button>
            </div>
        </div>
    </div>
    
    <!-- Update Prompt Modal -->
    <div id="update-prompt-modal" class="modal" style="position: fixed; inset: 0; background: rgba(0,0,0,0.5); z-index: 1000; align-items: center; justify-content: center; opacity: 0; pointer-events: none; transition: opacity 0.2s ease; display: flex; backdrop-filter: blur(4px);">
        <div id="update-prompt-content" style="background: var(--bg-card); border: 1px solid var(--border-color); border-radius: 12px; width: 400px; max-width: 90%; overflow: hidden; display: flex; flex-direction: column; transform: scale(0.95); transition: transform 0.2s ease;">
            <div style="padding: 20px; border-bottom: 1px solid var(--border-color); display: flex; justify-content: space-between; align-items: center;">
                <h2 style="margin: 0; font-size: 18px; font-weight: 600; color: #4ade80;">Update Available</h2>
                <button class="btn-icon" onclick="document.getElementById('update-prompt-modal').classList.remove('show');" style="margin: -8px;"><svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round"><line x1="18" y1="6" x2="6" y2="18"></line><line x1="6" y1="6" x2="18" y2="18"></line></svg></button>
            </div>
            <div style="padding: 20px;">
                <p id="update-prompt-text" style="color: var(--text-muted); margin-bottom: 0; font-size: 14px;">A new version of RoPilot is available. Would you like to download and install it now?</p>
            </div>
            <div style="padding: 16px 20px; background: rgba(0,0,0,0.2); border-top: 1px solid var(--border-color); display: flex; justify-content: flex-end; gap: 12px;">
                <button class="btn-secondary" onclick="document.getElementById('update-prompt-modal').classList.remove('show');" style="padding: 8px 16px; background: rgba(255,255,255,0.1); border: none; border-radius: 8px; color: white; cursor: pointer;">Skip</button>
                <button class="btn-primary" id="btn-start-update" style="padding: 8px 16px; background: #4ade80; color: #000; border: none; border-radius: 8px; font-weight: 600; cursor: pointer;">Install Now</button>
            </div>
        </div>
    </div>

    <!-- Toast Container -->
    <div id="toast-container" class="toast-container"></div>

<style>
    #rename-group-modal.show, #delete-group-modal.show, #kill-modal.show, #remove-modal.show, #kill-all-modal.show { opacity: 1 !important; pointer-events: auto !important; }
    #rename-group-modal.show #rename-group-modal-content, #delete-group-modal.show #delete-group-modal-content, #kill-modal.show #kill-modal-content, #remove-modal.show #remove-modal-content, #kill-all-modal.show #kill-all-modal-content { transform: scale(1) !important; }
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
                if (!jobId || typeof jobId !== 'string') return 'None';
                let hash = 0;
                for (let i = 0; i < jobId.length; i++) {
                    hash = ((hash << 5) - hash) + jobId.charCodeAt(i);
                    hash |= 0;
                }
                return '#' + Math.abs(hash).toString(16).substring(0, 6).toUpperCase();
            }
            
            window.renderAccounts = function(accounts) {
                try {
                    if (accounts.length === 0) {
                        accountsGrid.innerHTML = `
                            <div class="empty-state">
                                <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="1.5" stroke-linecap="round" stroke-linejoin="round"><path d="M17 21v-2a4 4 0 0 0-4-4H5a4 4 0 0 0-4 4v2"></path><circle cx="9" cy="7" r="4"></circle><path d="M23 21v-2a4 4 0 0 0-3-3.87"></path><path d="M16 3.13a4 4 0 0 1 0 7.75"></path></svg>
                                <h3>No Accounts Found</h3>
                                <p>Click the "Add Account" button to add your Roblox account.</p>
                            </div>
                        `;
                        return;
                    }

                    let accountsStr = JSON.stringify(accounts.map(a => ({id: a.Id, uid: a.UserId, stat: a.Status, grp: a.Group, pid: a.ProcessId})));
                    let canQuickUpdate = false;
                    if (window.lastRenderedAccountsString === accountsStr && window.lastRenderedSearchTerm === currentSearchTerm && window.lastRenderedAnalyticsSearchTerm === currentAnalyticsSearchTerm) {
                        canQuickUpdate = true;
                    }
                    window.lastRenderedAccountsString = accountsStr;
                    window.lastRenderedSearchTerm = currentSearchTerm;
                    window.lastRenderedAnalyticsSearchTerm = currentAnalyticsSearchTerm;

                    if (canQuickUpdate) {
                        accounts.forEach(acc => {
                            let userId = acc.Id || acc.UserId || '0';
                            let instanceVal = document.getElementById(`instance-val-${userId}`);
                            if (instanceVal) {
                                instanceVal.innerText = (acc.ProcessId && acc.Status !== 0) ? acc.ProcessId.toString() : 'None';
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

                    let html = '';
                    if (filtered.length === 0) {
                        html = `
                            <div class="empty-state">
                                <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="1.5" stroke-linecap="round" stroke-linejoin="round"><circle cx="11" cy="11" r="8"></circle><line x1="21" y1="21" x2="16.65" y2="16.65"></line></svg>
                                <h3>No Results</h3>
                                <p>No accounts match your current search.</p>
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
                    
                    let isSearching = currentSearchTerm !== '';
                    
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
                            
                            if (acc.Status === 1 || acc.Status === 2) {
                                statusColorClass = 'online';
                                statusText = 'Online';
                                statusValueClass = 'green';
                            }
                                            
                            let instanceText = (acc.ProcessId && acc.Status !== 0) ? acc.ProcessId.toString() : 'None';
                            let avatarSrc = acc.ThumbnailUrl ? acc.ThumbnailUrl : "https://tr.rbxcdn.com/30DAY-AvatarHeadshot-15E6D8A279B926E6C5779D6BA1D97ACD-Bc/150/150/AvatarHeadshot/Png/noFilter";
                            let username = escapeHtml(acc.Username);
                            let userId = escapeHtml(acc.Id || acc.UserId || '0');
                            let cookie = escapeHtml(acc.Cookie);

                            html += `
                            <div class="card" data-cookie="${cookie}">
                                <div class="card-header">
                                    <div class="avatar">
                                        <img src="${avatarSrc}" onerror="this.src='https://tr.rbxcdn.com/38c6edcb50633730ff4cf39458e0c139/150/150/AvatarHeadshot/Png'"/>
                                        <div class="status-dot ${statusColorClass}"></div>
                                    </div>
                                    <div class="user-info">
                                        <span class="username">${username}</span>
                                        <span class="userid">${userId}</span>
                                    </div>
                                </div>
                                
                                <div class="card-stats">
                                    <div class="stat-box">
                                        <span class="stat-label">Status</span>
                                        <span class="stat-value ${statusValueClass}">${statusText}</span>
                                    </div>
                                    <div class="stat-box">
                                        <span class="stat-label">Instance</span>
                                        <span class="stat-value" id="instance-val-${userId}">${escapeHtml(instanceText)}</span>
                                    </div>
                                </div>
                                
                                <div class="card-actions">
                                    <button class="btn-launch" id="launch-${acc.Id || acc.UserId || cookie}" onclick="window.launchAccount('${cookie}', '${username}', this)">
                                        <svg width="14" height="14" viewBox="0 0 24 24" fill="currentColor"><path d="M5 3l14 9-14 9V3z"/></svg> Launch
                                    </button>
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
                                    <h3>No Active Instances</h3>
                                    <p>Launch an account to see real-time analytics.</p>
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
                                            <span class="username">${escapeHtml(acc.Username)}</span>
                                            <span class="userid" style="color: var(--accent-color);">PID: ${acc.ProcessId || 'Loading...'}</span>
                                        </div>
                                    </div>
                                    <div style="display: flex; flex-direction: column; gap: 12px;">
                                        <div style="display: flex; align-items: center; gap: 12px;">
                                            <div style="display: flex; align-items: center; gap: 8px; width: 60px;">
                                                <svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="var(--text-muted)" stroke-width="2"><rect x="4" y="4" width="16" height="16" rx="2" ry="2"></rect><rect x="9" y="9" width="6" height="6"></rect><line x1="9" y1="1" x2="9" y2="4"></line><line x1="15" y1="1" x2="15" y2="4"></line><line x1="9" y1="20" x2="9" y2="23"></line><line x1="15" y1="20" x2="15" y2="23"></line><line x1="20" y1="9" x2="23" y2="9"></line><line x1="20" y1="14" x2="23" y2="14"></line><line x1="1" y1="9" x2="4" y2="9"></line><line x1="1" y1="14" x2="4" y2="14"></line></svg>
                                                <span style="font-size: 12px; color: var(--text-muted); font-weight: 600;">CPU</span>
                                            </div>
                                            <div style="flex-grow: 1; height: 4px; background: rgba(255,255,255,0.1); border-radius: 2px; overflow: hidden;"><div id="analytics-cpu-bar-${acc.Id || acc.UserId}" style="width: ${Math.min(acc.CpuUsage, 100)}%; height: 100%; background: ${acc.CpuUsage > 80 ? '#ff5252' : 'var(--accent-color)'};"></div></div>
                                            <span id="analytics-cpu-val-${acc.Id || acc.UserId}" style="font-size: 13px; font-weight: 700; width: 45px; text-align: right;">${acc.CpuUsage.toFixed(1)}%</span>
                                        </div>
                                        <div style="display: flex; align-items: center; gap: 12px;">
                                            <div style="display: flex; align-items: center; gap: 8px; width: 60px;">
                                                <svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="var(--text-muted)" stroke-width="2"><rect x="2" y="3" width="20" height="18" rx="2" ry="2"></rect><line x1="2" y1="9" x2="22" y2="9"></line><line x1="2" y1="15" x2="22" y2="15"></line><line x1="6" y1="3" x2="6" y2="21"></line><line x1="10" y1="3" x2="10" y2="21"></line><line x1="14" y1="3" x2="14" y2="21"></line><line x1="18" y1="3" x2="18" y2="21"></line></svg>
                                                <span style="font-size: 12px; color: var(--text-muted); font-weight: 600;">RAM</span>
                                            </div>
                                            <div style="flex-grow: 1; height: 4px; background: rgba(255,255,255,0.1); border-radius: 2px; overflow: hidden;"><div id="analytics-ram-bar-${acc.Id || acc.UserId}" style="width: ${Math.min((acc.RamUsage / 2048) * 100, 100)}%; height: 100%; background: #a270ff;"></div></div>
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
                                <pre style="text-align: left; background: rgba(0,0,0,0.5); padding: 28px 12px 12px 12px; border-radius: 8px; font-size: 10px; max-width: 100%; overflow-x: auto; margin: 0; color: #ff5252;">${errStr}</pre>
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
                    let input = document.getElementById('group-input');
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
                    let ogText = btnElement.innerHTML;
                    btnElement.innerHTML = '<svg class="spinner" width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round" style="flex-shrink: 0;"><path d="M21 12a9 9 0 1 1-6.219-8.56"></path></svg>';
                    setTimeout(() => { btnElement.innerHTML = ogText; }, 3000);
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
            
            // UI Button Listeners
            let btnAddAccount = document.getElementById('btn-add-account');
            let addMenu = document.getElementById('add-menu');
            if (btnAddAccount && addMenu) {
                btnAddAccount.addEventListener('click', (e) => {
                    e.stopPropagation();
                    addMenu.classList.toggle('show');
                });
            }
            
            document.addEventListener('click', () => {
                if (addMenu) addMenu.classList.remove('show');
            });

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
                    document.getElementById('sidebar').classList.toggle('collapsed');
                });
            }

            let navAccounts = document.getElementById('nav-accounts');
            let navAnalytics = document.getElementById('nav-analytics');
            let navSettings = document.getElementById('nav-settings');
            let pageAccounts = document.getElementById('page-accounts');
            let pageAnalytics = document.getElementById('page-analytics');
            let pageSettings = document.getElementById('page-settings');
            let pages = [pageAccounts, pageAnalytics, pageSettings];

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
                    let c = input ? input.value.trim() : '';
                    if (c !== '') {
                        window.chrome.webview.postMessage(JSON.stringify({ action: 'add_cookie', cookie: c }));
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
                
                let toast = document.createElement('div');
                toast.className = 'toast ' + (isError ? 'error' : 'success');
                toast.innerHTML = `
                    <svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="${isError ? '#f87171' : '#4ade80'}" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
                        ${isError ? '<circle cx="12" cy="12" r="10"></circle><line x1="12" y1="8" x2="12" y2="12"></line><line x1="12" y1="16" x2="12.01" y2="16"></line>' : '<path d="M22 11.08V12a10 10 0 1 1-5.93-9.14"></path><polyline points="22 4 12 14.01 9 11.01"></polyline>'}
                    </svg>
                    <span>${escapeHtml(msg)}</span>
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
                if (isManual) {
                    let hasRunning = currentAccounts.some(a => a.Status === 1 || a.Status === 2);
                    if (!hasRunning) {
                        if (window.showStatus) window.showStatus("No instance is running", true);
                        return;
                    }
                }

                let modal = document.getElementById('kill-all-modal');
                let title = document.getElementById('kill-all-title');
                let desc = document.getElementById('kill-all-desc');
                
                if (title && desc) {
                    if (isManual) {
                        title.textContent = 'Kill All Instances';
                        desc.textContent = 'Are you sure you want to terminate all currently running Roblox instances? All active games will be closed immediately.';
                    } else {
                        title.textContent = 'Running Instances Detected';
                        desc.textContent = 'To make multi-instance work properly, you must terminate all currently running Roblox instances before launching new ones. Do you want to terminate them now?';
                    }
                }
                
                if (modal) modal.classList.add('show');
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
</script>
)HTML";
