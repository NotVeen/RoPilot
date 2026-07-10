#pragma once
#include <string>
#include "UI_CSS.h"
#include "UI_Body.h"
#include "UI_JS.h"

inline const std::string GetUIHtml() {
    return std::string(R"HTML(<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>RoPilot</title>
    <link href="https://fonts.googleapis.com/css2?family=Inter:wght@400;500;600;700&family=Outfit:wght@400;500;600;700&family=Plus+Jakarta+Sans:wght@400;500;600;700&family=Poppins:wght@400;500;600;700&family=Roboto:wght@400;500;700&display=swap" rel="stylesheet">
    <style>
)HTML") + UI_CSS + R"HTML(
    </style>
</head>
<body>
)HTML" + UI_BODY + R"HTML(
    <script src="https://cdn.jsdelivr.net/npm/sortablejs@1.15.7/Sortable.min.js"></script>
    <script>
)HTML" + UI_JS + R"HTML(
    </script>
</body>
</html>
)HTML";
}