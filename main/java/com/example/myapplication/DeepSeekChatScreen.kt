// DeepSeekChatScreen.kt
package com.example.myapplication

import androidx.compose.foundation.background
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.Spacer
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.height
import androidx.compose.foundation.layout.imePadding
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.layout.size
import androidx.compose.foundation.lazy.LazyColumn
import androidx.compose.foundation.lazy.items
import androidx.compose.foundation.lazy.rememberLazyListState
import androidx.compose.foundation.shape.RoundedCornerShape
import androidx.compose.foundation.text.BasicText
import androidx.compose.foundation.text.KeyboardActions
import androidx.compose.foundation.text.KeyboardOptions
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.filled.History
import androidx.compose.material.icons.filled.Send
import androidx.compose.material3.Card
import androidx.compose.material3.CardDefaults
import androidx.compose.material3.CircularProgressIndicator
import androidx.compose.material3.DropdownMenu
import androidx.compose.material3.DropdownMenuItem
import androidx.compose.material3.ExperimentalMaterial3Api
import androidx.compose.material3.Icon
import androidx.compose.material3.IconButton
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.OutlinedTextField
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.runtime.LaunchedEffect
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableStateListOf
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.rememberCoroutineScope
import androidx.compose.runtime.setValue
import androidx.compose.runtime.snapshots.SnapshotStateList
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.draw.clip
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.platform.LocalContext
import androidx.compose.ui.platform.LocalFocusManager
import androidx.compose.ui.res.stringResource
import androidx.compose.ui.text.SpanStyle
import androidx.compose.ui.text.buildAnnotatedString
import androidx.compose.ui.text.font.FontWeight
import androidx.compose.ui.text.withStyle
import androidx.compose.ui.unit.dp
import androidx.compose.ui.unit.sp
import com.example.myapplication.api.ApiClient
import com.google.gson.Gson
import kotlinx.coroutines.launch
import okio.buffer
import okio.source
import retrofit2.Response
import com.example.myapplication.api.Message
import android.content.Context
import androidx.compose.runtime.saveable.rememberSaveable
import androidx.compose.ui.text.input.ImeAction
import androidx.compose.material.icons.filled.Add
import androidx.compose.material3.TopAppBar
import java.util.UUID

// 修改数据存储工具类以支持多对话
object ChatStorage {
    private const val PREFS_NAME = "chat_prefs"
    private const val CONVERSATIONS_KEY = "chat_conversations"
    private const val CURRENT_CONVERSATION_KEY = "current_conversation"

    data class Conversation(
        val id: String,
        val title: String,
        val messages: List<ChatMessage>,
        val timestamp: Long = System.currentTimeMillis()
    )

    fun saveConversations(context: Context, conversations: List<Conversation>) {
        val prefs = context.getSharedPreferences(PREFS_NAME, Context.MODE_PRIVATE)
        val json = Gson().toJson(conversations)
        prefs.edit().putString(CONVERSATIONS_KEY, json).apply()
    }

    fun loadConversations(context: Context): List<Conversation> {
        val prefs = context.getSharedPreferences(PREFS_NAME, Context.MODE_PRIVATE)
        val json = prefs.getString(CONVERSATIONS_KEY, null)
        return if (json != null) {
            Gson().fromJson(json, Array<Conversation>::class.java).toList()
        } else {
            // 如果没有历史记录，创建一个初始对话
            listOf(createNewConversation(context))
        }
    }

    fun saveCurrentConversationId(context: Context, id: String) {
        val prefs = context.getSharedPreferences(PREFS_NAME, Context.MODE_PRIVATE)
        prefs.edit().putString(CURRENT_CONVERSATION_KEY, id).apply()
    }

    fun getCurrentConversationId(context: Context): String? {
        val prefs = context.getSharedPreferences(PREFS_NAME, Context.MODE_PRIVATE)
        return prefs.getString(CURRENT_CONVERSATION_KEY, null)
    }

    fun createNewConversation(context: Context): Conversation {
        val newConversation = Conversation(
            id = UUID.randomUUID().toString(),
            title = "新对话",
            messages = listOf(ChatMessage(
                text = "我是元爪宇宙AI助手，请发送你的问题",
                isUser = false,
                timestamp = System.currentTimeMillis()
            ))
        )
        saveCurrentConversationId(context, newConversation.id)
        return newConversation
    }
}

data class ChatMessage(
    val text: String,
    val isUser: Boolean,
    val timestamp: Long = System.currentTimeMillis()
) : java.io.Serializable

@OptIn(ExperimentalMaterial3Api::class)
@Composable
fun DeepSeekChatScreen() {
    val context = LocalContext.current
    val messages = remember { mutableStateListOf<ChatMessage>() }
    var inputText by remember { mutableStateOf("") }
    var isLoading by remember { mutableStateOf(false) }
    var isStreaming by remember { mutableStateOf(false) }
    val focusManager = LocalFocusManager.current
    val coroutineScope = rememberCoroutineScope()
    val listState = rememberLazyListState()

    // 新增状态：对话列表和当前对话
    var conversations by remember { mutableStateOf<List<ChatStorage.Conversation>>(emptyList()) }
    var currentConversationId by remember { mutableStateOf<String?>(null) }
    var showConversationMenu by remember { mutableStateOf(false) }

    // 加载对话历史
    LaunchedEffect(Unit) {
        val loadedConversations = ChatStorage.loadConversations(context)
        conversations = loadedConversations

        val savedConversationId = ChatStorage.getCurrentConversationId(context)
        currentConversationId = savedConversationId ?: loadedConversations.firstOrNull()?.id

        // 加载当前对话的消息
        currentConversationId?.let { id ->
            val conversation = loadedConversations.find { it.id == id }
            conversation?.let {
                messages.clear()
                messages.addAll(it.messages)
            }
        }
    }

    // 保存当前对话到对话列表
    LaunchedEffect(messages.size) {
        if (messages.isNotEmpty() && currentConversationId != null) {
            val updatedConversations = conversations.map { conversation ->
                if (conversation.id == currentConversationId) {
                    // 更新对话标题为第一条用户消息或保持原样
                    val firstUserMessage = messages.firstOrNull { it.isUser }?.text
                    val newTitle = firstUserMessage?.take(20) ?: conversation.title

                    conversation.copy(
                        title = newTitle,
                        messages = messages.toList(),
                        timestamp = System.currentTimeMillis()
                    )
                } else {
                    conversation
                }
            }

            conversations = updatedConversations
            ChatStorage.saveConversations(context, updatedConversations)
        }

        if (messages.isNotEmpty()) {
            listState.animateScrollToItem(0)
        }
    }

    Column(
        modifier = Modifier
            .fillMaxSize()
            .background(MaterialTheme.colorScheme.background)
            .imePadding()
    ) {
        // 添加顶部应用栏
        TopAppBar(
            title = {
                Text(text = "元爪宇宙AI助手")
            },
            actions = {
                // 新建对话按钮
                IconButton(
                    onClick = {
                        coroutineScope.launch {
                            // 创建新对话
                            val newConversation = ChatStorage.createNewConversation(context)
                            conversations = conversations + newConversation
                            currentConversationId = newConversation.id

                            // 更新消息列表
                            messages.clear()
                            messages.addAll(newConversation.messages)

                            // 保存对话列表
                            ChatStorage.saveConversations(context, conversations)
                        }
                    }
                ) {
                    Icon(
                        imageVector = Icons.Default.Add,
                        contentDescription = "新对话"
                    )
                }

                // 历史对话按钮
                IconButton(
                    onClick = { showConversationMenu = true }
                ) {
                    Icon(
                        imageVector = Icons.Default.History,
                        contentDescription = "历史对话"
                    )
                }

                // 历史对话下拉菜单
                DropdownMenu(
                    expanded = showConversationMenu,
                    onDismissRequest = { showConversationMenu = false }
                ) {
                    if (conversations.isEmpty()) {
                        DropdownMenuItem(
                            text = { Text("没有历史对话") },
                            onClick = { showConversationMenu = false }
                        )
                    } else {
                        conversations.sortedByDescending { it.timestamp }.forEach { conversation ->
                            DropdownMenuItem(
                                text = {
                                    Text(
                                        text = conversation.title,
                                        maxLines = 1
                                    )
                                },
                                onClick = {
                                    currentConversationId = conversation.id
                                    ChatStorage.saveCurrentConversationId(context, conversation.id)
                                    messages.clear()
                                    messages.addAll(conversation.messages)
                                    showConversationMenu = false
                                }
                            )
                        }
                    }
                }
            }
        )

        LazyColumn(
            modifier = Modifier
                .weight(1f)
                .padding(horizontal = 16.dp),
            state = listState,
            reverseLayout = true
        ) {
            items(messages.reversed()) { message ->
                MessageBubble(message = message)
                Spacer(modifier = Modifier.height(8.dp))
            }
        }

        if (isLoading || isStreaming) {
            Box(
                modifier = Modifier
                    .fillMaxWidth()
                    .padding(16.dp),
                contentAlignment = Alignment.Center
            ) {
                CircularProgressIndicator(
                    modifier = Modifier.size(24.dp),
                    strokeWidth = 2.dp
                )
            }
        }

        Row(
            modifier = Modifier
                .fillMaxWidth()
                .padding(16.dp),
            verticalAlignment = Alignment.CenterVertically
        ) {
            OutlinedTextField(
                value = inputText,
                onValueChange = { inputText = it },
                modifier = Modifier
                    .weight(1f)
                    .clip(RoundedCornerShape(24.dp)),
                placeholder = { Text(stringResource(R.string.type_message_hint)) },
                keyboardOptions = KeyboardOptions(imeAction = ImeAction.Send),
                keyboardActions = KeyboardActions(
                    onSend = {
                        if (inputText.isNotBlank() && !isLoading) {
                            coroutineScope.launch {
                                sendMessage(
                                    inputText,
                                    messages,
                                    onLoadingChange = { isLoading = it },
                                    onStreamingChange = { isStreaming = it }
                                )
                                inputText = ""
                                focusManager.clearFocus()
                            }
                        }
                    }
                ),
                singleLine = false,
                maxLines = 3
            )

            Spacer(modifier = Modifier.size(8.dp))

            IconButton(
                onClick = {
                    if (inputText.isNotBlank() && !isLoading) {
                        coroutineScope.launch {
                            sendMessage(
                                inputText,
                                messages,
                                onLoadingChange = { isLoading = it },
                                onStreamingChange = { isStreaming = it }
                            )
                            inputText = ""
                            focusManager.clearFocus()
                        }
                    }
                },
                enabled = inputText.isNotBlank() && !isLoading
            ) {
                Icon(
                    imageVector = Icons.Default.Send,
                    contentDescription = stringResource(R.string.send_message),
                    tint = if (inputText.isNotBlank() && !isLoading) {
                        MaterialTheme.colorScheme.primary
                    } else {
                        MaterialTheme.colorScheme.onSurface.copy(alpha = 0.3f)
                    }
                )
            }
        }
    }
}

// 其他组件和函数保持不变...
@Composable
fun MessageBubble(message: ChatMessage) {
    val bubbleColor = if (message.isUser) {
        MaterialTheme.colorScheme.primary
    } else {
        MaterialTheme.colorScheme.surfaceVariant
    }
    val textColor = if (message.isUser) {
        MaterialTheme.colorScheme.onPrimary
    } else {
        MaterialTheme.colorScheme.onSurface
    }

    Box(
        modifier = Modifier
            .fillMaxWidth()
            .padding(vertical = 4.dp),
        contentAlignment = if (message.isUser) Alignment.CenterEnd else Alignment.CenterStart
    ) {
        Card(
            colors = CardDefaults.cardColors(
                containerColor = bubbleColor,
                contentColor = textColor
            ),
            shape = RoundedCornerShape(16.dp)
        ) {
            Column(
                modifier = Modifier.padding(16.dp)
            ) {
                FormattedMessageText(text = message.text, textColor = textColor)
            }
        }
    }
}

@Composable
fun FormattedMessageText(text: String, textColor: Color) {
    val paragraphs = text.split("\n\n")

    paragraphs.forEachIndexed { index, paragraph ->
        if (index > 0) {
            Spacer(modifier = Modifier.height(8.dp))
        }

        when {
            paragraph.startsWith("## ") -> {
                // Heading style
                Text(
                    text = paragraph.removePrefix("## "),
                    color = textColor,
                    fontWeight = FontWeight.Bold,
                    fontSize = 18.sp
                )
            }
            paragraph.startsWith("**") && paragraph.endsWith("**") -> {
                // Bold text
                Text(
                    text = paragraph.removeSurrounding("**"),
                    color = textColor,
                    fontWeight = FontWeight.Bold
                )
            }
            paragraph.startsWith("* ") -> {
                // List item
                Row {
                    Text(text = "• ", color = textColor)
                    Text(
                        text = paragraph.removePrefix("* "),
                        color = textColor
                    )
                }
            }
            paragraph.contains("**") -> {
                // Mixed bold and normal text
                val annotatedString = buildAnnotatedString {
                    val parts = paragraph.split("**")
                    parts.forEachIndexed { i, part ->
                        if (i % 2 == 1) {
                            // Bold text
                            withStyle(SpanStyle(fontWeight = FontWeight.Bold)) {
                                append(part)
                            }
                        } else {
                            append(part)
                        }
                    }
                }
                Text(
                    text = annotatedString,
                    color = textColor
                )
            }
            else -> {
                // Normal text
                Text(
                    text = paragraph,
                    color = textColor
                )
            }
        }
    }
}

private suspend fun sendMessage(
    text: String,
    messages: MutableList<ChatMessage>,
    onLoadingChange: (Boolean) -> Unit,
    onStreamingChange: (Boolean) -> Unit
) {
    messages.add(ChatMessage(text = text, isUser = true))

    val chatMessages = messages.map {
        Message(
            role = if (it.isUser) "user" else "assistant",
            content = it.text
        )
    }

    onLoadingChange(true)
    onStreamingChange(true)

    try {
        val request = ApiClient.ChatRequest(
            messages = chatMessages,
            stream = true
        )

        val response = ApiClient.deepSeekApiService.chatCompletions(
            authHeader = "Bearer ${ApiClient.API_KEY}",
            request = request
        )

        if (response.isSuccessful) {
            val responseBody = response.body()
            if (responseBody != null) {
                val aiMessage = ChatMessage(text = "", isUser = false)
                messages.add(aiMessage)
                val messageIndex = messages.lastIndex

                responseBody.source().use { source ->
                    val bufferedSource = source.buffer()
                    var accumulatedText = ""

                    while (true) {
                        val line = bufferedSource.readUtf8Line() ?: break
                        if (line.startsWith("data: ")) {
                            val json = line.substring(6).trim()
                            if (json == "[DONE]") break

                            try {
                                val streamResponse = Gson().fromJson(json, ApiClient.StreamResponse::class.java)
                                val content = streamResponse.choices.first().delta.content ?: ""

                                if (content.isNotEmpty()) {
                                    accumulatedText += content
                                    messages[messageIndex] = aiMessage.copy(text = accumulatedText)
                                }
                            } catch (e: Exception) {
                                e.printStackTrace()
                            }
                        }
                    }
                }
            }
        } else {
            messages.add(ChatMessage(
                text = "请求失败: ${response.code()} ${response.message()}",
                isUser = false
            ))
        }
    } catch (e: Exception) {
        messages.add(ChatMessage(
            text = "请求出错: ${e.localizedMessage}",
            isUser = false
        ))
    } finally {
        onLoadingChange(false)
        onStreamingChange(false)
    }
}