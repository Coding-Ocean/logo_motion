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
        //色合いと点の大きさ
        Hue = hue;
        Radius = 9;
        //目標位置
        Target.x = targetX;
        Target.y = targetY;
        //位置は目標位置と同じにしておく
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
        //Targetに向かう、望まれる加速度ベクトル
        VECTOR2 acc = target - Pos;
        //近づいたらスピードを遅くする
        float speed = MaxSpeed;
        float distance = acc.mag();
        if (distance < NearDistance) {
            speed = map(distance, 0, NearDistance, 0, MaxSpeed);
        }
        //望まれる加速度ベクトルの大きさをspeedにする
        acc.setMag(speed);
        //Velを少しtargetに向かわせる加速度ベクトルを求める
        acc -= Vel;
        acc.limmit(MaxForce);
        applyForce(acc);
    }
    void runAway(const VECTOR2& target) {
        //targetから逃げる加速度ベクトル
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
        //初期位置
        Pos.x = random(width);
        Pos.y = random(height);
        //初速度ベクトル
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