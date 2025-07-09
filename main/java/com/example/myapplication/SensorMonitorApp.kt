package com.example.myapplication

import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.Spacer
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.height
import androidx.compose.foundation.layout.heightIn
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.layout.size
import androidx.compose.foundation.layout.width
import androidx.compose.foundation.rememberScrollState
import androidx.compose.foundation.shape.CornerSize
import androidx.compose.foundation.text.KeyboardOptions
import androidx.compose.foundation.verticalScroll
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.filled.Cloud
import androidx.compose.material.icons.filled.Lock
import androidx.compose.material.icons.filled.LockOpen
import androidx.compose.material.icons.filled.Star
import androidx.compose.material3.Button
import androidx.compose.material3.Card
import androidx.compose.material3.CardDefaults
import androidx.compose.material3.CenterAlignedTopAppBar
import androidx.compose.material3.ExperimentalMaterial3Api
import androidx.compose.material3.Icon
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.OutlinedTextField
import androidx.compose.material3.Scaffold
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.runtime.DisposableEffect
import androidx.compose.runtime.LaunchedEffect
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableIntStateOf
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.setValue
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.text.font.FontWeight
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.unit.dp
import androidx.compose.ui.unit.sp
import com.example.myapplication.ui.theme.MyApplicationTheme
import org.json.JSONObject
import java.io.BufferedReader
import java.io.InputStreamReader
import java.io.OutputStream
import java.net.NetworkInterface
import java.net.ServerSocket
import java.net.Socket
import java.nio.charset.Charset
import java.util.concurrent.Executors
import java.util.concurrent.atomic.AtomicBoolean
import androidx.compose.material.icons.filled.*
import androidx.compose.material3.ButtonDefaults
import androidx.compose.material3.IconButton
import androidx.compose.material3.IconToggleButton
import androidx.compose.ui.graphics.vector.ImageVector
import androidx.compose.ui.text.input.KeyboardType


@OptIn(ExperimentalMaterial3Api::class)
@Composable
fun SensorMonitorApp() {
    var deviceData by remember { mutableStateOf(DeviceData()) }
    var serverStatus by remember { mutableStateOf("服务器未启动") }
    var clientConnected by remember { mutableStateOf(false) }
    var communicationLog by remember { mutableStateOf(listOf<String>()) }
    var serverIp by remember { mutableStateOf("获取中...") }
    var clientCount by remember { mutableIntStateOf(0) }
    var clientIp by remember { mutableStateOf("") }

    var messageInput by remember { mutableStateOf("") }

    // 获取本机IP地址
    LaunchedEffect(Unit) {
        serverIp = getLocalIpAddress() ?: "无法获取IP"
    }

    // 添加消息到通信日志
    fun addToLog(message: String) {
        communicationLog = communicationLog + "${System.currentTimeMillis().toTimeString()}: $message"
        if (communicationLog.size > 50) {
            communicationLog = communicationLog.drop(1)
        }
    }

    // TCP服务器
    val tcpServer = remember {
        TcpServer(
            port = 12345,
            onStart = {
                serverStatus = "服务器运行中 (端口: 12345)"
                addToLog("服务器启动成功")
            },
            onNewClient = { ip ->
                clientConnected = true
                clientCount++
                clientIp = ip
                addToLog("客户端连接: $ip (总数: $clientCount)")
            },
            onError = { e, message ->
                addToLog("错误: $message - ${e?.message}")
            },
            onMessage = { ip, message, dataType ->
                addToLog("来自 $ip 的${dataType}数据: $message")
                try {
                    if (dataType == "JSON") {
                        deviceData = deviceData.updateFromReceivedJson(message)
                        addToLog("传感器数据已更新")
                    }
                } catch (e: Exception) {
                    addToLog("JSON解析错误: ${e.message}")
                }
            },
            onClientDisconnect = { ip ->
                clientConnected = false
                clientCount--
                addToLog("客户端断开连接: $ip (剩余: $clientCount)")
            }
        )
    }



    // 启动TCP服务器
    LaunchedEffect(Unit) {
        try {
            tcpServer.startServer()
        } catch (e: Exception) {
            addToLog("服务器启动失败: ${e.message}")
        }
    }

    // 确保在Composable销毁时停止服务器
    DisposableEffect(Unit) {
        onDispose {
            tcpServer.stopServer()
            addToLog("服务器已停止")
        }
    }

    Scaffold(
        modifier = Modifier.fillMaxSize(),
        topBar = {
            CenterAlignedTopAppBar(
                title = { Text("元爪宇宙监控系统") }
            )
        }
    ) { innerPadding ->
        Column(
            modifier = Modifier
                .padding(innerPadding)
                .verticalScroll(rememberScrollState())
                .padding(16.dp),
            horizontalAlignment = Alignment.CenterHorizontally
        ) {
            // 1. 传感器数据宫格
            SensorDataGrid(
                deviceData = deviceData,
                modifier = Modifier.fillMaxWidth()
            )

            Spacer(modifier = Modifier.height(16.dp))

            // 2. 设备控制
            ControlButtonsSection(
                deviceData = deviceData,
                onToggleLight = {
                    deviceData = deviceData.copy(ledState = !deviceData.ledState)
                    tcpServer.sendMessage(deviceData.toDeviceControlJsonString())
                    addToLog("发送设备控制命令: ${deviceData.toDeviceControlJsonString()}")
                },
                onToggleDoor = {
                    deviceData = deviceData.copy(doorState = !deviceData.doorState)
                    tcpServer.sendMessage(deviceData.toDeviceControlJsonString())
                    addToLog("发送设备控制命令: ${deviceData.toDeviceControlJsonString()}")
                },
                onToggleLightState = {
                    deviceData = deviceData.copy(lightAdjustState = !deviceData.lightAdjustState)
                    tcpServer.sendMessage(deviceData.toDeviceControlJsonString())
                    addToLog("发送设备控制命令: ${deviceData.toDeviceControlJsonString()}")
                },
                onToggleFeed = {
                    deviceData = deviceData.copy(feedTriggered = false)
                },
                isConnected = clientConnected,
                modifier = Modifier.fillMaxWidth(),
                onSendControlCommand = { json ->
                    tcpServer.sendMessage(json)
                    addToLog("发送设备控制命令: $json")
                },
                onSendFeedCommand = { json ->
                    tcpServer.sendMessage(json)
                    addToLog("发送喂食命令: $json")
                }
            )

            Spacer(modifier = Modifier.height(16.dp))

            // 3. 设置面板
            SettingsPanel(
                deviceData = deviceData,
                onDataChange = { newData ->
                    deviceData = newData
                },
                modifier = Modifier.fillMaxWidth(),
                isConnected = clientConnected,
                onSendSettings = {
                    tcpServer.sendMessage(deviceData.toSettingsJsonString())
                    addToLog("发送完整设置: ${deviceData.toSettingsJsonString()}")
                },
                tcpServer = tcpServer,  // Pass the tcpServer
                addToLog = { message -> addToLog(message) }  // Pass the addToLog function
            )

            Spacer(modifier = Modifier.height(100.dp))



            // 3. 服务器信息
            ServerInfoDisplay(
                ipAddress = serverIp,
                status = serverStatus,
                isConnected = clientConnected,
                clientCount = clientCount,
                clientIp = clientIp,
                modifier = Modifier.fillMaxWidth()
            )

            Spacer(modifier = Modifier.height(16.dp))

            // 4. 通信日志
            CommunicationLogDisplay(
                logMessages = communicationLog,
                modifier = Modifier.fillMaxWidth()
            )

            Spacer(modifier = Modifier.height(16.dp))

            // 5. 信息发送
            MessageInputSection(
                message = messageInput,
                onMessageChange = { messageInput = it },
                onSendMessage = {
                    if (it.isNotBlank()) {
                        tcpServer.sendMessage(it)
                        addToLog("发送消息: $it")
                        messageInput = ""
                    }
                },
                isConnected = clientConnected,
                modifier = Modifier.fillMaxWidth()
            )
        }
    }
}



@Composable
fun SensorDataGrid(
    deviceData: DeviceData,
    modifier: Modifier = Modifier
) {
    Card(
        modifier = modifier
            .fillMaxWidth()
            .heightIn(min = 280.dp),
        colors = CardDefaults.cardColors(
            containerColor = MaterialTheme.colorScheme.surfaceVariant
        ),
        shape = MaterialTheme.shapes.large.copy(CornerSize(16.dp))
    ) {
        Column(
            modifier = Modifier.padding(16.dp),
            verticalArrangement = Arrangement.spacedBy(12.dp)
        ) {
            Text(
                text = "传感器数据",
                fontSize = 20.sp,
                fontWeight = FontWeight.Bold,
                color = MaterialTheme.colorScheme.primary,
                modifier = Modifier.padding(bottom = 8.dp)
            )

            // 第一行
            Row(
                modifier = Modifier.fillMaxWidth(),
                horizontalArrangement = Arrangement.spacedBy(16.dp)
            ) {
                SensorDataItemLarge(
                    label = "室温",
                    value = "${"%.1f".format(deviceData.roomTemperature)}°C",
                    icon = Icons.Default.Home,  // 使用房屋图标表示室温
                    modifier = Modifier.weight(1f)
                )
                SensorDataItemLarge(
                    label = "湿度",
                    value = "${"%.1f".format(deviceData.humidity)}%",
                    icon = Icons.Default.Water,
                    modifier = Modifier.weight(1f)
                )
            }

            // 第二行
            Row(
                modifier = Modifier.fillMaxWidth(),
                horizontalArrangement = Arrangement.spacedBy(16.dp)
            ) {
                SensorDataItemLarge(
                    label = "体温",
                    value = "${"%.1f".format(deviceData.bodyTemperature)}°C",
                    icon = Icons.Default.Face,  // 使用人脸图标表示体温
                    modifier = Modifier.weight(1f)
                )
                SensorDataItemLarge(
                    label = "食物重量",
                    value = "${"%.1f".format(deviceData.foodWeight)}g",
                    icon = Icons.Default.Fastfood,
                    modifier = Modifier.weight(1f)
                )
            }

            // 第三行
            Row(
                modifier = Modifier.fillMaxWidth(),
                horizontalArrangement = Arrangement.spacedBy(16.dp)
            ) {
                SensorDataItemLarge(
                    label = "光照强度",
                    value = "${deviceData.lightIntensity}%",
                    icon = Icons.Default.LightMode,
                    modifier = Modifier.weight(1f)
                )
                SensorDataItemLarge(
                    label = "WC计数",
                    value = deviceData.wcCount.toString(),
                    icon = Icons.Default.Pets,
                    modifier = Modifier.weight(1f)
                )
            }
        }
    }
}


@Composable
fun SensorDataItemLarge(
    label: String,
    value: String,
    icon: ImageVector,
    modifier: Modifier = Modifier
) {
    Card(
        modifier = modifier.height(80.dp),
        colors = CardDefaults.cardColors(
            containerColor = MaterialTheme.colorScheme.surface
        ),
        shape = MaterialTheme.shapes.medium.copy(CornerSize(12.dp))
    ) {
        Row(
            modifier = Modifier
                .fillMaxSize()
                .padding(12.dp),
            verticalAlignment = Alignment.CenterVertically,
            horizontalArrangement = Arrangement.spacedBy(12.dp)
        ) {
            Icon(
                imageVector = icon,
                contentDescription = label,
                modifier = Modifier.size(36.dp),
                tint = MaterialTheme.colorScheme.primary
            )
            Column {
                Text(
                    text = label,
                    fontSize = 14.sp,  // 从这里修改，原为16.sp
                    fontWeight = FontWeight.Medium,
                    color = MaterialTheme.colorScheme.onSurfaceVariant
                )
                Text(
                    text = value,
                    fontSize = 20.sp,
                    fontWeight = FontWeight.Bold,
                    color = MaterialTheme.colorScheme.primary
                )
            }
        }
    }
}



// 1. 修改DeviceData类
data class DeviceData(
    // 传感器数据 (只接收)
    val roomTemperature: Double = 0.0,
    val humidity: Double = 0.0,
    val bodyTemperature: Double = 0.0,
    val foodWeight: Double = 0.0,
    val lightIntensity: Int = 0,
    val wcCount: Int = 0,

    // 设备控制状态 (双向)
    val ledState: Boolean = false,        // 灯光开关
    val doorState: Boolean = false,      // 门状态
    val feedTriggered: Boolean = false,  // 喂食触发标志(只发送true)
    val lightAdjustState: Boolean = false, // 灯光调节状态(true=调亮, false=调暗)

    // 参数设置 (只发送)
    val tempHigh: Float = 30f,
    val tempLow: Float = 23f,
    val humHigh: Int = 70,
    val humLow: Int = 30,
    val feedWeight: Int = 5,
    val disinfectionTime: Int = 5
) {
    // 发送控制命令的JSON
    fun toDeviceControlJsonString(): String {
        return """
        {
            "ledstate": $ledState,
            "doorstate": $doorState,
            "lightstate": $lightAdjustState,
            "foodstate": false
        }
        """.trimIndent()
    }

    // 发送喂食状态的JSON (只发送true)
    fun toFeedControlJsonString(): String {
        return """{"foodstate": true}"""
    }

    // 发送参数设置的JSON
    fun toSettingsJsonString(): String {
        return """
        {
            "temph": $tempHigh,
            "templ": $tempLow,
            "humh": $humHigh,
            "huml": $humLow,
            "foodweight": $feedWeight,
            "xiaodutime": $disinfectionTime
        }
        """.trimIndent()
    }

    // 接收的JSON (不再包含feedState)
    fun updateFromReceivedJson(json: String): DeviceData {
        return try {
            val obj = JSONObject(json)
            this.copy(
                roomTemperature = obj.optDouble("temperature"),
                humidity = obj.optDouble("humidity"),
                bodyTemperature = obj.optDouble("tiwen"),
                foodWeight = obj.optDouble("weight"),
                lightIntensity = obj.optInt("sun"),
                wcCount = obj.optInt("WC"),
                ledState = obj.optBoolean("led", this.ledState),
                doorState = obj.optBoolean("doorstate", this.doorState),
                lightAdjustState = obj.optBoolean("ledstate", this.lightAdjustState),
                // 不再更新feedState
            )
        } catch (e: Exception) {
            e.printStackTrace()
            this
        }
    }
}

@Composable
fun ControlButtonsSection(
    deviceData: DeviceData,
    onToggleLight: () -> Unit,
    onToggleDoor: () -> Unit,
    onToggleFeed: () -> Unit,
    onToggleLightState: () -> Unit,
    isConnected: Boolean,
    modifier: Modifier = Modifier,
    onSendControlCommand: (String) -> Unit,
    onSendFeedCommand: (String) -> Unit  // New parameter for feed command
) {
    Card(
        modifier = modifier,
        colors = CardDefaults.cardColors(containerColor = MaterialTheme.colorScheme.surfaceVariant),
        shape = MaterialTheme.shapes.large.copy(CornerSize(16.dp))
    ) {
        Column(
            modifier = Modifier.padding(16.dp),
            verticalArrangement = Arrangement.spacedBy(12.dp)
        ) {
            Text(
                text = "设备控制",
                fontSize = 20.sp,
                fontWeight = FontWeight.Bold,
                color = MaterialTheme.colorScheme.primary
            )

            // 第一行: 灯光和门
            Row(
                modifier = Modifier.fillMaxWidth(),
                horizontalArrangement = Arrangement.spacedBy(16.dp)
            ) {
                Button(
                    onClick = {
                        onToggleLight()

                    },
                    modifier = Modifier.weight(1f),
                    enabled = isConnected,
                    shape = MaterialTheme.shapes.medium.copy(CornerSize(12.dp)),
                    colors = ButtonDefaults.buttonColors(
                        containerColor = if (deviceData.ledState) MaterialTheme.colorScheme.primaryContainer
                        else MaterialTheme.colorScheme.surface
                    )
                ) {
                    Icon(
                        imageVector = Icons.Filled.Lightbulb,
                        contentDescription = "控制灯光",
                        modifier = Modifier.size(24.dp),
                        tint = if (deviceData.ledState) Color.Yellow else Color.Gray
                    )
                    Spacer(modifier = Modifier.width(8.dp))
                    Text(text = if (deviceData.ledState) "关灯" else "开灯")
                }

                Button(
                    onClick = {
                        val newDoorState = !deviceData.doorState
                        onToggleDoor()

                    },
                    modifier = Modifier.weight(1f),
                    enabled = isConnected,
                    shape = MaterialTheme.shapes.medium.copy(CornerSize(12.dp)),
                    colors = ButtonDefaults.buttonColors(
                        containerColor = if (deviceData.doorState) MaterialTheme.colorScheme.primaryContainer
                        else MaterialTheme.colorScheme.surface
                    )
                ) {
                    Icon(
                        imageVector = if (deviceData.doorState) Icons.Filled.LockOpen else Icons.Filled.Lock,
                        contentDescription = "控制门",
                        modifier = Modifier.size(24.dp),
                        tint = if (deviceData.doorState) Color.Green else Color.Gray
                    )
                    Spacer(modifier = Modifier.width(8.dp))
                    Text(text = if (deviceData.doorState) "关门" else "开门")
                }
            }

            // 第二行: 喂食和灯光调节
            Row(
                modifier = Modifier.fillMaxWidth(),
                horizontalArrangement = Arrangement.spacedBy(16.dp)
            ) {
                Button(
                    onClick = {
                        // 1. 临时设置 feedTriggered = true（用于生成JSON）
                        val tempData = deviceData.copy(feedTriggered = true)
                        // 2. 发送 true
                        onSendFeedCommand(tempData.toFeedControlJsonString())
                        // 3. 立即重置本地状态为 false（不发送）
                        onToggleFeed()  // 这会触发父组件更新状态为 false
                    },
                    modifier = Modifier.weight(1f),
                    enabled = isConnected,
                    colors = ButtonDefaults.buttonColors(
                        containerColor = MaterialTheme.colorScheme.primaryContainer
                    )
                ) {
                    Icon(Icons.Filled.Fastfood, "喂食", tint = Color(0xFF8B4513))
                    Spacer(Modifier.width(8.dp))
                    Text("喂食")
                }

                Button(
                    onClick = {
                        val newState = !deviceData.lightAdjustState
                        onToggleLightState()

                    },
                    modifier = Modifier.weight(1f),
                    enabled = isConnected,
                    shape = MaterialTheme.shapes.medium.copy(CornerSize(12.dp)),
                    colors = ButtonDefaults.buttonColors(
                        containerColor = if (deviceData.lightAdjustState) MaterialTheme.colorScheme.primaryContainer
                        else MaterialTheme.colorScheme.surface
                    )
                ) {
                    Icon(
                        imageVector = Icons.Filled.LightMode,
                        contentDescription = "灯光调节",
                        modifier = Modifier.size(24.dp),
                        tint = if (deviceData.lightAdjustState) Color.Yellow else Color.Gray
                    )
                    Spacer(modifier = Modifier.width(8.dp))
                    Text(text = if (deviceData.lightAdjustState) "调暗" else "调亮")
                }
            }
        }
    }
}


@Composable
fun SettingsPanel(
    deviceData: DeviceData,
    onDataChange: (DeviceData) -> Unit,
    modifier: Modifier = Modifier,
    isConnected: Boolean,
    onSendSettings: () -> Unit,
    tcpServer: TcpServer,  // Add this parameter
    addToLog: (String) -> Unit  // Add this parameter
) {
    Card(
        modifier = modifier,
        colors = CardDefaults.cardColors(containerColor = MaterialTheme.colorScheme.surfaceVariant),
        shape = MaterialTheme.shapes.large.copy(CornerSize(16.dp))
    ) {
        Column(
            modifier = Modifier.padding(16.dp),
            verticalArrangement = Arrangement.spacedBy(12.dp)
        ) {
            Text(
                text = "参数设置",
                fontSize = 20.sp,
                fontWeight = FontWeight.Bold,
                color = MaterialTheme.colorScheme.primary
            )

            // Temperature settings row
            Row(
                modifier = Modifier.fillMaxWidth(),
                horizontalArrangement = Arrangement.spacedBy(16.dp)
            ) {
                NumberInputWithArrows(
                    value = deviceData.tempHigh,
                    onValueChange = { newValue ->
                        if (newValue >= deviceData.tempLow) {
                            onDataChange(deviceData.copy(tempHigh = newValue))
                        }
                    },
                    label = "温度上限(℃)",
                    modifier = Modifier.weight(1f),
                    enabled = isConnected,
                    minValue = deviceData.tempLow,
                    maxValue = 50f,
                    step = 1f,
                    decimalPlaces = 0
                )

                NumberInputWithArrows(
                    value = deviceData.tempLow,
                    onValueChange = { newValue ->
                        if (newValue <= deviceData.tempHigh) {
                            onDataChange(deviceData.copy(tempLow = newValue))
                        }
                    },
                    label = "温度下限(℃)",
                    modifier = Modifier.weight(1f),
                    enabled = isConnected,
                    minValue = 0f,
                    maxValue = deviceData.tempHigh,
                    step = 1f,
                    decimalPlaces = 0
                )
            }

            // Humidity settings row
            Row(
                modifier = Modifier.fillMaxWidth(),
                horizontalArrangement = Arrangement.spacedBy(16.dp)
            ) {
                NumberInputWithArrows(
                    value = deviceData.humHigh.toFloat(),
                    onValueChange = { newValue ->
                        if (newValue >= deviceData.humLow) {
                            onDataChange(deviceData.copy(humHigh = newValue.toInt()))
                        }
                    },
                    label = "湿度上限(%)",
                    modifier = Modifier.weight(1f),
                    enabled = isConnected,
                    minValue = deviceData.humLow.toFloat(),
                    maxValue = 100f,
                    step = 1f,
                    decimalPlaces = 0
                )

                NumberInputWithArrows(
                    value = deviceData.humLow.toFloat(),
                    onValueChange = { newValue ->
                        if (newValue <= deviceData.humHigh) {
                            onDataChange(deviceData.copy(humLow = newValue.toInt()))
                        }
                    },
                    label = "湿度下限(%)",
                    modifier = Modifier.weight(1f),
                    enabled = isConnected,
                    minValue = 0f,
                    maxValue = deviceData.humHigh.toFloat(),
                    step = 1f,
                    decimalPlaces = 0
                )
            }

            // Feed weight and disinfection time in one row
            Row(
                modifier = Modifier.fillMaxWidth(),
                horizontalArrangement = Arrangement.spacedBy(16.dp)
            ) {
                NumberInputWithArrows(
                    value = deviceData.feedWeight.toFloat(),
                    onValueChange = { newValue ->
                        onDataChange(deviceData.copy(feedWeight = newValue.toInt()))
                    },
                    label = "投喂重量(g)",
                    modifier = Modifier.weight(1f),
                    enabled = isConnected,
                    minValue = 0f,
                    maxValue = 1000f,
                    step = 5f,
                    decimalPlaces = 0
                )

                NumberInputWithArrows(
                    value = deviceData.disinfectionTime.toFloat(),
                    onValueChange = { newValue ->
                        onDataChange(deviceData.copy(disinfectionTime = newValue.toInt()))
                    },
                    label = "消毒时间(秒)",
                    modifier = Modifier.weight(1f),
                    enabled = isConnected,
                    minValue = 0f,
                    maxValue = 10000f,
                    step = 1f,
                    decimalPlaces = 0
                )
            }

            // Send settings button
            Button(
                onClick = {
                    onSendSettings()
                    // 直接发送设置JSON，不包含控制命令
                    tcpServer.sendMessage(deviceData.toSettingsJsonString())
                    addToLog("发送设置参数: ${deviceData.toSettingsJsonString()}")
                },
                modifier = Modifier.fillMaxWidth(),
                enabled = isConnected,
                shape = MaterialTheme.shapes.medium.copy(CornerSize(12.dp))
            ) {
                Icon(
                    imageVector = Icons.Default.Settings,
                    contentDescription = "发送设置",
                    modifier = Modifier.size(24.dp)
                )
                Spacer(modifier = Modifier.width(8.dp))
                Text("发送设置参数")
            }
        }
    }
}

@Composable
fun NumberInputWithArrows(
    value: Float,
    onValueChange: (Float) -> Unit,
    label: String,
    modifier: Modifier = Modifier,
    enabled: Boolean = true,
    minValue: Float = Float.MIN_VALUE,
    maxValue: Float = Float.MAX_VALUE,
    step: Float = 1f,  // Default to 1 unit steps
    decimalPlaces: Int = 0  // Default to integer values
) {
    var textValue by remember(value) { mutableStateOf(if (decimalPlaces == 0) value.toInt().toString() else "%.${decimalPlaces}f".format(value)) }

    LaunchedEffect(value) {
        textValue = if (decimalPlaces == 0) value.toInt().toString() else "%.${decimalPlaces}f".format(value)
    }

    Column(modifier = modifier) {
        OutlinedTextField(
            value = textValue,
            onValueChange = { newValue ->
                textValue = newValue
                val parsedValue = newValue.toFloatOrNull()
                if (parsedValue != null && parsedValue in minValue..maxValue) {
                    onValueChange(parsedValue)
                }
            },
            label = { Text(label) },
            modifier = Modifier.fillMaxWidth(),
            enabled = enabled,
            keyboardOptions = KeyboardOptions(keyboardType = KeyboardType.Number),
            trailingIcon = {
                Column {
                    IconButton(
                        onClick = {
                            val newValue = (value + step).coerceAtMost(maxValue)
                            if (newValue != value) onValueChange(newValue)
                        },
                        enabled = enabled && value < maxValue,
                        modifier = Modifier.size(24.dp)
                    ) {
                        Icon(
                            imageVector = Icons.Default.ArrowDropUp,
                            contentDescription = "增加",
                            tint = if (enabled && value < maxValue) MaterialTheme.colorScheme.primary
                            else MaterialTheme.colorScheme.onSurface.copy(alpha = 0.38f)
                        )
                    }
                    IconButton(
                        onClick = {
                            val newValue = (value - step).coerceAtLeast(minValue)
                            if (newValue != value) onValueChange(newValue)
                        },
                        enabled = enabled && value > minValue,
                        modifier = Modifier.size(24.dp)
                    ) {
                        Icon(
                            imageVector = Icons.Default.ArrowDropDown,
                            contentDescription = "减少",
                            tint = if (enabled && value > minValue) MaterialTheme.colorScheme.primary
                            else MaterialTheme.colorScheme.onSurface.copy(alpha = 0.38f)
                        )
                    }
                }
            }
        )

        if (value < minValue || value > maxValue) {
            Text(
                text = when {
                    value < minValue -> "值不能小于${if (decimalPlaces == 0) minValue.toInt() else minValue}"
                    value > maxValue -> "值不能大于${if (decimalPlaces == 0) maxValue.toInt() else maxValue}"
                    else -> ""
                },
                color = MaterialTheme.colorScheme.error,
                fontSize = 12.sp,
                modifier = Modifier.padding(start = 16.dp, top = 4.dp)
            )
        }
    }
}


@Composable
fun MessageInputSection(
    message: String,
    onMessageChange: (String) -> Unit,
    onSendMessage: (String) -> Unit,
    isConnected: Boolean,
    modifier: Modifier = Modifier
) {
    Card(
        modifier = modifier,
        colors = CardDefaults.cardColors(containerColor = MaterialTheme.colorScheme.surfaceVariant),
        shape = MaterialTheme.shapes.large.copy(CornerSize(16.dp))
    ) {
        Column(
            modifier = Modifier.padding(16.dp),
            verticalArrangement = Arrangement.spacedBy(12.dp)
        ) {
            Text(
                text = "消息发送",
                fontSize = 20.sp,
                fontWeight = FontWeight.Bold,
                color = MaterialTheme.colorScheme.primary
            )

            OutlinedTextField(
                value = message,
                onValueChange = onMessageChange,
                label = { Text("输入要发送的消息") },
                modifier = Modifier.fillMaxWidth(),
                enabled = isConnected
            )

            Button(
                onClick = { onSendMessage(message) },
                modifier = Modifier.fillMaxWidth(),
                enabled = isConnected && message.isNotBlank(),
                shape = MaterialTheme.shapes.medium.copy(CornerSize(12.dp))
            ) {
                Icon(
                    imageVector = Icons.Filled.Send,
                    contentDescription = "发送消息",
                    modifier = Modifier.size(24.dp)
                )
                Spacer(modifier = Modifier.width(8.dp))
                Text("发送消息")
            }
        }
    }
}

@Composable
fun ServerInfoDisplay(
    ipAddress: String,
    status: String,
    isConnected: Boolean,
    clientCount: Int,
    clientIp: String,
    modifier: Modifier = Modifier
) {
    Card(
        modifier = modifier,  // 使用外部传入的modifier
        colors = CardDefaults.cardColors(containerColor = MaterialTheme.colorScheme.surfaceVariant)
    ) {
        Column(
            modifier = Modifier.padding(16.dp),
            verticalArrangement = Arrangement.spacedBy(8.dp)
        ) {
            Text(
                text = "服务器信息",
                fontSize = 20.sp,
                fontWeight = FontWeight.Bold,
                color = MaterialTheme.colorScheme.primary
            )

            Row(
                modifier = Modifier.fillMaxWidth(),
                horizontalArrangement = Arrangement.SpaceBetween
            ) {
                Text(text = "IP地址:", fontWeight = FontWeight.Medium)
                Text(text = ipAddress)
            }

            Row(
                modifier = Modifier.fillMaxWidth(),
                horizontalArrangement = Arrangement.SpaceBetween
            ) {
                Text(text = "运行状态:", fontWeight = FontWeight.Medium)
                Text(text = status)
            }

            Row(
                modifier = Modifier.fillMaxWidth(),
                horizontalArrangement = Arrangement.SpaceBetween
            ) {
                Text(text = "客户端连接:", fontWeight = FontWeight.Medium)
                Text(
                    text = if (isConnected) "已连接 (总数: $clientCount)" else "未连接",
                    color = if (isConnected) MaterialTheme.colorScheme.primary else MaterialTheme.colorScheme.error
                )
            }

            if (isConnected) {
                Row(
                    modifier = Modifier.fillMaxWidth(),
                    horizontalArrangement = Arrangement.SpaceBetween
                ) {
                    Text(text = "客户端IP:", fontWeight = FontWeight.Medium)
                    Text(text = clientIp)
                }
            }
        }
    }
}

@Composable
fun CommunicationLogDisplay(
    logMessages: List<String>,
    modifier: Modifier = Modifier
) {
    Card(
        modifier = Modifier.fillMaxWidth().padding(16.dp),
        colors = CardDefaults.cardColors(containerColor = MaterialTheme.colorScheme.surfaceVariant),
        shape = MaterialTheme.shapes.large.copy(CornerSize(16.dp))
    ) {
        Column(
            modifier = Modifier
                .padding(16.dp)
                .heightIn(min = 100.dp, max = 200.dp)
        ) {
            Text(
                text = "通信日志",
                fontSize = 20.sp,
                fontWeight = FontWeight.Bold,
                color = MaterialTheme.colorScheme.primary
            )

            Spacer(modifier = Modifier.height(8.dp))

            Column(
                modifier = Modifier
                    .verticalScroll(rememberScrollState())
                    .weight(1f)
            ) {
                logMessages.forEach { message ->
                    Text(
                        text = message,
                        fontSize = 12.sp,
                        modifier = Modifier.padding(vertical = 2.dp)
                    )
                }
            }
        }
    }
}



class TcpServer(
    private val port: Int,
    private val onStart: () -> Unit,
    private val onNewClient: (String) -> Unit,
    private val onError: (Throwable?, String) -> Unit,
    private val onMessage: (String, String, String) -> Unit,
    private val onClientDisconnect: (String) -> Unit
) {
    private var serverSocket: ServerSocket? = null
    private var clientSocket: Socket? = null
    private var outputStream: OutputStream? = null
    private val executor = Executors.newFixedThreadPool(4)
    private val isRunning = AtomicBoolean(false)

    fun startServer() {
        executor.execute {
            try {
                serverSocket = ServerSocket(port)
                isRunning.set(true)
                onStart()

                while (isRunning.get()) {
                    try {
                        clientSocket = serverSocket?.accept()
                        clientSocket?.let { socket ->
                            val clientIp = socket.inetAddress.hostAddress ?: "未知IP"
                            onNewClient(clientIp)

                            // 为每个客户端创建新的输出流
                            outputStream = socket.getOutputStream()

                            // 单独线程处理客户端消息
                            executor.execute {
                                try {
                                    val input = socket.getInputStream()
                                    val reader = BufferedReader(InputStreamReader(input, Charset.forName("GBK")))
                                    val buffer = CharArray(1024)

                                    while (isRunning.get() && !socket.isClosed) {
                                        if (input.available() > 0) {
                                            val bytesRead = reader.read(buffer)
                                            if (bytesRead == -1) break

                                            val textMessage = String(buffer, 0, bytesRead).trim()

                                            // 判断数据类型
                                            val dataType = when {
                                                isJson(textMessage) -> "JSON"
                                                isPrintableText(textMessage) -> "文本"
                                                else -> "二进制"
                                            }

                                            onMessage(clientIp, textMessage, dataType)
                                        } else {
                                            Thread.sleep(100)
                                        }
                                    }
                                } catch (e: Exception) {
                                    if (isRunning.get()) {
                                        onError(e, "读取错误")
                                    }
                                } finally {
                                    try {
                                        socket.close()
                                    } catch (e: Exception) {
                                        onError(e, "关闭客户端socket错误")
                                    }
                                    onClientDisconnect(clientIp)
                                }
                            }
                        }
                    } catch (e: Exception) {
                        if (isRunning.get()) {
                            onError(e, "接受客户端连接错误")
                        }
                    }
                }
            } catch (e: Exception) {
                onError(e, "服务器启动失败")
            } finally {
                stopServer()
            }
        }
    }

    fun sendMessage(message: String) {
        executor.execute {
            try {
                outputStream?.let {
                    // 使用GBK编码发送字符串
                    it.write(message.toByteArray(Charset.forName("GBK")))
                    it.flush()
                } ?: run {
                    onError(null, "发送失败: 无客户端连接")
                }
            } catch (e: Exception) {
                onError(e, "发送错误")
            }
        }
    }

    fun stopServer() {
        if (isRunning.compareAndSet(true, false)) {
            try {
                clientSocket?.close()
            } catch (e: Exception) {
                onError(e, "关闭客户端socket错误")
            }
            try {
                serverSocket?.close()
            } catch (e: Exception) {
                onError(e, "关闭服务器socket错误")
            }
            outputStream = null
        }
    }

    private fun isJson(text: String): Boolean {
        return try {
            text.trim().startsWith("{") && text.trim().endsWith("}")
        } catch (e: Exception) {
            false
        }
    }

    private fun isPrintableText(text: String): Boolean {
        return text.all { char ->
            char in '\u0020'..'\u007E' || char in '\u00A0'..'\u00FF' || char == '\n' || char == '\r'
        }
    }
}

// 获取本地IP地址
fun getLocalIpAddress(): String? {
    try {
        val interfaces = NetworkInterface.getNetworkInterfaces()
        while (interfaces.hasMoreElements()) {
            val intf = interfaces.nextElement()
            // 跳过回环接口和虚拟接口
            if (intf.isLoopback || intf.isVirtual || !intf.isUp) continue

            val addrs = intf.inetAddresses
            while (addrs.hasMoreElements()) {
                val addr = addrs.nextElement()
                if (!addr.isLoopbackAddress && addr is java.net.Inet4Address) {
                    return addr.hostAddress
                }
            }
        }
    } catch (e: Exception) {
        e.printStackTrace()
    }
    return null
}

// 时间戳转时间字符串
fun Long.toTimeString(): String {
    val date = java.util.Date(this)
    val format = java.text.SimpleDateFormat("HH:mm:ss.SSS")
    return format.format(date)
}

@Preview(showBackground = true)
@Composable
fun SensorMonitorAppPreview() {
    MyApplicationTheme {
        SensorMonitorApp()
    }
}