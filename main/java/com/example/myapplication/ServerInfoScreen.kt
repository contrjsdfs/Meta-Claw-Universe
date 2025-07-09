package com.example.myapplication

import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Spacer
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.height
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.rememberScrollState
import androidx.compose.foundation.verticalScroll
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.ui.Modifier
import androidx.compose.ui.unit.dp

@Composable
fun ServerInfoScreen(
    ipAddress: String,
    status: String,
    isConnected: Boolean,
    clientCount: Int,
    clientIp: String,
    logMessages: List<String>,
    message: String,
    onMessageChange: (String) -> Unit,
    onSendMessage: (String) -> Unit
) {
    Column(
        modifier = Modifier
            .fillMaxSize()
            .verticalScroll(rememberScrollState())
            .padding(16.dp)
    ) {
        // 服务器信息显示
        ServerInfoDisplay(
            ipAddress = ipAddress,
            status = status,
            isConnected = isConnected,
            clientCount = clientCount,
            clientIp = clientIp
        )

        Spacer(modifier = Modifier.height(16.dp))

        // 通信日志
        CommunicationLogDisplay(logMessages = logMessages)

        Spacer(modifier = Modifier.height(16.dp))

        // 消息输入和发送
        MessageInputSection(
            message = message,
            onMessageChange = onMessageChange,
            onSendMessage = onSendMessage,
            isConnected = isConnected
        )
    }
}