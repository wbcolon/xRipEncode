/*
 * This file is part of xRipEncode.
 *
 * xRipEncode is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * xRipEncode is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <QApplication>
#include "xApplication.h"

int main(int argc, char* argv[]) {
    QApplication ripEncodeApp(argc, argv);
    xApplication ripEncode;
    ripEncode.show();
    return ripEncodeApp.exec();

#if 0
    xAudioRipper audioRipper;
    audioRipper.detect();
    audioRipper.rip( { {1, "01 realize.wav"}, {2, "02 rejection.wav"}, {3, "03 shot in the dark.wav" } });
#endif
}
