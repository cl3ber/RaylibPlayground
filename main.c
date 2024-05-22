#include "raylib.h"
#include "rcamera.h"
#define RLIGHTS_IMPLEMENTATION
#include "raymath.h"
#include "extras/rlights.h"

Camera SetupCamera(void)
{
    Camera camera = { 0 };
    camera.position = (Vector3){ 0.0f, 2.0f, 4.0f };
    camera.target = (Vector3){ 0.0f, 2.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 60.0f; //Campo de visão do boneco
    camera.projection = CAMERA_PERSPECTIVE;

    return camera;
}

int MudarCamera(Camera *cam, int camMode)
{
    int mode = camMode;
    // printf(camMode);
    //Dá pra usar o GetKeyPressed para transformar isso num switch case
    if (IsKeyPressed(KEY_ONE))
    {
        mode = CAMERA_FREE;
        cam->up = (Vector3){ 0.0f, 1.0f, 0.0f };
    } 
    else if (IsKeyPressed(KEY_TWO))
    {
        mode = CAMERA_FIRST_PERSON;
        cam->up = (Vector3){ 0.0f, 1.0f, 0.0f };
    }
    else if (IsKeyPressed(KEY_THREE))
    {
        mode = CAMERA_THIRD_PERSON;
        cam->up = (Vector3){ 0.0f, 1.0f, 0.0f };
    }
    else if (IsKeyPressed(KEY_FOUR))
    {
        mode = CAMERA_ORBITAL;
        cam->up = (Vector3){ 0.0f, 1.0f, 0.0f };
    }

    if (IsKeyPressed(KEY_P))
    {
        if (cam->projection == CAMERA_PERSPECTIVE)
        {
            mode = CAMERA_THIRD_PERSON;
            cam->position = (Vector3){ 0.0f, 2.0f, -100.0f };
            cam->target = (Vector3){ 0.0f, 2.0f, 0.0f };
            cam->up = (Vector3){ 0.0f, 1.0f, 0.0f };
            cam->projection = CAMERA_ORTHOGRAPHIC;
            cam->fovy = 20.0f;
            CameraYaw(cam, -135 * DEG2RAD, true);
            CameraPitch(cam, -45 * DEG2RAD, true, true, false);
        }
        else if (cam->projection == CAMERA_ORTHOGRAPHIC)
        {
            mode = CAMERA_THIRD_PERSON;
            cam->position = (Vector3){ 0.0f, 2.0f, 10.0f };
            cam->target = (Vector3){ 0.0f, 2.0f, 0.0f };
            cam->up = (Vector3){ 0.0f, 1.0f, 0.0f };
            cam->projection = CAMERA_PERSPECTIVE;
            cam->fovy = 60.0f;
        }
    }

    return mode;
}

void CreateDebugInfo(Camera camera, int cameraMode) {
    DrawRectangle( 10, 10, 320, 93, Fade(SKYBLUE, 0.5f));
    DrawRectangleLines( 10, 10, 320, 93, BLUE);

    DrawRectangle(5, 5, 330, 100, Fade(SKYBLUE, 0.5f));
    DrawRectangleLines(5, 5, 330, 100, BLUE);

    DrawText("Camera controls:", 15, 15, 10, BLACK);
    DrawText("- Move keys: W, A, S, D, Space, Left-Ctrl", 15, 30, 10, BLACK);
    DrawText("- Look around: arrow keys or mouse", 15, 45, 10, BLACK);
    DrawText("- Camera mode keys: 1, 2, 3, 4", 15, 60, 10, BLACK);
    DrawText("- Zoom keys: num-plus, num-minus or mouse scroll", 15, 75, 10, BLACK);
    DrawText("- Camera projection key: P", 15, 90, 10, BLACK);

    DrawRectangle(600, 5, 195, 100, Fade(SKYBLUE, 0.5f));
    DrawRectangleLines(600, 5, 195, 100, BLUE);

    DrawText("Camera status:", 610, 15, 10, BLACK);
    DrawText(TextFormat("- Mode: %s", (cameraMode == CAMERA_FREE) ? "FREE" :
                                          (cameraMode == CAMERA_FIRST_PERSON) ? "FIRST_PERSON" :
                                              (cameraMode == CAMERA_THIRD_PERSON) ? "THIRD_PERSON" :
                                                  (cameraMode == CAMERA_ORBITAL) ? "ORBITAL" : "CUSTOM"), 610, 30, 10, BLACK);
    DrawText(TextFormat("- Projection: %s", (camera.projection == CAMERA_PERSPECTIVE) ? "PERSPECTIVE" :
                                                (camera.projection == CAMERA_ORTHOGRAPHIC) ? "ORTHOGRAPHIC" : "CUSTOM"), 610, 45, 10, BLACK);
    DrawText(TextFormat("- Position: (%06.3f, %06.3f, %06.3f)", camera.position.x, camera.position.y, camera.position.z), 610, 60, 10, BLACK);
    DrawText(TextFormat("- Target: (%06.3f, %06.3f, %06.3f)", camera.target.x, camera.target.y, camera.target.z), 610, 75, 10, BLACK);
    DrawText(TextFormat("- Up: (%06.3f, %06.3f, %06.3f)", camera.up.x, camera.up.y, camera.up.z), 610, 90, 10, BLACK);
}

int main (void)
{
    const int screenWidth = 1080;
    const int screenHeight = 720;

    SetConfigFlags(FLAG_MSAA_4X_HINT);  // Enable Multi Sampling Anti Aliasing 4x (if available)
    InitWindow(screenWidth, screenHeight, "Testando o boneco");

    Camera camera = SetupCamera();

    int cameraMode = CAMERA_FIRST_PERSON;

    Vector3 cubePosition = { 0.0f, -8.0f, 0.0f };

    SetTargetFPS(60);


    Shader shader = LoadShader("C:\\Users\\cl3be\\OneDrive\\FATEC\\Jogo\\RaylibPlayground\\resources\\lighting.vs", "C:\\Users\\cl3be\\OneDrive\\FATEC\\Jogo\\RaylibPlayground\\resources\\lighting.fs");
    shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(shader, "viewPos");
    int ambientLoc = GetShaderLocation(shader, "ambient");
    SetShaderValue(shader, ambientLoc, (float[4]){ 0.1f, 0.1f, 0.1f, 1.0f }, SHADER_UNIFORM_VEC4);
    Light light = CreateLight(LIGHT_POINT, (Vector3){ -2, 1, -2 }, Vector3Zero(), YELLOW, shader);

    float cameraPos[3] = { camera.position.x, camera.position.y, camera.position.z };
    SetShaderValue(shader, shader.locs[SHADER_LOC_VECTOR_VIEW], cameraPos, SHADER_UNIFORM_VEC3);

    //arvre
    Model model = LoadModel("C:\\Users\\cl3be\\OneDrive\\FATEC\\Jogo\\RaylibPlayground\\resources\\tree.obj");                 // Load model
    Texture2D texture = LoadTexture("C:\\Users\\cl3be\\OneDrive\\FATEC\\Jogo\\RaylibPlayground\\resources\\arvre1.png"); // Load model texture
    model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;
    BoundingBox bounds = GetMeshBoundingBox(model.meshes[0]);

    //Onde a mágica acontece
    //WindowShouldClose é um método que valida se no decorrer do loop o usuário pressionou alguma tecla de interrupção.
    while (!WindowShouldClose())
    {
        DisableCursor();
        /*
        Usa o '&' para passar a referência da câmera para a função. 
        Isso é importante porque na maioria das vezes a passagem é feita por valor (onde uma cópia da câmera seria feita), e nesses casos 
        A câmera que o jogo está usando não seria atualizada. 
        Um exemplo idiota: Imagina que a câemra seja um livro, e você vai pedir um autógrafo:
            - Se você não usar o '&', o autor iria pegar um outro livro idêntico ao teu, autografar e te agradecer (Seu livro tá sem autográfo ainda) - Esse conceito chamamos de passagem por valor.
            - Se você usar o '&', o autor pega o seu livro e autógrafa - Esse conceito chamamos de passagem por referência.
        Nem sempre você quer forçar ou usar a passagem por referência. Mas é bom explicar porque é um conceito muitas vezes esquecido.
        Ref: https://www.codingame.com/playgrounds/24988/programacao-c/passagem-de-parametro-por-referencia
        */
        cameraMode= MudarCamera(&camera, cameraMode);
        UpdateCamera(&camera, cameraMode);

        light.enabled = true;
        light.position = (Vector3){ camera.position.x, camera.position.y, camera.position.z };
        UpdateLightValues(shader, light);
        //Abertura de bloco para indicar que você vai começar a alocar os objetos do jogo.
        BeginDrawing();
            //Não é comum usar identação dentro de um bloco. Aqui faz sentido devido as etapas que esão sendo definidas (Primeiro begindrawning para abrir um buffer e depois no Begin3DMode para inclusão dos objetos)
            ClearBackground(RAYWHITE);

            BeginMode3D(camera);
                //BeginShaderMode(shader);
                //EndShaderMode();
                DrawPlane(Vector3Zero(), (Vector2) { 10.0, 10.0 }, RAYWHITE);
                //DrawCube(Vector3Zero(), 2.0, 4.0, 2.0, BLACK);
                DrawModel(model, cubePosition, 0.1f, WHITE);
                DrawBoundingBox(bounds, RED);

                //DrawPlane((Vector3){ 0.0f, 0.0f, 0.0f }, (Vector2){ 32.0f, 32.0f }, RAYWHITE);
                // DrawCube((Vector3){ -16.0f, 2.5f, 0.0f }, 1.0f, 5.0f, 32.0f, LIGHTGRAY);
                // DrawCube((Vector3){ 16.0f, 2.5f, 0.0f }, 1.0f, 5.0f, 32.0f, LIGHTGRAY);
                // DrawCube((Vector3){ 0.0f, 2.5f, 16.0f }, 32.0f, 5.0f, 1.0f, LIGHTGRAY);
                // DrawCube((Vector3){ 0.0f, 2.5f, -16.0f }, 32.0f, 5.0f, 1.0f, LIGHTGRAY);
                // DrawCube((Vector3){ 0.0f, 2.5f, 16.0f }, 1.0f, 5.0f, 32.0f, LIGHTGRAY);
                //DrawSphereEx(light.position, 0.2f, 8, 8, MAROON);
                // DrawCube(cubePosition, 2.0f, 2.0f, 2.0f, RED);
                // DrawCubeWires(cubePosition, 2.0f, 2.0f, 2.0f, MAROON);

                DrawGrid(32, 1.0f);

                /*if (cameraMode == CAMERA_THIRD_PERSON)
                {
                    DrawCube(camera.target, 0.5f, 0.5f, 0.5f, RED);
                    DrawCubeWires(camera.target, 0.5f, 0.5f, 0.5f, BLACK);
                }*/
            EndMode3D();

            CreateDebugInfo(camera, cameraMode);


        EndDrawing();

    }

    CloseWindow();

    return 0;
}
