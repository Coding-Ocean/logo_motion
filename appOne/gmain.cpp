#include"libOne.h"
class VEHICLE {
    VECTOR2 Target;
    VECTOR2 Pos;
    VECTOR2 Vel;
    VECTOR2 Acc;
    float Radius;
    float MaxSpeed;
    float ArriveMaxForce;
    float FleeMaxForce;
    float Hue;
public:
    VEHICLE(float x, float y, float hue) {
        //点の大きさと色相
        Radius = 9;
        Hue = hue;
        //目標位置
        Target.x = x;
        Target.y = y;
        //初期位置
        Pos.x = -Radius;
        Pos.y = height / 2;
        //最高速度
        MaxSpeed = 10;
        //ハンドルを切る最高の力
        ArriveMaxForce = 0.5f;
        FleeMaxForce = 10;
    }
    void resetRandomPosVel() {
        //初期位置
        Pos.x = random(width);
        Pos.y = random(height);
        //初速度ベクトル
        float deg = random(360.0f);
        angleMode(DEGREES);
        Vel.x = cos(deg) * 15;
        Vel.y = sin(deg) * 15;
    }
    void arrive(const VECTOR2& target) {
        //Targetに向かう、望まれるベクトル
        VECTOR2 desired = target - Pos;
        //近づいたらスピードを遅くする
        float speed = MaxSpeed;
        float distance = desired.mag();
        if (distance < 100) {
            speed = map(distance, 0, 100, 0, MaxSpeed);
        }
        //desireの大きさをspeedにする
        desired.setMag(speed);
        //ハンドルを切るベクトルを求める
        VECTOR2 steer = desired - Vel;
        //Accの大きさをArriveMaxForce以下に制限する
        steer.limmit(ArriveMaxForce);
        Acc += steer;

    }
    void flee(const VECTOR2& target) {
        //targetから逃げるベクトル
        VECTOR2 desired = Pos - target;
        float distance = desired.mag();
        if (distance < 80) {
            desired.setMag(MaxSpeed);
            //加速度ベクトルを求める
            VECTOR2 steer = desired - Vel;
            steer.limmit(FleeMaxForce);
            Acc += steer;
        }
    }
    void update() {
        if (isTrigger(KEY_SPACE)) {
            resetRandomPosVel();
        }
        else if (isPress(KEY_S)) {
            arrive(VECTOR2(width/2,height+Radius));
        }
        else if (isPress(KEY_A)) {
            arrive(VECTOR2(-Radius, height / 2));
        }
        else {
            arrive(Target);
            //マウスカーソルから逃げる
            flee(VECTOR2(mouseX, mouseY));
        }
        Vel += Acc;
        Pos += Vel;
        Acc *= 0;
    }
    void draw() {
        colorMode(HSV);
        angleMode(DEGREES);
        stroke(Hue,255,255);
        strokeWeight(Radius);
        point(Pos.x, Pos.y);
    }
};

#include<vector>
void create(std::vector<VEHICLE>& vehicles) {
    //１点、１点、ターゲットの位置（x,y）と色相(hue)を
    //ファイルデータから読み込んで設定する
    FILE* fp;
    fopen_s(&fp, "coding_ocean.csv", "r");
    float ofsX = (width - 1450) / 2;
    float ofsY = (height - 160) / 2;
    float x, y;
    while (fscanf_s(fp, "%f,%f", &x, &y) != EOF) {
        float hue = map(y, 0, 160, 150, 210);
        vehicles.push_back(VEHICLE(x + ofsX, y + ofsY, hue));
    }
    fclose(fp);
}
void gmain() {
    window(1920, 1080, full);
    hideCursor();
    std::vector<VEHICLE> vehicles;
    create(vehicles);
    while (notQuit){
        clear(1);
        point(mouseX, mouseY);
        for (auto& vehicle : vehicles) {
            vehicle.update();
            vehicle.draw();
        }
    }
}
