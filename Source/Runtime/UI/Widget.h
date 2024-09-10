#pragma once

#include <vector>

class Widget;

class WidgetSlot
{
    std::shared_ptr<Widget> childWidget;
public:

    std::vector<WidgetSlot> operator+(WidgetSlot&& nSlot) {
        return {*this, nSlot};
    }

    std::vector<WidgetSlot> operator+(std::vector<WidgetSlot>&& nSlots) {
        std::vector<WidgetSlot> slots(nSlots.size() + 1);
        slots.push_back(*this);
        slots.insert(slots.end(), nSlots.begin(), nSlots.end());
        return slots;
    }
};

class Widget
{
protected:
    std::vector<WidgetSlot> slots;
public:

    Widget* operator[](WidgetSlot& nSlot) {
        slots.push_back(nSlot);
        return this;
    }

    Widget* operator[](WidgetSlot&& nSlot) {
        slots.push_back(nSlot);
        return this;
    }

    Widget* operator[](std::vector<WidgetSlot>& nSlots) {
        slots = nSlots;
        return this;
    }

    Widget* operator[](std::vector<WidgetSlot>&& nSlots) {
        slots = nSlots;
        return this;
    }
};