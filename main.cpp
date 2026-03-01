#include <raylib.h>
#include <raymath.h>

int main()
{   

    enum GameState{
        MENU,
        GAME
    };

    GameState currentState = MENU;

    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();
    const float boardScale = 0.01f;
    const float skaterScale = 0.025f;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "cheapskater");

    DisableCursor(); // lock mouse like real games

    Camera3D camera = { 0 };
    camera.fovy = 60.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    Vector3 playerPos = { 0.0f, 0.0f, 0.0f };
    float playerYaw = 0.0f;

    const float playerSpeed = 9.0f;
    const float mouseSensitivity = 0.003f;
    const float cameraDistance = 6.0f;
    const float cameraHeight = 3.0f;

    SetTargetFPS(60);

    Model skateboard = LoadModel("resources/skateboard_free_model(1).glb");
    Model skater = LoadModel("resources/village_head.glb");

    Vector3 velocity = {0};
    float acceleration = 25.0f;
    float friction = 5.0f;

    float turnSpeed = 2.5f;
    float leanAmount = 0.0f;

    Vector3 cameraTargetPos = playerPos;
    Vector3 cameraCurrentPos = {0};
    float cameraSmooth = 6.0f;

    while (!WindowShouldClose())
    {
        float delta = GetFrameTime();

        // --- Mouse Look ---
        Vector2 mouseDelta = GetMouseDelta();
        playerYaw -= mouseDelta.x * mouseSensitivity;

        // Forward direction based on yaw
        Vector3 forward = {
            sinf(playerYaw),
            0.0f,
            cosf(playerYaw)
        };

        Vector3 right = {
            forward.z,
            0.0f,
            -forward.x
        };

        // --- Movement ---
        if (IsKeyDown(KEY_W)){
            velocity = Vector3Add(velocity,Vector3Scale(forward,acceleration*delta));
            playerPos = Vector3Add(playerPos, Vector3Scale(forward, playerSpeed * delta));
        }
        if (IsKeyDown(KEY_S)){
            velocity = Vector3Add(velocity,Vector3Scale(forward,-acceleration*delta));
            playerPos = Vector3Add(playerPos, Vector3Scale(forward, -playerSpeed * delta));
        }
        
        float speed = Vector3Length(velocity);

        if(IsKeyDown(KEY_A) && speed > 0.1f){
            playerYaw += turnSpeed*speed*0.05f*delta;
            leanAmount = 20.0f;
        }
        if(IsKeyDown(KEY_D) && speed > 0.1f){
            playerYaw -= turnSpeed*speed*0.05f*delta;   
            leanAmount = -20.0f;
        }

        velocity = Vector3Scale(velocity,1.0f-friction*delta);
        playerPos = Vector3Add(playerPos,Vector3Scale(velocity,delta));

        // --- Camera Position ---
        Vector3 desiredOffset = {
            -sinf(playerYaw)*cameraDistance,
            cameraHeight,
            -cosf(playerYaw)*cameraDistance
        };

        Vector3 desiredPosition = Vector3Add(playerPos,desiredOffset);

        cameraCurrentPos = Vector3Lerp(cameraCurrentPos,desiredPosition,cameraSmooth*delta);
        cameraTargetPos = Vector3Lerp(cameraTargetPos,playerPos,cameraSmooth*delta);

        camera.position = cameraCurrentPos;
        camera.target = cameraTargetPos;
        camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };

        // --- Drawing ---
        BeginDrawing();
        ClearBackground(BLACK);

        if(currentState == MENU){
            DrawText("CHEAPSKATER",350,200,40,GREEN);
            DrawText("Press E to start",330,260,20,DARKGREEN);

            if(IsKeyPressed(KEY_E)){
                currentState = GAME;
            }
        }
        else if(currentState == GAME){

            BeginMode3D(camera);

                DrawPlane((Vector3){0,0,0}, (Vector2){75,75}, LIGHTGRAY);
                // DrawCylinderEx(Vector3{0,1,0},Vector3{10,1,5},0.1f,0.1f,8,YELLOW);
                // DrawCylinderWiresEx(Vector3{0,1,0},Vector3{10,1,5},0.1f,0.1f,8,BLACK);

                Vector3 boardPos = playerPos;
                boardPos.y += 0.15f;

                Vector3 skaterPos = boardPos;
                skaterPos.y += 1.0f;

                leanAmount = Lerp(leanAmount,0.0f,8.0f*delta);
                camera.up = Vector3Normalize((Vector3){leanAmount*0.01f,1.0f,0.0f});

                Matrix rotY = MatrixRotateY(playerYaw);
                Matrix rotZ = MatrixRotateZ(leanAmount*DEG2RAD);
                Matrix transform = MatrixMultiply(rotZ,rotY);

                skateboard.transform = transform;
                skater.transform = transform;

                DrawModel(skater,skaterPos,skaterScale,WHITE);
                DrawModel(skateboard,boardPos,boardScale,WHITE);

            EndMode3D();
            
        }
        EndDrawing();
    }

    UnloadModel(skateboard);
    CloseWindow();
    return 0;
}