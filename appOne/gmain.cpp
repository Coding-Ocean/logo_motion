#include"libOne.h"
class VEHICLE {
public:
    VEHICLE(float x, float y, float hue) {
        Target.x = x;
        Target.y = y;
        Pos.x = random(width);
        Pos.y = random(height);
        float deg = random(360.0f);
        angleMode(DEGREES);
        Vel.x = cos(deg) * 20;
        Vel.y = sin(deg) * 20;
        Radius = 9;
        MaxSpeed = 10;
        MaxForce = 0.5f;
        FleeForce = 10;
        Hue = hue;
    }
    void arrive(VECTOR2 target) {
        //Targetに向かう、望まれるベクトル
        VECTOR2 desired = target - Pos;
        float distance = length(desired);
        //近づいたらスピードを遅くする
        float speed = MaxSpeed;
        if (distance < 100) {
            speed = map(distance, 0, 100, 0, MaxSpeed);
        }
        if (speed > 0) {
            //desireの大きさをspeedにする
            desired *= (speed / distance);
            //加速度ベクトルを求める
            Acc = desired - Vel;
            distance = length(Acc);
            if (distance > MaxForce) {
                //Accの大きさをMaxForceにする
                Acc *= (MaxForce / distance);
            }
        }
    }
    void flee(VECTOR2 target) {
        //targetから逃げるベクトル
        VECTOR2 desired = Pos - target;
        float distance = length(desired);
        if (distance < 80) {
            desired *= (MaxSpeed / distance);
            //加速度ベクトルを求める
            Acc = desired - Vel;
            distance = length(Acc);
            Acc *= (FleeForce / distance);
        }
    }

    void update() {
        if (isPress(KEY_S)) {
            arrive(VECTOR2(width/2,height+10));
        }
        else if (isPress(KEY_A)) {
            arrive(VECTOR2(-5, height / 2));
        }
        else {
            arrive(Target);
            flee(VECTOR2(mouseX, mouseY));
        }
        Pos += Vel;
        Vel += Acc;
        Acc *= 0;
    }
    void show() {
        colorMode(HSV);
        stroke(Hue,255,255);
        strokeWeight(Radius);
        point(Pos.x, Pos.y);
    }

    VECTOR2 Target;
    VECTOR2 Pos;
    VECTOR2 Vel;
    VECTOR2 Acc;
    float Radius;
    float MaxSpeed;
    float MaxForce;
    float FleeForce;
    float Hue;
};


#include<vector>
void gmain() {
    window(1920, 1080, full);
    std::vector<VEHICLE> vehicle;
    FILE* fp;
    fopen_s(&fp, "coding_ocean.csv", "r");
    float ofsX = (width - 1450) / 2;
    float ofsY = (height - 160) / 2;
    float x, y;
    while (fscanf_s(fp, "%f,%f", &x, &y) != EOF) {
        float hue = map(y, 0, 160, 150, 210);
        vehicle.push_back(VEHICLE(ofsX + x, ofsY + y, hue));
    }
    fclose(fp);
    int num = (int)vehicle.size();
    while (notQuit){
        clear(1);
        for (int i = 0; i < num; i++) {
            vehicle[i].update();
            vehicle[i].show();
        }
    }
}


/*
Vehicle.prototype.behaviors = function() {
    arrive = this.arrive(this.target);
    this.applyForce(arrive);

    mouse = createVector(mouseX, mouseY);
    flee = this.flee(mouse);
    this.applyForce(flee);
}
Vehicle.prototype.arrive = function(target) {
    desired = p5.Vector.sub(target, this.pos);
    d = desired.mag();
    speed = this.maxspeed;
    if (d < 100) {
        speed = map(d, 0, 100, 0, this.maxspeed);
    }
    desired.setMag(speed);
    steer = p5.Vector.sub(desired, this.vel);
    steer.limit(this.maxforce);
    return steer;
}
Vehicle.prototype.flee = function(target) {
    desired = p5.Vector.sub(target, this.pos);
    d = desired.mag();
    //steer.mult(0);
    if (d < 80) {
        desired.setMag(this.maxspeed);
        desired.mult(-1);
        steer = p5.Vector.sub(desired, this.vel);
        steer.limit(this.maxforce);
        steer.mult(10);
    }
    return steer;
}
Vehicle.prototype.applyForce = function(f) {
    this.acc.add(f);
}

Vehicle.prototype.update = function() {
    this.pos.add(this.vel);
    this.vel.add(this.acc);
    this.acc.mult(0);
}

Vehicle.prototype.show = function() {
    stroke(255);
    strokeWeight(this.r);
    point(this.pos.x, this.pos.y);
}
*/