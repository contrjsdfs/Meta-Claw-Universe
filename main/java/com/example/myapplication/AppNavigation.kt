package com.example.myapplication

import androidx.compose.foundation.layout.WindowInsets
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.layout.systemBarsPadding
import androidx.compose.material3.Icon
import androidx.compose.material3.NavigationBar
import androidx.compose.material3.NavigationBarItem
import androidx.compose.material3.Scaffold
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.runtime.getValue
import androidx.compose.ui.res.painterResource
import androidx.compose.ui.res.stringResource
import androidx.navigation.NavController
import androidx.navigation.NavGraph.Companion.findStartDestination
import androidx.navigation.compose.NavHost
import androidx.navigation.compose.composable
import androidx.navigation.compose.currentBackStackEntryAsState
import androidx.navigation.compose.rememberNavController
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.unit.dp

// 在 com.example.myapplication 包下的 Screen.kt 或相应文件中
sealed class Screen(val route: String, val iconResId: Int, val titleResId: Int) {
    object Sensor : Screen("sensor", R.drawable.ic_sensor, R.string.sensor_title)
    object Video : Screen("video", R.drawable.ic_video, R.string.video_title)
    object Chat : Screen("chat", R.drawable.ic_chat, R.string.chat_title) // 新增
}

@Composable
fun AppNavigation() {
    val navController = rememberNavController()

    Scaffold(
        containerColor = Color.Transparent,
        bottomBar = {
            BottomNavigationBar(navController)
        },
        contentWindowInsets = WindowInsets(0, 0, 0, 0),
        modifier = Modifier.systemBarsPadding() // 添加系统栏padding
    ) { innerPadding ->
        NavHost(
            navController = navController,
            startDestination = Screen.Sensor.route,
            modifier = Modifier.padding(innerPadding)
        ) {
            composable(Screen.Sensor.route) {
                SensorMonitorApp()
            }
            composable(Screen.Video.route) {
                VideoStreamScreen(streamUrl = "rtsp://172.20.10.5:8554/chnout")
            }
            composable(Screen.Chat.route) {
                DeepSeekChatScreen()
            }
        }
    }
}

@Composable
private fun BottomNavigationBar(navController: NavController) {
    val items = listOf(Screen.Sensor, Screen.Video, Screen.Chat)
    val navBackStackEntry by navController.currentBackStackEntryAsState()
    val currentRoute = navBackStackEntry?.destination?.route

    NavigationBar(
        modifier = Modifier.padding(0.dp)
    ) {
        items.forEach { screen ->
            NavigationBarItem(
                icon = { Icon(painterResource(screen.iconResId), contentDescription = null) },
                label = { Text(stringResource(screen.titleResId)) },
                selected = currentRoute == screen.route,
                onClick = {
                    navController.navigate(screen.route) {
                        popUpTo(navController.graph.findStartDestination().id) {
                            saveState = true
                        }
                        launchSingleTop = true
                        restoreState = true
                    }
                },
                modifier = Modifier.padding(0.dp) // 确保项目也没有内边距
            )
        }
    }
}