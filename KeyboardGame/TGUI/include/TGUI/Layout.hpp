/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TGUI - Texus' Graphical User Interface
// Copyright (C) 2012-2023 Bruno Van de Velde (vdv_b@tgui.eu)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef TGUI_LAYOUT_HPP
#define TGUI_LAYOUT_HPP

#include <TGUI/Config.hpp>
#include <TGUI/Vector2.hpp>

#if !TGUI_EXPERIMENTAL_USE_STD_MODULE
    #include <type_traits>
    #include <functional>
    #include <memory>
    #include <string>
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TGUI_MODULE_EXPORT namespace tgui
{
    class BackendGui;
    class Widget;
    class Container;

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Class to store the left, top, width or height of a widget
    ///
    /// You don't have to create an instance of this class, numbers are implicitly cast to this class.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    class TGUI_API Layout
    {
    public:

        /// The operation which the layout has to perform to find its value
        enum class Operation
        {
            Value,
            Plus,
            Minus,
            Multiplies,
            Divides,
            Minimum,
            Maximum,
            BindingPosX, // X position, same as BindingLeft if widget origin isn't changed
            BindingPosY, // Y position, same as BindingTop if widget origin isn't changed
            BindingLeft,
            BindingTop,
            BindingWidth,
            BindingHeight,
            BindingInnerWidth,
            BindingInnerHeight,
            BindingString
        };


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    public:

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Default constructor
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        Layout() = default;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Constructor to implicitly construct from numeric constant
        ///
        /// @param constant  Value of the layout
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        template <typename T, typename = typename std::enable_if_t<std::is_arithmetic<T>::value, T>>
        Layout(T constant) :
            m_value{static_cast<float>(constant)}
        {
        }


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Constructs the layout based on a string which will be parsed to determine the value of the layout
        ///
        /// @param expression  String to parse
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        Layout(const char* expression) :
            Layout{String{expression}}
        {
        }


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Constructs the layout based on a string which will be parsed to determine the value of the layout
        ///
        /// @param expression  String to parse
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        Layout(String expression);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @internal
        /// @brief Constructs a layout with a binding to a widget
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        explicit Layout(Operation operation, Widget* boundWidget);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @internal
        /// @brief Constructs the layout with a mathematical operation between two other layouts
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        explicit Layout(Operation operation, std::unique_ptr<Layout> leftOperand, std::unique_ptr<Layout> rightOperand);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Copy constructor
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        Layout(const Layout& other);

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Move constructor
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        Layout(Layout&& other) noexcept;

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Copy assignment operator
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        Layout& operator=(const Layout& other);

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Move assignment operator
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        Layout& operator=(Layout&& other) noexcept;

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Destructor
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ~Layout();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Replaces the value of the layout without overwriting its parent
        ///
        /// @param value  New value of the layout
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void replaceValue(const Layout& value);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Return the cached value of the layout
        ///
        /// @return Value of the layout
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        TGUI_NODISCARD float getValue() const
        {
            return m_value;
        }


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Return whether the layout stores a constant value
        ///
        /// @return Value of the layout
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        TGUI_NODISCARD bool isConstant() const
        {
            return m_operation == Operation::Value;
        }


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @internal
        /// @brief Converts the layout to a string representation
        ///
        /// @return String representation of layout
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        TGUI_NODISCARD String toString() const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @internal
        /// @brief Provides the layout access to the widget (and its parent) which allows searching potentially referred widgets
        ///        and sets a callback function so that the widget can be alerted when the value of the widget changes.
        ///
        /// The xAxis parameter tells the layout whether it contains the left/width or top/height value.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void connectWidget(Widget* widget, bool xAxis, std::function<void()> valueChangedCallbackHandler);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @internal
        /// @brief If the layout has bound a widget and the widget gets destroyed, this function is called
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void unbindWidget();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @internal
        /// @brief Recalculate the value of the layout based on the operation it contains and tell the parent layout to
        ///        do the same when the value of the layout has changed.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void recalculateValue();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @internal
        /// Returns a pointer to the left operand (or nullptr if this layout does not store an operation on two operands)
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        TGUI_NODISCARD Layout* getLeftOperand() const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @internal
        /// Returns a pointer to the right operand (or nullptr if this layout does not store an operation on two operands)
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        TGUI_NODISCARD Layout* getRightOperand() const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    private:


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // If a widget is bound, inform it that the layout no longer binds it
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void unbindLayout();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Resets the parent pointers of the left and right operands if they exist and tell the bound widget that this layout
        // requires information about changes to its position or size when the operation requires a widget to be bound.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void resetPointers();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Check whether sublayouts contain a string that refers to a widget which should be bound.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void parseBindingStringRecursive(Widget* widget, bool xAxis);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Find the widget corresponding to the given name and bind it if found
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void parseBindingString(const String& expression, Widget* widget, bool xAxis);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    private:

        float m_value = 0;
        Layout* m_parent = nullptr;
        Operation m_operation = Operation::Value;
        std::unique_ptr<Layout> m_leftOperand = nullptr; // The left operand of the operation in case the operation is a math operation
        std::unique_ptr<Layout> m_rightOperand = nullptr; // The left operand of the operation in case the operation is a math operation
        Widget* m_boundWidget = nullptr; // The widget on which this layout depends in case the operation is a binding
        String m_boundString; // String referring to a widget on which this layout depends in case the layout was created from a string and contains a binding operation
        std::function<void()> m_connectedWidgetCallback = nullptr; // Function to call when the value of the layout changes in case the layout and sublayouts are not all constants

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    };


    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Class to store the position or size of a widget
    ///
    /// You don't have to explicitly create an instance of this class, sf::Vector2f and tgui::Vector2f are implicitly converted.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    class TGUI_API Layout2d
    {
    public:

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Default constructor to implicitly construct from a tgui::Vector2f.
        ///
        /// @param constant  Value of the layout
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        Layout2d(Vector2f constant = {0, 0}) :
            x{constant.x},
            y{constant.y}
        {
        }


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Constructor to create the Layout2d from two Layout classes
        ///
        /// @param layoutX  x component
        /// @param layoutY  y component
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        Layout2d(Layout layoutX, Layout layoutY) :
            x{std::move(layoutX)},
            y{std::move(layoutY)}
        {
        }


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Constructs the Layout2d based on a string which will be parsed to determine the value of the layouts
        ///
        /// @param expression  String to parse
        ///
        /// The expression will be passed to both the x and y layouts.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        Layout2d(const char* expression) :
            x{expression},
            y{expression}
        {
        }


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Constructs the Layout2d based on a string which will be parsed to determine the value of the layouts
        ///
        /// @param expression  String to parse
        ///
        /// The expression will be passed to both the x and y layouts.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        Layout2d(const String& expression) :
            x{expression},
            y{expression}
        {
        }


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Returns the cached value of the layout
        ///
        /// @return Value of the layout
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        TGUI_NODISCARD Vector2f getValue() const
        {
            return {x.getValue(), y.getValue()};
        }


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @internal
        /// @brief Converts the layout to a string representation
        ///
        /// @return String representation of layout
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        TGUI_NODISCARD String toString() const
        {
            return U"(" + x.toString() + U", " + y.toString() + U")";
        }

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    public:

        Layout x;
        Layout y;
    };


    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Unary minus operator for the Layout class
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    TGUI_NODISCARD TGUI_API Layout operator-(Layout right);

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief + operator for the Layout class
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    TGUI_NODISCARD TGUI_API Layout operator+(Layout left, Layout right);

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief - operator for the Layout class
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    TGUI_NODISCARD TGUI_API Layout operator-(Layout left, Layout right);

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief * operator for the Layout class
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    TGUI_NODISCARD TGUI_API Layout operator*(Layout left, Layout right);

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief / operator for the Layout class
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    TGUI_NODISCARD TGUI_API Layout operator/(Layout left, Layout right);

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Unary minus operator for the Layout2d class
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    TGUI_NODISCARD TGUI_API Layout2d operator-(Layout2d right);

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief + operator for the Layout2d class
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    TGUI_NODISCARD TGUI_API Layout2d operator+(Layout2d left, Layout2d right);

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief - operator for the Layout2d class
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    TGUI_NODISCARD TGUI_API Layout2d operator-(Layout2d left, Layout2d right);

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief * operator for the Layout2d class
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    TGUI_NODISCARD TGUI_API Layout2d operator*(Layout2d left, const Layout& right);

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief * operator for the Layout2d class
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    TGUI_NODISCARD TGUI_API Layout2d operator*(const Layout& left, Layout2d right);

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief / operator for the Layout2d class
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    TGUI_NODISCARD TGUI_API Layout2d operator/(Layout2d left, const Layout& right);


    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    inline namespace bind_functions
    {
        /// @brief Bind to the x position of the widget (same as bindLeft unless widget origin is changed)
        TGUI_NODISCARD TGUI_API Layout bindPosX(const std::shared_ptr<Widget>& widget);

        /// @brief Bind to the y position of the widget (same as bindTop unless widget origin is changed)
        TGUI_NODISCARD TGUI_API Layout bindPosY(const std::shared_ptr<Widget>& widget);

        /// @brief Bind to the left position of the widget
        TGUI_NODISCARD TGUI_API Layout bindLeft(const std::shared_ptr<Widget>& widget);

        /// @brief Bind to the top position of the widget
        TGUI_NODISCARD TGUI_API Layout bindTop(const std::shared_ptr<Widget>& widget);

        /// @brief Bind to the width of the widget
        TGUI_NODISCARD TGUI_API Layout bindWidth(const std::shared_ptr<Widget>& widget);

        /// @brief Bind to the height of the widget
        TGUI_NODISCARD TGUI_API Layout bindHeight(const std::shared_ptr<Widget>& widget);

        /// @brief Bind to the inner width of the container widget
        TGUI_NODISCARD TGUI_API Layout bindInnerWidth(const std::shared_ptr<Container>& container);

        /// @brief Bind to the inner height of the container widget
        TGUI_NODISCARD TGUI_API Layout bindInnerHeight(const std::shared_ptr<Container>& container);

        /// @brief Bind to the right position of the widget
        TGUI_NODISCARD TGUI_API Layout bindRight(const std::shared_ptr<Widget>& widget);

        /// @brief Bind to the bottom of the widget
        TGUI_NODISCARD TGUI_API Layout bindBottom(const std::shared_ptr<Widget>& widget);

        /// @brief Bind to the position of the widget
        TGUI_NODISCARD TGUI_API Layout2d bindPosition(const std::shared_ptr<Widget>& widget);

        /// @brief Bind to the size of the widget
        TGUI_NODISCARD TGUI_API Layout2d bindSize(const std::shared_ptr<Widget>& widget);

        /// @brief Bind to the inner size of the container widget
        TGUI_NODISCARD TGUI_API Layout2d bindInnerSize(const std::shared_ptr<Container>& container);

        /// @brief Bind to the width of the gui view
        TGUI_NODISCARD TGUI_API Layout bindWidth(const BackendGui& gui);

        /// @brief Bind to the height of the gui view
        TGUI_NODISCARD TGUI_API Layout bindHeight(const BackendGui& gui);

        /// @brief Bind to the size of the gui view
        TGUI_NODISCARD TGUI_API Layout2d bindSize(const BackendGui& gui);

        /// @brief Bind to the minimum value of two layouts
        TGUI_NODISCARD TGUI_API Layout bindMin(const Layout& value1, const Layout& value2);

        /// @brief Bind to the maximum value of two layouts
        TGUI_NODISCARD TGUI_API Layout bindMax(const Layout& value1, const Layout& value2);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // TGUI_LAYOUT_HPP
