
#pragma once

#define _CAM_01 79156
#define _CAM_02 67724

//int cam_ide=0;

class CCamIde{

    int cam_ide;

    static int  get_cam();
    static void set_cam(int cam);
};
