package io.github.youi1987.glestest

import android.app.Activity
import android.os.Bundle

class RectActivity : Activity() {
    private lateinit var rectView: RectView

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        rectView = RectView(this)
        setContentView(rectView)
    }

    override fun onPause() {
        super.onPause()
        rectView.onPause()
    }

    override fun onResume() {
        super.onResume()
        rectView.onResume()
    }
}