package io.github.youi1987.glestest

import android.app.Activity
import android.os.Bundle

class CubeActivity : Activity() {
    private lateinit var cubeView: CubeView

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        cubeView = CubeView(this)
        setContentView(cubeView)
    }

    override fun onPause() {
        super.onPause()
        cubeView.onPause()
    }

    override fun onResume() {
        super.onResume()
        cubeView.onResume()
    }
}