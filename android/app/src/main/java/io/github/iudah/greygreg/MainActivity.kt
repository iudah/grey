package io.github.iudah.greygreg

import android.app.NativeActivity

class MainActivity : NativeActivity() {
  companion object {
    private val tag = "GreyGreg"

    // init {
    //   try {
    //     Log.i(tag, "Loading library")
    //     System.loadLibrary("greg")
    //     Log.i(tag, "Loaded library")
    //   } catch (e: UnsatisfiedLinkError) {
    //     var toast_text = "Native library failed to load: ${e.message ?: "Unknown error"}"
    //     Log.e(tag, toast_text)
    //   }
    // }
  }
}
