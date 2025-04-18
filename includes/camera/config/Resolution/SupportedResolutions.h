#pragma once

#include "./Definition.h"

namespace espkit::cam::resolution {
    static Definition yolo("yolo", FRAMESIZE_96X96, 96, 96);
    static Definition qqvga("qqvga", FRAMESIZE_QQVGA, 160, 120);
    static Definition yoloxl("yoloxl", FRAMESIZE_128X128, 128, 128);
    static Definition qcif("qcif", FRAMESIZE_QCIF, 176, 144);
    static Definition hqvga("hqvga", FRAMESIZE_HQVGA, 240, 176);
    static Definition square("square", FRAMESIZE_240X240, 240, 240);
    static Definition qvga("qvga", FRAMESIZE_QVGA, 320, 240);
    static Definition squarexl("squarexl", FRAMESIZE_320X320, 320, 320);
    static Definition cif("cif", FRAMESIZE_CIF, 400, 296);
    static Definition hvga("hvga", FRAMESIZE_HVGA, 480, 320);
    static Definition vga("vga", FRAMESIZE_VGA, 640, 480);
    static Definition svga("svga", FRAMESIZE_SVGA, 800, 600);
    static Definition xga("xga", FRAMESIZE_XGA, 1024, 768);
    static Definition hd("hd", FRAMESIZE_HD, 1280, 720);
    static Definition sxga("sxga", FRAMESIZE_SXGA, 1280, 1024);
    static Definition uxga("uxga", FRAMESIZE_UXGA, 1600, 1200);
    static Definition fhd("fhd", FRAMESIZE_FHD, 1920, 1080);
    static Definition p_hd("p_hd", FRAMESIZE_P_HD, 720, 1280);
    static Definition p_3mp("p_3mp", FRAMESIZE_P_3MP, 864, 1536);
    static Definition qxga("qxga", FRAMESIZE_QXGA, 2048, 1536);
    static Definition qhd("qhd", FRAMESIZE_QHD, 2560, 1440);
    static Definition wqxga("wqxga", FRAMESIZE_WQXGA, 2560, 1600);
    static Definition p_fhd("p_fhd", FRAMESIZE_P_FHD, 1080, 1920);
    static Definition qsxga("qsxga", FRAMESIZE_QSXGA, 2560, 1920);
    static Definition pi("pi", FRAMESIZE_5MP, 2592, 1944);
    static const uint8_t supportedResolutionsCount = 25;
    static const Definition *supportedResolutions[] = {
            &yolo,
            &qqvga,
            &yoloxl,
            &qcif,
            &hqvga,
            &square,
            &qvga,
            &squarexl,
            &cif,
            &hvga,
            &vga,
            &svga,
            &xga,
            &hd,
            &sxga,
            &uxga,
            &fhd,
            &p_hd,
            &p_3mp,
            &qxga,
            &qhd,
            &wqxga,
            &p_fhd,
            &qsxga,
            &pi
    };

}