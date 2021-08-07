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
        //�_�̑傫���ƐF��
        Radius = 9;
        Hue = hue;
        //�ڕW�ʒu
        Target.x = x;
        Target.y = y;
        //�����ʒu
        Pos.x = -Radius;
        Pos.y = height / 2;
        //�ō����x
        MaxSpeed = 10;
        //�n���h����؂�ō��̗�
        ArriveMaxForce = 0.5f;
        FleeMaxForce = 10;
    }
    void resetRandomPosVel() {
        //�����ʒu
        Pos.x = random(width);
        Pos.y = random(height);
        //�����x�x�N�g��
        float deg = random(360.0f);
        angleMode(DEGREES);
        Vel.x = cos(deg) * 15;
        Vel.y = sin(deg) * 15;
    }
    void arrive(const VECTOR2& target) {
        //Target�Ɍ������A�]�܂��x�N�g��
        VECTOR2 desired = target - Pos;
        //�߂Â�����X�s�[�h��x������
        float speed = MaxSpeed;
        float distance = desired.mag();
        if (distance < 100) {
            speed = map(distance, 0, 100, 0, MaxSpeed);
        }
        //desire�̑傫����speed�ɂ���
        desired.setMag(speed);
        //�n���h����؂�x�N�g�������߂�
        VECTOR2 steer = desired - Vel;
        //Acc�̑傫����ArriveMaxForce�ȉ��ɐ�������
        steer.limmit(ArriveMaxForce);
        Acc += steer;

    }
    void flee(const VECTOR2& target) {
        //target���瓦����x�N�g��
        VECTOR2 desired = Pos - target;
        float distance = desired.mag();
        if (distance < 80) {
            desired.setMag(MaxSpeed);
            //�����x�x�N�g�������߂�
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
            //�}�E�X�J�[�\�����瓦����
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
    //�P�_�A�P�_�A�^�[�Q�b�g�̈ʒu�ix,y�j�ƐF��(hue)��
    //�t�@�C���f�[�^����ǂݍ���Őݒ肷��
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
