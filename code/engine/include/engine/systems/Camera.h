#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include "engine/ecs/System.h"
#include "engine/ecs/Entity.h"

namespace EisEngine::systems {
    using Transform = EisEngine::components::Transform;
    using System = EisEngine::ecs::System;

    // Current supported camera modes: Parallel perspective & Orthographic.
    enum CameraMode{
        PERSPECTIVE = 0,
        ORTHO = 1
    };

    /// The system taking care of the camera, framing the game's graphics to the screen's display.
    class Camera : public System {
    public:
        ~Camera() override;

        /// Creates a new camera for the scene.
        /// @param screenDimensions - Vector2: the width and height of the camera FOV in pixels.
        explicit Camera(Game &engine, const Vector2& screenDimensions, CameraMode cameraMode = PERSPECTIVE);

        /// Calculates and returns the view-projection matrix.
        /// Projection matrix being the directions in which the camera is pointing,
        /// View matrix being the camera's position & rotation in world space.
        /// @return projection x view, a 4x4 matrix representing the camera's FOV from its position
        glm::mat4 GetVPMatrix() const;

        /// Zooms into the scene, restraining the FOV.
        /// @param value: the zoom factor applied to the camera.\n
        /// \zoom @b in: value > 0 \n=> camera will zoom IN on the scene, RESTRAINING the FOV.\n
        /// \n @b out: value &lt 0 \n=> camera will zoom OUT of the scene, WIDENING the FOV.
        void Zoom(float value){ m_zoom = clamp(m_zoom + value, minZoomVal, maxZoomVal);}

        /// Hard set of the camera's zoom value.
        /// @param value: the zoom factor the camera will take on with this call.
        /// default @b value = 1.
        void SetZoom(float value = 1) { m_zoom = clamp(value, minZoomVal, maxZoomVal);}

        /// Returns the current zoom factor.
        [[nodiscard]] float GetZoomFactor() const { return m_zoom;}
        /// Returns screen width in pixels.
        [[nodiscard]] int GetWidth() const { return m_screenWidth;}
        /// Returns screen height in pixels.
        [[nodiscard]] int GetHeight() const { return m_screenHeight;}

        /// A pointer to the transform assigned to this object.
        Transform* transform;
        /// Calculates the projection matrix, representing the transformation from
        /// camera space into 2D screen coordinates.
        [[nodiscard]] glm::mat4 GetProjectionMatrix() const;
        /// Calculates the view matrix, representing the transformation required to
        /// translate world space transforms to camera space.
        [[nodiscard]] glm::mat4 CalculateViewMatrix() const;

        /// Sets a new FOV value for the active camera.
        void SetFOV(const float& val) {fov = val;}
        /// Switches camera mode to the requested style.
        void SetCameraMode(const CameraMode& newMode);
        /// [Debug function] outputs the direction towards which the camera is looking.
        Vector3 viewDirection() const;
        /// Rotates the camera to look at a given point.
        void LookAt(const Vector3& pos) const;
    private:
        /// A pointer to the entity assigned to this object.
        Entity* entity;

        /// The camera's aspect ratio, as width/height.
        float aspectRatio;
        /// The distance at which the camera starts to clip due to proximity to the camera.
        float nearClip;
        /// The distance at which the camera starts to clip due to distance from the camera.
        float farClip = 10;
        /// the screen width in pixels.
        int m_screenWidth;
        /// the screen height in pixels.
        int m_screenHeight;
        /// the zoom value of the current camera.
        float m_zoom = 1.0f;
        /// the furthest the camera can zoom out of the scene.
        float minZoomVal = 0.01f;
        /// the nearest the camera can zoom into the scene.
        float maxZoomVal = 5.0f;
        /// the perspective camera FOV value in degrees.
        float fov = 90;
        /// The camera mode
        CameraMode mode = PERSPECTIVE;

        /// updates the aspect ratio to width/height.
        void UpdateAspectRatio() { aspectRatio = m_screenHeight != 0 ? (float) m_screenWidth / (float) m_screenHeight : 0; }
    };
}
