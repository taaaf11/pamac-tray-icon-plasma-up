#!/usr/bin/python
# This script uses polib and Pamac GTK's .po files to generate Qt translation files for tray application
# Usage: ./translation_converter.py <path-to-po-files>

import os
import sys
import polib
import subprocess

subprocess.call(["lupdate", "pamac_tray.cpp", "-ts", "pamac_tray.ts"])
subprocess.call(["lconvert", "pamac_tray.ts", "-o", "pamac_tray.po"])
os.remove('pamac_tray.ts')

sourcefile = polib.pofile("pamac_tray.po")
print(sourcefile.metadata)

for filename in os.listdir(sys.argv[1]):
    if not filename.endswith(".po"):
        continue
    print(filename)
    base = os.path.basename(filename)
    language = os.path.splitext(base)[0]

    file = polib.pofile(sys.argv[1]+filename)

    result = polib.POFile()
    result.metadata = sourcefile.metadata

    lst = file.translated_entries()

    for el in lst:
        e = next((e for e in sourcefile if e.msgid == el.msgid.replace('%u', '%n')), None)
        if e and ('src/tray.vala', '') in el.occurrences:

            if not el.msgstr_plural:
                e.msgstr = el.msgstr
            else:
                e.msgstr_plural = el.msgstr_plural
                for plural in e.msgstr_plural:
                    e.msgstr_plural[plural] = e.msgstr_plural[plural].replace('%u', '%n')

            result.append(e)

    result.save(f"pamac_tray_{language}.po")

    subprocess.call(["lconvert", f"pamac_tray_{language}.po", "-o", f"pamac_tray_{language}.ts"])


    subprocess.call(["lrelease", f"pamac_tray_{language}.ts"])
    os.remove(f"pamac_tray_{language}.ts")
    os.remove(f"pamac_tray_{language}.po")

    os.rename(f"pamac_tray_{language}.qm", f"i18n/pamac_tray_{language}.qm")

os.remove("pamac_tray.po")
