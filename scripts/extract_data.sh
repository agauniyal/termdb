#!/bin/sh

cd "${MESON_SOURCE_ROOT}"
cp test/data.tar.gz "${MESON_BUILD_ROOT}"
cd "${MESON_BUILD_ROOT}"
tar -xzf data.tar.gz
