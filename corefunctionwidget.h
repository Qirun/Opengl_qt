#ifndef COREFUNCTIONWIDGET_H
#define COREFUNCTIONWIDGET_H

#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShader>
#include <QOpenGLTexture>
#include <QKeyEvent>
#include <QOpenGLShaderProgram>
#include <QTime>
#include "Camera.h"

QT_BEGIN_NAMESPACE
namespace Ui { class CoreFunctionWidget; }
QT_END_NAMESPACE

class CoreFunctionWidget : public QOpenGLWidget
                           , protected /*QOpenGLExtraFunctions*/QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    explicit CoreFunctionWidget(QWidget *parent = nullptr);
    ~CoreFunctionWidget() Q_DECL_OVERRIDE;

    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;


    void keyPressEvent(QKeyEvent *event)Q_DECL_OVERRIDE;
    void keyReleaseEvent(QKeyEvent *event) Q_DECL_OVERRIDE;

    Camera *camera;

    QVector<QVector3D> vertices;
    QVector<QVector2D> uvs;
    QVector<QVector3D> normals;
    bool loadflag = false;
    char mode = 0;
    bool file_changed = false;

    float box_x;
    float box_y;
protected:
    void initializeGL()  Q_DECL_OVERRIDE;
    void resizeGL(int w, int h) Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;




private:
    bool createShader();
    uint loadTexture(const QString& path);

private:
    QOpenGLShaderProgram lightingShader;
    //lampShader;

    QTimer* m_pTimer = nullptr;
    int     m_nTimeValue = 0;



    //uint VBO, cubeVAO, lightVAO;
    uint VAO;
    uint VBO_vertex;
    uint VBO_uv;
    uint normalbuffer;

    //uint diffuseMap;
    //uint specularMap;

    uint texture1;

    // camera

    bool m_bLeftPressed;
    QPoint m_lastPos;
    Ui::CoreFunctionWidget *ui;

    //QOpenGLShaderProgram shaderProgram;

};

#endif // COREFUNCTIONWIDGET_H
