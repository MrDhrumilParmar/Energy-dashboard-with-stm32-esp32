from pathlib import Path
import re

WEB_DIR = Path("web")
OUT_FILE = Path("src/web.h")

html = (WEB_DIR / "index.html").read_text(encoding="utf-8")
css  = (WEB_DIR / "style.css").read_text(encoding="utf-8")
js   = (WEB_DIR / "app.js").read_text(encoding="utf-8")

# INLINE CSS (robust)
html = re.sub(
    r'<link[^>]*href=["\']style\.css["\'][^>]*>',
    f"<style>\n{css}\n</style>",
    html,
    flags=re.IGNORECASE
)


# Inline JS
html = re.sub(
    r'<script\s+src=["\']app\.js["\']\s*></script>',
    f"<script>\n{js}\n</script>",
    html,
    flags=re.IGNORECASE
)

# Safety check for raw literal terminator
if ")rawliteral" in html:
    raise RuntimeError("HTML contains )rawliteral which breaks raw string literal")

OUT_FILE.write_text(
f'''#pragma once
//#include <pgmspace.h>

const char INDEX_HTML[] PROGMEM = R"rawliteral(
{html}
)rawliteral";
''',
encoding="utf-8"
)

print("✅ Web UI bundled and generated successfully")
