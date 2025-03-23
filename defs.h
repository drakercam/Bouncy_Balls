#ifndef DEFS_H
#define DEFS_H

#include <string>
#include <raylib.h>
#define RAYGUI_IMPLEMENTATION
#include <raygui.h>
#include <rlgl.h>
#include <raymath.h>
#include <iostream>

// this is where I will define many structs I will use for rendering

namespace Raylib
{
    typedef struct Window
    {
        int windowWidth;
        int windowHeight;
        std::string name;
        Color bg;

        void CreateWindow(const int& width, const int& height, const std::string& name, const Color& c)
        {
            this->windowWidth = width;
            this->windowHeight = height;
            this->name = name;
            this->bg = c;
        }

    } Window;

    typedef struct Camera
    {
        Camera3D cam;

        void SetupCamera()
        {
            cam.position = (Vector3){ 0.0f, 2.0f, 10.0f };   // Move camera back
            cam.target = (Vector3){ 0.0f, 0.0f, 0.0f };     // Look at target pos
            cam.up = (Vector3){ 0.0f, 1.0f, 0.0f };
            cam.fovy = 45.0f;
            cam.projection = CAMERA_PERSPECTIVE;
        }

        void MoveCamera(float dt)
        {
            float move_speed = 10.0f * dt;  // Movement speed
            float rotation_speed = 1.5f * dt; // Rotation speed

            // Forward direction (z-axis movement)
            Vector3 forward = Vector3Normalize(Vector3Subtract(cam.target, cam.position));
            // Right direction (x-axis movement)
            Vector3 right = Vector3CrossProduct(forward, cam.up);

            // Move forward/backward
            if (IsKeyDown(KEY_W))
            {
                cam.position = Vector3Add(cam.position, Vector3Scale(forward, move_speed));
                cam.target = Vector3Add(cam.target, Vector3Scale(forward, move_speed));
            }
            if (IsKeyDown(KEY_S))
            {
                cam.position = Vector3Subtract(cam.position, Vector3Scale(forward, move_speed));
                cam.target = Vector3Subtract(cam.target, Vector3Scale(forward, move_speed));
            }

            // Move left/right
            if (IsKeyDown(KEY_A))
            {
                cam.position = Vector3Subtract(cam.position, Vector3Scale(right, move_speed));
                cam.target = Vector3Subtract(cam.target, Vector3Scale(right, move_speed));
            }
            if (IsKeyDown(KEY_D))
            {
                cam.position = Vector3Add(cam.position, Vector3Scale(right, move_speed));
                cam.target = Vector3Add(cam.target, Vector3Scale(right, move_speed));
            }

            // Move up/down
            if (IsKeyDown(KEY_UP))
            {
                cam.position.y += move_speed;
                cam.target.y += move_speed;
            }
            if (IsKeyDown(KEY_DOWN))
            {
                cam.position.y -= move_speed;
                cam.target.y -= move_speed;
            }

            // Rotate camera left/right (yaw rotation)
            if (IsKeyDown(KEY_E))
            {
                Matrix rotation = MatrixRotateY(-rotation_speed);
                Vector3 direction = Vector3Transform(Vector3Subtract(cam.target, cam.position), rotation);
                cam.target = Vector3Add(cam.position, direction);
            }
            if (IsKeyDown(KEY_Q))
            {
                Matrix rotation = MatrixRotateY(rotation_speed);
                Vector3 direction = Vector3Transform(Vector3Subtract(cam.target, cam.position), rotation);
                cam.target = Vector3Add(cam.position, direction);
            }
        }
    } Camera;

    typedef struct Text
    {
        std::string text;
        int x, y;
        int fontSize;
        Color color;
        
        void CreateText(const std::string& t, int x, int y, int fs, const Color& color)
        {
            this->text = t;
            std::cout << text << std::endl;
            this->x = x;
            this->y = y;
            this->fontSize = fs;
            this->color = color;
        }

        void CustomDrawText() { DrawText(text.c_str(), x, y, fontSize, color); }

    } Text;

    typedef struct CustomTexture2D
    {
        Texture2D tex;
        int x, y;
        Color tint;

        void CreateTexture(const Texture2D& t, const int& x, const int& y, const Color& c)
        {
            this->tex = t;
            this->x = x;
            this->y = y;
            this->tint = c;
        }

        void DrawTextureCustom()
        {
            DrawTexture(tex, x, y, tint);
        }

    } CustomTexture2D;

    typedef struct Rectangle 
    {   
        int x;
        int y;
        int width;
        int height;
        Color color;

        void CreateRectangle(int x, int y, int w, int h)
        {
            this->x = x;
            this->y = y;
            this->width = w;
            this->height = h;
        }

        void DrawFilledRect() { DrawRectangle(x, y, width, height, color); }

        void DrawRectLines(const Color& border) { DrawRectangleLines(x, y, width, height, border); }

        void DrawGradVRect(const Color& top, const Color& bottom) { DrawRectangleGradientV(x, y, width, height, top, bottom); }

        void DrawGradHRect(const Color& left, const Color& right) { DrawRectangleGradientH(x, y, width, height, left, right); }

    } Rectangle;

    typedef struct Cube 
    {
        Vector3 position;
        float width, height, length, rotation_angle;
        Color color;
        Camera camera;              // A cube will have its own camera so I can switch between no-clip and follow view
                                    // will eventually implement a camera for each system allowing us to switch between object
                                    // control, will use ray-object collision to determine if the camera is looking at object
                                    // and if so, if the player presses a certain button, then they set the camera to the camera
                                    // of that object, so they become it (will become useful once I introduce models)

        void SetupCubeCamera()
        {
            camera.cam.position = (Vector3){ position.x, position.y, 10.0f };   // Move camera back
            camera.cam.target = (Vector3){ position.x, position.y, 0.0f };                  // Look at target pos
            camera.cam.up = (Vector3){ 0.0f, 1.0f, 0.0f };
            camera.cam.fovy = 60.0f;
            camera.cam.projection = CAMERA_PERSPECTIVE;
        }

        void CreateCube(const Vector3& pos, float w, float h, float l, const Color& c)
        {
            this->position = pos;
            this->width = w;
            this->height = h;
            this->length = l;
            this->color = c;
            this->rotation_angle = 0.0f;
        }

        void Draw3DCube()
        {
            rlPushMatrix();
            rlTranslatef(position.x, position.y, position.z);
            rlRotatef(rotation_angle, 0.0f, 1.0f, 0.0f); // Rotate around Y-axis
            DrawCube(Vector3{0, 0, 0}, width, height, length, color);
            rlPopMatrix();
        }

        void Draw3DCubeLines(const Color& border)
        {
            rlPushMatrix();
            rlTranslatef(position.x, position.y, position.z);
            rlRotatef(rotation_angle, 0.0f, 1.0f, 0.0f); // Rotate around Y-axis
            DrawCubeWires(Vector3{0, 0, 0}, width, height, length, border);
            rlPopMatrix();
        }

        void MoveCube(float dt)
        {
            float move_speed = 10.0f * dt;   // Movement speed
            float rotation_speed = 1.5f * dt; // Rotation speed

            // Forward direction
            Vector3 forward = Vector3Normalize(Vector3Subtract(camera.cam.target, camera.cam.position));
            Vector3 right = Vector3CrossProduct(forward, camera.cam.up);

            // Move cube forward/backward
            if (IsKeyDown(KEY_W))
            {
                position = Vector3Add(position, Vector3Scale(forward, move_speed));
            }
            if (IsKeyDown(KEY_S))
            {
                position = Vector3Subtract(position, Vector3Scale(forward, move_speed));
            }

            // Move cube left/right
            if (IsKeyDown(KEY_A))
            {
                position = Vector3Subtract(position, Vector3Scale(right, move_speed));
            }
            if (IsKeyDown(KEY_D))
            {
                position = Vector3Add(position, Vector3Scale(right, move_speed));
            }

            // Rotate cube left/right
            if (IsKeyDown(KEY_Q)) // Rotate left
            {
                Matrix rotation = MatrixRotateY(rotation_speed);
                Vector3 direction = Vector3Subtract(camera.cam.target, camera.cam.position);
                direction = Vector3Transform(direction, rotation);
                camera.cam.target = Vector3Add(camera.cam.position, direction);

                rotation_angle += 1.5f;
            }

            if (IsKeyDown(KEY_E)) // Rotate right
            {
                Matrix rotation = MatrixRotateY(-rotation_speed);
                Vector3 direction = Vector3Subtract(camera.cam.target, camera.cam.position);
                direction = Vector3Transform(direction, rotation);
                camera.cam.target = Vector3Add(camera.cam.position, direction);

                rotation_angle -= 1.5f;
            }

            // Move up/down with a tilt
            if (IsKeyDown(KEY_UP))
            {
                camera.cam.position.y += move_speed;  // Move camera up
                camera.cam.target.y += move_speed * 0.7f; // Slight tilt downward
            }
            if (IsKeyDown(KEY_DOWN))
            {
                camera.cam.position.y -= move_speed; // Move camera down
                camera.cam.target.y -= move_speed * 0.7f; // Slight tilt upward

                // Reset when reaching original height
                if (camera.cam.position.y <= position.y + 2.0f) 
                {
                    camera.cam.position.y = position.y; // Reset height
                    camera.cam.target.y = position.y; // Reset target look level
                }
            }


            // Rotate camera around the cube
            if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_RIGHT))
            {
                float angle = (IsKeyDown(KEY_LEFT) ? rotation_speed : -rotation_speed); // Determine rotation direction
                Matrix rotation = MatrixRotateY(angle);

                // Get current direction from cube to camera
                Vector3 direction = Vector3Subtract(camera.cam.position, position);
                
                // Rotate the direction
                direction = Vector3Transform(direction, rotation);

                // Update camera position and ensure it still looks at the cube
                camera.cam.position = Vector3Add(position, direction);
                camera.cam.target = position;  // Keep looking at the cube
            }


            // Ensure camera is always looking at the cube
            camera.cam.target = position;

            // UpdateCameraPosition();        // Keep camera behind cube
        }

        void UpdateCameraPosition()
        {
            Vector3 offset = { 0.0f, 2.0f, 10.0f }; // Offset behind cube

            // Set camera position behind the cube
            camera.cam.position = (Vector3){ position.x, position.y + offset.y, position.z + offset.z };

            // Ensure the camera always looks at the cube
            camera.cam.target = position;

            // Debugging camera position
            std::cout << "Camera Position: " << camera.cam.position.x << ", " 
                    << camera.cam.position.y << ", " << camera.cam.position.z << std::endl;
            std::cout << "Camera Target: " << camera.cam.target.x << ", " 
                    << camera.cam.target.y << ", " << camera.cam.target.z << std::endl;
        }


    } Cube;

    typedef struct Circle 
    {
        Vector2 position;
        float radius;
        Color color;
        Vector2 velocity;

        void CreateCircle(int x, int y, float r, const Color& c)
        {
            this->position.x = x;
            this->position.y = y;
            this->radius = r;
            this->color = c;
        }

        void DrawFilledCircle() { DrawCircle(position.x, position.y, radius, color); }
        void DrawCircleOutline(const Color& border) { DrawCircleLines(position.x, position.y, radius, border); }
        void DrawGradCircle(const Color& inner, const Color& outer) { DrawCircleGradient(position.x, position.y, radius, inner, outer); } 

    } Circle;

    typedef struct Ellipse
    {
        int centerX;
        int centerY;
        float radiusH;
        float radiusV;
        Color color;

        void CreateEllipse(int x, int y, float rH, float rV, const Color& color)
        {
            this->centerX = x;
            this->centerY = y;
            this->radiusH = rH;
            this->radiusV = rV;
            this->color = color;
        }

        void DrawFilledEllipse() { DrawEllipse(centerX, centerY, radiusH, radiusV, color); }
        void DrawEllipseOutline(const Color& border) { DrawEllipseLines(centerX, centerY, radiusH, radiusV, border); }

    } Ellipse;

    typedef struct Sphere 
    {
        Vector3 centerPos;
        float radius;
        Color color;

        void CreateSphere(const Vector3& centerP, float r, const Color& c)
        {
            this->centerPos = centerP;
            this->radius = r;
            this->color = c;
        }

        void Draw3DSphere() { DrawSphere(centerPos, radius, color); }
        void Draw3DSphereLines(const Color& border) { DrawSphereWires(centerPos, radius, 16, 16, border); }

    } Sphere;

    typedef struct Triangle
    {
        Vector2 v1, v2, v3;
        Color color;

        void CreateTriangle(const Vector2& v1, const Vector2& v2, const Vector2& v3, const Color& c)
        {
            this->v1 = v1;
            this->v2 = v2;
            this->v3 = v3;
            this->color = color;
        }

        // remember vertexs go in clockwise order
        void DrawFilledTriangle() { DrawTriangle(v1, v2, v3, color); }

    } Triangle;

    typedef struct Line
    {
        Vector2 start;
        Vector2 end;
        Color color;

        void CreateLine(const int sX, const int sY, const int eX, const int eY, const Color& c)
        {
            this->start.x = sX;
            this->start.y = sY;
            this->end.x = eX;
            this->end.y = eY;
            this->color = c;
        }

        void DrawLineFilled() { DrawLine(start.x, start.y, end.x, end.y, color); }

    } Line;

    typedef struct Cylinder
    {
        Vector3 position;
        float radiusTop;
        float radiusBottom;
        float height;
        int slices;
        Color color;

        void CreateCylinder(const Vector3& pos, float rT, float rB, float h, int s, const Color& c)
        {
            this->position = pos;
            this->radiusTop = rT;
            this->radiusBottom = rB;
            this->height = h;
            this->slices = s;
            this->color = c;
        }

        void Draw3DCylinder() { DrawCylinder(position, radiusTop, radiusBottom, height, slices, color); }
        void Draw3DCylinderLines(const Color& border) { DrawCylinderWires(position, radiusTop, radiusBottom, height, slices, border); }

    } Cylinder;

    void DrawTexturedCube(const Texture2D& tex, const Shader& shader)
    {
        rlEnableShader(shader.id);
        rlEnableTexture(tex.id);
        rlBegin(RL_QUADS);

        // Front face
        rlTexCoord2f(0, 0); rlVertex3f(-1, -1, -1);
        rlTexCoord2f(1, 0); rlVertex3f( 1, -1, -1);
        rlTexCoord2f(1, 1); rlVertex3f( 1,  1, -1);
        rlTexCoord2f(0, 1); rlVertex3f(-1,  1, -1);

        // Back face
        rlTexCoord2f(1, 0); rlVertex3f(-1, -1,  1);
        rlTexCoord2f(1, 1); rlVertex3f(-1,  1,  1);
        rlTexCoord2f(0, 1); rlVertex3f( 1,  1,  1);
        rlTexCoord2f(0, 0); rlVertex3f( 1, -1,  1);

        rlEnd();
        rlDisableTexture();
        rlDisableShader();
    }

    Mesh CreateTexturedCube()
    {
        Mesh mesh = { 0 };
        mesh.triangleCount = 12;
        mesh.vertexCount = 24;

        mesh.vertices = (float *)MemAlloc(mesh.vertexCount * 3 * sizeof(float));
        mesh.texcoords = (float *)MemAlloc(mesh.vertexCount * 2 * sizeof(float));
        mesh.indices = (unsigned short *)MemAlloc(mesh.triangleCount * 3 * sizeof(unsigned short));

        float verts[] = {
            // Front face
            -1, -1, -1,  1, -1, -1,  1,  1, -1, -1,  1, -1,
            // Back face
            -1, -1,  1, -1,  1,  1,  1,  1,  1,  1, -1,  1,
            // Left face
            -1, -1, -1, -1,  1, -1, -1,  1,  1, -1, -1,  1,
            // Right face
            1, -1, -1,  1, -1,  1,  1,  1,  1,  1,  1, -1,
            // Top face
            -1,  1, -1,  1,  1, -1,  1,  1,  1, -1,  1,  1,
            // Bottom face
            -1, -1, -1, -1, -1,  1,  1, -1,  1,  1, -1, -1
        };

        float uvs[] = {
            0, 0,  1, 0,  1, 1,  0, 1,
            1, 0,  1, 1,  0, 1,  0, 0,
            0, 0,  1, 0,  1, 1,  0, 1,
            1, 0,  1, 1,  0, 1,  0, 0,
            0, 1,  1, 1,  1, 0,  0, 0,
            0, 1,  1, 1,  1, 0,  0, 0
        };

        unsigned short indices[] = {
            0, 1, 2,  0, 2, 3,    // Front
            4, 5, 6,  4, 6, 7,    // Back
            8, 9,10,  8,10,11,    // Left
            12,13,14, 12,14,15,   // Right
            16,17,18, 16,18,19,   // Top
            20,21,22, 20,22,23    // Bottom
        };

        memcpy(mesh.vertices, verts, sizeof(verts));
        memcpy(mesh.texcoords, uvs, sizeof(uvs));
        memcpy(mesh.indices, indices, sizeof(indices));

        UploadMesh(&mesh, true);
        return mesh;
    }

    void RotateModel(Model *model, float angle, Vector3 axis)
    {
        Matrix rotation = MatrixRotate(axis, angle * DEG2RAD);
        model->transform = MatrixMultiply(model->transform, rotation);
    }

};

#endif