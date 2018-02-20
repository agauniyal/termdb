#!/usr/bin/env python3

from os.path import basename, realpath
from pathlib import Path
import re

if __name__ == "__main__":
    print("[docBuilder] Building Docs...")

    fileDir = Path(realpath(__file__))
    projectDir = Path(str(fileDir).split('scripts')[0])

    mdFiles = projectDir.joinpath('docs').glob("*.md")
    pattern = r"```cpp.*```"

    outDir = projectDir.joinpath("test")
    nameFormat = outDir.joinpath("docs_code_{number}.cpp")

    mesonFile = outDir.joinpath("meson.build")
    lineToAppend = '''{name} = executable('{name}', '{filename}', include_directories : inc, dependencies : [optional, variant])\n'''

    with open(str(mesonFile), 'a+') as meson:
        meson.seek(0)
        mesonContent = meson.read().splitlines(True)

        for eachMdFile in mdFiles:
            with open(str(eachMdFile)) as f:
                content = f.read()
                codeSegments = [x.split('```')[0]
                                for x in content.split('```cpp') if '```' in x]

                for idx, eachSegment in enumerate(codeSegments):
                    filename = str(nameFormat).format(number=idx+1)
                    x = basename(filename)

                    line = lineToAppend.format(
                        filename=x, name=x.split('.')[0])

                    if line not in mesonContent:
                        with open(filename, 'w') as f2:
                            f2.write(eachSegment)
                        meson.write(line)

    print("[docBuilder] Docs Building Completed...")
