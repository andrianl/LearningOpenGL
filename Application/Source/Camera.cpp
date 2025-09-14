#include "Camera.h"
#include "glm/ext/scalar_constants.hpp"

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

// Real camera constants
const float CIRCLE_OF_CONFUSION = 0.03f; // mm for 35mm format

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
	: front(glm::vec3(0.0f, 0.0f, -1.0f)),
	movementSpeed(SPEED),
	mouseSensitivity(SENSITIVITY),
	constrainPitch(true),
	firstMouse(true),
	lastX(0.0f),
	lastY(0.0f)
{
	this->position = position;
	this->worldUp = up;
	this->yaw = yaw;
	this->pitch = pitch;

	// Default real camera settings (similar to Canon 5D Mark IV)
	this->aperture = 5.6f;          // f/5.6
	this->focalLength = 50.0f;      // 50mm lens
	this->iso = 100;                // ISO 100
	this->shutterSpeed = 1.0f / 125.0f; // 1/125 second
	this->sensorSize = glm::vec2(36.0f, 24.0f); // Full frame sensor
	this->focusDistance = 10.0f;    // 10 meters
	this->cameraMode = Mode::MANUAL;
	this->autoFocus = false;

	updateCameraVectors();
	updateFOV();
	updateExposureValue();
}

void Camera::updateCameraVectors()
{
	// Calculate the new front vector
	glm::vec3 newFront;
	newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	newFront.y = sin(glm::radians(pitch));
	newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(newFront);

	// Re-calculate the right and up vector
	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}

void Camera::updateFOV()
{
	// Calculate FOV based on focal length and sensor size
	// FOV = 2 * atan(sensor_size / (2 * focal_length))

	float sensorDiagonal = glm::sqrt(sensorSize.x * sensorSize.x + sensorSize.y * sensorSize.y);
	fov = 2.0f * atan(sensorSize.y / (2.0f * focalLength)) * 180.0f / glm::pi<float>();
}

void Camera::updateExposureValue()
{
	// EV = log2(aperture^2 / shutter_speed) at ISO 100
	// Adjust for different ISO: EV_adjusted = EV + log2(ISO/100)
	float baseEV = log2f((aperture * aperture) / shutterSpeed);
	exposureValue = baseEV + log2f(iso / 100.0f);
}

glm::mat4 Camera::getViewMatrix() const
{
	return glm::lookAt(position, position + front, up);
}

glm::mat4 Camera::getProjectionMatrix(float screenWidth, float screenHeight,
	float nearPlane, float farPlane) const
{
	float aspectRatio = screenWidth / screenHeight;
	return glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
}

void Camera::processMovement(const glm::vec3& movementVector, float deltaTime)
{
	if (glm::length(movementVector) > 0.0f)
	{
		glm::vec3 normalizedMovement = glm::normalize(movementVector);
		float velocity = movementSpeed * deltaTime;

		position += (normalizedMovement.x * right +
			normalizedMovement.y * up +
			normalizedMovement.z * front) * velocity;
	}
}

void Camera::processMouseMovement(float xpos, float ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // Reversed since y-coordinates go from bottom to top
	lastX = xpos;
	lastY = ypos;

	xoffset *= mouseSensitivity;
	yoffset *= mouseSensitivity;

	yaw += xoffset;
	pitch += yoffset;

	// Constrain pitch to prevent screen flipping
	if (constrainPitch)
	{
		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;
	}

	updateCameraVectors();
}

void Camera::processMouseScroll(float yoffset)
{
	// Use scroll for changing focal length (zoom)
	float newFocalLength = focalLength - yoffset * 5.0f;
	setFocalLength(glm::clamp(newFocalLength, 14.0f, 200.0f));
}

void Camera::setAperture(float f_stop)
{
	aperture = glm::clamp(f_stop, 1.0f, 22.0f);
	updateExposureValue();
}

void Camera::setFocalLength(float mm)
{
	focalLength = glm::clamp(mm, 14.0f, 600.0f);
	updateFOV();
}

void Camera::setISO(int value)
{
	// Common ISO values: 50, 100, 200, 400, 800, 1600, 3200, 6400, 12800
	iso = glm::clamp(value, 50, 12800);
	updateExposureValue();
}

void Camera::setShutterSpeed(float seconds)
{
	shutterSpeed = glm::clamp(seconds, 1.0f / 8000.0f, 30.0f);
	updateExposureValue();
}

void Camera::setSensorSize(float width, float height)
{
	sensorSize = glm::vec2(width, height);
	updateFOV();
}

void Camera::setFocusDistance(float distance)
{
	focusDistance = glm::max(distance, 0.1f);
}

void Camera::autoExpose(float targetBrightness)
{
	// Simple auto exposure - adjust ISO first, then shutter speed
	if (cameraMode == Mode::AUTO)
	{
		// This is a simplified auto exposure algorithm
		// In reality, it would be based on light metering
		float currentEV = exposureValue;
		float targetEV = log2f(targetBrightness * 100.0f);

		if (currentEV < targetEV - 1.0f)
		{
			// Increase exposure: lower f-stop or increase ISO
			if (iso < 800)
				setISO(iso * 2);
			else if (aperture > 2.8f)
				setAperture(aperture / 1.4f);
		}
		else if (currentEV > targetEV + 1.0f)
		{
			// Decrease exposure
			if (aperture < 8.0f)
				setAperture(aperture * 1.4f);
			else if (iso > 100)
				setISO(iso / 2);
		}
	}
}

void Camera::autoFocusOnPoint(glm::vec3 worldPoint)
{
	if (autoFocus)
	{
		float distance = glm::length(worldPoint - position);
		setFocusDistance(distance);
	}
}

void Camera::setPortraitMode()
{
	setFocalLength(85.0f);
	setAperture(1.8f);
	setShutterSpeed(1.0f / 125.0f);
	setISO(200);
}

void Camera::setLandscapeMode()
{
	setFocalLength(24.0f);
	setAperture(8.0f);
	setShutterSpeed(1.0f / 60.0f);
	setISO(100);
}

void Camera::setSportsMode()
{
	setFocalLength(200.0f);
	setAperture(2.8f);
	setShutterSpeed(1.0f / 500.0f);
	setISO(400);
}

void Camera::setNightMode()
{
	setFocalLength(50.0f);
	setAperture(1.4f);
	setShutterSpeed(1.0f / 30.0f);
	setISO(1600);
}

float Camera::getDepthOfField() const
{
	// Simplified DOF calculation
	float hyperfocal = getHyperfocalDistance();

	if (focusDistance >= hyperfocal)
	{
		return INFINITY;
	}

	float nearLimit = (hyperfocal * focusDistance) / (hyperfocal + focusDistance);
	float farLimit = (hyperfocal * focusDistance) / (hyperfocal - focusDistance);

	return farLimit - nearLimit;
}

float Camera::getHyperfocalDistance() const
{
	// H = f² / (N * c) + f
	// where f = focal length, N = f-number, c = circle of confusion
	float f_mm = focalLength;
	float f_m = f_mm / 1000.0f; // convert to meters
	float c_m = CIRCLE_OF_CONFUSION / 1000.0f; // convert to meters

	return (f_m * f_m) / (aperture * c_m) + f_m;
}

glm::vec2 Camera::getDOFRange() const
{
	float hyperfocal = getHyperfocalDistance();

	float nearLimit = (hyperfocal * focusDistance) / (hyperfocal + focusDistance);
	float farLimit;

	if (focusDistance >= hyperfocal)
	{
		farLimit = INFINITY;
	}
	else
	{
		farLimit = (hyperfocal * focusDistance) / (hyperfocal - focusDistance);
	}

	return {nearLimit, farLimit};
}