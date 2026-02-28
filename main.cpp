#include <raylib.h>

int main(){
    const int screenWidth = 800;
    const int screenHeight = 480;

    InitWindow(screenWidth,screenHeight,"plane example");

    Camera3D camera{0};
    camera.position = (Vector3){0.0f,10.0f,10.0f};
    camera.target = (Vector3){0.0f,0.0f,0.0f};
    camera.up = (Vector3){0.0f,1.0f,0.0f};
    camera.fovy = 60.f;
    camera.projection = CAMERA_PERSPECTIVE;

    Vector3 cubePos = {0.0f,0.75f,0.0f};

    SetTargetFPS(60);
    
    while(!WindowShouldClose()){

        if(IsKeyDown(KEY_W)){
            cubePos.z += 0.1f;
        }
        if(IsKeyDown(KEY_S)){
            cubePos.z -= 0.1f;
        }
        if(IsKeyDown(KEY_D)){
            cubePos.x += 0.1f;
        }
        if(IsKeyDown(KEY_A)){
            cubePos.x -= 0.1f;

        }
        UpdateCamera(&camera,CAMERA_FREE);

        BeginDrawing();
            ClearBackground(RAYWHITE);

            BeginMode3D(camera);

                DrawPlane((Vector3){0,0,0},(Vector2){32,32},LIGHTGRAY);
                DrawCube(cubePos,1.5f,1.5f,1.5f,BLUE);
                DrawCubeWires(cubePos,1.5f,1.5f,1.5f,DARKBLUE);

            EndMode3D();

            DrawText("WASD to move : ",20,20,10,BLACK);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}