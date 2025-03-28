// include/Engine/Input/InputManager.hpp
#pragma once

#include <unordered_map>
#include <vector>
#include <array>
#include <functional>
#include <memory>

#include "Engine/Input/InputMapping.hpp"

namespace Engine
{

    class Window;

    /**
     * @brief Input manager class
     *
     * The input manager is responsible for tracking keyboard, mouse, and gamepad input.
     */
    class InputManager
    {
    public:
        /**
         * @brief Constructor
         */
        InputManager();

        /**
         * @brief Destructor
         */
        ~InputManager();

        /**
         * @brief Initializes the input manager
         * @param window Window to capture input from
         * @return True if initialization succeeded, false otherwise
         */
        bool initialize(Window *window);

        /**
         * @brief Updates the input manager
         */
        void update();

        /**
         * @brief Shuts down the input manager
         */
        void shutdown();

        /**
         * @brief Checks if a key is currently pressed
         * @param key Key to check
         * @return True if the key is pressed, false otherwise
         */
        bool isKeyPressed(Key key) const;

        /**
         * @brief Checks if a key was just pressed
         * @param key Key to check
         * @return True if the key was just pressed, false otherwise
         */
        bool isKeyJustPressed(Key key) const;

        /**
         * @brief Checks if a key was just released
         * @param key Key to check
         * @return True if the key was just released, false otherwise
         */
        bool isKeyJustReleased(Key key) const;

        /**
         * @brief Checks if a mouse button is currently pressed
         * @param button Mouse button to check
         * @return True if the mouse button is pressed, false otherwise
         */
        bool isMouseButtonPressed(MouseButton button) const;

        /**
         * @brief Checks if a mouse button was just pressed
         * @param button Mouse button to check
         * @return True if the mouse button was just pressed, false otherwise
         */
        bool isMouseButtonJustPressed(MouseButton button) const;

        /**
         * @brief Checks if a mouse button was just released
         * @param button Mouse button to check
         * @return True if the mouse button was just released, false otherwise
         */
        bool isMouseButtonJustReleased(MouseButton button) const;

        /**
         * @brief Gets the mouse position
         * @return Mouse position
         */
        const Vector2 &getMousePosition() const { return mousePosition; }

        /**
         * @brief Gets the mouse movement since the last update
         * @return Mouse movement
         */
        const Vector2 &getMouseMovement() const { return mouseMovement; }

        /**
         * @brief Gets the mouse scroll wheel movement
         * @return Mouse scroll wheel movement
         */
        const Vector2 &getMouseScroll() const { return mouseScroll; }

        /**
         * @brief Registers a key callback
         * @param key Key to register
         * @param state Key state to register for
         * @param callback Callback function
         * @return Callback ID
         */
        uint32_t registerKeyCallback(Key key, KeyState state, std::function<void()> callback);

        /**
         * @brief Unregisters a key callback
         * @param id Callback ID
         * @return True if the callback was unregistered, false otherwise
         */
        bool unregisterKeyCallback(uint32_t id);

    private:
        /**
         * @brief Window to capture input from
         */
        Window *window;

        /**
         * @brief Current keyboard state
         */
        std::array<bool, static_cast<size_t>(Key::Count)> keyState;

        /**
         * @brief Previous keyboard state
         */
        std::array<bool, static_cast<size_t>(Key::Count)> prevKeyState;

        /**
         * @brief Current mouse button state
         */
        std::array<bool, static_cast<size_t>(MouseButton::Count)> mouseButtonState;

        /**
         * @brief Previous mouse button state
         */
        std::array<bool, static_cast<size_t>(MouseButton::Count)> prevMouseButtonState;

        /**
         * @brief Current mouse position
         */
        Vector2 mousePosition;

        /**
         * @brief Previous mouse position
         */
        Vector2 prevMousePosition;

        /**
         * @brief Mouse movement since the last update
         */
        Vector2 mouseMovement;

        /**
         * @brief Mouse scroll wheel movement
         */
        Vector2 mouseScroll;

        /**
         * @brief Next callback ID
         */
        uint32_t nextCallbackId;

        /**
         * @brief Map of key callbacks
         */
        std::unordered_map<Key, std::vector<std::pair<uint32_t, std::function<void()>>>> keyPressedCallbacks;
        std::unordered_map<Key, std::vector<std::pair<uint32_t, std::function<void()>>>> keyReleasedCallbacks;
    };

} // namespace Engine

// include/Engine/Input/InputMapping.hpp
#pragma once

#include <string>
#include <unordered_map>

namespace Engine
{

    /**
     * @brief Keyboard keys
     */
    enum class Key
    {
        Unknown = 0,

        // Alphanumeric keys
        A,
        B,
        C,
        D,
        E,
        F,
        G,
        H,
        I,
        J,
        K,
        L,
        M,
        N,
        O,
        P,
        Q,
        R,
        S,
        T,
        U,
        V,
        W,
        X,
        Y,
        Z,
        Num0,
        Num1,
        Num2,
        Num3,
        Num4,
        Num5,
        Num6,
        Num7,
        Num8,
        Num9,

        // Function keys
        F1,
        F2,
        F3,
        F4,
        F5,
        F6,
        F7,
        F8,
        F9,
        F10,
        F11,
        F12,

        // Special keys
        Escape,
        Tab,
        CapsLock,
        Shift,
        Control,
        Alt,
        Space,
        Enter,
        Backspace,
        Delete,

        // Arrow keys
        Up,
        Down,
        Left,
        Right,

        Count
    };

    /**
     * @brief Mouse buttons
     */
    enum class MouseButton
    {
        Unknown = 0,
        Left,
        Right,
        Middle,
        Button4,
        Button5,
        Count
    };

    /**
     * @brief Gamepad buttons
     */
    enum class GamepadButton
    {
        Unknown = 0,
        A,
        B,
        X,
        Y,
        LBumper,
        RBumper,
        Back,
        Start,
        Guide,
        LThumb,
        RThumb,
        DPadUp,
        DPadRight,
        DPadDown,
        DPadLeft,
        Count
    };

    /**
     * @brief Gamepad axes
     */
    enum class GamepadAxis
    {
        Unknown = 0,
        LeftX,
        LeftY,
        RightX,
        RightY,
        LTrigger,
        RTrigger,
        Count
    };

    /**
     * @brief Key state
     */
    enum class KeyState
    {
        Pressed,
        Released,
        JustPressed,
        JustReleased
    };

    /**
     * @brief Input action
     */
    struct InputAction
    {
        std::string name;
        bool continuous;
    };

    /**
     * @brief Input binding
     */
    struct InputBinding
    {
        Key key;
        MouseButton mouseButton;
        GamepadButton gamepadButton;
        GamepadAxis gamepadAxis;
        float axisDeadZone;
        float axisScale;
    };

    /**
     * @brief Input mapping manager
     *
     * The input mapping manager is responsible for mapping input actions to input bindings.
     */
    class InputMapping
    {
    public:
        /**
         * @brief Constructor
         */
        InputMapping();

        /**
         * @brief Destructor
         */
        ~InputMapping();

        /**
         * @brief Registers an input action
         * @param name Action name
         * @param continuous True if the action is continuous, false if it's discrete
         * @return True if the action was registered, false if it already exists
         */
        bool registerAction(const std::string &name, bool continuous = false);

        /**
         * @brief Unregisters an input action
         * @param name Action name
         * @return True if the action was unregistered, false if it doesn't exist
         */
        bool unregisterAction(const std::string &name);

        /**
         * @brief Binds a key to an action
         * @param actionName Action name
         * @param key Key to bind
         * @return True if the binding was created, false if the action doesn't exist
         */
        bool bindKey(const std::string &actionName, Key key);

        /**
         * @brief Unbinds a key from an action
         * @param actionName Action name
         * @param key Key to unbind
         * @return True if the binding was removed, false if it doesn't exist
         */
        bool unbindKey(const std::string &actionName, Key key);

        /**
         * @brief Binds a mouse button to an action
         * @param actionName Action name
         * @param button Mouse button to bind
         * @return True if the binding was created, false if the action doesn't exist
         */
        bool bindMouseButton(const std::string &actionName, MouseButton button);

        /**
         * @brief Unbinds a mouse button from an action
         * @param actionName Action name
         * @param button Mouse button to unbind
         * @return True if the binding was removed, false if it doesn't exist
         */
        bool unbindMouseButton(const std::string &actionName, MouseButton button);

        /**
         * @brief Binds a gamepad button to an action
         * @param actionName Action name
         * @param button Gamepad button to bind
         * @return True if the binding was created, false if the action doesn't exist
         */
        bool bindGamepadButton(const std::string &actionName, GamepadButton button);

        /**
         * @brief Unbinds a gamepad button from an action
         * @param actionName Action name
         * @param button Gamepad button to unbind
         * @return True if the binding was removed, false if it doesn't exist
         */
        bool unbindGamepadButton(const std::string &actionName, GamepadButton button);

        /**
         * @brief Binds a gamepad axis to an action
         * @param actionName Action name
         * @param axis Gamepad axis to bind
         * @param deadZone Axis dead zone
         * @param scale Axis scale
         * @return True if the binding was created, false if the action doesn't exist
         */
        bool bindGamepadAxis(const std::string &actionName, GamepadAxis axis, float deadZone = 0.25f, float scale = 1.0f);

        /**
         * @brief Unbinds a gamepad axis from an action
         * @param actionName Action name
         * @param axis Gamepad axis to unbind
         * @return True if the binding was removed, false if it doesn't exist
         */
        bool unbindGamepadAxis(const std::string &actionName, GamepadAxis axis);

    private:
        /**
         * @brief Map of input actions
         */
        std::unordered_map<std::string, InputAction> actions;

        /**
         * @brief Map of input bindings
         */
        std::unordered_map<std::string, std::vector<InputBinding>> bindings;
    };

} // namespace Engine

// include/Engine/Math/Vector.hpp
#pragma once

#include <cmath>

namespace Engine
{

    /**
     * @brief 2D vector
     */
    struct Vector2
    {
        float x, y;

        /**
         * @brief Default constructor
         */
        Vector2() : x(0.0f), y(0.0f) {}

        /**
         * @brief Constructor
         * @param x X component
         * @param y Y component
         */
        Vector2(float x, float y) : x(x), y(y) {}

        /**
         * @brief Constructor (fills all components with the same value)
         * @param value Value to fill with
         */
        explicit Vector2(float value) : x(value), y(value) {}

        /**
         * @brief Addition operator
         * @param other Vector to add
         * @return Result of the addition
         */
        Vector2 operator+(const Vector2 &other) const
        {
            return Vector2(x + other.x, y + other.y);
        }

        /**
         * @brief Subtraction operator
         * @param other Vector to subtract
         * @return Result of the subtraction
         */
        Vector2 operator-(const Vector2 &other) const
        {
            return Vector2(x - other.x, y - other.y);
        }

        /**
         * @brief Multiplication operator (component-wise)
         * @param other Vector to multiply by
         * @return Result of the multiplication
         */
        Vector2 operator*(const Vector2 &other) const
        {
            return Vector2(x * other.x, y * other.y);
        }

        /**
         * @brief Division operator (component-wise)
         * @param other Vector to divide by
         * @return Result of the division
         */
        Vector2 operator/(const Vector2 &other) const
        {
            return Vector2(x / other.x, y / other.y);
        }

        /**
         * @brief Multiplication operator (scalar)
         * @param scalar Scalar to multiply by
         * @return Result of the multiplication
         */
        Vector2 operator*(float scalar) const
        {
            return Vector2(x * scalar, y * scalar);
        }

        /**
         * @brief Division operator (scalar)
         * @param scalar Scalar to divide by
         * @return Result of the division
         */
        Vector2 operator/(float scalar) const
        {
            return Vector2(x / scalar, y / scalar);
        }

        /**
         * @brief Addition assignment operator
         * @param other Vector to add
         * @return Reference to this vector
         */
        Vector2 &operator+=(const Vector2 &other)
        {
            x += other.x;
            y += other.y;
            return *this;
        }

        /**
         * @brief Subtraction assignment operator
         * @param other Vector to subtract
         * @return Reference to this vector
         */
        Vector2 &operator-=(const Vector2 &other)
        {
            x -= other.x;
            y -= other.y;
            return *this;
        }

        /**
         * @brief Multiplication assignment operator (component-wise)
         * @param other Vector to multiply by
         * @return Reference to this vector
         */
        Vector2 &operator*=(const Vector2 &other)
        {
            x *= other.x;
            y *= other.y;
            return *this;
        }

        /**
         * @brief Division assignment operator (component-wise)
         * @param other Vector to divide by
         * @return Reference to this vector
         */
        Vector2 &operator/=(const Vector2 &other)
        {
            x /= other.x;
            y /= other.y;
            return *this;
        }

        /**
         * @brief Multiplication assignment operator (scalar)
         * @param scalar Scalar to multiply by
         * @return Reference to this vector
         */
        Vector2 &operator*=(float scalar)
        {
            x *= scalar;
            y *= scalar;
            return *this;
        }

        /**
         * @brief Division assignment operator (scalar)
         * @param scalar Scalar to divide by
         * @return Reference to this vector
         */
        Vector2 &operator/=(float scalar)
        {
            x /= scalar;
            y /= scalar;
            return *this;
        }

        /**
         * @brief Negation operator
         * @return Negated vector
         */
        Vector2 operator-() const
        {
            return Vector2(-x, -y);
        }

        /**
         * @brief Equality operator
         * @param other Vector to compare with
         * @return True if the vectors are equal, false otherwise
         */
        bool operator==(const Vector2 &other) const
        {
            return x == other.x && y == other.y;
        }

        /**
         * @brief Inequality operator
         * @param other Vector to compare with
         * @return True if the vectors are not equal, false otherwise
         */
        bool operator!=(const Vector2 &other) const
        {
            return !(*this == other);
        }

        /**
         * @brief Calculates the dot product
         * @param other Vector to calculate the dot product with
         * @return Dot product
         */
        float dot(const Vector2 &other) const
        {
            return x * other.x + y * other.y;
        }

        /**
         * @brief Calculates the cross product
         * @param other Vector to calculate the cross product with
         * @return Cross product (scalar)
         */
        float cross(const Vector2 &other) const
        {
            return x * other.y - y * other.x;
        }

        /**
         * @brief Calculates the length squared
         * @return Length squared
         */
        float lengthSquared() const
        {
            return x * x + y * y;
        }

        /**
         * @brief Calculates the length
         * @return Length
         */
        float length() const
        {
            return std::sqrt(lengthSquared());
        }

        /**
         * @brief Normalizes the vector
         * @return Normalized vector
         */
        Vector2 normalized() const
        {
            float len = length();
            if (len == 0.0f)
            {
                return Vector2();
            }
            return Vector2(x / len, y / len);
        }

        /**
         * @brief Normalizes the vector in-place
         * @return Reference to this vector
         */
        Vector2 &normalize()
        {
            float len = length();
            if (len == 0.0f)
            {
                x = 0.0f;
                y = 0.0f;
            }
            else
            {
                x /= len;
                y /= len;
            }
            return *this;
        }

        /**
         * @brief Calculates the distance squared to another vector
         * @param other Vector to calculate the distance to
         * @return Distance squared
         */
        float distanceSquared(const Vector2 &other) const
        {
            float dx = x - other.x;
            float dy = y - other.y;
            return dx * dx + dy * dy;
        }

        /**
         * @brief Calculates the distance to another vector
         * @param other Vector to calculate the distance to
         * @return Distance
         */
        float distance(const Vector2 &other) const
        {
            return std::sqrt(distanceSquared(other));
        }

        /**
         * @brief Linear interpolation between two vectors
         * @param other Vector to interpolate to
         * @param t Interpolation factor (0.0 to 1.0)
         * @return Interpolated vector
         */
        Vector2 lerp(const Vector2 &other, float t) const
        {
            return Vector2(x + (other.x - x) * t, y + (other.y - y) * t);
        }

        /**
         * @brief Zero vector
         */
        static const Vector2 Zero;

        /**
         * @brief One vector (all components set to 1.0f)
         */
        static const Vector2 One;

        /**
         * @brief Unit vector pointing right
         */
        static const Vector2 Right;

        /**
         * @brief Unit vector pointing up
         */
        static const Vector2 Up;
    };

    /**
     * @brief 3D vector
     */
    struct Vector3
    {
        float x, y, z;

        /**
         * @brief Default constructor
         */
        Vector3() : x(0.0f), y(0.0f), z(0.0f) {}

        /**
         * @brief Constructor
         * @param x X component
         * @param y Y component
         * @param z Z component
         */
        Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

        /**
         * @brief Constructor (fills all components with the same value)
         * @param value Value to fill with
         */
        explicit Vector3(float value) : x(value), y(value), z(value) {}

        /**
         * @brief Constructor from Vector2
         * @param xy XY components
         * @param z Z component
         */
        Vector3(const Vector2 &xy, float z) : x(xy.x), y(xy.y), z(z) {}

        // Similar operations as Vector2, plus:

        /**
         * @brief Calculates the cross product
         * @param other Vector to calculate the cross product with
         * @return Cross product (vector)
         */
        Vector3 cross(const Vector3 &other) const
        {
            return Vector3(
                y * other.z - z * other.y,
                z * other.x - x * other.z,
                x * other.y - y * other.x);
        }

        /**
         * @brief Zero vector
         */
        static const Vector3 Zero;

        /**
         * @brief One vector (all components set to 1.0f)
         */
        static const Vector3 One;

        /**
         * @brief Unit vector pointing right
         */
        static const Vector3 Right;

        /**
         * @brief Unit vector pointing up
         */
        static const Vector3 Up;

        /**
         * @brief Unit vector pointing forward
         */
        static const Vector3 Forward;
    };

    /**
     * @brief 4D vector
     */
    struct Vector4
    {
        float x, y, z, w;

        /**
         * @brief Default constructor
         */
        Vector4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}

        /**
         * @brief Constructor
         * @param x X component
         * @param y Y component
         * @param z Z component
         * @param w W component
         */
        Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

        /**
         * @brief Constructor (fills all components with the same value)
         * @param value Value to fill with
         */
        explicit Vector4(float value) : x(value), y(value), z(value), w(value) {}

        /**
         * @brief Constructor from Vector3
         * @param xyz XYZ components
         * @param w W component
         */
        Vector4(const Vector3 &xyz, float w) : x(xyz.x), y(xyz.y), z(xyz.z), w(w) {}

        /**
         * @brief Constructor from Vector2
         * @param xy XY components
         * @param z Z component
         * @param w W component
         */
        Vector4(const Vector2 &xy, float z, float w) : x(xy.x), y(xy.y), z(z), w(w) {}

        // Similar operations as Vector2 and Vector3

        /**
         * @brief Zero vector
         */
        static const Vector4 Zero;

        /**
         * @brief One vector (all components set to 1.0f)
         */
        static const Vector4 One;
    };

    // Static member initialization
    // This would be in the cpp file, but for brevity we'll include it here
    inline const Vector2 Vector2::Zero = Vector2(0.0f, 0.0f);
    inline const Vector2 Vector2::One = Vector2(1.0f, 1.0f);
    inline const Vector2 Vector2::Right = Vector2(1.0f, 0.0f);
    inline const Vector2 Vector2::Up = Vector2(0.0f, 1.0f);

    inline const Vector3 Vector3::Zero = Vector3(0.0f, 0.0f, 0.0f);
    inline const Vector3 Vector3::One = Vector3(1.0f, 1.0f, 1.0f);
    inline const Vector3 Vector3::Right = Vector3(1.0f, 0.0f, 0.0f);
    inline const Vector3 Vector3::Up = Vector3(0.0f, 1.0f, 0.0f);
    inline const Vector3 Vector3::Forward = Vector3(0.0f, 0.0f, 1.0f);

    inline const Vector4 Vector4::Zero = Vector4(0.0f, 0.0f, 0.0f, 0.0f);
    inline const Vector4 Vector4::One = Vector4(1.0f, 1.0f, 1.0f, 1.0f);

} // namespace Engine