#pragma once

class Widget;

class WidgetSlot
{
    std::shared_ptr<Widget> childWidget;
public:
    static WidgetSlot New(std::shared_ptr<Widget>&& child);
    static WidgetSlot New(std::shared_ptr<Widget>& child);

};

class Widget
{
    
};