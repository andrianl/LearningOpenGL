#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include <algorithm>
#include <cmath>

class Camera
{
public:
	// Camera movement directions
	enum Movement {
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT,
		UP,
		DOWN
	};

	// Camera modes
	enum Mode {
		MANUAL,
		APERTURE_PRIORITY,
		SHUTTER_PRIORITY,
		AUTO
	};

private:
	// Camera position and orientation
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	// Euler angles
	float yaw;
	float pitch;

	// Real camera parameters
	float aperture;           // f-stop value (f/1.4, f/2.8, etc.)
	float focalLength;        // in millimeters (24mm, 50mm, 85mm, etc.)
	int iso;                  // ISO sensitivity (100, 200, 400, 800, etc.)
	float shutterSpeed;       // in seconds (1/60, 1/125, 1/250, etc.)
	glm::vec2 sensorSize;     // sensor size in mm (36x24 for full frame)

	// Calculated values
	float fov;                // Field of view based on focal length and sensor size
	float exposureValue;      // EV calculated from aperture, shutter, ISO

	// Camera mode and settings
	Mode cameraMode;
	bool autoFocus;
	float focusDistance;      // distance to focus point in meters

	// Movement and control settings
	float movementSpeed;
	float mouseSensitivity;
	bool constrainPitch;

	// FPS camera specific
	bool firstMouse;
	float lastX, lastY;

	// Internal methods
	void updateCameraVectors();
	void updateFOV();
	void updateExposureValue();

public:
	// Constructor
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f),
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
		float yaw = -90.0f,
		float pitch = 0.0f);

	// Core functionality
	glm::mat4 getViewMatrix() const;
	glm::mat4 getProjectionMatrix(float screenWidth, float screenHeight,
		float nearPlane = 0.1f, float farPlane = 100.0f) const;

	// FPS Camera controls
	void processMovement(const glm::vec3& movementVector, float deltaTime);
	void processMouseMovement(float xpos, float ypos);
	void processMouseScroll(float yoffset);

	// Real camera functionality
	void setMode(Mode mode) { cameraMode = mode; }
	void autoExpose(float targetBrightness = 0.5f); // Auto exposure calculation
	void autoFocusOnPoint(glm::vec3 worldPoint);     // Auto focus on world point

	// Lens and exposure controls
	void setAperture(float f_stop);
	void setFocalLength(float mm);
	void setISO(int value);
	void setShutterSpeed(float seconds);
	void setSensorSize(float width, float height);
	void setFocusDistance(float distance);

	// Camera presets (common real camera settings)
	void setPortraitMode();     // 85mm, f/1.8
	void setLandscapeMode();    // 24mm, f/8
	void setSportsMode();       // 200mm, f/2.8, high shutter
	void setNightMode();        // High ISO, wide aperture

	// Depth of Field calculations
	float getDepthOfField() const;           // Total DOF in meters
	float getHyperfocalDistance() const;     // Hyperfocal distance
	glm::vec2 getDOFRange() const;          // Near and far DOF limits

	// Getters
	float getAperture() const { return aperture; }
	float getFocalLength() const { return focalLength; }
	int getISO() const { return iso; }
	float getShutterSpeed() const { return shutterSpeed; }
	glm::vec2 getSensorSize() const { return sensorSize; }
	float getFOV() const { return fov; }
	float getExposureValue() const { return exposureValue; }
	float getFocusDistance() const { return focusDistance; }
	Mode getMode() const { return cameraMode; }

	glm::vec3 getPosition() const { return position; }
	glm::vec3 getFront() const { return front; }
	glm::vec3 getUp() const { return up; }
	glm::vec3 getRight() const { return right; }

	float getYaw() const { return yaw; }
	float getPitch() const { return pitch; }

	// Movement settings
	void setMovementSpeed(float speed) { movementSpeed = speed; }
	void setMouseSensitivity(float sensitivity) { mouseSensitivity = sensitivity; }
	float getMovementSpeed() const { return movementSpeed; }
	float getMouseSensitivity() const { return mouseSensitivity; }
};