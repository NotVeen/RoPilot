import re

with open('src/UI_Frontend.h', 'r', encoding='utf-8') as f:
    content = f.read()

# Add let resourceOptToggle
content = content.replace("let hardwareAccelToggle = document.getElementById('setting-hardware-accel');", "let hardwareAccelToggle = document.getElementById('setting-hardware-accel');\n            let resourceOptToggle = document.getElementById('setting-resource-opt');")

# Add resourceOptimizer to save_settings payloads
content = content.replace("hardwareAcceleration: hardwareAccelToggle ? hardwareAccelToggle.checked : true\n                    }));", "hardwareAcceleration: hardwareAccelToggle ? hardwareAccelToggle.checked : true,\n                        resourceOptimizer: resourceOptToggle ? resourceOptToggle.checked : false\n                    }));")
content = content.replace("hardwareAcceleration: e.target.checked\n                    }));", "hardwareAcceleration: e.target.checked,\n                        resourceOptimizer: resourceOptToggle ? resourceOptToggle.checked : false\n                    }));")

# Add resourceOptToggle listener
listener = """            if (resourceOptToggle) {
                resourceOptToggle.addEventListener('change', (e) => {
                    window.chrome.webview.postMessage(JSON.stringify({ 
                        action: 'save_settings',
                        autoUpdate: autoUpdateToggle ? autoUpdateToggle.checked : false,
                        runOnStartup: startupToggle ? startupToggle.checked : false,
                        minimizeToTrayOnClose: minimizeTrayToggle ? minimizeTrayToggle.checked : true,
                        alwaysOnTop: alwaysOnTopToggle ? alwaysOnTopToggle.checked : false,
                        autoKillOnExit: autoKillExitToggle ? autoKillExitToggle.checked : false,
                        hardwareAcceleration: hardwareAccelToggle ? hardwareAccelToggle.checked : true,
                        resourceOptimizer: e.target.checked
                    }));
                });
            }
"""
content = content.replace("if (hardwareAccelToggle) {", listener + "            if (hardwareAccelToggle) {")

# Add population logic
content = content.replace("if (hardwareAccelToggle) hardwareAccelToggle.checked = msg.hardwareAcceleration;", "if (hardwareAccelToggle) hardwareAccelToggle.checked = msg.hardwareAcceleration;\n                            if (resourceOptToggle) resourceOptToggle.checked = msg.resourceOptimizer;")

with open('src/UI_Frontend.h', 'w', encoding='utf-8') as f:
    f.write(content)
print("done")
