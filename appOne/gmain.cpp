#include<vector>
#include"DOT.h"

void create(std::vector<DOT>& dots) {
    //１点、１点、ターゲットの位置（x,y）と色相(hue)を
    //ファイルデータから読み込んで設定する
    float hue, targetX, targetY;
    float left = (width - 1450) / 2;
    float top = (height - 160) / 2;
    FILE* fp;
    fopen_s(&fp, "coding_ocean.csv", "r");
    while (fscanf_s(fp, "%f,%f", &targetX, &targetY) != EOF) {
        hue = map(targetY, 0, 160, 150, 210);
        dots.emplace_back(hue, left + targetX, top + targetY);
    }
    fclose(fp);
}

void gmain() {
    window(1920, 1080, full);
    hideCursor();
    std::vector<DOT> dots;
    create(dots);
    initDeltaTime();
    while (notQuit){
        setDeltaTime();
        clear();
        point(mouseX, mouseY);
        for (DOT& dot : dots) {
            dot.update();
            dot.draw();
        }
    }
}
