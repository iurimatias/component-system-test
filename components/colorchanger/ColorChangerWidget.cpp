#include "ColorChangerWidget.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QPalette>

ColorChangerWidget::ColorChangerWidget(QWidget* parent) : 
    QWidget(parent),
    changeColorButton(nullptr),
    currentColorIndex(0) {
    
    // Initialize colors
    colors = {Qt::red, Qt::green, Qt::blue, Qt::yellow};

    // Create UI elements
    changeColorButton = new QPushButton("Change Color", this);

    // Set up layout
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(changeColorButton);

    // Enable background filling
    setAutoFillBackground(true);

    // Connect button click to changeColor slot
    connect(changeColorButton, &QPushButton::clicked, this, &ColorChangerWidget::changeColor);

    // Set initial color
    changeColor();
}

void ColorChangerWidget::changeColor() {
    // Cycle through colors
    currentColorIndex = (currentColorIndex + 1) % colors.size();
    QPalette pal = palette();
    pal.setColor(QPalette::Window, colors[currentColorIndex]);
    setPalette(pal);
} 