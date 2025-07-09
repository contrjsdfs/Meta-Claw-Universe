package com.example.myapplication

import android.content.Context
import android.net.Uri
import android.view.ViewGroup
import androidx.compose.foundation.background
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.runtime.Composable
import androidx.compose.runtime.DisposableEffect
import androidx.compose.runtime.remember
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.platform.LocalContext
import androidx.compose.ui.viewinterop.AndroidView
import org.videolan.libvlc.LibVLC
import org.videolan.libvlc.Media
import org.videolan.libvlc.MediaPlayer
import org.videolan.libvlc.util.VLCVideoLayout

@Composable
fun VideoStreamScreen(streamUrl: String) {
    val context = LocalContext.current

    // 1. 初始化VLC相关组件（布局、播放器、LibVLC）
    val vlcVideoLayout = remember {
        VLCVideoLayout(context).apply {
            // 关键设置：将视频布局背景设为白色
            setBackgroundColor(android.graphics.Color.WHITE)
        }
    }
    val libVlc = remember { LibVLC(context, arrayListOf("--no-drop-late-frames")) }
    val mediaPlayer = remember { MediaPlayer(libVlc) }

    // 2. 播放控制逻辑
    DisposableEffect(Unit) {
        val media = Media(libVlc, Uri.parse(streamUrl)).apply {
            mediaPlayer.media = this
            release() // 及时释放Media资源
        }

        mediaPlayer.play() // 开始播放

        onDispose { // 组件销毁时释放资源
            mediaPlayer.stop()
            mediaPlayer.release()
            libVlc.release()
        }
    }

    // 3. 使用Box组合布局确保全白背景

    Box(
        modifier = Modifier
            .fillMaxSize()
            .background(Color.White) // 整个屏幕设为白色背景
    ) {
        AndroidView(
            factory = {
                vlcVideoLayout.apply {
                    layoutParams = ViewGroup.LayoutParams(
                        ViewGroup.LayoutParams.MATCH_PARENT,
                        ViewGroup.LayoutParams.MATCH_PARENT
                    )
                    mediaPlayer.attachViews(this, null, false, false)
                }
            },
            modifier = Modifier.fillMaxSize()
        )
    }
}