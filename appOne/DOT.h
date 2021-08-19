#pragma once
#include"libOne.h"
class DOT {
    float Hue=0;
    float Radius=0;
    VECTOR2 Target;
    VECTOR2 Pos;
    VECTOR2 Vel;
    VECTOR2 Acc;
    float MaxSpeed=0;
    float MaxForce=0;
    float NearDistance=0;
    float Elapsed = 0;
public:
    DOT(float hue, float targetX, float targetY) {
        //�F�����Ɠ_�̑傫��
        Hue = hue;
        Radius = 9;
        //�ڕW�ʒu
        Target.x = targetX;
        Target.y = targetY;
        //�ʒu�͖ڕW�ʒu�Ɠ����ɂ��Ă���
        Pos = Target;
    }
    void setMaxSpeed(float maxSpeed) {
        MaxSpeed = maxSpeed;
    }
    void setMaxForce(float maxForce) {
        MaxForce = maxForce;
    }
    void setNearDistance(float nearDistance) {
        NearDistance = nearDistance;
    }
    void chase(const VECTOR2& target) {
        //Target�Ɍ������A�]�܂������x�x�N�g��
        VECTOR2 acc = target - Pos;
        //�߂Â�����X�s�[�h��x������
        float speed = MaxSpeed;
        float distance = acc.mag();
        if (distance < NearDistance) {
            speed = map(distance, 0, NearDistance, 0, MaxSpeed);
        }
        //�]�܂������x�x�N�g���̑傫����speed�ɂ���
        acc.setMag(speed);
        //Vel������target�Ɍ����킹������x�x�N�g�������߂�
        acc -= Vel;
        acc.limmit(MaxForce);
        applyForce(acc);
    }
    void runAway(const VECTOR2& target) {
        //target���瓦��������x�x�N�g��
        VECTOR2 acc = Pos - target;
        float distance = acc.mag();
        if (distance < NearDistance) {
            acc.setMag(MaxSpeed);
            acc -= Vel;
            acc.limmit(MaxForce);
            applyForce(acc);
        }
    }
    void applyForce(const VECTOR2& force) {
        Acc += force;
    }
    void setRandomPosVel() {
        //�����ʒu
        Pos.x = random(width);
        Pos.y = random(height);
        //�����x�x�N�g��
        float deg = random(360.0f);
        angleMode(DEGREES);
        Vel.x = cos(deg) * 15;
        Vel.y = sin(deg) * 15;
    }
    void update() {
        setMaxSpeed(20);
        setMaxForce(3);
        setNearDistance(100);
        if (isPress(KEY_SPACE)) {
            Elapsed += delta;
            if (Elapsed > 0.1f) {
                setRandomPosVel();
                Elapsed = 0;
            }
        }
        else if (isPress(KEY_W)) {
            chase(VECTOR2(width / 2, -Radius));
        }
        else if (isPress(KEY_S)) {
            chase(VECTOR2(width / 2, height + Radius));
        }
        else if (isPress(KEY_A)) {
            chase(VECTOR2(-Radius, height / 2));
        }
        else if (isPress(KEY_D)) {
            chase(VECTOR2(Width + Radius, height / 2));
        }
        else {
            chase(Target);
            setMaxSpeed(40);
            setMaxForce(40);
            runAway(VECTOR2(mouseX, mouseY));
            Elapsed = 0.1f;
        }
        move();
    }
    void move() {
        Vel += Acc;
        Pos += Vel;
        Acc *= 0;
    }
    void draw() {
        colorMode(HSV);
        angleMode(DEGREES);
        stroke(Hue, 255, 255);
        strokeWeight(Radius);
        point(Pos.x, Pos.y);
    }
};