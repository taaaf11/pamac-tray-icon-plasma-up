#!/usr/bin/python
# This script uses polib and Pamac GTK's .po files to generate Qt translation files for tray application
# Usage: ./translation_converter.py <path-to-po-files>

import os
import sys
import polib
import subprocess

if len(sys.argv) == 1:
    print(f'Usage: {sys.argv[0]} <path-to-po-files>')
    exit(0)

subprocess.call(["lupdate", "pamac_tray.cpp", "main.cpp", "-ts", "pamac_tray.ts"])
subprocess.call(["lconvert", "pamac_tray.ts", "-o", "pamac_tray.po"])
os.remove('pamac_tray.ts')

tray_icon_source_po = polib.pofile("pamac_tray.po")

for pamac_po_filename in os.listdir(sys.argv[1]):

    if not pamac_po_filename.endswith(".po"):
        continue

    base = os.path.basename(pamac_po_filename)
    language = os.path.splitext(base)[0]

    pamac_po = polib.pofile(sys.argv[1] + "/" + pamac_po_filename)

    result = polib.POFile()
    result.metadata = tray_icon_source_po.metadata

    pamac_messages = pamac_po.translated_entries()

    for pamac_message in pamac_messages:
        if not ('src/tray.vala', '') in pamac_message.occurrences:
            continue

        tray_messages = [e for e in tray_icon_source_po if e.msgid == pamac_message.msgid.replace('%u', '%n').replace('_', '')]
        if not tray_messages:
            continue

        for tray_message in tray_messages:

            if not pamac_message.msgstr_plural:
                tray_message.msgstr = pamac_message.msgstr.replace('_', '')
            else:
                tray_message.msgstr_plural = pamac_message.msgstr_plural
                for plural in tray_message.msgstr_plural:
                    tray_message.msgstr_plural[plural] = tray_message.msgstr_plural[plural].replace('%u', '%n')

            result.append(tray_message)

    result.save(f"pamac_tray_{language}.po")

    subprocess.call(["lconvert", f"pamac_tray_{language}.po", "-o", f"pamac_tray_{language}.ts"])
    subprocess.call(["lrelease", f"pamac_tray_{language}.ts"])
    os.remove(f"pamac_tray_{language}.ts")
    os.remove(f"pamac_tray_{language}.po")

    os.rename(f"pamac_tray_{language}.qm", f"i18n/pamac_tray_{language}.qm")

os.remove("pamac_tray.po")
