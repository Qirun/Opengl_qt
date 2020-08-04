#include "corefunctionwidget.h"
#include <QDebug>
#include <QTimer>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QDateTime>
#include<QVector3D>
#include<QVector2D>
#include "loadobj.hpp"
// lighting

//static QVector3D lightPos(2, 3, 6);
static QVector3D lightPos(2, -1, 15);

CoreFunctionWidget::CoreFunctionWidget(QWidget *parent) : QOpenGLWidget(parent)
{

    camera   = new Camera(QVector3D(0.0f, 0.0f, 3.0f)) ;

    m_bLeftPressed = false;

    m_pTimer = new QTimer(this);
    connect(m_pTimer, &QTimer::timeout, this, [=]{
        m_nTimeValue += 1;
        update();
    });
    m_pTimer->start(40);//25 fps

}

CoreFunctionWidget::~CoreFunctionWidget()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO_vertex);
    glDeleteBuffers(1, &VBO_uv);
    glDeleteBuffers(1, &normalbuffer);
    delete camera;
    //glDeleteVertexArrays(1, &cubeVAO);
    //glDeleteBuffers(1, &VBO);
}

void CoreFunctionWidget::initializeGL(){

    this->initializeOpenGLFunctions();

    createShader();



    //loadOBJ("C:/Users/lenovo/Desktop/yin/qtGl_core/image/Charmander.obj", vertices, uvs, normals);


    // first, configure the cube's VAO (and VBO)

//    glGenVertexArrays(1, &VAO);
//    glBindVertexArray(VAO);
//
//    glGenBuffers(1, &VBO_vertex);
//
//    glBindBuffer(GL_ARRAY_BUFFER, VBO_vertex);//把新创建的缓冲绑定到GL_ARRAY_BUFFER目标上
//
//    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(QVector3D), &vertices[0], GL_STATIC_DRAW);
//
//
//    glGenBuffers(1, &VBO_uv);
//    glBindBuffer(GL_ARRAY_BUFFER, VBO_uv);
//    glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(QVector2D), &uvs[0], GL_STATIC_DRAW);
//
//    glGenBuffers(1, &normalbuffer);
//    glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
//    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(QVector3D), &normals[0], GL_STATIC_DRAW);
//
//    glBindBuffer(GL_ARRAY_BUFFER, VBO_vertex);
//    glVertexAttribPointer(
//        0,                  // attribute
//        3,                  // size
//        GL_FLOAT,           // type
//        GL_FALSE,           // normalized?
//        0,                  // stride
//        (void*)0            // array buffer offset
//    );
//    glEnableVertexAttribArray(0);
//
//    glBindBuffer(GL_ARRAY_BUFFER, VBO_uv);
//    glVertexAttribPointer(
//          1,                                // attribute
//          2,                                // size
//          GL_FLOAT,                         // type
//          GL_FALSE,                         // normalized?
//          0,                                // stride
//          (void*)0                          // array buffer offset
//      );
//    glEnableVertexAttribArray(1);
//
//    glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
//    glVertexAttribPointer(
//        2,                                // attribute
//        3,                                // size
//        GL_FLOAT,                         // type
//        GL_FALSE,                         // normalized?
//        0,                                // stride
//        (void*)0                          // array buffer offset
//    );
//    glEnableVertexAttribArray(2);
//
//    glBindBuffer(GL_ARRAY_BUFFER, 0);//解绑
//    glBindVertexArray(0);//解绑，后期要使用时，再绑定。


    texture1 = loadTexture(":/image/r.bmp");
    // shader configuration
    // --------------------

    lightingShader.bind();
    //lightingShader.setUniformValue("texture1", 0);
    glUniform1i(lightingShader.uniformLocation("texture1"), 0);
    lightingShader.release();


    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);


}

void CoreFunctionWidget::resizeGL(int w, int h){
    glViewport(0, 0, w, h);
}

void CoreFunctionWidget::paintGL(){

if(loadflag)
{
    // input
    // -----
    camera->processInput(0.5f);//speed
    lightingShader.bind();
    // render
    // ------
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // bind diffuse map
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);

    lightingShader.setUniformValue(lightingShader.uniformLocation("lightColor"),1.0f,1.0f,1.0f);

    QMatrix4x4 projection ;
    projection.perspective(camera->zoom, 1.0f * width() / height(), 0.1f, 100.0f);


    QMatrix4x4 view = camera->getViewMatrix();
    lightingShader.setUniformValue(lightingShader.uniformLocation("V"), view);

    QMatrix4x4 model ;
    lightingShader.setUniformValue(lightingShader.uniformLocation("M"), model);
    QMatrix4x4 MVP = projection * view * model;
    lightingShader.setUniformValue(lightingShader.uniformLocation("MVP"),MVP);



   // glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
   // glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &model[0][0]);
   // glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &view[0][0]);

    // be sure to activate shader when setting uniforms/drawing objects

    lightingShader.setUniformValue("LightPosition_worldspace", lightPos);
    //lightingShader.setUniformValue("Normal_cameraspace", camera->position);

    lightingShader.setUniformValue("projection", projection);




    //light properties
    //lightingShader.setUniformValue("light.ambient", QVector3D(0.2f, 0.2f, 0.2f));
    //lightingShader.setUniformValue("light.diffuse", QVector3D(0.5f, 0.5f, 0.5f));
    //lightingShader.setUniformValue("light.specular", QVector3D(1.0f, 1.0f, 1.0f));

    // material properties
    //lightingShader.setUniformValue("material.shininess", 64.0f);


    // view/projection transformations
    //QMatrix4x4 projection;
    //projection.perspective(camera->zoom, 1.0f * width() / height(), 0.1f, 100.0f);
    //QMatrix4x4 view = camera->getViewMatrix();

    //lightingShader.setUniformValue("projection", projection);
    //lightingShader.setUniformValue("view", view);

    // world transformation
    //QMatrix4x4 model;
    //lightingShader.setUniformValue("model", model);



    // bind specular map
    //glActiveTexture(GL_TEXTURE1);
    //glBindTexture(GL_TEXTURE_2D, specularMap);

    //render the cube
    //glBindVertexArray(cubeVAO);
    //glDrawArrays(GL_TRIANGLES, 0, 36);


    // also draw the lamp object
//    lampShader.bind();
//    lampShader.setUniformValue("projection", projection);
//    lampShader.setUniformValue("view", view);
//    model = QMatrix4x4();
//    model.translate(lightPos);
//    model.scale(0.2f); // a smaller cube
//    lampShader.setUniformValue("model", model);

    //glBindVertexArray(lightVAO);
    //glDrawArrays(GL_TRIANGLES, 0, 36);

    //loadOBJ("C:/Users/lenovo/Desktop/yin/qtGl_core/image/Charmander.obj", vertices, uvs, normals);


    // first, configure the cube's VAO (and VBO)

    if(file_changed)
    {
        glBindVertexArray(0);
        glDeleteBuffers(1,&VBO_vertex);
        glDeleteBuffers(1,&VBO_uv);
        glDeleteBuffers(1,&normalbuffer);

    }
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);


    glGenBuffers(1, &VBO_vertex);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_vertex);//把新创建的缓冲绑定到GL_ARRAY_BUFFER目标上

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(QVector3D), &vertices[0], GL_STATIC_DRAW);


    glGenBuffers(1, &VBO_uv);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_uv);
    glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(QVector2D), &uvs[0], GL_STATIC_DRAW);

    glGenBuffers(1, &normalbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(QVector3D), &normals[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_vertex);
    glVertexAttribPointer(
        0,                  // attribute
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
    );
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_uv);
    glVertexAttribPointer(
          1,                                // attribute
          2,                                // size
          GL_FLOAT,                         // type
          GL_FALSE,                         // normalized?
          0,                                // stride
          (void*)0                          // array buffer offset
      );
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
    glVertexAttribPointer(
        2,                                // attribute
        3,                                // size
        GL_FLOAT,                         // type
        GL_FALSE,                         // normalized?
        0,                                // stride
        (void*)0                          // array buffer offset
    );
    glEnableVertexAttribArray(2);

    if(mode==0)//https://模式选择www.khronos.org/registry/OpenGL-Refpages/es3/html/glDrawArrays.xhtml
    {
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());//实体显示
    }
    else if(mode==1)
    {
    glDrawArrays(GL_POINTS,0, vertices.size());//点线模式
    }
    else if(mode==2)//点云显示
    {
    glDrawArrays(GL_LINE_LOOP, 0, vertices.size());
    }
    lightingShader.release();
//}
}
}


void CoreFunctionWidget::keyPressEvent(QKeyEvent *event)
{
    int key = event->key();
    if (key >= 0 && key < 1024)

        camera->keys[key] = true;
}
void CoreFunctionWidget::keyReleaseEvent(QKeyEvent *event)
{
    int key = event->key();

    if (key >= 0 && key < 1024)
        camera->keys[key] = false;
}



void CoreFunctionWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){

        m_bLeftPressed = true;
        m_lastPos = event->pos();
    }
}

void CoreFunctionWidget::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);

    m_bLeftPressed = false;
}

void CoreFunctionWidget::mouseMoveEvent(QMouseEvent *event)
{
    int xpos = event->pos().x();
    int ypos = event->pos().y();

    int xoffset = xpos - m_lastPos.x();
    int yoffset = m_lastPos.y() - ypos;
    m_lastPos = event->pos();

    camera->processMouseMovement(xoffset, yoffset);
}

void CoreFunctionWidget::wheelEvent(QWheelEvent *event)
{
    QPoint offset = event->angleDelta();
    camera->processMouseScroll(offset.y()/20.0f);
}

bool CoreFunctionWidget::createShader()
{
    bool success = lightingShader.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader/6.1.coordinate_systems.vs");
    if (!success) {
        qDebug() << "shaderProgram addShaderFromSourceFile failed!" << lightingShader.log();
        return success;
    }

    success = lightingShader.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader/6.1.coordinate_systems.fs");
    if (!success) {
        qDebug() << "shaderProgram addShaderFromSourceFile failed!" << lightingShader.log();
        return success;
    }

    success = lightingShader.link();
    if(!success) {
        qDebug() << "shaderProgram link failed!" << lightingShader.log();
    }



    return success;
}

uint CoreFunctionWidget::loadTexture(const QString& path)
{
    uint textureID;
    glGenTextures(1, &textureID);

    QImage image = QImage(path).convertToFormat(QImage::Format_RGBA8888).mirrored(true, true);
    if (!image.isNull()) {
        qDebug()<< "load bmp...";
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width(), image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.bits());
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else
    {
        qDebug()<< "Failed to load texture" ;
    }

    return textureID;
}
