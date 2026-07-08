import sys

with open('src/RobloxAPI.cpp', 'r', encoding='utf-8') as f:
    api = f.read()

api = api.replace('\\n    bool GetPresence(', '\n    bool GetPresence(')

with open('src/RobloxAPI.cpp', 'w', encoding='utf-8') as f:
    f.write(api)
