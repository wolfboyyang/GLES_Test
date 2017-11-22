package io.github.youi1987.glestest

import android.app.Activity
import android.os.Bundle

class TriangleActivity : Activity() {
    lateinit var triangleView: TriangleView

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        triangleView = TriangleView(this)
        setContentView(triangleView)
    }

    override fun onPause() {
        super.onPause()
        triangleView.onPause()
    }

    override fun onResume() {
        super.onResume()
        triangleView.onResume()
    }
}