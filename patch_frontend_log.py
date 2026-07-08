import sys

with open('src/UI_Frontend.h', 'r', encoding='utf-8') as f:
    ui = f.read()

ui = ui.replace('div.onclick = () => { window.promptWearOutfit(outfit.id, outfit.name, outfit.imageUrl); };', 'console.log("Outfit Image:", outfit.imageUrl);\n                        div.onclick = () => { window.promptWearOutfit(outfit.id, outfit.name, outfit.imageUrl); };')

with open('src/UI_Frontend.h', 'w', encoding='utf-8') as f:
    f.write(ui)
