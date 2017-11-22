package io.github.youi1987.glestest

import android.content.Context
import android.opengl.GLSurfaceView
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

class TriangleView(context: Context) : GLSurfaceView(context) {
    init {
        setEGLConfigChooser(8, 8, 8, 0, 16, 0)
        setEGLContextClientVersion(3)
        setRenderer(TriangleRender())
    }

    inner class TriangleRender : GLSurfaceView.Renderer {
        override fun onDrawFrame(gl: GL10?) {
            step()
        }

        override fun onSurfaceChanged(gl: GL10?, width: Int, height: Int) {
            resize(width, height)
        }

        override fun onSurfaceCreated(gl: GL10?, config: EGLConfig?) {
            init()
        }
    }

    external fun init(): Boolean
    external fun resize(width: Int, height: Int)
    external fun step()

    companion object {

        init {
            System.loadLibrary("triangle-lib")
        }
    }
}