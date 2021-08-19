#include<vector>
#include"DOT.h"

bool create(std::vector<DOT>& dots) {
    //１点、１点、ターゲットの位置（x,y）と色相(hue)を
    //ファイルデータから読み込んで設定する
    float hue, targetX, targetY;
    char dummy[64];
    float left = (width - 1450) / 2;
    float top = (height - 160) / 2;
    FILE* fp=0;
    fopen_s(&fp, "coding_ocean_logo.csv", "r");
    if (fp) {
        fgets(dummy, 64, fp);
        fgets(dummy, 64, fp);
        while (fscanf_s(fp, "%f,%f", &targetX, &targetY) != EOF) {
            hue = map(targetY, 0, 160, 150, 210);
            dots.emplace_back(hue, left + targetX, top + targetY);
        }
        fclose(fp);
        return false;
    }
    else {
        return true;
    }
}

void gmain() {
    window(1920, 1080, full);
    hideCursor();
    std::vector<DOT> dots;
    if(create(dots))return;
    initDeltaTime();
    while (notQuit){
        setDeltaTime();
        clear();
        stroke(0, 0, 64);
        point(mouseX, mouseY);
        for (DOT& dot : dots) {
            dot.update();
            dot.draw();
        }
    }
}
