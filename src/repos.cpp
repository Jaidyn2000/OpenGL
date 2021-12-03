#include "app.cpp"

int main(){
    float deltaTime = 0;
    Triangle tri[100];

    for (int i=0; i<10; i++){
        for (int j=0; j<10; j++){
            tri[10*i+j] = Triangle(-0.5f + 0.1f*i,-0.5f+ 0.1f*j,-0.45f + 0.1f*i,-0.4f +  + 0.1f*j,-0.4f + 0.1f*i,-0.5f + 0.1f*j);
        }
    }
    for (int i=0; i< 100; i++){
            tri[i].drawTriangle();
            tri[i].move(deltaTime/100,0.0f);
        }
}