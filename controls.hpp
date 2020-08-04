#ifndef CONTROLS_HPP
#define CONTROLS_HPP
#include<QMatrix4x4>
#include <QKeyEvent>
#include <QVector3D>
#include<QTime>
void computeMatricesFromInputs();
QMatrix4x4 getViewMatrix();
QMatrix4x4 getProjectionMatrix();

#endif
