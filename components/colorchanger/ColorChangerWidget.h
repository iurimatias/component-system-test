#ifndef NEDRYSOFT_COLORCHANGER_COLORCHANGERWIDGET_H
#define NEDRYSOFT_COLORCHANGER_COLORCHANGERWIDGET_H

#include <QWidget>
#include <QColor>

class QPushButton;

class ColorChangerWidget : public QWidget {
    Q_OBJECT

public:
    explicit ColorChangerWidget(QWidget* parent = nullptr);

private slots:
    void changeColor();

private:
    QPushButton* changeColorButton;
    QList<QColor> colors;
    int currentColorIndex;
};

#endif // NEDRYSOFT_COLORCHANGER_COLORCHANGERWIDGET_H 