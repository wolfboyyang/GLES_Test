//
// Created by youi1 on 2017/11/24.
//

#define LOG_TAG "cube-lib"

#include "esUtil.h"
#include <cmath>
#include <ctime>
#include <string.h>

#define STR(s) #s
#define STRV(s) STR(s)

#define PI 3.14

#define POSITION_IND    0
#define COLOR_IND       1
#define MVP_IND         2
#define VERTEX_NUM      108
#define INDICES_NUM     36
#define MATRIX_COLUMN   4
#define MATRIX_ROW      4

GLuint program;
GLuint positionVBO;
GLuint colorVBO;
GLuint mvpVBO;
GLuint indicesIBO;
GLfloat aspect;
uint64_t lastFrameNs;
GLfloat angle;

const char VERTEX_SHADER[] =
        "#version 300 es\n"
                "layout(location = " STRV(POSITION_IND) ") in vec4 a_position;\n"
                "layout(location = " STRV(COLOR_IND) ") in vec4 a_color;\n"
                "layout(location = " STRV(MVP_IND) ") in mat4 a_mvpMatrix;\n"
                "out vec4 v_color;\n"
                "void main()\n"
                "{\n"
                "   v_color = a_color;\n"
                "   gl_Position = a_mvpMatrix * a_position;\n"
                "}\n";

const char FRAGMENT_SHADER[] =
        "#version 300 es\n"
                "precision mediump float;\n"
                "in vec4 v_color;\n"
                "layout(location = 0) out vec4 outColor;\n"
                "void main()\n"
                "{\n"
                "  outColor = v_color;\n"
                "}\n";

GLfloat VERTEX_POS[VERTEX_NUM] =
        {
                -0.5f, -0.5f, -0.5f,//0
                0.5f, -0.5f, 0.5f,//2
                -0.5f, -0.5f, 0.5f,//1
                -0.5f, -0.5f, -0.5f,//0
                0.5f, -0.5f, -0.5f,//3
                0.5f, -0.5f, 0.5f,//2
                -0.5f, 0.5f, -0.5f,//4
                -0.5f, 0.5f, 0.5f,//5
                0.5f, 0.5f, 0.5f,//6
                -0.5f, 0.5f, -0.5f,//4
                0.5f, 0.5f, 0.5f,//6
                0.5f, 0.5f, -0.5f,//7
                -0.5f, -0.5f, -0.5f,//0
                -0.5f, 0.5f, -0.5f,//4
                0.5f, 0.5f, -0.5f,//7
                -0.5f, -0.5f, -0.5f,//0
                0.5f, 0.5f, -0.5f,//7
                0.5f, -0.5f, -0.5f,//3
                -0.5f, -0.5f, 0.5f,//1
                0.5f, -0.5f, 0.5f,//2
                0.5f, 0.5f, 0.5f,//6
                -0.5f, -0.5f, 0.5f,//1
                0.5f, 0.5f, 0.5f,//6
                -0.5f, 0.5f, 0.5f,//5
                -0.5f, -0.5f, -0.5f,//0
                -0.5f, -0.5f, 0.5f,//1
                -0.5f, 0.5f, 0.5f,//5
                -0.5f, -0.5f, -0.5f,//0
                -0.5f, 0.5f, 0.5f,//5
                -0.5f, 0.5f, -0.5f,//4
                0.5f, -0.5f, -0.5f,//3
                0.5f, 0.5f, -0.5f,//7
                0.5f, 0.5f, 0.5f,//6
                0.5f, -0.5f, -0.5f,//3
                0.5f, 0.5f, 0.5f,//6
                0.5f, -0.5f, 0.5f//2
        };

const GLfloat cubeColor[] =
        {
                1.0f, 0.0f, 0.0f, 0.0f,
                1.0f, 0.0f, 0.0f, 0.0f,
                1.0f, 0.0f, 0.0f, 0.0f,
                1.0f, 0.0f, 0.0f, 0.0f,
                1.0f, 0.0f, 0.0f, 0.0f,
                1.0f, 0.0f, 0.0f, 0.0f,

                0.0f, 0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 1.0f, 0.0f,

                0.0f, 1.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f, 0.0f,

                1.0f, 1.0f, 0.0f, 0.0f,
                1.0f, 1.0f, 0.0f, 0.0f,
                1.0f, 1.0f, 0.0f, 0.0f,
                1.0f, 1.0f, 0.0f, 0.0f,
                1.0f, 1.0f, 0.0f, 0.0f,
                1.0f, 1.0f, 0.0f, 0.0f,

                0.0f, 1.0f, 1.0f, 0.0f,
                0.0f, 1.0f, 1.0f, 0.0f,
                0.0f, 1.0f, 1.0f, 0.0f,
                0.0f, 1.0f, 1.0f, 0.0f,
                0.0f, 1.0f, 1.0f, 0.0f,
                0.0f, 1.0f, 1.0f, 0.0f,

                1.0f, 0.0f, 1.0f, 0.0f,
                1.0f, 0.0f, 1.0f, 0.0f,
                1.0f, 0.0f, 1.0f, 0.0f,
                1.0f, 0.0f, 1.0f, 0.0f,
                1.0f, 0.0f, 1.0f, 0.0f,
                1.0f, 0.0f, 1.0f, 0.0f
        };


typedef struct {
    GLfloat m[MATRIX_ROW][MATRIX_COLUMN];
} Matrix;

bool
init() {
    program = createProgram(VERTEX_SHADER, FRAGMENT_SHADER);
    if (!program)
        return false;

    glGenBuffers(1, &positionVBO);
    glBindBuffer(GL_ARRAY_BUFFER, positionVBO);
    glBufferData(GL_ARRAY_BUFFER, VERTEX_NUM * sizeof(GLfloat), VERTEX_POS, GL_STATIC_DRAW);

    glGenBuffers(1, &colorVBO);
    glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
    glBufferData(GL_ARRAY_BUFFER, INDICES_NUM * 4 * sizeof(GLfloat), cubeColor, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
    glVertexAttribPointer(COLOR_IND, 4, GL_FLOAT,
                          GL_FALSE, 0, (const void *) NULL);
    glEnableVertexAttribArray(COLOR_IND);

    angle = float(drand48() * 360.0f);
    glGenBuffers(1, &mvpVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mvpVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Matrix), NULL, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

    glBindBuffer(GL_ARRAY_BUFFER, positionVBO);
    glVertexAttribPointer(POSITION_IND, 3, GL_FLOAT,
                          GL_FALSE, 3 * sizeof(GLfloat), (const void *) NULL);
    glEnableVertexAttribArray(POSITION_IND);

    glBindBuffer(GL_ARRAY_BUFFER, mvpVBO);

    glVertexAttribPointer(MVP_IND + 0, 4, GL_FLOAT, GL_FALSE, sizeof(Matrix),
                          (const void *) NULL);
    glVertexAttribPointer(MVP_IND + 1, 4, GL_FLOAT, GL_FALSE, sizeof(Matrix),
                          (const void *) (sizeof(GLfloat) * 4));
    glVertexAttribPointer(MVP_IND + 2, 4, GL_FLOAT, GL_FALSE, sizeof(Matrix),
                          (const void *) (sizeof(GLfloat) * 8));
    glVertexAttribPointer(MVP_IND + 3, 4, GL_FLOAT, GL_FALSE, sizeof(Matrix),
                          (const void *) (sizeof(GLfloat) * 12));
    glEnableVertexAttribArray(MVP_IND + 0);
    glEnableVertexAttribArray(MVP_IND + 1);
    glEnableVertexAttribArray(MVP_IND + 2);
    glEnableVertexAttribArray(MVP_IND + 3);

    glVertexAttribDivisor(MVP_IND + 0, 1);
    glVertexAttribDivisor(MVP_IND + 1, 1);
    glVertexAttribDivisor(MVP_IND + 2, 1);
    glVertexAttribDivisor(MVP_IND + 3, 1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesIBO);

    glUseProgram(program);

    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);

    return true;
}

void
matrixLoadIdentity(Matrix *result) {
    memset(result, 0, sizeof(Matrix));
    result->m[0][0] = 1.0f;
    result->m[1][1] = 1.0f;
    result->m[2][2] = 1.0f;
    result->m[3][3] = 1.0f;
}

void
matrixMultiply(Matrix *result, Matrix *srcA, Matrix *srcB) {
    Matrix tmp;
    int i;

    for (i = 0; i < 4; i++) {
        tmp.m[i][0] = (srcA->m[i][0] * srcB->m[0][0]) +
                      (srcA->m[i][1] * srcB->m[1][0]) +
                      (srcA->m[i][2] * srcB->m[2][0]) +
                      (srcA->m[i][3] * srcB->m[3][0]);

        tmp.m[i][1] = (srcA->m[i][0] * srcB->m[0][1]) +
                      (srcA->m[i][1] * srcB->m[1][1]) +
                      (srcA->m[i][2] * srcB->m[2][1]) +
                      (srcA->m[i][3] * srcB->m[3][1]);

        tmp.m[i][2] = (srcA->m[i][0] * srcB->m[0][2]) +
                      (srcA->m[i][1] * srcB->m[1][2]) +
                      (srcA->m[i][2] * srcB->m[2][2]) +
                      (srcA->m[i][3] * srcB->m[3][2]);

        tmp.m[i][3] = (srcA->m[i][0] * srcB->m[0][3]) +
                      (srcA->m[i][1] * srcB->m[1][3]) +
                      (srcA->m[i][2] * srcB->m[2][3]) +
                      (srcA->m[i][3] * srcB->m[3][3]);
    }

    memcpy(result, &tmp, sizeof(Matrix));
}

void
frustum(Matrix *result, float w, float h, float nearZ,
        float farZ) {
    float left = -w;
    float right = w;
    float bottom = -h;
    float top = h;
    float deltaX = right - left;
    float deltaY = top - bottom;
    float deltaZ = farZ - nearZ;
    Matrix frust;

    if ((nearZ <= 0.0f) || (farZ <= 0.0f) ||
        (deltaX <= 0.0f) || (deltaY <= 0.0f) || (deltaZ <= 0.0f)) {
        return;
    }

    frust.m[0][0] = 2.0f * nearZ / deltaX;
    frust.m[0][1] = frust.m[0][2] = frust.m[0][3] = 0.0f;

    frust.m[1][1] = 2.0f * nearZ / deltaY;
    frust.m[1][0] = frust.m[1][2] = frust.m[1][3] = 0.0f;

    frust.m[2][0] = (right + left) / deltaX;
    frust.m[2][1] = (top + bottom) / deltaY;
    frust.m[2][2] = -(nearZ + farZ) / deltaZ;
    frust.m[2][3] = -1.0f;

    frust.m[3][2] = -2.0f * nearZ * farZ / deltaZ;
    frust.m[3][0] = frust.m[3][1] = frust.m[3][3] = 0.0f;

    matrixMultiply(result, &frust, result);
}

void
translate(Matrix *result, GLfloat tx, GLfloat ty, GLfloat tz) {
    result->m[3][0] += (result->m[0][0] * tx + result->m[1][0] * ty + result->m[2][0] * tz);
    result->m[3][1] += (result->m[0][1] * tx + result->m[1][1] * ty + result->m[2][1] * tz);
    result->m[3][2] += (result->m[0][2] * tx + result->m[1][2] * ty + result->m[2][2] * tz);
    result->m[3][3] += (result->m[0][3] * tx + result->m[1][3] * ty + result->m[2][3] * tz);
}

void
rotate(Matrix *result, GLfloat angle, GLfloat x, GLfloat y, GLfloat z) {
    GLfloat sinAngle, cosAngle;
    GLfloat mag = sqrtf(x * x + y * y + z * z);

    sinAngle = sinf(float(angle * PI / 180.0f));
    cosAngle = cosf(float(angle * PI / 180.0f));

    if (mag > 0.0f) {
        GLfloat xx, yy, zz, xy, yz, zx, xs, ys, zs;
        GLfloat oneMinusCos;
        Matrix rotMat;

        x /= mag;
        y /= mag;
        z /= mag;

        xx = x * x;
        yy = y * y;
        zz = z * z;
        xy = x * y;
        yz = y * z;
        zx = z * x;
        xs = x * sinAngle;
        ys = y * sinAngle;
        zs = z * sinAngle;
        oneMinusCos = 1.0f - cosAngle;

        rotMat.m[0][0] = (oneMinusCos * xx) + cosAngle;
        rotMat.m[0][1] = (oneMinusCos * xy) - zs;
        rotMat.m[0][2] = (oneMinusCos * zx) + ys;
        rotMat.m[0][3] = 0.0F;

        rotMat.m[1][0] = (oneMinusCos * xy) + zs;
        rotMat.m[1][1] = (oneMinusCos * yy) + cosAngle;
        rotMat.m[1][2] = (oneMinusCos * yz) - xs;
        rotMat.m[1][3] = 0.0F;

        rotMat.m[2][0] = (oneMinusCos * zx) - ys;
        rotMat.m[2][1] = (oneMinusCos * yz) + xs;
        rotMat.m[2][2] = (oneMinusCos * zz) + cosAngle;
        rotMat.m[2][3] = 0.0F;

        rotMat.m[3][0] = 0.0F;
        rotMat.m[3][1] = 0.0F;
        rotMat.m[3][2] = 0.0F;
        rotMat.m[3][3] = 1.0F;

        matrixMultiply(result, &rotMat, result);
    }
}

void
update(float deltaTime) {
    Matrix *matrixBuf;
    Matrix perspective;

    matrixLoadIdentity(&perspective);
    GLfloat frustumW, frustumH;

    frustumH = tanf(float(60.0f / 360.0f * PI)) * 1.5f;
    frustumW = frustumH * aspect;

    frustum(&perspective, frustumW, frustumH, 1.0f, 20.0f);

    glBindBuffer(GL_ARRAY_BUFFER, mvpVBO);
    matrixBuf = (Matrix *) glMapBufferRange(GL_ARRAY_BUFFER, 0, sizeof(Matrix),
                                            GL_MAP_WRITE_BIT);

    Matrix modelview;

    matrixLoadIdentity(&modelview);

    translate(&modelview, 0, 0, -2.0f);

    angle += (deltaTime * 40.0f);
    if (angle >= 360.0f) {
        angle -= 360.0f;
    }

    rotate(&modelview, angle, 1.0, 0.0, 1.0);

    matrixMultiply(&matrixBuf[0], &modelview, &perspective);

    glUnmapBuffer(GL_ARRAY_BUFFER);
}

extern "C" {
JNIEXPORT jboolean JNICALL Java_io_github_youi1987_glestest_CubeView_init(JNIEnv *env, jclass type);
JNIEXPORT void JNICALL Java_io_github_youi1987_glestest_CubeView_resize(JNIEnv *env, jclass type,
                                                                        jint width, jint height);
JNIEXPORT void JNICALL Java_io_github_youi1987_glestest_CubeView_render(JNIEnv *env, jclass type);
JNIEXPORT void JNICALL Java_io_github_youi1987_glestest_CubeView_destroy(JNIEnv *env, jclass type);
}

jboolean
Java_io_github_youi1987_glestest_CubeView_init(JNIEnv *env, jclass type) {
    if (init())return JNI_TRUE;
    return JNI_FALSE;
}


void
Java_io_github_youi1987_glestest_CubeView_resize(JNIEnv *env, jclass type,
                                                 jint width, jint height) {
    aspect = (GLfloat) width / (GLfloat) height;
    glViewport(0, 0, width, height);
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void
Java_io_github_youi1987_glestest_CubeView_render(JNIEnv *env, jclass type) {
    timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    auto nowNs = now.tv_sec * 1000000000ull + now.tv_nsec;
    if (lastFrameNs > 0) {
        float dt = float(nowNs - lastFrameNs) * 0.000000001f;
        update(dt);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    lastFrameNs = nowNs;
}

void
Java_io_github_youi1987_glestest_CubeView_destroy(JNIEnv *env, jclass type) {
    glDeleteBuffers(1, &positionVBO);
    glDeleteBuffers(1, &colorVBO);
    glDeleteBuffers(1, &mvpVBO);
    glDeleteBuffers(1, &indicesIBO);
    glDeleteProgram(program);
}

#undef LOG_TAG