#pragma once

#include "./Definition.h"

namespace espkit::cam::pinout {
    static const Definition aithinker("aithinker", 5, 18, 19, 21, 36, 39, 34, 35, 0, 22, 25, 23, 26, 27, 32, -1);
    static const Definition xiao("xiao", 15, 17, 18, 16, 14, 12, 11, 48, 10, 13, 38, 47, 40, 39, -1, -1);
    static const Definition wroomS3("wroom_s3", 11, 9, 8, 10, 12, 18, 17, 16, 15, 13, 6, 7, 4, 5, -1, -1);
    static const Definition ttgoPlus("ttgo_plus", 34, 13, 26, 35, 39, 38, 37, 36, 4, 25, 5, 27, 18, 23, -1, -1);
    static const Definition ttgoPir("ttgo_pir", 5, 14, 4, 15, 18, 23, 36, 39, 32, 19, 27, 25, 13, 12, 26, -1);
    static const Definition m5("m5_default", 32, 35, 34, 5, 39, 18, 36, 19, 27, 21, 22, 26, 25, 23, -1, 15);
    static const Definition m5Fisheye("m5_fisheye", 32, 35, 34, 5, 39, 18, 36, 19, 27, 21, 25, 26, 22, 23, -1, 15);
    static const Definition m5Timerx("m5_timerx", 32, 35, 34, 5, 39, 18, 36, 19, 27, 21, 25, 26, 22, 23, -1, 15);
    static const Definition espeye("espeye", 34, 13, 14, 35, 39, 38, 37, 36, 4, 25, 5, 27, 18, 23, -1, -1);
    static const Definition espeyeS3("espeye_s3", 11, 9, 8, 10, 12, 18, 17, 16, 15, 13, 6, 7, 4, 5, -1, -1);
    static const Definition wrover("wrover", 4, 5, 18, 19, 36, 39, 34, 35, 21, 22, 25, 23, 26, 27, -1, -1);
    static const uint8_t supportedPinoutsCount = 11;
    static const Definition *supportedPinouts[supportedPinoutsCount] = {
            &aithinker,
            &xiao,
            &wroomS3,
            &ttgoPlus,
            &ttgoPir,
            &m5,
            &m5Fisheye,
            &m5Timerx,
            &espeye,
            &espeyeS3,
            &wrover
    };
}